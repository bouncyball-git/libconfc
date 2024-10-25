#ifndef LIBCONFC_H
#define LIBCONFC_H

typedef enum { false, true } bool;

typedef struct conf_t {
   char *key;
   char *value;
   struct conf_t *next;
} conf_t;

conf_t *conf_init();
int conf_load(conf_t *cfg, const char *file_name);
int conf_save(conf_t *cfg, const char *file_name);
int conf_set_val(conf_t *cfg, const char *key, const char *value);
int conf_add_1st(conf_t **cfg, const char *key, const char *value);
const char *conf_get_val(conf_t *cfg, const char *key);
void conf_print(conf_t *cfg);
void conf_remove(conf_t **cfg, const char *key);
void conf_free(conf_t *cfg);
bool conf_check_val(conf_t *cfg, const char *key, const char *value);

#endif
