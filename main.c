/********************************************************
    Project Overview: Using a linked list to create a
    string type with simple functionality using base
    C with stdio and stdlib.

    Purpose: To practice linked lists

    Date Edited: 08/23/2023 - Dylan Morgan
********************************************************/

#include <stdio.h>
#include <stdlib.h>


/****************************************************************
    This is the basic structure for a node. It contains a  single
    char for data and two pointers, one is used to point to the
    next node, and the other points to the previous node. We are
    doing this to create a doubly linked list for easier
    navigation. Helpful for insertion and deletion
****************************************************************/
typedef struct Node {
    char data;
    struct Node* next;
    struct Node* prev;
} Node;

/****************************************************************
    This is the basic structure for our linked list. It contains
    a pointer to the head (first node) and a pointer to the
    tail (last node)
****************************************************************/
typedef struct {
    Node* head;
    Node* tail;
} LinkedList;

/*********************************
    Creating an iterator rather
    than a cursor as it allows
    multiple to traverse
    simultaneously if ever needed
*********************************/
typedef struct {
    Node* current;
} Iterator;

/****************************************************************
    Data structure for a string, it is for now just a linked list
****************************************************************/
typedef struct {
    LinkedList string_list;
} StringLibrary;

/****************************************************************
    This function initializes a linked list to have NULL pointers
****************************************************************/
void InitLinkedList(LinkedList *list)
{
    list->head = NULL;
    list->tail = NULL;
}

/****************************************************************
    This function will append a single character to a linked list
****************************************************************/
void Append (LinkedList *list, char data)
{
    //Allocate memory for a new node
    Node* new_node = (Node *)malloc(sizeof(Node));

    //Error Catching
    if(!new_node)
    {
        fprintf(stderr, "MEMORY ALLOCATION FAILED!\n");
        exit(EXIT_FAILURE);
    }

    //Set the new node's data, then the next and prev pointers to NULL
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    //If the head of the list is null set the new node to be the head and the tail, else
    if(!list->head)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        //The previous node to our new node is set to the current tail, the current tails next is set to our new node, then
        //Our current tail becomes the new node
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
}

/****************************************************************
    Simple function to get an iterator node, gets the head node
****************************************************************/
Iterator GetIterator(StringLibrary* str)
{
    LinkedList* list = &str->string_list;
    Iterator itr;
    itr.current = list->head;
    return itr;
}

/****************************************************************
    This function will insert a given character after the
    FIRST instance of a target character from the itr
****************************************************************/
void InsertAfterCharacter(StringLibrary* str, Iterator* itr, char target, char data)
{
    //Getting the linked list from the str type
    LinkedList* list = &str->string_list;

    //Setting our node pointer to the current itr where we are starting
    Node* current_node = itr->current;

    //As long as current node is not NULL
    while(current_node)
    {
        //Check if the value of our current node matches the target character
        if(current_node->data == target)
        {
            //Start creating a new node for the new character
            Node* new_node = (Node* )malloc(sizeof(Node));

            //Error handling
            if(!new_node)
            {
                fprintf(stderr, "MEMORY ALLOCATION FAILED!\n");
                exit(EXIT_FAILURE);
            }

            //Initializing new node
            new_node->data = data;
            new_node->prev = current_node;
            new_node->next = current_node->next;

            //If the current node is not the tail of our list, we need to set the next node's previous pointer to point to the new node
            if(current_node->next)
            {
                current_node->next->prev = new_node;
            }
            //Add the new node to be the current node's next pointer
            current_node->next = new_node;

            //If the current node is the tail, then the tail needs to be the new node
            if(current_node == list->tail)
            {
                list->tail = new_node;
            }
            //Update iterator position to be the new node
            itr->current = new_node;
            return;
        }

        //If the value of the current node did not match the target, we go to the next node
        current_node = current_node->next;
    }
}

/****************************************************************
    This function will insert a given character after the
    EACH instance of a target character
****************************************************************/
void InsertAfterCharacterRecursive(StringLibrary* str, Node* node, char target, char data)
{
    //If the node is null return
    if(!node)
    {
        return;
    }

    //Grabbing the linked list from the string
    LinkedList* list = &str->string_list;

    //If the node's value matches the target we will create a new node
    if(node->data == target)
    {
        //Allocating memory for a new node
        Node* new_node = (Node* )malloc(sizeof(Node));

        //Error handling
        if(!new_node)
        {
            fprintf(stderr, "MEMORY ALLOCATION FAILED!\n");
            exit(EXIT_FAILURE);
        }

        //Initializing new node
        new_node->data = data;
        new_node->prev = node;
        new_node->next = node->next;

        //If node has a next node, set the next node's previous pointer to the new node
        if(node->next)
        {
            node->next->prev = new_node;
        }

        //Set the current node's next pointer to the new node
        node->next = new_node;

        //If the current node is the tail of the list, we update the tail to be the new node
        if(node == list->tail)
        {
            list->tail = new_node;
        }

        //Call this function for recursion using the node after the new node as the current node now
        InsertAfterCharacterRecursive(str, new_node->next, target, data);
    }
    else
    {   //If the current node's value didn't match, call function again with the next node
        InsertAfterCharacterRecursive(str, node->next, target, data);
    }
}

