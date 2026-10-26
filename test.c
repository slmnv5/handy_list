#include "handy_list.h"

#include <stdio.h>
#include <string.h>

// Instantiating lists for 3 different types
DEFINE_LIST(char, char, (a == b))
DEFINE_LIST(int, int, (a == b))
DEFINE_LIST(long, long, (a == b))

// Use a typedef for pointer type to get macro name with 'string'
typedef char *string_t;
DEFINE_LIST(string_t, string, (strcmp(a, b) == 0))

// helper fuction for tests
static void report(const char *test_name, bool condition) {
  printf("[%s] %s\n", condition ? "PASS" : "FAIL", test_name);
}

int main(void) {
  // -------------------------------------------------------------
  // TEST CASE 1: CHAR LIST
  // -------------------------------------------------------------
  printf("--- Test Case 1: List Basics ---\n");
  list_char_t list_char;
  list_char_init(&list_char);

  report("New list has size of 0", list_char_size(&list_char) == 0);
  report("Iterator has next on empty list is false", !list_char_has_next(&list_char));

  list_char_add(&list_char, 'A');
  list_char_add(&list_char, 'B');
  list_char_add(&list_char, 'C');
  report("Size test after add", list_char_size(&list_char) == 3);

  bool matches_order = true;
  char expected_chars[3] = {'C', 'B', 'A'};
  int idx = 0;

  while (list_char_has_next(&list_char) && idx < 3) {
    if (list_char_get_next(&list_char) != expected_chars[idx++]) {
      matches_order = false;
    }
  }
  report("Test list order after add", matches_order);
  report("Test end of list, has next is false", !list_char_has_next(&list_char));

  list_char_free(&list_char);
  report("Size drops to 0 after free", list_char_size(&list_char) == 0);
  printf("\n");

  // -------------------------------------------------------------
  // TEST CASE 2: INT LIST DELETION
  // -------------------------------------------------------------
  printf("--- Test Case 2: Int List Deletions ---\n");
  list_int_t list_int;
  list_int_init(&list_int);

  // List order: [30, 20, 10]
  list_int_add(&list_int, 10);
  list_int_add(&list_int, 20);
  list_int_add(&list_int, 30);

  report("Contains is false for value 99", !list_int_contains(&list_int, 99));

  report("Contains 20 is true", list_int_contains(&list_int, 20));
  list_int_delete(&list_int, 20);
  report("Size change after deletion", list_int_size(&list_int) == 2);
  report("Contains 20 is false after deletion", !list_int_contains(&list_int, 20));

  list_int_delete(&list_int, 30);
  report("Size change after deletion", list_int_size(&list_int) == 1);

  list_int_reset_iter(&list_int);
  if (list_int_has_next(&list_int)) {
    report("Remaining node has value 10", list_int_get_next(&list_int) == 10);
  }

  list_int_delete(&list_int, 10);
  report("List size drops to zero", list_int_size(&list_int) == 0);
  report("List head is NULL", list_int.head == NULL);

  list_int_free(&list_int);
  printf("\n");

  // -------------------------------------------------------------
  // TEST CASE 3: STRING LIST
  // -------------------------------------------------------------
  printf("--- Test Case 3: String List ---\n");
  list_string_t list_string;
  list_string_init(&list_string);

  // Two identical string values
  char word1[] = "payload";
  char word2[] = "payload";

  list_string_add(&list_string, word1);

  report("List locates element by value", list_string_contains(&list_string, word2));

  list_string_add(&list_string, "unique");
  list_string_add(&list_string, "payload");
  report("List with duplicates size test", list_string_size(&list_string) == 3);

  list_string_delete(&list_string, "payload");
  size_t size_post_delete = list_string_size(&list_string);
  report("Delete removes one instance of a duplicate", size_post_delete == 2);

  list_string_free(&list_string);
  printf("\n");

  // -------------------------------------------------------------
  // TEST CASE 4: SPECIAL TESTS
  // -------------------------------------------------------------
  printf("--- Test Case 4: While Loop and Empty List ---\n");

  list_int_init(&list_int);

  // List: [30 -> 20 -> 10]
  list_int_add(&list_int, 10);
  list_int_add(&list_int, 20);
  list_int_add(&list_int, 30);

  int elements_processed = 0;
  list_int_reset_iter(&list_int);

  while (list_int_has_next(&list_int)) {
    list_int_get_next(&list_int);
    elements_processed++;
  }

  report("While loop visits every node", elements_processed == 3);
  list_int_free(&list_int);

  list_int_t empty_list;
  list_int_init(&empty_list);

  int garbage_value = list_int_get_next(&empty_list);

  report("Get next on empty list without crashing", garbage_value == 0);
  printf("\n");

  // -------------------------------------------------------------
  // TEST CASE 5: ITERATOR RESET
  // -------------------------------------------------------------
  printf("--- Test Case 5: Duplicate and Iterator Resets ---\n");

  list_int_add(&list_int, 10);
  list_int_add(&list_int, 20);
  list_int_add(&list_int, 30);
  list_int_add(&list_int, 99);

  list_int_reset_iter(&list_int);

  if (list_int_has_next(&list_int)) {
    list_int_get_next(&list_int);
  }
  list_int_reset_iter(&list_int);
  report("Reset iterator returns back to the head item", list_int_get_next(&list_int) == 99);

  list_int_free(&list_int);
  printf("\n");

  // -------------------------------------------------------------
  // TEST CASE 6: TESTING LARGE LISTS
  // -------------------------------------------------------------
  printf("--- Test Case 6: Testing Large Lists ---\n");
  list_long_t mass_list;
  list_long_init(&mass_list);

  bool works_correctly = true;
  for (long i = 0; i < 1000; i++) {
    list_long_add(&mass_list, i);
  }

  if (list_long_size(&mass_list) != 1000) {
    works_correctly = false;
  }
  report("List handles 1,000 elements", works_correctly);

  list_long_free(&mass_list);
  printf("\n");

  return 0;
}
