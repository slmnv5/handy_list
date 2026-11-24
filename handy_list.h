#ifndef HANDY_LIST_H
#define HANDY_LIST_H

#include <stdbool.h>
#include <stdlib.h>

#define LIST_TYPE(name) list_##name##_t
#define NODE_TYPE(name) list_node_##name##_t
#define FN(name, action) list_##name##_##action

#define DECLARE_LIST(type, name)                                                                   \
  typedef struct list_node_##name {                                                                \
    type data;                                                                                     \
    struct list_node_##name *next;                                                                 \
  } list_node_##name##_t;                                                                          \
                                                                                                   \
  typedef struct {                                                                                 \
    list_node_##name##_t *head;                                                                    \
    list_node_##name##_t *current;                                                                 \
    size_t size;                                                                                   \
  } list_##name##_t;

#define DEFINE_LIST_INIT(type, name)                                                               \
  static inline void FN(name, init)(LIST_TYPE(name) * list) {                                      \
    list->head = NULL;                                                                             \
    list->current = NULL;                                                                          \
    list->size = 0;                                                                                \
  }

#define DEFINE_LIST_ADD_HEAD(type, name)                                                                \
  static inline bool FN(name, add_head)(LIST_TYPE(name) * list, type value) {                           \
    NODE_TYPE(name) *n = (NODE_TYPE(name) *)malloc(sizeof(NODE_TYPE(name)));                       \
    if (!n) return false;                                                                          \
    n->data = value;                                                                               \
    n->next = list->head;                                                                          \
    list->head = n;                                                                                \
    list->size++;                                                                                  \
    return true;                                                                                   \
  }


#define DEFINE_LIST_ADD_TAIL(type, name)                                                           \
  static inline bool FN(name, add_tail)(LIST_TYPE(name) * list, type value) {                      \
    NODE_TYPE(name) *n = (NODE_TYPE(name) *)malloc(sizeof(NODE_TYPE(name)));                       \
    if (!n) return false;                                                                          \
    n->data = value;                                                                               \
    n->next = NULL;                                                                                \
    if (list->head == NULL) {                                                                      \
      list->head = n;                                                                              \
    } else {                                                                                       \
      NODE_TYPE(name) *curr = list->head;                                                          \
      while (curr->next != NULL) curr = curr->next;                                                \
      curr->next = n;                                                                              \
    }                                                                                              \
    list->size++;                                                                                  \
    return true;                                                                                   \
  }

#define DEFINE_LIST_POP_HEAD(type, name)                                                           \
  static inline bool FN(name, pop_head)(LIST_TYPE(name) * list, type *out) {                       \
    if (list->head == NULL) return false;                                                          \
    NODE_TYPE(name) *n = list->head;                                                               \
    if (out) *out = n->data;                                                                       \
    list->head = n->next;                                                                          \
    if (list->current == n) list->current = NULL;                                                  \
    free(n);                                                                                       \
    list->size--;                                                                                  \
    return true;                                                                                   \
  }

  #define DEFINE_LIST_POP_TAIL(type, name)                                                           \
  static inline bool FN(name, pop_tail)(LIST_TYPE(name) * list, type *out) {                       \
    if (list->head == NULL) return false;                                                          \
    NODE_TYPE(name) *curr = list->head;                                                            \
    NODE_TYPE(name) *prev = NULL;                                                                  \
    while (curr->next != NULL) {                                                                   \
      prev = curr;                                                                                 \
      curr = curr->next;                                                                           \
    }                                                                                              \
    if (out) *out = curr->data;                                                                    \
    if (prev == NULL) {                                                                            \
      list->head = NULL;                                                                           \
    } else {                                                                                       \
      prev->next = NULL;                                                                           \
    }                                                                                              \
    if (list->current == curr) list->current = prev;                                               \
    free(curr);                                                                                    \
    list->size--;                                                                                  \
    return true;                                                                                   \
  }



