#include <stdio.h>
#include <string.h>
#include <stdlib.h>

enum comands {BASE, PUSH, ADD, SUB, MULT, DIV, BRANCH, PRINT, JUMP, GT, GTE, NOT};
enum truth {TRUE, FALSE};

typedef struct comand_stack_item {
	char * comand_text;
	int comand_id;
	int comand_value;
	int index;
	struct comand_stack_item * next_comand;	
} comand_stack_item;

typedef struct stack_item {
	int value;		
	int base;		
	struct stack_item* next_item;
} stack_item;

void print_comand_stack (comand_stack_item * csi) {
	printf("%s", csi ->comand_text); 
	if (csi ->next_comand) {
		print_comand_stack(csi -> next_comand);
	}
}

void print_stack (stack_item * si) {
	printf("%d\n", si->value);
	if (si -> next_item) {
		print_stack(si ->next_item);
	}
}

comand_stack_item * parse_input(char * input_file) {
	FILE * f = fopen(input_file, "r");
	if (!f) {
		printf("Invalid file name.\n");
		exit(0);	
	}

	comand_stack_item * new_stack = malloc(sizeof(comand_stack_item));
	new_stack->comand_text = malloc(sizeof(char) * 4);
	strcpy(new_stack->comand_text, "END");
	new_stack->comand_text[3] = '\n';
	new_stack -> comand_id = BASE;
	new_stack -> next_comand = NULL ;
	new_stack -> index = 0;

	comand_stack_item * prev = new_stack;
	comand_stack_item * new = new_stack;
	int count = 1;

	char * line = NULL;
	unsigned long size = 0;
	int nread;

	while ((nread = getline(&line, &size, f)) != -1) {
		new = malloc(sizeof(comand_stack_item));
		//write onto the stack
		new->comand_text = malloc(sizeof(char) * (size + 1));
		strcpy(new->comand_text, line);
		new->comand_text[size] = '\n';
		new -> index = count;
		
		//parse to get type and value 
		char * string = line;
		char * val = strstr(line, ".");
		int string_length = strlen(line) - strlen(val);

		//enum comands {BASE, PUSH, ADD, SUB, MULT, DIV, BRANCH, PRINT};
		if (strncmp(line, "PUSH", string_length) == 0) {
			new->comand_id = PUSH;
			new->comand_value = atoi(val + 1);
		}
		
		if (strncmp(line, "GT", string_length) == 0) {
			new->comand_id = GT;
		}
		
		if (strncmp(line, "NOT", string_length) == 0) {
			new->comand_id = NOT;
		}

		if (strncmp(line, "GTE", string_length) == 0) {
			new->comand_id = GTE;
		}
		
		if (strncmp(line, "ADD", string_length) == 0) {
			new->comand_id = ADD;
		}

		if (strncmp(line, "SUB", string_length) == 0) {
			new->comand_id = SUB;
		}
		
		if (strncmp(line, "MULT", string_length) == 0) {
			new->comand_id = MULT;
		}

		if (strncmp(line, "DIV", string_length) == 0) {
			new->comand_id = DIV;
		}
		
		if (strncmp(line, "BRANCH", string_length) == 0) {
			new->comand_id = BRANCH;
			new->comand_value = atoi(val + 1);
		}
		
		if (strncmp(line, "JUMP", string_length) == 0) {
			new->comand_id = JUMP;
			new->comand_value = atoi(val + 1);
		}
		
		if (strncmp(line, "PRINT", string_length) == 0) {
			new->comand_id = PRINT;
		}
		
		new-> comand_id = BASE;
		new-> next_comand = prev;

		prev = new;
		count ++;
	}

	fclose(f);
	return new;

}

