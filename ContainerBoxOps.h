#ifndef CONTAINER_BOX_TOOLS_H
#define CONTAINER_BOX_TOOLS_H

// Header files
#include <stdlib.h>  // for malloc
#include <stdio.h>    // for printing recursion ops
#include <stdbool.h>  // for Boolean
#include <unistd.h>  // for sleep
#include "InsideBoxTools.h"
#include "PointTools.h"

// Constants
extern const char DEFAULT_FIELD_CHAR;
extern const int NO_BOXES_AVAILABLE;
extern const int FILL_BOX;  // used to control fillBoxLocation function
extern const int CLEAR_BOX;  // used to control fillBoxLocation function
extern const int MAX_NUM_BOXES;

// Data Structures

typedef struct ContainerBoxDataStruct
   {
    char **field;

    InsideBoxData *insideBoxList;

    int width;

    int height;

    int numBoxes;
   } ContainerBoxData;

// Prototypes
bool checkForFitInField( ContainerBoxData *containerBox,
                                PointData testLocation, InsideBoxData testBox );

void clearContainerBox( ContainerBoxData *containerBox );
char **create2DArray( int maxRows, int maxCols );
void displayField( ContainerBoxData *containerBox, bool displayFlag );
void fillBoxLocation( ContainerBoxData *containerBox, PointData boxLocation,
                                        InsideBoxData fillBox, int clearFlag );
bool fillContainerBox( ContainerBoxData *containerBox, bool displayFlag );
bool findNextOpenLocation( const ContainerBoxData *containerBox,
                                                       PointData *returnPoint );
int findNextUnusedBoxIndex( ContainerBoxData *containerBox, int startAtIndex );
void initializeContainerBox( ContainerBoxData *containerBox, int numBoxes );
void rotate( InsideBoxData *box );

#endif   // CONTAINER_BOX_TOOLS_H
