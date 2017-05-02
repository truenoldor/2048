#ifndef CONLIST_h
#define CONLIST_h

#include "oxygine-framework.h"
#include <functional>

#define T oxygine::Vector2

struct Polygon_
{
	Polygon_(){ p = NULL; size = 0; }
	oxygine::Vector2 *p;
	int size;
};

struct Node
{
	int key;
	T p;
	Node *next;
};

struct SList
{
	SList() { size = 0; head = tail = NULL; }
	SList( SList &s ){ size = s.size; head = s.head; tail = s.tail; }
	void appendtoTail(const T &t);
	void delete_(int _key);
	void print_list();
	Node* find(int );
	void clear();

	int size;
	Node *head, *tail;
};
//добавление элемента T в список
void SList::appendtoTail(const T &t)
{
// 	if ( tail )
// 	{
// 		T p = ( tail->p - t );
// 		if ( p.length() < 2.f )
// 		{
// 			return;
// 		}
// 	}
	Node *node = new Node;
	if(!node) return;
	node->key = size;
	node->p = t;
	if(size == 0)
	{
		node->next = node;
		head = tail = node;
		size = 1;
		return;
	}
	tail->next = node;
	tail = node;
	node->next = head;
	size++;
}
//поиск
Node * SList::find(int key)
{
	Node *n = head;

	while( n->next != head )
	{
		if( n->next->key == key )	return n;
		n = n->next;
	}
	if( n->next->key == key )	return n;
	return NULL;
}
//удаление
void SList::delete_(int _key)
{
	Node *n = find(_key), *t;

	if( n == NULL || n->next->key != _key )	return;

	if( head == n->next )
		head = n->next->next;

	if( tail == n->next )
	{
		tail = n;
		head = tail->next->next;
	}
	size--;

	t = n->next->next;
	delete(n->next);
	n->next = t;
}
//очистка списка
void SList::clear()
{
	Node *pn = head, *pr;
/**/
	while( pn != tail )
	{
		pr = pn;
		pn = pn->next;
		delete(pr);
		pr = NULL;
	}
	delete(pn);
	pn = NULL;
/**/
	head = tail = NULL;
	size = 0;
}
#endif


