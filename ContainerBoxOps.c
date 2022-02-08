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
    // initialize variables
    PointData currentLocation;
    InsideBoxData unusedBox;
    bool locationFlag;
    bool fitFlag;
    bool boxFillFlag = false;
    int unusedBoxIndex = 0;
    int startAtIndex = 0;
    int unusedBoxIndexIterator;
    char idLetterChar;

    locationFlag = findNextOpenLocation( containerBox, &currentLocation );
    unusedBoxIndex = findNextUnusedBoxIndex( containerBox, startAtIndex );
    unusedBoxIndexIterator = unusedBoxIndex;

    if ( unusedBoxIndex == NO_BOXES_AVAILABLE )
       {
        return true;
       }

    while ( boxFillFlag == false && unusedBoxIndexIterator < containerBox->numBoxes)
       {
        // get the unused box
        unusedBox = containerBox->insideBoxList[unusedBoxIndexIterator];

        // fit check
        fitFlag = checkForFitInField( containerBox, currentLocation, unusedBox);

        if ( containerBox->insideBoxList[unusedBoxIndexIterator].usedState == true )
           {
            fitFlag = false;
           }

        // if flag from checkForFitInField is true
        if ( fitFlag == true && locationFlag == true)
           {
            //  place the box in the valid location
            fillBoxLocation( containerBox, currentLocation, unusedBox, FILL_BOX );
            boxFillFlag = true;

            displayField( containerBox, displayFlag );

            // set used flag to true
            containerBox->insideBoxList[unusedBoxIndexIterator].usedState = true;

            fillContainerBox( containerBox, displayFlag );
           }

        else if ( locationFlag == true )
           {
            // get idLetter
            idLetterChar = unusedBox.idLetter;

            // current box id failed, rotating
            printf("\nBox %c first attempt failed, rotating\n", idLetterChar);

            // rotate the box before testing again
            rotate(&unusedBox);

            // fit check
            fitFlag = checkForFitInField( containerBox, currentLocation, unusedBox);

            if ( fitFlag == true && locationFlag == true)
               {
                //  place the box in the valid location
                fillBoxLocation( containerBox, currentLocation, unusedBox, FILL_BOX );
                boxFillFlag = true;

                displayField( containerBox, displayFlag );

                // set used flag to true
                containerBox->insideBoxList[unusedBoxIndexIterator].usedState = true;

                return fillContainerBox( containerBox, displayFlag );
               }

            printf("\nBox %c second attempt failed, trying another box\n", idLetterChar);
           }

        unusedBoxIndexIterator++;
       }

    printf("\nFailed at this location, backtracking\n");

    containerBox->insideBoxList[unusedBoxIndex - 1].usedState = true;
    //fillBoxLocation( containerBox, currentLocation, unusedBox, CLEAR_BOX );
    return fillContainerBox( containerBox, displayFlag );
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
