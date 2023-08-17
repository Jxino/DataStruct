#include <stdio.h>
#include "list.h"

#define debug printf("%s,%d: ", __FILE__, __LINE__); printf

#define DEBUG
#ifdef DEBUG
void test_int() {
    List* list = L_CREATE(sizeof(int), "i");
    debug("--\n");
    int x = 196;
    L_INSERT(list, 0, x);
    x = 203;
    L_INSERT(list, 1, x);
    x = 315;
    L_INSERT(list, 2, x);
    x = 317;
    L_INSERT(list, 2, x);
    x = 416;
    L_INSERT(list, 3, x);
    L_PRINT(list, NULL);
    int y = 315;
    int idx = L_FIND(list, y);
    debug("idx = %d\n", idx);
    L_DELETE(list, 2);
    L_PRINT(list, NULL);
    L_SORT(list, NULL);
    L_PRINT(list, NULL);    
    L_DESTROY(list);  
}

void test_char() {
    List* list = List_create(sizeof(int), "i");
    debug("--\n");
    int x = 196;
    List_insert(list, 0, (void*) &x);
    x = 203;
    List_insert(list, 1, (void*) &x);
    x = 315;
    List_insert(list, 2, (void*) &x);
    x = 317;
    List_insert(list, 2, (void*) &x);
    x = 416;
    List_insert(list, 3, (void*) &x);
    List_print(list, NULL);
    int y = 315;
    int idx = List_find(list, (void*) &y);
    debug("idx = %d\n", idx);
    List_retrieve(list, 3, &y);
    printf("y=%d\n", y);
    List_delete(list, 2);
    List_print(list, NULL);
    List_sort(list, NULL);
    List_print(list, NULL);    
    List_destroy(list);
}

void test_str() {
    List* list = L_CREATE(sizeof(char*), "s");
    debug("--\n");
    const char* x = "ABC";
    L_INSERT(list, 0, x);
    const char* y = "DEF";
    L_INSERT(list, 1, y);
    const char* z = "EFG";
    L_INSERT(list, 2, z);
    const char* w = "HIJ";
    L_INSERT(list, 2, w);
    const char* u = "XYZ";
    L_INSERT(list, 3, u);
    char* r; L_RETR(list, 3, &r);
    printf("r=%s\n", r);
    L_PRINT(list, NULL);
    const char* a = "KIM";
    int idx = L_FIND(list, a);
    debug("idx = %d\n", idx);
    L_DELETE(list, 2);
    L_PRINT(list, NULL);
    L_SORT(list, NULL);
    L_PRINT(list, NULL);
    L_DESTROY(list);    
}

int main() {
    test_int();
    test_char();
    test_str();
    return 0;
}
#endif