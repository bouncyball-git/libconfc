#include <stdio.h>
#include "libconfc.h"

int main(int argc, char **argv)
{
    if(argc < 2) {
	printf("Error: config file not specified\nUsage: %s initial.conf\n", argv[0]);
	return 1;
    }

    conf_t *conf_test = conf_init();
    if(!conf_load(conf_test, argv[1])) {
    	printf("Error loading config file\n");
	return 1;
    }

    printf("\nTest 1: print initial config\n");
    conf_print(conf_test);
    
    printf("\nTest 2: print only 'ip' and 'mac' key-value:\n");
    const char *val = conf_get_val(conf_test, "ip");
    if(val) printf("%s = %s\n", "ip", val);
    val = conf_get_val(conf_test, "mac");
    if(val) printf("%s = %s\n", "mac", val);
    
    printf("\nTest 3: remove key 'gateway' and print full config\n");
    conf_remove(&conf_test, "gateway");
    conf_print(conf_test);
    
    printf("\nTest 4: remove last key and print full config\n");
    conf_remove(&conf_test, "hostname");
    conf_print(conf_test);
    
    printf("\nTest 5: remove first key and print full config\n");
    conf_remove(&conf_test, "interface");
    conf_print(conf_test);

    printf("\nTest 6: check if key 'interface' exists\n");
    val = conf_get_val(conf_test, "interface");
    !val ? printf("'interface' does not exist\n") : printf("'interface' does exist. val = %s\n", val);
	
    printf("\nTest 7: check if key 'mac' exists\n");
    val = conf_get_val(conf_test, "mac");
    !val ? printf("'mac' does not exist\n") : printf("'mac' does exist. val = %s\n", val);

    printf("\nTest 8: check if key 'mac' = 'f0:19:54:27:7d:ab'\n");
    bool chk = conf_check_val(conf_test, "mac", "f0:79:59:67:7d:ad");
    printf("%s\n", chk ? "true" : "false");

    printf("\nTest 9: check if key 'mac' = 'a3:b1:cd:d7:e9:f5'\n");
    chk = conf_check_val(conf_test, "mac", "a3:b1:cd:d7:e9:f5");
    printf("%s\n", chk ? "true" : "false");

    printf("\nTest 10: add new key 'domain1' to the bottom of config\n");
    conf_set_val(conf_test, "domain1", "last.org");
    conf_print(conf_test);

    printf("\nTest 11: add new key 'domain0' to the top of config\n");
    conf_add_1st(&conf_test, "domain0", "first.org");
    conf_print(conf_test);

    printf("\nTest 12: update 'IP' value\n");
    conf_set_val(conf_test, "IP", "1.1.1.1");
    conf_print(conf_test);

    printf("\nTest 13: save modified config to 'modded.conf'\n");
    if(!conf_save(conf_test, "modded.conf")) {
    	printf("Error writing config file\n");
	return 1;
    } else {
	printf("Done!\n");
    }
    
    conf_free(conf_test);
}
