#include <stdio.h>
#include <stdlib.h>

#include "map.h"

#define debug printf("%s,%d: ", __FILE__, __LINE__); printf

int main() {
    Map* map = Map_create(sizeof(int), "i", sizeof(char), "c");
  {  
    int key = 3;
    char val = 'U';
    Map_put(map, &key, &val);
  }
  {  
    int key = 5;
    char val = 'B';
    Map_put(map, &key, &val);
  }
  {  
    int key = 7;
    char val = 'O';
    Map_put(map, &key, &val);
  }
  {  
    int key = 11;
    char val = 'A';
    Map_put(map, &key, &val);
  }
  {  
    int key = 13;
    char val = 'T';
    Map_put(map, &key, &val);
  }
    Map_print(map);

    return 0;
}