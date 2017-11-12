/*******************************************************************\

Module: Container for C-Strings

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include <cassert>
#include <cstring>
#include <iostream>
#include <util/string_container.h>

string_containert string_container __attribute__((init_priority(101)));

unsigned string_containert::get(const char *s)
{
  std::string string_ptr(s);

  hash_tablet::iterator it=hash_table.find(string_ptr);
  
  if(it!=hash_table.end())
    return it->second;

  unsigned r=hash_table.size();

  // these are stable
  string_list.emplace_back(s);

  hash_table[string_list.back()]=r;
  
  // these are not
  string_vector.push_back(&string_list.back());

  return r;
}

unsigned string_containert::get(const std::string &s)
{

  hash_tablet::iterator it=hash_table.find(s);
  
  if(it!=hash_table.end())
    return it->second;

  unsigned r=hash_table.size();

  // these are stable
  string_list.push_back(s);

  hash_table[string_list.back()]=r;
  
  // these are not
  string_vector.push_back(&string_list.back());

  return r;
}

// To avoid the static initialization order fiasco, it's important to have all
// the globals that interact with the string pool initialized in the same
// translation unit. This ensures that the string_container object is
// initialized before all of the attribute-name globals are. Somewhat miserable.

#include <expr.cpp>
#include <irep.cpp>
#include <type.cpp>
