/*******************************************************************\

Module:

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <iostream>
#include <util/location.h>

std::string locationt::as_string() const
{
  std::string dest;

  const irep_idt &file = get_file();
  const irep_idt &line = get_line();
  const irep_idt &column = get_column();
  const irep_idt &function = get_function();
  const irep_idt &bytecode=get_java_bytecode_index(); 

  if(file != "")
  {
    if(dest != "")
      dest += " ";
    dest += "file " + id2string(file);
  }
  if(line != "")
  {
    if(dest != "")
      dest += " ";
    dest += "line " + id2string(line);
  }
  if(column != "")
  {
    if(dest != "")
      dest += " ";
    dest += "column " + id2string(column);
  }
  if(function != "")
  {
    if(dest != "")
      dest += " ";
    dest += "function " + id2string(function);
  }

  return dest;
}

void locationt::merge(const locationt &from)
{
  forall_named_irep(it, from.get_named_sub())
  {
    if(get(it->first).empty())
      set(it->first, it->second);
  }
}

/// Get a path to the file, including working directory.
/// \return Full path unless the file name is empty or refers
///   to a built-in, in which case {} is returned.
optionalt<std::string> locationt::full_path() const
{
  const auto file = id2string(get_file());

  if(file.empty() || is_built_in(file))
    return {};

  return concat_dir_file(id2string(get_working_directory()), file);
}

std::ostream &operator<<(std::ostream &out, const locationt &location)
{
  if(location.is_nil())
    return out;

  out << location.as_string();

  return out;
}
