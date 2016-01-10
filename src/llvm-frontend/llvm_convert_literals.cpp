/*
 * llvmtypecheck.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: mramalho
 */

#include "llvm_convert.h"

#include <arith_tools.h>
#include <bitvector.h>
#include <expr_util.h>
#include <c_types.h>
#include <string_constant.h>

void llvm_convertert::convert_character_literal(
  const clang::CharacterLiteral char_literal,
  exprt &dest)
{
  typet type;
  get_type(char_literal.getType(), type);

  dest =
    constant_exprt(
      integer2binary(char_literal.getValue(), bv_width(type)),
      integer2string(char_literal.getValue()),
      type);
}

void llvm_convertert::convert_string_literal(
  const clang::StringLiteral string_literal,
  exprt &dest)
{
  string_constantt string(string_literal.getString().str());
  dest.swap(string);
}

void llvm_convertert::convert_integer_literal(
  llvm::APInt val,
  typet type,
  exprt &dest)
{
  assert(type.is_unsignedbv() || type.is_signedbv());

  exprt the_val;
  if (type.is_unsignedbv())
  {
    the_val =
      constant_exprt(
        integer2binary(val.getZExtValue(), bv_width(type)),
        integer2string(val.getZExtValue()),
        type);
  }
  else
  {
    the_val =
      constant_exprt(
        integer2binary(val.getSExtValue(), bv_width(type)),
        integer2string(val.getSExtValue()),
        type);
  }

  dest.swap(the_val);
}

void llvm_convertert::convert_float_literal(
  llvm::APFloat val,
  typet type,
  exprt &dest)
{
  if(!config.ansi_c.use_fixed_for_float)
  {
    std::cerr << "floatbv unsupported, sorry" << std::endl;
    abort();
  }

  llvm::SmallVector<char, 32> string;
  val.toString(string, 32, 0);

  unsigned width = bv_width(type);
  mp_integer value;
  std::string float_string;

  if(!val.isInfinity())
  {
    mp_integer significand;
    mp_integer exponent;

    float_string = parse_float(string, significand, exponent);

    unsigned fraction_bits;
    const std::string &integer_bits = type.integer_bits().as_string();

    if (integer_bits == "")
      fraction_bits = width / 2;
    else
      fraction_bits = width - atoi(integer_bits.c_str());

    mp_integer factor = mp_integer(1) << fraction_bits;
    value = significand * factor;

    if(exponent < 0)
      value /= power(10, -exponent);
    else
    {
      value *= power(10, exponent);

      if(value <= -power(2, width - 1) - 1)
      {
        // saturate: use "smallest value"
        value = -power(2, width - 1);
      }
    }
  }
  else
  {
    // saturate: use "biggest value"
    value = power(2, width - 1) - 1;
    float_string = "2147483647.99999999976716935634613037109375";
  }

  dest =
    constant_exprt(
      integer2binary(value, bv_width(type)),
      float_string,
      type);
}

std::string llvm_convertert::parse_float(
  llvm::SmallVector<char, 32> &src,
  mp_integer &significand,
  mp_integer &exponent)
{
  // {digit}{dot}{31 digits}[+-]{exponent}

  unsigned p = 0;

  // get whole number
  std::string str_whole_number = "";
  str_whole_number += src[p++];

  // skip dot
  assert(src[p++] == '.');

  // get fraction part
  std::string str_fraction_part = "";
  while (src[p] != 'E')
    str_fraction_part += src[p++];

  // skip E
  assert(src[p++] == 'E');

  // get exponent
  assert(src[p] == '+' || src[p] == '-');

  // skip +
  if(src[p] == '+')
    p++;

  std::string str_exponent = "";
  str_exponent += src[p++];

  while (p < src.size())
    str_exponent += src[p++];

  std::string str_number = str_whole_number + str_fraction_part;

  if (str_number.empty())
    significand = 0;
  else
    significand = string2integer(str_number);

  if (str_exponent.empty())
    exponent = 0;
  else
    exponent = string2integer(str_exponent);

  exponent -= str_fraction_part.size();

  return str_whole_number;
}
