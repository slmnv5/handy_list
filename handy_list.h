#ifndef HANDY_LIST_H
#define HANDY_LIST_H

#include <stdbool.h>
#include <stdlib.h>

#define LIST_TYPE(name) list_##name##_t
#define NODE_TYPE(name) list_node_##name##_t
#define FN(name, action) list_##name##_##action

#define DECLARE_LIST(type, name)                                               \
  typedef struct list_node_##name {                                            \
    type data;                                                                 \
    struct list_node_##name *next;                                             \
  } list_node_##name##_t;                                                      \
                                                                               \
  typedef struct {                                                             \
    list_node_##name##_t *head;                                                \
    list_node_##name##_t *current;                                             \
    size_t size;                                                               \
  } list_##name##_t;


#define DEFINE_LIST_INIT(type, name)                                           \
  static inline void FN(name, init)(LIST_TYPE(name) * list) {                  \
    list->head = NULL;                                                         \
    list->current = NULL;                                                      \
    list->size = 0;                                                            \
  }

#define DEFINE_LIST_ADD(type, name)                                            \
  static inline bool FN(name, add)(LIST_TYPE(name) * list, type value) {       \
    NODE_TYPE(name) *n = (NODE_TYPE(name) *)malloc(sizeof(NODE_TYPE(name)));   \
    if (!n)                                                                    \
      return false;                                                            \
    n->data = value;                                                           \
    n->next = list->head;                                                      \
    list->head = n;                                                            \
    list->size++;                                                              \
    return true;                                                               \
  }

#define DEFINE_LIST_DELETE(type, name, comp)                                   \
  static inline bool FN(name, delete)(LIST_TYPE(name) * list, type value) {    \
    NODE_TYPE(name) *temp = list->head, *prev = NULL;                          \
    while (temp != NULL) {                                                     \
      type a = temp->data;                                                     \
      type b = value;                                                          \
      if (comp) {                                                              \
        if (prev == NULL)                                                      \
          list->head = temp->next;                                             \
        else                                                                   \
          prev->next = temp->next;                                             \
        if (list->current == temp)                                             \
          list->current = prev;                                                \
        free(temp);                                                            \
        list->size--;                                                          \
        return true;                                                           \
      }                                                                        \
      prev = temp;                                                             \
      temp = temp->next;                                                       \
    }                                                                          \
    return false;                                                              \
  }

#define DEFINE_LIST_CONTAINS(type, name, comp)                                 \
  static inline bool FN(name, contains)(LIST_TYPE(name) * list, type value) {  \
    NODE_TYPE(name) *temp = list->head;                                        \
    while (temp != NULL) {                                                     \
      type a = temp->data;                                                     \
      type b = value;                                                          \
      if (comp)                                                                \
        return true;                                                           \
      temp = temp->next;                                                       \
    }                                                                          \
    return false;                                                              \
  }

#define DEFINE_LIST_SIZE(type, name)                                           \
  static inline size_t FN(name, size)(LIST_TYPE(name) * list) {                \
    return list->size;                                                         \
  }

#define DEFINE_LIST_ITERATOR(type, name)                                       \
  static inline bool FN(name, has_next)(LIST_TYPE(name) * list) {              \
    if (list->current == NULL && list->head != NULL)                           \
      return true;                                                             \
    return (list->current != NULL && list->current->next != NULL);             \
  }                                                                            \
  static inline type FN(name, get_next)(LIST_TYPE(name) * list) {              \
    if (list->current == NULL)                                                 \
      list->current = list->head;                                              \
    else if (list->current->next != NULL)                                      \
      list->current = list->current->next;                                     \
    if (list->current != NULL)                                                 \
      return list->current->data;                                              \
    type fallback = {0};                                                       \
    return fallback;                                                           \
  }                                                                            \
  static inline void FN(name, reset_iterator)(LIST_TYPE(name) * list) {        \
    list->current = NULL;                                                      \
  }

#define DEFINE_LIST_FREE(type, name)                                           \
  static inline void FN(name, free)(LIST_TYPE(name) * list) {                  \
    NODE_TYPE(name) *curr = list->head;                                        \
    while (curr != NULL) {                                                     \
      NODE_TYPE(name) *next = curr->next;                                      \
      free(curr);                                                              \
      curr = next;                                                             \
    }                                                                          \
    list->head = NULL;                                                         \
    list->current = NULL;                                                      \
    list->size = 0;                                                            \
  }

#define DEFINE_LIST(type, name, comp)                                          \
  DECLARE_LIST(type, name)                                                     \
  DEFINE_LIST_INIT(type, name)                                                 \
  DEFINE_LIST_ADD(type, name)                                                  \
  DEFINE_LIST_DELETE(type, name, comp)                                         \
  DEFINE_LIST_CONTAINS(type, name, comp)                                       \
  DEFINE_LIST_SIZE(type, name)                                                 \
  DEFINE_LIST_ITERATOR(type, name)                                             \
  DEFINE_LIST_FREE(type, name)

#endif // HANDY_LIST_H
