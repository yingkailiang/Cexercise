/* write add and remove func for linked list each node
   has one int data and a pointer to next node.
   the function are add(int data) and remove (int data)
   and use the heap to create/delete nodes
   (dynamic create node like : linklist *str = malloc(sizeof(linklist)*10) )
*/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

/*
 linked list
 @parm int data
 @parm point to next node
*/
typedef struct linklist {
  int num;
  struct linklist *next;  
}linklist;

/*
  dynamic create link node
  @parm struct linklist point header
  @parm int position :start from 0
  @parm int value 
*/
int addNode(linklist *head,int position,int value)
{ linklist *ptr =head,*preptr=head,newptr ={0,NULL};
  //if not exist,become first node
  if (ptr ==NULL)
  {
    newptr.num =value; 
    newptr.next =NULL;   
    return 0;
  } 
  
  int i=0;
  assert(position >=0 );
  preptr=ptr;
  while(i<position && ptr->next !=NULL) {
  preptr=ptr;
  ptr = ptr -> next;
  i++;
  }
  assert(ptr!=NULL && i==position );
  newptr.next = preptr->next;
  assert(newptr.next == preptr->next);
  preptr->next =&newptr;
  newptr.num = value;
  return 1;
} //end func addNode
/*
  dynamic delete node from linklist tail
  @parm linklist head point
  @parm int position
*/
void delNode(linklist *head,int position)
{
  linklist *ptr =head,*preptr=head;
  if(ptr ==NULL)
  {
    printf("can not delete empty linklist");
  } 
  int i=0;
  assert(position>=0);
  preptr=ptr;
  while(i<position)
  {
   preptr = ptr;
   ptr= ptr->next;   
   i++;
  }
  
  preptr->next =preptr->next->next;
} //end func delNode
/*
 read all num from linklist
 @parm point of linklist header
*/
void readList(linklist *head)
{
  linklist *ptr =head;
  if(ptr ==NULL)
  {
   printf("There is no node in list");
  } 
  while(ptr !=NULL)
  { 
   printf("%d ",ptr->num); 
   ptr = ptr->next;
  }
  printf("\n");
} //end func readList

/*
test add node and delete node
*/
void main()
{
  linklist *str =malloc(sizeof(linklist)*10) ;

   int i =0;
   while(i<10)
   {
    str[i].num=i;
    if(i<9)
    str[i].next=&str[i+1];
    i++;
   } 
   
   str[i-1].next=NULL;
   readList(str);
   addNode(str,2,9999);
   readList(str);
   delNode(str,2); 
   readList(str);
   free(str);
}//end main 
