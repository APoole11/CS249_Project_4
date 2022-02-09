bool fillContainerBox( ContainerBoxData *containerBox, bool displayFlag )
   {
    // initialize variables
    PointData currentLocation;
    //PointData tempLocation;
    InsideBoxData currentBox;
    InsideBoxData holdBox;
    bool locationFlag;
    bool fitFlag;
    bool boxFillFlag = false;
    int unusedBoxIndex = 0;
    int startAtIndex = 0;
    int unusedBoxIterator;
    char idLetterChar;

    // determine the locatiion
    locationFlag = findNextOpenLocation( containerBox, &currentLocation );

    // determine the index of the next unused box
    unusedBoxIndex = findNextUnusedBoxIndex( containerBox, startAtIndex );

    // set the iterator to the unused box index
    unusedBoxIterator = unusedBoxIndex;

    // set hold box equal to the box at the unused box index
    holdBox = containerBox->insideBoxList[unusedBoxIndex];

    printf("\n\n\nHold Box Letter Id: %c\n\n\n", holdBox.idLetter);

    if ( unusedBoxIndex == NO_BOXES_AVAILABLE ) // && locationflag == false ????????????
       {
        return true;
       }

    while ( boxFillFlag == false && unusedBoxIterator < containerBox->numBoxes)
       {
        // get the current box
        currentBox = containerBox->insideBoxList[unusedBoxIterator];

        // fit check
        fitFlag = checkForFitInField( containerBox, currentLocation, currentBox);

        if ( containerBox->insideBoxList[unusedBoxIterator].usedState == true )
           {
            fitFlag = false;
           }

        // if flag from checkForFitInField is true
        if ( fitFlag == true && locationFlag == true)
           {
            //  place the box in the valid location
            fillBoxLocation( containerBox, currentLocation, holdBox, FILL_BOX );
            boxFillFlag = true;

            displayField( containerBox, displayFlag );

            // set used flag to true
            containerBox->insideBoxList[unusedBoxIterator].usedState = true;

            if ( !fillContainerBox( containerBox, displayFlag ) )
               {
                fillBoxLocation( containerBox, currentLocation, holdBox, CLEAR_BOX );
                rotate(&holdBox);
                containerBox->insideBoxList[unusedBoxIndex - 1].usedState = false;
                return fillContainerBox( containerBox, displayFlag );
               }
           }

        else if ( locationFlag == true )
           {
            // get idLetter
            idLetterChar = currentBox.idLetter;

            // current box id failed, rotating
            printf("\nBox %c first attempt failed, rotating\n", idLetterChar);

            // rotate the box before testing again
            rotate(&currentBox);

            // fit check
            fitFlag = checkForFitInField( containerBox, currentLocation, currentBox);

            if ( fitFlag == true && locationFlag == true)
               {
                //  place the box in the valid location
                fillBoxLocation( containerBox, currentLocation, currentBox, FILL_BOX );
                boxFillFlag = true;

                displayField( containerBox, displayFlag );

                // set used flag to true
                containerBox->insideBoxList[unusedBoxIterator].usedState = true;

                if ( !fillContainerBox( containerBox, displayFlag ) )
                   {
                    fillBoxLocation( containerBox, currentLocation, holdBox, CLEAR_BOX );
                    rotate(&holdBox);
                    containerBox->insideBoxList[unusedBoxIndex - 1].usedState = false;
                    return fillContainerBox( containerBox, displayFlag );
                   }
               }

            printf("\nBox %c second attempt failed, trying another box\n", idLetterChar);
           }

        unusedBoxIterator++;
       }

    if ( unusedBoxIterator + 1 == containerBox->numBoxes)
       {
        return true;
       }

    else
       {
        printf("\nFailed at this location, backtracking\n");
        holdBox = containerBox->insideBoxList[unusedBoxIndex - 1];
        // tempLocation.pos_X = currentLocation.pos_X - holdBox.width;
        // tempLocation.pos_Y = currentLocation.pos_;
        fillBoxLocation( containerBox, currentLocation, holdBox, CLEAR_BOX );
        displayField( containerBox, displayFlag );
        rotate(&holdBox);
        containerBox->insideBoxList[unusedBoxIndex].usedState = false;
        return false;
       }
   }
