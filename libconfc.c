#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "libconfc.h"

conf_t *conf_init()
{
    conf_t *cfg = malloc(sizeof *cfg);
    cfg->key = NULL;
    cfg->value = NULL;
    cfg->next = NULL;
    return cfg;
}

// initialize first element, update or add config element to the end of chain
int conf_set_val(conf_t *cfg, const char *key, const char *value)
{
    // case 1: set values right after conf_init()
    if(cfg->key == NULL)
    {
        if((cfg->key = malloc(strlen(key) + 1)) == NULL) return 0;
        strcpy(cfg->key, key);
        if((cfg->value = malloc(strlen(value) + 1)) == NULL) {
            free(cfg->key);
	    cfg->key = NULL;
    	    return 0;
    	}
        strcpy(cfg->value, value);
        return 1;
    }


    conf_t *c;
    for(c = cfg ; ; c = c->next)
    {
	// case 2: key matched, update value
        if(!strcasecmp(key, c->key))
        {
            free(c->value);
            if((c->value = malloc(strlen(value) + 1)) == NULL) return 0;
            strcpy(c->value, value);
            return 1;
        }

	// case 3: key not matched and this is last struct, add new struct to the end of the chain
        if(c->next == NULL)
        {
            if((c->next = malloc(sizeof(conf_t))) == NULL) return 0;
            c = c->next;
            if((c->key = malloc(strlen(key) + 1)) == NULL) {
        	free(c->next);
                c->next = NULL;
        	return 0;
            }
            strcpy(c->key, key);
            if((c->value = malloc(strlen(value) + 1)) == NULL) {
                free(c->key);
	        c->key = NULL;
        	free(c);
                c = NULL;
                return 0;
	    }
            strcpy(c->value, value);
            c->next = NULL;
            return 1;
        }
    }
}

// get value by key
const char *conf_get_val(conf_t *cfg, const char *key)
{
    conf_t *c;
    for(c = cfg; c != NULL; c = c->next)
    {
        if(!strcasecmp(key, c->key))
        {
            return c->value;
        }
    }
    return NULL;
}

// remove config element from chain
void conf_remove(conf_t **cfg, const char *key)
{
    conf_t *c, *cp = *cfg;
    for(c = *cfg; c != NULL; c = c->next)
    {
        if(!strcasecmp(key, c->key))
        {
	    if(c == *cfg) *cfg = c->next;
	    cp->next = c->next;
	    free(c->key); c->key = NULL;
            free(c->value); c->value = NULL;
	    free(c); c = NULL;
	    return;
        }
        cp = c;
    }
}

// load config from file
int conf_load(conf_t *cfg, const char *file_name)
{
    char line_buf[128] = { 0 };
    char key[64] = { 0 };
    char value[64] = { 0 };

    FILE* f = fopen(file_name, "r");
    if(!f) return 0;
    
    while(fgets(line_buf, sizeof line_buf, f) != NULL)
    {
        if(line_buf[0] == '#' || line_buf[0] == '\n') continue;
        sscanf(line_buf, "%[^:\t =\n]%*[:\t =\n]%s", key, value);
        conf_set_val(cfg, key, value);
        memset(key, 0x0, sizeof(key));
        memset(value, 0x0, sizeof(value));
    }

    fclose(f);
    return 1;
}

// save config to file
int conf_save(conf_t *cfg, const char *file_name)
{
    FILE* f; 
    f = fopen(file_name, "r");
    if(!f)
    {
        f = fopen(file_name, "w");
        if(!f) return 0;

        conf_t *c;
        for(c = cfg; c != NULL; c = c->next)
        {
            if(fprintf(f, "%s = %s\n", c->key, c->value) < 0)
            {
                fclose(f);
                return 0;
            }
        }
    }

    fclose(f);
    return 1;
}

// debug: print all elements of config
void conf_print(conf_t *cfg)
{
    conf_t * ct = cfg;
    do {
        printf("%s = %s\n", ct->key, ct->value);
        ct = ct->next;
    } while(ct != NULL);
}

// check if key has concret value
bool conf_check_val(conf_t *cfg, const char *key, const char *value)
{
    if(!strcmp(conf_get_val(cfg, key), value)) return true;
    else return false;
}

// adds config element to the beginning of chain
int conf_add_1st(conf_t **cfg, const char *key, const char *value)
{
    conf_t *c;
    if((c = malloc(sizeof(conf_t))) == NULL) return 0;
    if((c->key = malloc(strlen(key) + 1)) == NULL) {
	free(c);
        c = NULL;
        return 0;
    }    
    strcpy(c->key, key);
    if((c->value = malloc(strlen(value) + 1)) == NULL) {
	free(c->key);
        c->key = NULL;
	free(c);
        c = NULL;
	return 0;
    }
    strcpy(c->value, value);
    c->next = *cfg;
    *cfg = c;
    return 1;
}

// deletes whole config from memory
void conf_free(conf_t *cfg)
{
    conf_t *c = cfg, *ct;
    do {
        ct = c;
        c = c->next;
        free(ct->key); ct->key = NULL;
        free(ct->value); ct->value = NULL;
        free(ct); ct = NULL;
    } while (c != NULL);
}