/****************************************************************
    This function will delete a given character at the
    FIRST instance of said character
****************************************************************/
void DeleteCharacter(StringLibrary* str, char target)
{
    //Grabbing the linked list
    LinkedList* list = &str->string_list;

    //Setting a cursor node to the head
    Node* current_node = list->head;

    //As long as our current_node (cursor) is not null we will keep searching through the list
    while(current_node)
    {
        //If the value at our cursor matches the target we begin deletion
        if(current_node->data == target)
        {
            //If there is a previous node, we set it's next pointer to our current next pointer
            if(current_node->prev)
            {
                current_node->prev->next = current_node->next;
            }
            //If there wasn't then this was the head which we will now set to the next pointer
            else
            {
                list->head = current_node->next;
            }

            //If there is a next node, set it's previous pointer to the current previous pointer
            if(current_node->next)
            {
                current_node->next->prev = current_node->prev;
            }
            //Otherwise this was the tail and we update that to be the previous pointer
            else
            {
                list->tail = current_node->prev;
            }

            //Grabbing the pointer to the next node from the current node before deletion
            Node* next_node = current_node->next;

            //Deallocating the current node
            free(current_node);

            //Setting our current node to be that next node
            current_node = next_node;
            return;
        }
        //If there was not a match, go to the next node.
        current_node = current_node->next;
    }
}

/****************************************************************
    This function will delete ALL instances of a given char
****************************************************************/
void DeleteCharacterRecursive(StringLibrary* str, Node* node, char target)
{
    //If current node is null return
    if(!node)
    {
        return;
    }

    //Grabbing the linked list
    LinkedList* list = &str->string_list;

    //If the value of the current node matches the target, begin deletion
    if(node->data == target)
    {
        //Update pointers so that current node can be deleted, similar to non recursive delete function
        if(node->prev)
        {
            node->prev->next = node->next;
        }
        else
        {
            list->head = node->next;
        }

        if(node->next)
        {
            node->next->prev = node->prev;
        }
        else
        {
            list->tail = node->prev;
        }

        Node *next_node = node->next;
        free(node);

        //After we deallocate this node, call the function again on the next node
        DeleteCharacterRecursive(str, next_node, target);
    }
    else
    {   //If there wasn't a match, call the function again on the next node
        DeleteCharacterRecursive(str, node->next, target);
    }
}

/****************************************************************
    Simple function to print a linked list of chars
****************************************************************/
void Print(LinkedList* list)
{
    //Set a cursor node
    Node* current_node = list->head;

    //Loop through list and print value of each node
    while(current_node)
    {
        printf("%c", current_node->data);
        current_node = current_node->next;
    }

    //After list is printed, print a new line
    printf("\n");
}

/****************************************************************
    Simple function to print a linked list of chars in reverse
****************************************************************/
void PrintReverse(LinkedList* list)
{
    //Set the cursor to the tail
    Node* current_node = list->tail;

    //Loop through list in reverse using prev pointers
    while (current_node) {
        printf("%c", current_node->data);
        current_node = current_node->prev;
    }
    printf("\n");
}

/****************************************************************
    Simple function to append a char* to a string object
****************************************************************/
void AppendString(StringLibrary* str, const char* data)
{
    int i =0;
    while(data[i] != '\0')
    {
        Append(&(str->string_list), data[i]);
        i++;
    }
}

/****************************************************************
    Simple function to create a string object
****************************************************************/
void CreateString(StringLibrary* str, const char* data)
{
    InitLinkedList(&(str->string_list));
    AppendString(str, data);
}

/****************************************************************
    Simple function to print a string
****************************************************************/
void PrintStr(StringLibrary* str)
{
    Print(&(str->string_list));
}

/****************************************************************
    Simple function to print a string in reverse
****************************************************************/
void PrintReverseStr(StringLibrary* str)
{
    PrintReverse(&(str->string_list));
}

/****************************************************************
    Function will deallocate used memory from a string
****************************************************************/
void Deallocate(StringLibrary* str)
{
    LinkedList* list = &str->string_list;
    Node* current_node = list->head;
    while(current_node)
    {
        Node* temp = current_node;
        current_node = current_node->next;
        free(temp);
    }
    list->tail = NULL;
}



int main()
{
    //Initialize a string object called str
    StringLibrary str;

    //Create a string using str and set it to Hello
    CreateString(&str, "Hello");

    //Append World! to our str
    AppendString(&str, "World!");

    //Display the string
    printf("Original String: ");
    PrintStr(&str);

    //Display the string in reverse!
    printf("Reversed String: ");
    PrintReverseStr(&str);

    //Create an iterator used for insertion
    Iterator itr = GetIterator(&str);

    //Insert a space after the first o
    InsertAfterCharacter(&str, &itr, 'o', ' ');
    printf("After Insertion: ");
    PrintStr(&str);

    //Insert an X after each l
    InsertAfterCharacterRecursive(&str, str.string_list.head, 'l', 'X');
    printf("After Recursive Insertion: ");
    PrintStr(&str);

    //Delete the first space
    DeleteCharacter(&str, ' ');
    printf("After Deletion: ");
    PrintStr(&str);

    //Delete each X
    DeleteCharacterRecursive(&str, str.string_list.head, 'X');
    printf("After Recursive Deletion: ");
    PrintStr(&str);

    //Delete the string
    Deallocate(&str);
    return 0;
}
