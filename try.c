#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Step 1: Allocate memory for an initial string and assign a value
    char *str = malloc(20 * sizeof(char));
    if (str == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }
    strcpy(str, "Original string");

    printf("Before freeing: %s\n", str);
	
    // Step 2: Free the allocated memory for the original string
    free(str);

    // Step 3: Allocate memory for a new string and assign a different value
    str = malloc(30 * sizeof(char));  // Allocate more space for a longer string
    if (str == NULL) {
        perror("Failed to allocate memory");
        return 1;
    }
    strcpy(str, "This is the new string!");

    // Step 4: Print the new string
    printf("After replacing: %s\n", str);

    // Step 5: Free the new string
    free(str);

    return 0;
}
