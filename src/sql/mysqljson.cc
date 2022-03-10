#undef WITH_PERFSCHEMA_STORAGE_ENGINE

#include "mysqljson.h"
#include <iostream>
#include "sql_class.h"
#include "opt_costconstantcache.h"
#include "item_json_func.h"
#include <iterator>

#ifdef WITH_PERFSCHEMA_STORAGE_ENGINE
#include "../storage/perfschema/pfs_server.h"
#endif /* WITH_PERFSCHEMA_STORAGE_ENGINE */

#ifdef __cplusplus
extern "C" {
#endif

void init()
{
  system_charset_info= &my_charset_utf8mb4_bin;

  const char * name = "mysqljson";

  char *argv[] = { const_cast<char*>(name), 0 };
  set_remaining_args(1, argv);

  #ifdef WITH_PERFSCHEMA_STORAGE_ENGINE
  pre_initialize_performance_schema();
  #endif /*WITH_PERFSCHEMA_STORAGE_ENGINE */

  MY_INIT(name);

  sys_var_init();
  init_common_variables();
  randominit(&sql_rand, 0, 0);
  transaction_cache_init();
  init_optimizer_cost_module(false);
}

int format_json(const char * value, int length, char ** result)
{
  const char *msg;

  Json_dom *dom = Json_dom::parse(value, length, &msg, NULL, false);

  String buffer;

  if (dom == NULL) {
    buffer.append(msg);
    buffer.append("\n");

    *result = buffer.c_ptr();

    return 1;
  }

  Json_wrapper w(dom);

  THD thd(false);
  thd.set_new_thread_id();
  thd.thread_stack= (char*) &thd;
  thd.store_globals();

  w.to_string(&buffer, true, "format");

  thd.cleanup_after_query();

  *result = buffer.c_ptr();

  return 0;
}

#ifdef __cplusplus
}
#endif

int main(int _argc, char **_argv)
{
  init();

  std::cin >> std::noskipws;
  std::istream_iterator<char> it(std::cin);
  std::istream_iterator<char> end;
  std::string value(it, end);

  char *output;

  int err = format_json(value.data(), value.length(), &output);

  std::cout << output;

  exit(err);
}