//enum comands {BASE, PUSH, ADD, SUB, MULT, DIV, BRANCH, PRINT, JUMP};
void interpret(comand_stack_item * comands) {
	comand_stack_item * comand_stack_head = comands;
	stack_item * stack = malloc(sizeof(stack_item));	
	stack->base = TRUE;

	stack_item * new;
	int val1, val2;
	stack_item * delete;
	int target_index;

	while (1) {
		//go through the stack!
		switch (comands->comand_id) {
			case(BASE):
				printf("Done Evaluation.\n");
				exit(0);
				break;
			case(PUSH):
				new = malloc(sizeof(stack_item));
				new ->value = comands ->comand_value;
				new -> next_item = stack;
				new -> base = 0;
				stack = new;
				comands = comands->next_comand;
				break;
			case(GTE):
				if (stack->base == 1) {
					printf("Invalid stack for gte operation.\n");
					exit(0);
				}
				val1 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);
				if (stack->base == 1) {
					printf("Invalid stack for gte operation.\n");
					exit(0);
				}
				val2 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);

				new = malloc(sizeof(stack_item));
				new ->value = val1>=val2;
				new -> next_item = stack;
				new -> base = 0;
				stack = new;
				comands = comands->next_comand;
				break;
			case(GT):
				if (stack->base == 1) {
					printf("Invalid stack for gt operation.\n");
					exit(0);
				}
				val1 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);
				if (stack->base == 1) {
					printf("Invalid stack for gt operation.\n");
					exit(0);
				}
				val2 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);

				new = malloc(sizeof(stack_item));
				new ->value = val1>val2;
				new -> next_item = stack;
				new -> base = 0;
				stack = new;
				comands = comands->next_comand;
				break;
			case(ADD):
				if (stack->base == 1) {
					printf("Invalid stack for add operation.\n");
					exit(0);
				}
				val1 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);
				if (stack->base == 1) {
					printf("Invalid stack for add operation.\n");
					exit(0);
				}
				val2 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);

				new = malloc(sizeof(stack_item));
				new ->value = val1+val2;
				new -> next_item = stack;
				new -> base = 0;
				stack = new;
				comands = comands->next_comand;
				break;
			case(SUB):
				if (stack->base == 1) {
					printf("Invalid stack for sub operation.\n");
					exit(0);
				}
				val1 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);
				if (stack->base == 1) {
					printf("Invalid stack for sub operation.\n");
					exit(0);
				}
				val2 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);

				new = malloc(sizeof(stack_item));
				new ->value = val1-val2;
				new -> next_item = stack;
				new -> base = 0;
				stack = new;
				comands = comands->next_comand;
				break;
			case (MULT):
				if (stack->base == 1) {
					printf("Invalid stack for mult operation.\n");
					exit(0);
				}
				val1 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);
				if (stack->base == 1) {
					printf("Invalid stack for mult operation.\n");
					exit(0);
				}
				val2 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);

				new = malloc(sizeof(stack_item));
				new ->value = val1*val2;
				new -> next_item = stack;
				new -> base = 0;
				stack = new;
				comands = comands->next_comand;
				break;
			case (DIV):
				if (stack->base == 1) {
					printf("Invalid stack for div operation.\n");
					exit(0);
				}
				val1 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);
				if (stack->base == 1) {
					printf("Invalid stack for div operation.\n");
					exit(0);
				}
				val2 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);

				new = malloc(sizeof(stack_item));
				new ->value = val1/val2;
				new -> next_item = stack;
				new -> base = 0;
				stack = new;
				comands = comands->next_comand;
				break;
			case (NOT):
				if (stack->base == 1) {
					printf("Invalid stack for not operation.\n");
					exit(0);
				}
				val1 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);

				new = malloc(sizeof(stack_item));
				new ->value = !val1;
				new -> next_item = stack;
				new -> base = 0;
				stack = new;
				comands = comands->next_comand;
				break;
			case (BRANCH):
				if (stack->base == 1) {
					printf("Invalid stack for branch operation.\n");
					exit(0);
				}
				val1 = stack->value;
				delete = stack;	
				stack = stack -> next_item;
				free(delete);

				if (val1 >= 1) {
					target_index = comands->index + comands->comand_value;
					comand_stack_item * comand_replacement = comand_stack_head;
					while (comand_replacement) {
						if (comand_replacement->index == target_index) {
							break;
						}
						comand_replacement = comand_replacement->next_comand;
					}
					if (!comand_replacement) {
						printf("Invalid branch.\n");
					}
					comands = comand_replacement;
				} else {
					comands = comands->next_comand;
				}	
				break;
			case (PRINT):
				printf("%d\n", stack->value);
				comands = comands -> next_comand;	
				break;
			case (JUMP):
				target_index = comands->index + comands->comand_value;
				comand_stack_item * comand_replacement = comand_stack_head;
				while (comand_replacement) {
					if (comand_replacement->index == target_index) {
						break;
					}
					comand_replacement = comand_replacement->next_comand;
				}
				if (!comand_replacement) {
					printf("Invalid jump.\n");
				}
				comands = comand_replacement;
				break;
			default:
				printf("Invalid Comand: %s\n", comands->comand_text);
				exit(0);
		}
		
	}

}

int main() {
	comand_stack_item * p = parse_input("./input");
	printf("Stack input:\n");
	print_comand_stack(p);
	printf("Starting virtual machine:\n");
	interpret(p);

}
