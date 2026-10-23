#include "handy_list.h"

#include <stdio.h>
#include <string.h>

// Instantiating lists for different types with a single macro call
DEFINE_LIST(char, char, (a == b))
DEFINE_LIST(int, int, (a == b))
DEFINE_LIST(long, long, (a == b))

// Use a typedef for pointer types to keep macro names clean
typedef char *string_t;
DEFINE_LIST(string_t, string, (strcmp(a, b) == 0))

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

  report("New list reports initial size of 0", list_char_size(&list_char) == 0);
  report("Iterator has next on empty list reports false",
         !list_char_has_next(&list_char));

  list_char_add(&list_char, 'A');
  list_char_add(&list_char, 'B');
  list_char_add(&list_char, 'C');
  report("Size equals 3", list_char_size(&list_char) == 3);

  bool matches_order = true;
  char expected_chars[3] = {'C', 'B', 'A'};
  int idx = 0;

  while (list_char_has_next(&list_char) && idx < 3) {
    if (list_char_get_next(&list_char) != expected_chars[idx++]) {
      matches_order = false;
    }
  }
  report("Test list order", matches_order);
  report("Test end of list has next is false", !list_char_has_next(&list_char));

  list_char_free(&list_char);
  report("Size drops to 0 after free", list_char_size(&list_char) == 0);
  printf("\n");

  // -------------------------------------------------------------
  // TEST CASE 2: INT LIST DELETION
  // -------------------------------------------------------------
  printf("--- Test Case 2: Int List Deletions ---\n");
  list_int_t list_int;
  list_int_init(&list_int);

  // Setting up structural anchors: [30 -> 20 -> 10] (assuming head-insert)
  list_int_add(&list_int, 10);
  list_int_add(&list_int, 20);
  list_int_add(&list_int, 30);

  report("Contains is false for value 99", !list_int_contains(&list_int, 99));

  report("Contains is true for value 20", list_int_contains(&list_int, 20));
  list_int_delete(&list_int, 20);
  report("Size drops after deletion", list_int_size(&list_int) == 2);
  report("Contains is false for value 20 after deletion", !list_int_contains(&list_int, 20));

  list_int_delete(&list_int, 30);
  report("Size drops to 1 after deletion", list_int_size(&list_int) == 1);

  list_int_reset_iterator(&list_int);
  if (list_int_has_next(&list_int)) {
    report("Remaining node matches value 10",
           list_int_get_next(&list_int) == 10);
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

  // Adding matching strings allocated under unique stack positions
  char word1[] = "payload";
  char word2[] = "payload"; // Identical text value, different pointer footprint

  list_string_add(&list_string, word1);

  report("List locates element by value",
         list_string_contains(&list_string, word2));

  list_string_add(&list_string, "unique");
  list_string_add(&list_string, "payload"); // Duplicate text entry
  report("List includes duplicates", list_string_size(&list_string) == 3);

  list_string_delete(&list_string, "payload");
  size_t size_post_delete = list_string_size(&list_string);
  report("Delete runs safely for duplicates", size_post_delete < 3);

  list_string_free(&list_string);
  printf("\n");

  // -------------------------------------------------------------
  // TEST CASE 4: SPECIAL TESTS
  // -------------------------------------------------------------
  printf("--- Test Case 4: While Loop and Empty List ---\n");
  list_int_t sample_list;
  list_int_init(&sample_list);

  // We put 3 items in: [30 -> 20 -> 10]
  list_int_add(&sample_list, 10);
  list_int_add(&sample_list, 20);
  list_int_add(&sample_list, 30);

  int elements_processed = 0;
  list_int_reset_iterator(&sample_list);

  while (list_int_has_next(&sample_list)) {
    list_int_get_next(&sample_list);
    elements_processed++;
  }

  report("While loop visits every node", elements_processed == 3);
  list_int_free(&sample_list);

  list_int_t empty_list;
  list_int_init(&empty_list);

  int garbage_value = list_int_get_next(&empty_list);

  report("Get next on empty list without crashing", garbage_value == 0);
  printf("\n");

  // -------------------------------------------------------------
  // TEST CASE 5: DUPLICATE BEHAVIOUR AND ITERATOR RESETS
  // -------------------------------------------------------------
  printf("--- Test Case 5: Duplicate Behaviour and Iterator Resets ---\n");
  list_int_t advanced_list;
  list_int_init(&advanced_list);

  // 1. Testing how delete handles a single match out of duplicate values
  list_int_add(&advanced_list, 42);
  list_int_add(&advanced_list, 77);
  list_int_add(&advanced_list, 42);
  
  list_int_delete(&advanced_list, 42);
  report("Delete removes one instance of a duplicate", list_int_size(&advanced_list) == 2);

  // 2. Testing if resetting the iterator works in the middle of reading
  list_int_add(&advanced_list, 99); 
  list_int_reset_iterator(&advanced_list);
  
  if (list_int_has_next(&advanced_list)) {
    list_int_get_next(&advanced_list); 
  }
  list_int_reset_iterator(&advanced_list);
  report("Reset iterator returns back to the head item", list_int_get_next(&advanced_list) == 99);

  list_int_free(&advanced_list);
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

  list_long_free(&mass_list);
  report("List handles 1,000 elements successfully", works_correctly);
  printf("\n");

  return 0;
}

