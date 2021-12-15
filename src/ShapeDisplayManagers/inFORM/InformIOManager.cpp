//
//  InformIOManager.cpp
//  TMG Shape Displays
//
//  Created by Daniel Leithinger on 3/3/10.
//  Copyright 2010 MIT Media Lab. All rights reserved.
//

#include "InformIOManager.hpp"


// Create new informIOManager instance, setting up inFORM-specific board
// configuration
InformIOManager::InformIOManager() {
    configureBoards();
}


// setup inFORM-specific board configuration
void InformIOManager::configureBoards() {
    for (int i = 0; i < 60; i++) {
        // every 3rd and 4th board is mounted upside down
        pinBoards[i].invertHeight = (((i / 2) % 2) == 0) ? false : true;

        // determine which serial connection each board is on
        if (i < 28) {
            pinBoards[i].serialConnection = 0;
        } else {
            pinBoards[i].serialConnection = 1;
        }
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            int currentRow = (int)(i / 2);
            int currentColumn = j + (i % 2 * 6);
            pinBoards[i].heights[j] = 0;
            pinBoards[i].pinCoordinates[j][0] = currentRow;
            pinBoards[i].pinCoordinates[j][1] = currentColumn;
        }
    }

    for (int i = 60; i < 150; i++) {
        // every 4th, 5th and 6th board is mounted upside down
        pinBoards[i].invertHeight = ((((i-60) / 3) % 2) == 0) ? false : true;

        // determine which serial connection each board is on
        if (i < 90) {
            pinBoards[i].serialConnection = 2;
        } else if (i < 120) {
            pinBoards[i].serialConnection = 3;
        } else {
            pinBoards[i].serialConnection = 4;
        }
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            int currentRow = (int)((i - 60) / 3);
            int currentColumn = 12 + j + ((i-60) % 3 * 6);
            pinBoards[i].heights[j] = 0;
            pinBoards[i].pinCoordinates[j][0] = currentRow;
            pinBoards[i].pinCoordinates[j][1] = currentColumn;
        }
    }

    // switch the coordinates for boards that have been set out of order for serial connection 0 and 1:
    int wrongCols[8]= {0, 3, 4, 23, 26, 27, 28, 29};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            for (int k = 0; k < 2; k++) {
                unsigned char temp = pinBoards[(wrongCols[i] * 2) + 1].pinCoordinates[j][k];
                pinBoards[(wrongCols[i] * 2) + 1].pinCoordinates[j][k] = pinBoards[(wrongCols[i] * 2)].pinCoordinates[j][k];
                pinBoards[(wrongCols[i] * 2)].pinCoordinates[j][k] = temp;
            }
        }
    }

    // change the row order according to how the Arduinos have been mounted
    int rowOrder[30] = {1, 0, 2, 3, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 14, 15,
            16, 17, 19, 18, 21, 20, 23, 22, 25, 24, 27, 26, 28, 29};
    for (int i = 0; i < 15; i++) {
        int evenIndex = i * 2;
        int oddIndex = evenIndex + 1;
        if (rowOrder[evenIndex] > rowOrder[oddIndex]) {
            // for each pin, swap x and y coordinates between the even and odd indexed boards
            for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
                for (int k = 0; k < 2; k++) {
                    unsigned char temp = pinBoards[2 * oddIndex].pinCoordinates[j][k];
                    pinBoards[2 * oddIndex].pinCoordinates[j][k] = pinBoards[2 * evenIndex].pinCoordinates[j][k];
                    pinBoards[2 * evenIndex].pinCoordinates[j][k] = temp;
                    
                    temp = pinBoards[2 * oddIndex + 1].pinCoordinates[j][k];
                    pinBoards[2 * oddIndex + 1].pinCoordinates[j][k] = pinBoards[2 * evenIndex + 1].pinCoordinates[j][k];
                    pinBoards[2 * evenIndex + 1].pinCoordinates[j][k] = temp;
                }
            }
        }
    }
    
    // invert pin order if the pins have been mounted inverted for first 60 boards
    for (int i = 0; i < 60; i++) {
        if (!pinBoards[i].invertHeight) {
            for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
                pinBoards[i].pinCoordinates[j][1] = 11 - pinBoards[i].pinCoordinates[j][1];
            }
        }
    }
    // do the same for the remaining
    for (int i = 60; i < 150; i++) {
        if (!pinBoards[i].invertHeight) {
            for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
                pinBoards[i].pinCoordinates[j][1] = 12 + 29 - pinBoards[i].pinCoordinates[j][1];
            }
        }
    }

    // last, orient the x-y coordinate axes to the desired external axes
    for (int i = 0; i < 150; i++) {
        for (int j = 0; j < NUM_PINS_ARDUINO; j++) {
            unsigned char j0 = pinBoards[i].pinCoordinates[j][0];
            unsigned char j1 = pinBoards[i].pinCoordinates[j][1];
            pinBoards[i].pinCoordinates[j][0] = SHAPE_DISPLAY_SIZE_X - 1 - j1;
            pinBoards[i].pinCoordinates[j][1] = j0;
        }
    }

    // flag configuration as complete
    boardsAreConfigured = true;
}
