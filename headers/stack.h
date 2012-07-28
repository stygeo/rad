#pragma once

#include <stdlib.h>

// Link class used to construct the stack
template<class t>
struct ll_link {
  t       data;
  ll_link *next;
};

// rd_stack class
template<class t>
class rd_stack {
  public:
    rd_stack()     { llstart = NULL; n = 0; }
    ~rd_stack()    { empty(); }
    void empty();             // make empty stack
    void push(t node);        // push item on stack
    t pop();                  // pop item from stack
    inline t get_top()        { return llstart->data; }
    t get_no(int e);          // get item no. n from stack
    void do_for_all(void(*process)(t));
    void do_for_all_arg(void(*process)(t,void*), void *arg);
    inline int len()          { return n; }
    inline bool is_empty()    { return n == 0; }
    inline void show() {
      ll_link<t> *start;
      start = llstart;
      puts("+-----------------------------+");
      puts("| Stack                       |");
      puts("+-----------------------------+");
      while(start != NULL) {
        printf("| %-27s |\n", start->data->to_s());
        start = start->next;
      }
      puts("+-----------------------------+");
    }
  private:
    ll_link<t> *llstart;      // Start list< last in >
    int n;                    // Number of links
};

template<class t>
void rd_stack<t>::empty ()  {
   ll_link<t> *search, *del;

   search = llstart;
   while (search != NULL)    {
      del = search;
      search = search->next;
      delete del;
   }
   llstart = NULL; n = 0;
}

template<class t>
void rd_stack<t>::do_for_all(void(*process)(t)) {
  ll_link<t> *search;

  search = llstart;
  while(search != NULL) {
    process(search->data);
    search = search->next;
  }
}

template<class t>
void rd_stack<t>::do_for_all_arg(void(*process)(t, void*), void *arg) {
  ll_link<t> *search;

  search = llstart;
  while(search != NULL) {
    process(search->data, arg);
    search = search->next;
  }
}

template<class t>
t rd_stack<t>::pop() {
  t rv; ll_link<t> *del;

  rv = llstart->data;
  del = llstart;
  llstart = llstart->next; n--;

  delete del;

  return rv;
}

template<class t>
t rd_stack<t>::get_no(int e) {
  int i;
  ll_link<t> *link;

  link = llstart;
  i = n - e - 1;
  while(i > 0) { link = link->next; i--;}

  return link->data;
}

template<class t>
void rd_stack<t>::push(t node) {
  ll_link<t> *new_link;

  new_link = new ll_link<t>();
  new_link->data = node;
  new_link->next = llstart;
  llstart = new_link;
  n++;
}

