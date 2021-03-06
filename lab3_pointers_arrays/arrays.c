//-----------------------------------------------------------------
/*
few words about arrays in C:
___________________________________________________________
1) start with index 0, first member has zero index

2) the size of a static array must be known at compile time

3) array definition:

type name[size];
example:
int my_array[5];

4) in **ANSI C** size is fixed and known at compile time, variable can not be used:

int X;
double pole[X]; //not possible in ANSI C, possible since C99
if supported this is called VLA - Variable Length Arrays and a few rules apply:
VLA:
-can not be global
-can not be used in structures
-sizeof() of VLA is runtime evaluated

5)initialization and accessing static array members:
double array[3]={1.0,5.5,6.5};
array[0]=2.0;

6)!!!!!!Arrays can not be manipulated as a whole block (zeroed, compared, assigned )!!!!!!
you always have to use a per index algorithm for such things

7)arrays and sizeof():
for static arrays sizeof() return !!whole array size!!

napr:

short array[5]
int size=sizeof(array) // size = 5 x sizeof(short) !!!however this does not work for array as function parameter!!! 

8)passing arrays to functions
- array is never passed by value (never copied into functions local copy) not even static one
- always passed by reference which makes sizeof operator unable to determine actual external(to a function) array sizes
	- sizeof() return just size of the pointer not the actual array size inside the function
- array dimensions are not passed as well (not even if array static) 
- dimensions must be passed as separate parameters
 
9) multidimensional static arrays are arranged as a single block of memory => array[1][0] is the same member as array[0][3] 


*/

//-----------------------------------------------------------------
//Multidimensional array variants
//-----------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
//we will go through this later
void print_array(int array[][3],int dim);
void print_array_different(int **array,int I,int J);

