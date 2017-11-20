#include <stdio.h>
#include <stdlib.h>
#include "ltrx_webui.h"

void printList(struct locationUrl *list, unsigned int cnt) {
    unsigned int i;
    for (i = 0; i < cnt; i++) {
        printf("location: %s, proxy_url: %s\n", list[i].location, list[i].url);
    }
}

int main() {
    struct locationUrl *list;
    unsigned int cnt;
    int rc;

    /* To register URL */
    rc = webui_registerURL("/root/", "https://127.0.0.1:9123");
    if (rc == E_OK)
	    printf("Url registered successfully\n");
    else
	    printf("Error, return value :%d\n", rc);

    /* To register URL */
    rc = webui_registerURL("/", "https://127.0.0.1:9124");
    if (rc == E_OK)
	    printf("Url registered successfully\n");
	else
	    printf("Error, return value :%d\n", rc);

    /* To print entire list of proxy URLs */
    rc = webui_listURL(&list, &cnt);
    if (rc == E_OK) { 
	    printList(list, cnt);
	    free(list);
    } else {
	    printf("Error, return value :%d\n", rc);
    }

    /* To delete a proxy URL */
    rc = webui_deleteURL("/root/");
    if (rc == E_OK)
	    printf("Url deleted successfully\n");
    else
	    printf("Error, return value :%d\n", rc);

    /* To print entire list of proxy URLs */
    if (!webui_listURL(&list, &cnt)) {
        printList(list, cnt);
		free(list);
    }

    return 0;
}
