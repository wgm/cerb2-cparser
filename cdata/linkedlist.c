/* linkedlist - a singularly linked list
 * Copyright (c) 2002 Michael B. Allen <mballen@erols.com>
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include "cdata.h"

#ifdef MEMWATCH
#include <memwatch.h>
#warning "USING MEMWATCH"
#endif

struct node {
  void *ptr;
  void *data;
};

struct linkedlist {
  unsigned int max_size;
  unsigned int size;
  struct node *first;
  struct node *last;
  struct node *iter;
};

struct linkedlist *
linkedlist_new(unsigned int max_size)
{
  struct linkedlist *l;

  if ((l = (struct linkedlist*) malloc(sizeof(struct linkedlist))) == NULL) {
//bgh    PMNO(errno);
    return NULL;
  }
  l->max_size = max_size == 0 ? INT_MAX : max_size;
  l->size = 0;
  l->first = l->last = NULL;
  l->iter=NULL;
  return l;
}
void
linkedlist_del(struct linkedlist *l, void (*free_element)(void *))
{
  struct node *next;
  struct node *tmp;

  if (l == NULL) {
    return;
  }
  if(NULL!=l->first) {
    next = l->first;
    while (next != NULL) {
      if (NULL!=free_element && NULL!=next->data) {
        free_element(next->data);
      }
      tmp = next;
      next = next->ptr;
      free(tmp);
      tmp=NULL;
    }
  }
  free(l);
  l=NULL;
}

void
linkedlist_clear(struct linkedlist *l, void (*free_element)(void *))
{
  struct node *next = l->first;
  struct node *tmp;

  if (l == NULL) {
    return;
  }
  while (next != NULL) {
    if (free_element) {
      free_element(next->data);
    }
    tmp = next;
    next = next->ptr;
    free(tmp);
  }
  l->first = l->last = l->iter = NULL;
  l->size = 0;
}
int
linkedlist_add(struct linkedlist *l, void *data)
{
  struct node *n;

  if (l == NULL) {
    errno = EINVAL;
//bgh    PMNF(errno, ": l=NULL");
    return 0;
  }
  if (l->size == l->max_size) {
    errno = ERANGE;
//bgh    PMNF(errno, ": size=%u,max_size=%u", l->size, l->max_size);
    return 0;
  }
  n = malloc(sizeof *n);
  if (n == NULL) {
    return 0;
  }

  n->data = data;

  n->ptr = NULL;
  if (l->iter == NULL) {
    l->iter = n;
  }
  if (l->size == 0) {
    l->first = l->last = n;
  } else {
    l->last->ptr = n;
    l->last = n;
  }
  l->size++;
  return 1;
}
int
linkedlist_insert(struct linkedlist *l, unsigned int idx, void *data)
{
  struct node *n;

  if (l == NULL || data == NULL) {
    errno = EINVAL;
//bgh    PMNF(errno, ": l=%p,data=%p", l, data);
    return 0;
  }
  if (idx > l->size || l->size == l->max_size) {
    errno = ERANGE;
//bgh    PMNF(errno, ": idx=%u,size=%u,max_size=%u", idx, l->size, l->max_size);
    return 0;
  }
  n = malloc(sizeof *n);
  if (n == NULL) {
    return 0;
  }
  n->data = data;
  n->ptr = NULL;
  if (l->size == 0) {
    if (l->iter == NULL) {
      l->iter = n;
    }
    l->first = l->last = n;
  } else {
    if (idx == 0) {
      if (l->iter == l->first) {
        l->iter = n;
      }
      n->ptr = l->first;
      l->first = n;
    } else if (idx == l->size) {
      if (l->iter == NULL) {
        l->iter = n;
      }
      l->last->ptr = n;
      l->last = n;
    } else {
      struct node *tmp, *foo;
      unsigned int i;

      tmp = l->first;
      n->ptr = tmp->ptr;
      for (i = 1; i < idx; i++) {
        tmp = tmp->ptr;
        n->ptr = tmp->ptr;
      }
      foo = n->ptr;
      tmp->ptr = n;

      if (l->iter == n->ptr) {
        l->iter = n;
      }
    }

  }
  l->size++;

  return 1;
}
int
linkedlist_is_empty(const struct linkedlist *l)
{
  return l == NULL || l->size == 0;
}
unsigned int
linkedlist_size(const struct linkedlist *l)
{
  return l == NULL ? 0 : l->size;
}
void *
linkedlist_get(const struct linkedlist *l, unsigned int idx)
{
  if (l == NULL) {
    errno = EINVAL;
//bgh    PMNF(errno, ": l=%p", l);
    return NULL;
  }
  if (idx >= l->size) {
    errno = ERANGE;
//bgh    PMNF(errno, ": idx=%u,size=%u", idx, l->size);
    return NULL;
  }
  if (idx == 0) {
    return l->first->data;
  } else if (idx == l->size - 1) {
    return l->last->data;
  } else {
    struct node *next = l->first;
    unsigned int i = 0;

    while (i++ < idx) {
      next = next->ptr;
    }
    return next->data;
  }
}
void *
linkedlist_get_last(const struct linkedlist *l)
{
  if (l == NULL) {
    errno = EINVAL;
//bgh    PMNF(errno, ": l=%p", l);
    return NULL;
  }
  if (l->size == 0) {
    return NULL;
  }
  return l->last->data;
}
void
linkedlist_iterate(struct linkedlist *l)
{
  if (l) {
    l->iter = l->first;
  }
}
void *
linkedlist_next(struct linkedlist *l)
{
  void *r;

  if (l == NULL) {
    return NULL;
  }
  if (l->iter == NULL) {
    return NULL;
  }
  r = l->iter->data;
  l->iter = l->iter->ptr;
  return r;
}
void *
linkedlist_remove(struct linkedlist *l, unsigned int idx)
{
  void *result;

  if (l == NULL) {
    errno = EINVAL;
//bgh    PMNF(errno, ": l=%p", l);
    return NULL;
  }
  if (idx >= l->size) {
    return NULL;
  }
  if (idx == 0) {
    struct node *tmp;

    if (l->iter == l->first) {
      l->iter = l->first->ptr;
    }
    result = l->first->data;
    tmp = l->first;
    l->first = l->first->ptr;
    free(tmp);
  } else {
    struct node *n, *tmp;
    unsigned int i;

    n = l->first;
    for (i = 1; i < idx; i++) {
      n = n->ptr;
    }
    tmp = n->ptr;
    n->ptr = tmp->ptr;
    if (tmp == l->last) {
      l->last = n;
    }

    if (l->iter == tmp) {
      l->iter = tmp->ptr;
    }
    result = tmp->data;
    free(tmp);
  }
  l->size--;
  return result;
}
void *
linkedlist_remove_last(struct linkedlist *l)
{
  void *result;

  if (l == NULL) {
    errno = EINVAL;
//bgh    PMNF(errno, ": l=%p", l);
    return NULL;
  }
  if (l->size == 0) {
    return NULL;
  }
  if (l->iter == l->last) {
    l->iter = NULL;
  }
  if (l->size == 1) {
    result = l->first->data;
    free(l->first);
    l->first = l->last = NULL;
  } else {
    struct node *n;

    result = l->last->data;
    n = l->first;
    while (n->ptr != l->last) {
      n = n->ptr;
    }
    free(l->last);
    l->last = n;
    n->ptr = NULL;
  }
  l->size--;
  return result;
}

