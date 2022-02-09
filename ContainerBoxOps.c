// Header files
#include "ContainerBoxOps.h"

// constant definitions
const char DEFAULT_FIELD_CHAR = '-';
const int NO_BOXES_AVAILABLE = -1;
const int FILL_BOX = 102;  // used to control fillBoxLocation function
const int CLEAR_BOX = 103;  // used to control fillBoxLocation function
const int MAX_NUM_BOXES = 26;  // number of letters in alphabet

// function implementations

bool checkForFitInField( ContainerBoxData *containerBox,
                                 PointData testLocation, InsideBoxData testBox )
   {
    int rowIndex, colIndex;
    int heightLimit = testLocation.pos_Y - testBox.height;
    int widthLimit = testLocation.pos_X + testBox.width;
    int aboveBox = -1;

    if( heightLimit < aboveBox || widthLimit > containerBox->width )
       {
        return false;
       }

    for( rowIndex = testLocation.pos_Y; rowIndex > heightLimit; rowIndex-- )
       {
        for( colIndex = testLocation.pos_X; colIndex < widthLimit; colIndex++ )
           {
            if( containerBox->field[ rowIndex ][ colIndex ]
                                                         != DEFAULT_FIELD_CHAR )
               {
                return false;
               }
           }
       }

    return true;
   }

void clearContainerBox( ContainerBoxData *containerBox )
   {
    int rowIndex;

    for( rowIndex = 0; rowIndex < containerBox->height; rowIndex++ )
       {
        free( containerBox->field[ rowIndex ] );
       }

    containerBox->field = NULL;

    containerBox->width = containerBox->height = containerBox->numBoxes = 0;

    free( containerBox->insideBoxList );

    containerBox->insideBoxList = NULL;
   }

char **create2DArray( int maxRows, int maxCols )
   {
    int rowIndex;

    char **array = (char **)malloc( maxRows * sizeof( char * ) );

    for( rowIndex = 0; rowIndex < maxRows; rowIndex++ )
       {
        array[ rowIndex ] = (char *)malloc( maxCols * sizeof( char ) );
       }

    return array;
   }

void displayField( ContainerBoxData *containerBox, bool displayFlag )
   {
    int rowIndex, colIndex;

    if( displayFlag )
       {
        printf( "\n" );

        for( colIndex = 0; colIndex < containerBox->width + 2; colIndex++ )
           {
            printf( "%c", '=' );
           }

        printf( "\n" );

        for( rowIndex = 0; rowIndex < containerBox->height; rowIndex++ )
           {
            printf( "|" );

            for( colIndex = 0; colIndex < containerBox->width; colIndex++ )
               {
                printf( "%c", containerBox->field[ rowIndex ][ colIndex ] );
               }

            printf( "|\n" );
           }

        for( colIndex = 0; colIndex < containerBox->width + 2; colIndex++ )
           {
            printf( "%c", '=' );
           }

        printf( "\n" );
       }

    //sleep( 1 );  // pauses program for number of seconds
   }

void fillBoxLocation( ContainerBoxData *containerBox, PointData boxLocation,
                                         InsideBoxData fillBox, int clearFlag )
   {
    int rowIndex, colIndex;
    int heightLimit = boxLocation.pos_Y - fillBox.height;
    int widthLimit = boxLocation.pos_X + fillBox.width;
    char fillChar = fillBox.idLetter;

    if( clearFlag == CLEAR_BOX )
       {
        fillChar = DEFAULT_FIELD_CHAR;
       }

    for( rowIndex = boxLocation.pos_Y; rowIndex > heightLimit; rowIndex-- )
       {
        for( colIndex = boxLocation.pos_X; colIndex < widthLimit; colIndex++ )
           {
            containerBox->field[ rowIndex ][ colIndex ] = fillChar;
           }
       }
   }

