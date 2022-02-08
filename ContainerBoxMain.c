// Header files
#include <stdio.h>
#include <string.h>
#include "ContainerBoxOps.h"
#include "InsideBoxTools.h"

// constant
const int FIRST_ARGUMENT = 1;
const bool FORCE_DISPLAY = true;

int main( int argc, char *argv[] )
   {
    // initialize function/variables

       // Data Set 1 - simple 10 x 10
/*       int heightList[] = { 4, 6, 4, 3 };
       int widthList[]  = { 4, 7, 6, 6 };
       int containerBoxHeight = 10,
           containerBoxWidth  = 10,
           numBoxes           = 4;
*/
       // Data Set 2 - little long
       int heightList[] = { 3, 4, 4, 4, 2, 2, 5, 7, 5, 5 };
       int widthList[] =  { 3, 3, 2, 5, 3, 4, 7, 6, 5, 3 };
       int containerBoxHeight = 15,
           containerBoxWidth  = 12,
           numBoxes           = 10;

       // Data Set 3 - long one
/*       int heightList[] = { 4, 2, 2, 5, 7, 5, 5, 3, 4, 4 };
       int widthList[] =  { 5, 3, 4, 7, 6, 5, 3, 3, 3, 2 };
       int containerBoxHeight = 15,
           containerBoxWidth  = 12,
           numBoxes           = 10;
*/
       // Data Set 4 - really long
/*       int heightList[] = { 7, 5, 5, 3, 4, 4, 4, 2, 2, 5 };
       int widthList[] =  { 6, 5, 3, 3, 3, 2, 5, 3, 4, 7 };
       int containerBoxHeight = 15,
           containerBoxWidth  = 12,
           numBoxes           = 10;
*/
    ContainerBoxData containerBox;
    int index;
    char nextLetter = 'A';
    bool displayFlag = false;

    if( argc > 1 && strcmp( argv[ FIRST_ARGUMENT ], "-verbose" ) == 0 )
       {
        displayFlag = true;
       }

    // initialize container box (limits)
    containerBox.height = containerBoxHeight;
    containerBox.width = containerBoxWidth;

    // initialize container box (field & box list)
    initializeContainerBox( &containerBox, numBoxes );

    if( displayFlag )
       {
        printf( "Loading Boxes:\n" );
       }

    // set up inside boxes
    for( index = 0; index < numBoxes; index++, nextLetter++ )
       {
        containerBox.insideBoxList[ index ].idLetter = nextLetter;
        containerBox.insideBoxList[ index ].height = heightList[ index ];
        containerBox.insideBoxList[ index ].width = widthList[ index ];
        containerBox.insideBoxList[ index ].usedState = false;

        if( displayFlag )
           {
            printf( "Letter: %c", containerBox.insideBoxList[ index ].idLetter );
            printf( ", height: %d", containerBox.insideBoxList[ index ].height );
            printf( ", width: %d\n", containerBox.insideBoxList[ index ].width );
           }
       }

    // call to fill container box, test for success
    if( fillContainerBox( &containerBox, displayFlag ) )
       {
        printf( "\nPacking solution found.\n" );
       }

    else
       {
        printf( "\nPacking solution not found.\n" );
       }

    // display resulting field
    displayField( &containerBox, FORCE_DISPLAY );

    // end program

       // clear memory used by field array
          // function: clear2DArray
       clearContainerBox( &containerBox );

       // print end statement
          // function: printf
       printf( "\nProgram End\n" );

    return 0;
   }