#define DEFINE_LIST_DELETE(type, name, compare)                                                       \
  static inline bool FN(name, delete)(LIST_TYPE(name) * list, type value) {                        \
    NODE_TYPE(name) *n = list->head, *prev = NULL;                                                 \
    while (n != NULL) {                                                                            \
      type a = n->data;                                                                            \
      type b = value;                                                                              \
      if (compare) {                                                                                  \
        if (prev == NULL)                                                                          \
          list->head = n->next;                                                                    \
        else                                                                                       \
          prev->next = n->next;                                                                    \
        if (list->current == n) list->current = prev;                                              \
        free(n);                                                                                   \
        list->size--;                                                                              \
        return true;                                                                               \
      }                                                                                            \
      prev = n;                                                                                    \
      n = n->next;                                                                                 \
    }                                                                                              \
    return false;                                                                                  \
  }

#define DEFINE_LIST_CONTAINS(type, name, compare)                                                     \
  static inline bool FN(name, contains)(LIST_TYPE(name) * list, type value) {                      \
    NODE_TYPE(name) *n = list->head;                                                               \
    while (n != NULL) {                                                                            \
      type a = n->data;                                                                            \
      type b = value;                                                                              \
      if (compare) return true;                                                                       \
      n = n->next;                                                                                 \
    }                                                                                              \
    return false;                                                                                  \
  }

#define DEFINE_LIST_SIZE(type, name)                                                               \
  static inline size_t FN(name, size)(LIST_TYPE(name) * list) { return list->size; }

#define DEFINE_LIST_HAS_NEXT(type, name)                                                           \
  static inline bool FN(name, has_next)(LIST_TYPE(name) * list) {                                  \
    if (list->current == NULL && list->head != NULL) return true;                                  \
    return (list->current != NULL && list->current->next != NULL);                                 \
  }

#define DEFINE_LIST_GET_NEXT(type, name)                                                           \
  static inline type FN(name, get_next)(LIST_TYPE(name) * list) {                                  \
    if (list->current == NULL)                                                                     \
      list->current = list->head;                                                                  \
    else if (list->current->next != NULL)                                                          \
      list->current = list->current->next;                                                         \
    if (list->current != NULL) return list->current->data;                                         \
    type fallback = {0};                                                                           \
    return fallback;                                                                               \
  }

#define DEFINE_LIST_RESET_ITER(type, name)                                                         \
  static inline void FN(name, reset_iter)(LIST_TYPE(name) * list) { list->current = NULL; }

#define DEFINE_LIST_FREE(type, name)                                                               \
  static inline void FN(name, free)(LIST_TYPE(name) * list) {                                      \
    NODE_TYPE(name) *curr = list->head;                                                            \
    while (curr != NULL) {                                                                         \
      NODE_TYPE(name) *next = curr->next;                                                          \
      free(curr);                                                                                  \
      curr = next;                                                                                 \
    }                                                                                              \
    list->head = NULL;                                                                             \
    list->current = NULL;                                                                          \
    list->size = 0;                                                                                \
  }

#define DEFINE_LIST(type, name, compare)                                                              \
  DECLARE_LIST(type, name)                                                                         \
  DEFINE_LIST_INIT(type, name)                                                                     \
  DEFINE_LIST_ADD_HEAD(type, name)                                                                      \
  DEFINE_LIST_ADD_TAIL(type, name)                                                                 \
  DEFINE_LIST_POP_HEAD(type, name)                                                                 \
  DEFINE_LIST_POP_TAIL(type, name)                                                                 \
  DEFINE_LIST_DELETE(type, name, compare)                                                             \
  DEFINE_LIST_CONTAINS(type, name, compare)                                                           \
  DEFINE_LIST_SIZE(type, name)                                                                     \
  DEFINE_LIST_HAS_NEXT(type, name)                                                                 \
  DEFINE_LIST_GET_NEXT(type, name)                                                                 \
  DEFINE_LIST_RESET_ITER(type, name)                                                               \
  DEFINE_LIST_FREE(type, name)

#endif // HANDY_LIST_H
