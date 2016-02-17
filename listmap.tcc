// $Id: listmap.tcc,v 1.7 2015-04-28 19:22:02-07 - - $

#include "listmap.h"
#include "trace.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);

   while (anchor() != anchor()->next) {
      this->erase(this->begin());
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);

   // If we're empty, just go ahead and insert a new element
   if (this->begin() == this->end()) {
      node* newNode = new node(anchor(), anchor(), pair);
      anchor()->next = newNode;
      anchor()->prev = newNode;
      return newNode;
   }

   // Else, see if we already exist
   iterator it = this->find(pair.first);
   if (it == iterator()) {
      // If our key does not exist, create it and insert it
      node* someNode = anchor()->next;

      while (less(someNode->value.first, pair.first)
             and someNode != anchor()) {
         someNode = someNode->next;
      }

      node* newNode = new node(someNode, someNode->prev, pair);
      someNode->prev->next = newNode;
      someNode->prev = newNode;
      return newNode;
   } else {
      // Update the value of our node
      it->second = pair.second;
      return it;
   }
   return it;
}

//
// listmap::find(const key_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that) {
   TRACE ('l', that);

   for (node* someNode = anchor()->next; someNode != anchor();
        someNode = someNode->next) {
      if (someNode->value.first == that) {
         return someNode;
      }
   }

   return iterator();
}

//
// iterator listmap::erase (iterator position)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('l', &*position);

   node* someNode = anchor()->next;
   while (someNode->value.first != position->first
          and someNode != anchor()) {
      someNode = someNode->next;
   }

   if (someNode != anchor()) {
      someNode->prev->next = someNode->next;
      someNode->next->prev = someNode->prev;
      someNode->next = nullptr;
      someNode->prev = nullptr;

      delete someNode;

      position.erase();
   }

   return iterator();
}

/*
   operator <<
*/
template <typename Key, typename Value, class Less>
ostream& operator<<(ostream& out,
                    listmap<Key,Value,Less>& map) {
   for (typename listmap<Key,Value,Less>::iterator it = map.begin();
        it != map.end(); ++it) {
      out << it->first << " " << it->second << endl;
   }

   return out;
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}

/*
   void listmap::iterator::erase()
*/
template <typename Key, typename Value, class Less>
void listmap<Key,Value,Less>::iterator::erase() {
   this->where = nullptr;
   //delete this;
}

//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

