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

    // Sleep( 1000 );  // pauses program for number of milliseconds
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
       // next open location
    PointData nextLocation;

    InsideBoxData unusedBox;

       // various flags
    bool locationFlag;
    bool fitFlag;

    int unusedBoxIndex;


    // find the next open test location
    locationFlag = findNextOpenLocation( containerBox, &nextLocation );

    // get the next box index
    unusedBoxIndex = findNextUnusedBoxIndex( containerBox, nextLocation );

    // get next box to fit check
    unusedBox = containerBox->insideBoxList[unusedBoxIndex];

    // if location flag is true, test current box at current location
    fitFlag = checkForFitInField( containerBox, nextLocation, unusedBox);

    // if flag from checkForFitInField is true
    if ( fitFlag == true )
       {
        //  place the box in the valid location
           // function: fillBoxLocation
        fillBoxLocation( containerBox, nextLocation, unusedBox, FILL_BOX );
        // set used flag to true
       }



    return false;  // temporary stub return
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