bool fillContainerBox( ContainerBoxData *containerBox, bool displayFlag )
   {
    // initialize constant
    int ROTATION_COUNTER = 2;

    // initialize variables
    int currentBoxIndex;
    int startAtIndex = 0;
    int index;
    InsideBoxData currentBox;
    PointData currentLocation;
    bool locationFlag;
    char idLetterChar;
    bool nextLevelDownFlag;

    // determine current location
    locationFlag = findNextOpenLocation( containerBox, &currentLocation );

    // get box index
    currentBoxIndex = findNextUnusedBoxIndex( containerBox, startAtIndex );

    if ( !locationFlag && currentBoxIndex == NO_BOXES_AVAILABLE )
       {
        return true;
       }

    // start a loop for when no boxes found
    while ( currentBoxIndex != NO_BOXES_AVAILABLE )
       {
        // get current box
        currentBox = containerBox->insideBoxList[ currentBoxIndex ];

        // loop twice for rotation if needed
        for ( index = 0; index < ROTATION_COUNTER; index++ )
           {
            // get current box?????????
            currentBox = containerBox->insideBoxList[ currentBoxIndex ];

            // if the current box does fit
            if ( checkForFitInField( containerBox, currentLocation, currentBox) )
               {
                // paint the box
                fillBoxLocation( containerBox, currentLocation, containerBox->insideBoxList[ currentBoxIndex ], FILL_BOX);

                startAtIndex++;

                // set the box as used
                containerBox->insideBoxList[ currentBoxIndex ].usedState = true;

                // display the field with the new box in place
                displayField( containerBox, displayFlag );

                // determine the success of the next level down
                nextLevelDownFlag = fillContainerBox( containerBox, displayFlag );

                // if the next level down is successful, return true on this level
                if ( nextLevelDownFlag )
                   {
                    // return true
                    return true;
                   }

                // if next level down returns false
                // unset the current box used
                containerBox->insideBoxList[ currentBoxIndex ].usedState = false;

                // clear the current box back to the default character
                fillBoxLocation( containerBox, currentLocation, containerBox->insideBoxList[ currentBoxIndex ], CLEAR_BOX);

                startAtIndex--;

                // display the field with the box removed
                displayField( containerBox, displayFlag );
               }

            else if ( index == 0 )
               {
                idLetterChar = currentBox.idLetter;

                // rotate box ( not in the if statement for box fit )
                rotate( &containerBox->insideBoxList[ currentBoxIndex ] );

                if ( displayFlag )
                   {
                    printf("\nBox %c first attempt failed, rotating\n", idLetterChar);
                   }
               }

            else if ( index == 1 && displayFlag )
               {
                idLetterChar = currentBox.idLetter;

                printf("\nBox %c second attempt failed, trying another box\n", idLetterChar);
               }
           }

        // increment start at index
        startAtIndex++;

        // get new box index
        currentBoxIndex = findNextUnusedBoxIndex( containerBox, startAtIndex );
       }

    if ( displayFlag )
       {
        // print the backtracking statement
        printf("\nFailed at this location, backtracking\n");
       }

    // return false when all boxes and orientations are exhausted at that position
    return false;
   }

bool findNextOpenLocation( const ContainerBoxData *containerBox,
                                                        PointData *returnPoint )
   {
    int rowIndex, colIndex;

    for( rowIndex = containerBox->height - 1; rowIndex >= 0; rowIndex-- )
       {
        for( colIndex = 0; colIndex < containerBox->width; colIndex++ )
           {
            if( containerBox->field[ rowIndex ][ colIndex ]
                                                         == DEFAULT_FIELD_CHAR )
               {
                returnPoint->pos_X = colIndex;
                returnPoint->pos_Y = rowIndex;

                return true;
               }
           }
       }

    return false;
   }

int findNextUnusedBoxIndex( ContainerBoxData *containerBox, int startAtIndex )
   {
    while( startAtIndex < containerBox->numBoxes )
       {
        if( !containerBox->insideBoxList[ startAtIndex ].usedState )
           {
            return startAtIndex;
           }

        startAtIndex++;
       }

    return NO_BOXES_AVAILABLE;
   }

void initializeContainerBox( ContainerBoxData *containerBox, int numBoxes )
   {
    int rowIndex, colIndex;

    containerBox->field = create2DArray( containerBox->height,
                                                          containerBox->width );

    for( rowIndex = 0; rowIndex < containerBox->height; rowIndex++ )
       {
        for( colIndex = 0; colIndex < containerBox->width; colIndex++ )
           {
            containerBox->field[ rowIndex ][ colIndex ] = DEFAULT_FIELD_CHAR;
           }
       }

    containerBox->insideBoxList
                = (InsideBoxData *)malloc( numBoxes * sizeof( InsideBoxData ) );

    containerBox->numBoxes = numBoxes;
   }

void rotate( InsideBoxData *box )
   {
    int tempWidth = box->width;

    box->width = box->height;

    box->height = tempWidth;
   }
