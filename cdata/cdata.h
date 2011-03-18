#ifndef STACK_H
#define STACK_H

/* stack - a dynamically resizing stack
 */

struct stack;
struct stack *stack_new(unsigned int max);
void stack_del(struct stack *s, void (*free_data_fn)(void *));

void stack_clear(struct stack *s, void (*free_data_fn)(void *));
int stack_push(struct stack *s, void *data);
void *stack_pop(struct stack *s);
int stack_is_empty(const struct stack *s);
unsigned int stack_size(const struct stack *s);
void stack_iterate(struct stack *s);
void *stack_next(struct stack *s);
void *stack_peek(struct stack *s);

#endif /* STACK_H */

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/* linkedlist - a singularly linked list
 */

struct linkedlist;

struct linkedlist *linkedlist_new(unsigned int);
void linkedlist_del(struct linkedlist *l, void (*free_data_fn)(void *));

void linkedlist_clear(struct linkedlist *l, void (*free_data_fn)(void *));
int linkedlist_add(struct linkedlist *l, void *data);
int linkedlist_insert(struct linkedlist *l, unsigned int idx, void *data);
int linkedlist_is_empty(const struct linkedlist *l);
unsigned int linkedlist_size(const struct linkedlist *l);
void *linkedlist_get(const struct linkedlist *l, unsigned int idx);
void *linkedlist_get_last(const struct linkedlist *l);
void linkedlist_iterate(struct linkedlist *l);
void *linkedlist_next(struct linkedlist *l);
void *linkedlist_remove(struct linkedlist *l, unsigned int idx);
void *linkedlist_remove_last(struct linkedlist *l);

#endif /* LINKEDLIST_H */
