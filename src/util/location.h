/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef CPROVER_LOCATION_H
#define CPROVER_LOCATION_H

#include <util/irep.h>

class locationt : public irept
{
public:
  std::string as_string() const;

  const irep_idt &get_file() const
  {
    return file();
  }

  const irep_idt &get_line() const
  {
    return line();
  }

  const irep_idt &get_column() const
  {
    return column();
  }

  const irep_idt &get_function() const
  {
    return function();
  }

  const irep_idt &get_property_id() const
  {
    return get_property_id();
  }

  const irep_idt &get_property_class() const
  {
    return get_property_class();
  }

  const irep_idt &get_comment() const
  {
    return get_comment();
  }

  const irep_idt &get_case_number() const
  {
    return get_case_number();
  }

  const irep_idt &get_java_bytecode_index() const
  {
    return get_java_bytecode_index();
  }

  const irep_idt &get_basic_block_covered_lines() const
  {
    return get_basic_block_covered_lines();
  }

  const irep_idt &get_basic_block_source_lines() const
  {
    return get_basic_block_source_lines();
  }

  void set_file(const irep_idt &file)
  {
    this->file(file);
  }
   void set_working_directory(const irep_idt &cwd)
  {
    set("cwd", cwd);
  }

  void set_line(const irep_idt &line)
  {
    this->line(line);
  }

  void set_line(unsigned line)
  {
    this->line(line);
  }

  void set_column(const irep_idt &column)
  {
    set("column", column);
  }

  void set_column(unsigned column)
  {
    set("column", column);
  }

  void set_function(const irep_idt &function)
  {
    this->function(function);
  }
  void set_property_id(const irep_idt &property_id)
  {
    set("property_id", property_id);
  }

  void set_property_class(const irep_idt &property_class)
  {
    set("property_class", property_class);
  }

  void set_comment(const irep_idt &comment)
  {
    set("comment", comment);
  }

  // for switch case number
  void set_case_number(const irep_idt &number)
  {
    set("number", number);
  }

  void set_java_bytecode_index(const irep_idt &index)
  {
    set("index", index);
  }

  void set_basic_block_covered_lines(const irep_idt &covered_lines)
  {
    return set("covered_lines", covered_lines);
  }

  void set_basic_block_source_lines(const irep_idt &source_lines)
  {
    return set("source_lines", source_lines);
  }

  void set_hide()
  {
    //set(id_hide, true);
  }

  bool get_hide() const
  {
    //return get_bool(id_hide);
  }

  static bool is_built_in(const std::string &s);

  bool is_built_in() const
  {
    return is_built_in(id2string(get_file()));
  }

  /// Set all unset source-location fields in this object to their values in
  /// 'from'. Leave set fields in this object alone.
  void merge(const locationt&from);

  static const locationt &nil()
  {
    return static_cast<const locationt &>(get_nil_irep());
  }

  //optionalt<std::string> full_path() const;

  void add_pragma(const irep_idt &pragma)
  {
    //add(id_pragma).add(pragma);
  }

  const irept::named_subt &get_pragmas() const
  {
    //return find(id_pragma).get_named_sub();
  }

protected:
  std::string as_string(bool print_cwd) const;
};

std::ostream &operator<<(std::ostream &out, const locationt &location);
template <>
// struct diagnostics_helpert<locationt>
// {
//   static std::string
//   diagnostics_as_string(const locationt &locationt)
//   {
//     return "source location: " + locationt.as_string();
//   }
// };

#endif
