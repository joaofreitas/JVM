/*
 * methodArea.c
 *
 *  Created on: 22/01/2011
 *      Author: joaofreitas
 */

#include "methodArea.h"

void addClass(class *class)
{
	method_area *m_as;
	u2 index = method_area_end->index;

	if (method_area_pointer == NULL)
	{
		method_area_pointer = malloc(sizeof(method_area));
		method_area_pointer->index = 0;
		method_area_pointer->class = class;
		method_area_pointer->next = NULL;
		method_area_end = method_area_pointer;
	}
	else
	{
		m_as = malloc(sizeof(method_area));
		m_as->index = index+1;
		m_as->class = class;
		m_as->next = NULL;
		method_area_end->next = m_as;
		method_area_end = m_as;
	}
}

class *getClass(u2 index) {
	class *c;

	/*TODO*/
}
