#include "boolector_conv.h"

smt_convt *
create_new_boolector_solver(bool is_cpp, bool int_encoding,
                            const namespacet &ns)
{
  return new boolector_convt(is_cpp, int_encoding, ns);
}

boolector_convt::boolector_convt(bool is_cpp, bool int_encoding,
                                 const namespacet &ns)
  : smt_convt(true, int_encoding, ns, is_cpp, false, false, true)
{
  if (int_encoding) {
    std::cerr << "Boolector does not support integer encoding mode"<< std::endl;
    abort();
  }

  btor = boolector_new();
  boolector_enable_model_gen(btor);
}

boolector_convt::~boolector_convt(void)
{
}

smt_convt::resultt
boolector_convt::dec_solve()
{
  int result = boolector_sat(btor);

  if (result == BOOLECTOR_SAT)
    return P_SATISFIABLE;
  else if (result == BOOLECTOR_UNSAT)
    return P_UNSATISFIABLE;
  else
    return P_ERROR;
}

tvt
boolector_convt::l_get(const smt_ast *l)
{
  assert(l->sort->id == SMT_SORT_BOOL);
  const btor_smt_ast *ast = btor_ast_downcast(l);
  char *result = boolector_bv_assignment(btor, ast->e);

  assert(result != NULL && "Boolector returned null bv assignment string");

  tvt t;

  switch (*result) {
  case '1':
    t = tvt(tvt::TV_TRUE);
    break;
  case '0':
    t = tvt(tvt::TV_FALSE);
    break;
  case 'x':
    t = tvt(tvt::TV_UNKNOWN);
    break;
  default:
    std::cerr << "Boolector bv model string \"" << result << "\" not of the "
              << "expected format" << std::endl;
    abort();
  }

  boolector_free_bv_assignment(btor, result);
  return t;
}

const std::string
boolector_convt::solver_text()
{
  return "Boolector";
}

void
boolector_convt::assert_ast(const smt_ast *a)
{
  const btor_smt_ast *ast = btor_ast_downcast(a);
  boolector_assert(btor, ast->e);
}

