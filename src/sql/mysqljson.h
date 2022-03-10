#ifndef MYSQLJSON_H_
#define MYSQLJSON_H_

#ifdef __cplusplus
extern "C" {
#endif

void init();
int format_json(const char * value, int length, char ** result);

#ifdef __cplusplus
}
#endif

#endif
