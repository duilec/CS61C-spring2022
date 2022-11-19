#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head) {
    if (head == NULL) {
		return 0;
	}
	node* fast_ptr = head;
    node* slow_ptr = head;
	while(1){
		// we should use 'next', NOT index in array
		fast_ptr = fast_ptr->next->next;
		slow_ptr = slow_ptr->next;
		// note: In gdb, address of '0x0' equals '[34m0x0[m'	
		if (fast_ptr == NULL || fast_ptr->next == NULL) { // or !fast_ptr || !fast_ptr->next
			return 0;
		}
		if (fast_ptr == slow_ptr) {
			return 1;
		}
	}
	return 0;
}
