// Chris Delaney
// coding assignment 1
// mergesort

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

struct node {
	int data;
	struct node* next;
};

// This function prints out the linked list
void print_list(struct node* testnode) {
	struct node* curr = testnode;
	while(curr != NULL) {
		printf("%d\n", curr->data);
		curr = curr->next;
	}
}

// This function adds a node to the front of the linked list
void add_front(struct node** head, int num) {
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->data = num;
	node->next = *head;
	*head = node;
}

// This function adds a node to the back of a linked list
void add_back(struct node** head, int num) {
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->next = NULL;
	node->data = num;
	if(*head) {
		struct node* p = *head;
		while(p->next) {
			p = p->next;
		}
		p->next = node;
	} else {
		*head = node;
	}
}

// This function merges two linked lists and returns a pointer to
// the merged list
struct node* merge(struct node* head1, struct node* head2) {
	struct node* newhead = NULL;
	if(head1 == NULL) {
		return head2;
	}
	if(head2 == NULL) {
		return head1;
	}
	while(head1 != NULL && head2 != NULL) {
		if(head1->data <= head2->data) {
			add_back(&newhead,head1->data);
			head1 = head1->next;
		} else {
			add_back(&newhead,head2->data);
			head2 = head2->next;
		}
	}
	if(head1 == NULL) {
		while(head2 != NULL) {
			add_back(&newhead,head2->data);
			head2 = head2->next;
		}
	} else {
		while(head1 != NULL) {
			add_back(&newhead,head1->data);
			head1 = head1->next;
		}
	}
	return newhead;
}

// This function returns the size of a linked list
int list_size(struct node* head) {
	int i = 0;
	while (head) {
		i++;
		head = head->next;
	}
	return i;
}

// This function splits a linked list into two halves over and over thru
// recursion until the list is composed of individual pieces and then 
// merges the individual pieces into sorted order. It then returns a 
// pointer to the final sorted linked list
struct node* mergesort(struct node* head, int size) {
	if(size<2) {
		return head;
	}
		struct node* head1 = NULL;
		struct node* head2 = NULL;
		int size1 = size / 2;
		int size2 = size - size1;
		int i = 0;
		int j = 0;
		if(size1>=1 || size2>=1) {
			while(i<size1) {
				add_back(&head1,head->data);
				head = head->next;
				i++;
			}
			while(j<size2) {
				add_back(&head2,head->data);
				head = head->next;
				j++;
			}
		}
		return merge(mergesort(head1,size1),mergesort(head2,size2));
}

// main function to test mergesort
int main() {
	struct node* testernode = NULL;
	struct node* testernode2 = NULL;
	add_front(&testernode,5);
	add_front(&testernode,9);
	add_front(&testernode,13);
	add_front(&testernode,11);
	add_front(&testernode2,23);
	add_front(&testernode2,4);
	add_front(&testernode2,18);
	struct node* newhead = merge(testernode,testernode2);
	printf("The merged list is: ");
	printf("\n");
	print_list(newhead);
	printf("\n");
	struct node * finalhead = mergesort(newhead,list_size(newhead));
	printf("The final sorted list is: ");
	printf("\n");
	print_list(finalhead);
	return 0;
}