int main(void)
{


/*
Variant 1.
**static two-dimensional array**


- per rows as a continous memory block 
- p1 is a constant pointer, (remains the same till the program end)
*/
 int p1[2][3];//usage of this array is standard, just as one would expect
 int i,j,k=0;
 for(i=0;i<2;i++)
  for(j=0;j<3;j++)
     p1[i][j]=k++;//fill the array 0-5
 puts("Array print begin (p1)");
 print_array(p1,2);
 puts("Array print end (p1)");
 // p1 is a pointer to  a two-dimensional array of integers having 3 members per row 
 // p1[i] is a pointer to i-th row
 // *(p1+i) is the same as p1[i]
		// its an address of i-th row
		// or a pointer to an i-th row
 // p1[i][j] is an element with index [i][j], of type int of course

//Just out of curiosity
puts("Just out of curiosity:\n");
printf("int takes sizeof(int)=%d memory space\n",(int)sizeof(int));
printf("p1=    %p address of the array begin, first row address\n", p1); 
printf("p1+1=  %p address of second row\n", (p1+1));  //notice the pointer arithmetic
printf("*p1=   %p the same addres as previous, but now its an address of first element of type int.\n", *p1);  
printf("*p1+1= %p address of second element in the first row\n", (*p1+1)); // pointer arithmetic once again
printf("*(*p1+1)= %d second element of the first row (its the integer)\n",(*(*p1+1)));


//print_array_different(p1,2,3);// would generate a warning and wont work properly since it expects a different data structure than the one used in p1
                      // p1 is a two-dimensional array with two rows and three columns

/*
Variant 2.
** one-dimensional array of two pointers  **
(both pointing to one row)

*/
 int *p2[2]; // static array of two pointers to int
//let the pointers point to something useful, like an newly allocated memory
 p2[0] = (int *) malloc(3*sizeof(int)); //memory taken form heap long enough to store three integers (one row)
if (!p2[0]){printf("Memory allocation error!"); return 1;} //every allocation has to be tested for success
 int* useless = (int*) malloc(1000); //useless dynamic/heap allocation just to show/assure our two useful memory blocks(rows) will have nothing in common = will not be aligned anyhow
                                     
 p2[1] = (int *) malloc(3*sizeof(int));//same allocation as before for the second row
if (!p2[0]){printf("Memory allocation error!"); return 1;}


//now we can work with our new memory and treat it like an array
  k=0;
  for(i=0;i<2;i++) 
   for(j=0;j<3;j++) 
     p2[i][j]=k++;
//ATTENTION: p2[0][3] is not the same element as p2[1][0] any more, our two dynamically allocated rows are two completely separate pieces of memory and are not aligned anyhow

// print_array(p2,2); //would generate a warning and wont work correctly
                //print_array() awaits a pointer to an array of three integers... not an array of two pointer to int 

puts("\nArray print begin print_array_different(p2,2,3)");
print_array_different(p2,2,3);//works OK, p2 is a pointer to pointer to int which is OK 
puts("Array print p2 end");

puts("\n\nJust out of curiosity p2:");
printf("pointer to int takes sizeof(int*)=%d of memory space. \n",(int)sizeof(int*));
printf("p2=     %p first element address (element is of type pointer to int)\n",p2);
printf("p2+1=   %p second element address --------||-----------\n",p2+1);
puts("Lets see where those poiners point...");
printf("*p2=    %p address of the first element(row) of three integers array.\n",(void *)*p2);
printf("*(p2+1)=%p and an address of second row,...NOTE the difference \n",(void *)*(p2+1));



/******************
Variant 3.
** pointer to an array**
*/
 int (*p3)[3]; //p3 is not an array, but a single pointer to an array of three integers
		// for now there is only enough memory to store one pointer
 p3 = (int (*)[3]) malloc(2*3*sizeof(int)); // let us allocate a continuous block of 6 integers and typecast the result to "pointer to 3 x int"
if (!p3){printf("Memory allocation error!"); return 1;}

 k=0;                                       
// now p3 can be used as a two-dimensional array as expected, moreover it is continuous
// p3[0][3] is the same element as p3[1][0]
 for(i=0;i<2;i++)
  for(j=0;j<3;j++)
     p3[i][j]=k++;
    //when it commes to memory arrangement p3 looks the same as p1 it is a continuous block of 6 integers, so it is safe to call 

puts("Array print begin print_array(p3,2)");
print_array(p3,2);
puts("Array print end.");


/****************
Variant 4.
 pointer to a pointer
*/
 int** p4; //p4 is a pointer to a pointer to int
 p4 = (int**) malloc( 2*sizeof(int*)); // p4 now points to a first element of array of two pointers to integers 
 //so we can start using the elements
 p4[0]=(int*) malloc(3*sizeof(int));
 p4[1]=(int*) malloc(3*sizeof(int)); //both elements now point to array of three integers

 //now p4 behaves pretty much the same as p2
 k=0;
 for(i=0;i<2;i++)
  for(j=0;j<3;j++)
     p4[i][j]=k++;
puts("Array print begin print_array_different(p4,2,3)");
print_array_different(p4,2,3);
puts("Array print end");

}


/*
print_array function prints the array, compiler knows the row legth 
row count has to be passed as a parameter
*/
void print_array(int array[][3],int dim) //ATTENTION first dimension is always lost, and has to be passed separately
{ 
 int i,j;
 for (i=0;i<dim;i++)
  for(j=0;j<3;j++)
    {
    printf("%d ",array[i][j]);
    puts("");
    }
}

/*
print_array_different function gets only a generic pointer to pointer to int
knowing the dimensions (passed separately) allows us to use that pointer as a two-dimensional array
*/
void print_array_different(int **array,int I,int J)
 {
  int i,j;
 for (i=0;i<I;i++)
  for(j=0;j<J;j++)
    {
    printf("%d ",array[i][j]);
    puts("");
    }
 }

/*EXERCISE:
1) There are some allocations in the program not being tested for success, correct it.
2) All the dynamically allocated memory should be freed, correct it as well.
*/