smt_ast *
boolector_convt::mk_func_app(const smt_sort *s, smt_func_kind k,
                               const smt_ast * const *args,
                               unsigned int numargs)
{
  const btor_smt_ast *asts[4];
  unsigned int i;

  assert(numargs <= 4);
  for (i = 0; i < numargs; i++)
    asts[i] = btor_ast_downcast(args[i]);

  switch (k) {
  case SMT_FUNC_BVADD:
    return new btor_smt_ast(s, boolector_add(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVSUB:
    return new btor_smt_ast(s, boolector_sub(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVMUL:
    return new btor_smt_ast(s, boolector_sub(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVSMOD:
    return new btor_smt_ast(s, boolector_smod(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVUMOD:
    return new btor_smt_ast(s, boolector_urem(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVSDIV:
    return new btor_smt_ast(s, boolector_sdiv(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVUDIV:
    return new btor_smt_ast(s, boolector_udiv(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVSHL:
    return new btor_smt_ast(s, boolector_sll(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVLSHR:
    return new btor_smt_ast(s, boolector_srl(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVASHR:
    return new btor_smt_ast(s, boolector_sra(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVNEG:
    return new btor_smt_ast(s, boolector_neg(btor, asts[0]->e));
  case SMT_FUNC_BVNOT:
  case SMT_FUNC_NOT:
    return new btor_smt_ast(s, boolector_not(btor, asts[0]->e));
  case SMT_FUNC_BVNXOR:
    return new btor_smt_ast(s, boolector_xnor(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVNOR:
    return new btor_smt_ast(s, boolector_nor(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVNAND:
    return new btor_smt_ast(s, boolector_nand(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVXOR:
  case SMT_FUNC_XOR:
    return new btor_smt_ast(s, boolector_xor(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVOR:
  case SMT_FUNC_OR:
    return new btor_smt_ast(s, boolector_or(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVAND:
  case SMT_FUNC_AND:
    return new btor_smt_ast(s, boolector_and(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_IMPLIES:
    return new btor_smt_ast(s, boolector_implies(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVULT:
    return new btor_smt_ast(s, boolector_ult(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVSLT:
    return new btor_smt_ast(s, boolector_slt(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVULTE:
    return new btor_smt_ast(s, boolector_ulte(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVSLTE:
    return new btor_smt_ast(s, boolector_slte(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVUGT:
    return new btor_smt_ast(s, boolector_ugt(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVSGT:
    return new btor_smt_ast(s, boolector_sgt(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVUGTE:
    return new btor_smt_ast(s, boolector_ugte(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_BVSGTE:
    return new btor_smt_ast(s, boolector_sgte(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_EQ:
    return new btor_smt_ast(s, boolector_eq(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_NOTEQ:
    return new btor_smt_ast(s, boolector_ne(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_ITE:
    return new btor_smt_ast(s, boolector_cond(btor, asts[0]->e, asts[1]->e,
                                              asts[2]->e));
  case SMT_FUNC_STORE:
    return new btor_smt_ast(s, boolector_write(btor, asts[0]->e, asts[1]->e,
                                               asts[2]->e));
  case SMT_FUNC_SELECT:
    return new btor_smt_ast(s, boolector_read(btor, asts[0]->e, asts[1]->e));
  case SMT_FUNC_CONCAT:
    return new btor_smt_ast(s, boolector_concat(btor, asts[0]->e, asts[1]->e));
  default:
    std::cerr << "Unhandled SMT func \"" << smt_func_name_table[k]
              << "\" in boolector conv" << std::endl;
    abort();
  }
}

smt_sort *
boolector_convt::mk_sort(const smt_sort_kind k, ...)
{
  // Boolector doesn't have any special handling for sorts, they're all always
  // explicit arguments to functions. So, just use the base smt_sort class.
  va_list ap;
  smt_sort *s = NULL, *dom, *range;
  unsigned long uint;
  bool thebool;

  va_start(ap, k);
  switch (k) {
  case SMT_SORT_INT:
  case SMT_SORT_REAL:
    std::cerr << "Boolector does not support integer encoding mode"<< std::endl;
    abort();
  case SMT_SORT_BV:
    uint = va_arg(ap, unsigned long);
    thebool = va_arg(ap, int);
    thebool = thebool;
    s = new smt_sort(k, uint);
    break;
  case SMT_SORT_ARRAY:
    dom = va_arg(ap, smt_sort *); // Consider constness?
    range = va_arg(ap, smt_sort *);
    s = new smt_sort(k, range->data_width, dom->data_width);
    break;
  case SMT_SORT_BOOL:
    s = new smt_sort(k);
    break;
  default:
    std::cerr << "Unhandled SMT sort in boolector conv" << std::endl;
    abort();
  }

  return s;
}

smt_ast *
boolector_convt::mk_smt_int(const mp_integer &theint __attribute__((unused)), bool sign __attribute__((unused)))
{
  std::cerr << "Boolector can't create integer sorts" << std::endl;
  abort();
}

smt_ast *
boolector_convt::mk_smt_real(const std::string &str __attribute__((unused)))
{
  std::cerr << "Boolector can't create Real sorts" << std::endl;
  abort();
}

smt_ast *
boolector_convt::mk_smt_bvint(const mp_integer &theint, bool sign,
                              unsigned int w)
{
  if (w > 32) {
    std::cerr << "Boolector only uses 'int's for passing integers around; "
              << "unless you're on an ILP64 machine, this " << w << " bit "
              << "model can't bee constructed" << std::endl;
    abort();
  }

  BtorNode *node;
  if (sign) {
    node = boolector_int(btor, theint.to_long(), w);
  } else {
    node = boolector_unsigned_int(btor, theint.to_ulong(), w);
  }

  const smt_sort *s = mk_sort(SMT_SORT_BV, w, sign);
  return new btor_smt_ast(s, node);
}

smt_ast *
boolector_convt::mk_smt_bool(bool val)
{
  BtorNode *node = (val) ? boolector_true(btor) : boolector_false(btor);
  const smt_sort *sort = mk_sort(SMT_SORT_BOOL);
  return new btor_smt_ast(sort, node);
}

smt_ast *
boolector_convt::mk_smt_symbol(const std::string &name, const smt_sort *s)
{
  symtable_type::iterator it = symtable.find(name);
  if (it != symtable.end())
    return it->second;

  BtorNode *node;

  switch(s->id) {
  case SMT_SORT_BV:
    node = boolector_var(btor, s->data_width, name.c_str());
    break;
  case SMT_SORT_BOOL:
    node = boolector_var(btor, 1, name.c_str());
    break;
  case SMT_SORT_ARRAY:
    node = boolector_array(btor, s->data_width, s->domain_width, name.c_str());
    break;
  default:
    return NULL; // Hax.
  }

  btor_smt_ast *ast = new btor_smt_ast(s, node);

  symtable.insert(symtable_type::value_type(name, ast));
  return ast;
}

smt_sort *
boolector_convt::mk_struct_sort(const type2tc &type __attribute__((unused)))
{
  abort();
}

smt_sort *
boolector_convt::mk_union_sort(const type2tc &type __attribute__((unused)))
{
  abort();
}

smt_ast *
boolector_convt::mk_extract(const smt_ast *a, unsigned int high,
                            unsigned int low, const smt_sort *s)
{
  const btor_smt_ast *ast = btor_ast_downcast(a);
  BtorNode *b = boolector_slice(btor, ast->e, high, low);
  return new btor_smt_ast(s, b);
}

expr2tc
boolector_convt::get_bool(const smt_ast *a __attribute__((unused)))
{
  abort();
}

expr2tc
boolector_convt::get_bv(const type2tc &t __attribute__((unused)), const smt_ast *a __attribute__((unused)))
{
  abort();
}

expr2tc
boolector_convt::get_array_elem(const smt_ast *array __attribute__((unused)), uint64_t index __attribute__((unused)), const smt_sort *sort __attribute__((unused)))
{
  abort();
}
