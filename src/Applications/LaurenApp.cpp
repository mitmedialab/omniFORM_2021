//
//  AxisCheckerApp.cpp
//  emptyExample
//
//  Created by Daniel Levine on 5/19/21.
//

#include "AxisCheckerApp.hpp"


void AxisCheckerApp::update(float dt) {
    normalizedPhase += dt * 0.5;
    tally++;
    tally %= SHAPE_DISPLAY_SIZE_X + SHAPE_DISPLAY_SIZE_Y;
    updateHeights();
}

void AxisCheckerApp::updateHeights() {
    if (checkerboard) {
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int height;
                if (x < SHAPE_DISPLAY_SIZE_X / 2 && y < SHAPE_DISPLAY_SIZE_Y / 2) {
                    height = 40;
                } else if (x < SHAPE_DISPLAY_SIZE_X / 2) {
                    height = 250;
                } else if (y < SHAPE_DISPLAY_SIZE_Y / 2) {
                    height = 110;
                } else {
                    height = 180;
                }
                int xy = heightsForShapeDisplay.getPixelIndex(x, y);
                heightsForShapeDisplay[xy] = height;
            }
        }
        
        
    } else {
//        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
//            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
//                int height;
//                if (tally < SHAPE_DISPLAY_SIZE_X) {
//                    height = x == tally ? 255 : 0;
//                } else {
//                    height = SHAPE_DISPLAY_SIZE_X + y == tally ? 255 : 0;
//                }
//                int xy = heightsForShapeDisplay.getPixelIndex(x, y);
//                heightsForShapeDisplay[xy] = height;
//            }
//        }
        
        
        count++;
        if(count > 200){
            count = 0;
        }
        cout << "Count Num:" << count << "|" << ofGetElapsedTimeMillis() <<endl;
        
        //left highest juggle level
        for (int x = 1 ; x < 2; x++) {
            for (int y = 16; y < 19; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
       
                if (count<100) {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                } else {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                }
                
            }
        }
        //middle juggle level 1
        for (int x = 2 ; x < 3; x++) {
            for (int y = 16; y < 17; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
       
                if (count<100) {
                    heightsForShapeDisplay[xy] = 180; // 0 - 255 // 40 - 200
                } else {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                }
                
            }
        }
        
        //middle juggle level 2
        for (int x = 2 ; x < 3; x++) {
            for (int y = 17; y < 18; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
       
                if (count<100) {
                    heightsForShapeDisplay[xy] = 200; // 0 - 255 // 40 - 200
                } else {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                }
                
            }
        }
        
        //middle juggle level 3
        for (int x = 2 ; x < 3; x++) {
            for (int y = 18; y < 19; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
       
                if (count<100) {
                    heightsForShapeDisplay[xy] = 180; // 0 - 255 // 40 - 200
                } else {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                }
                
            }
        }
        //right lowest juggle level
        for (int x = 3 ; x < 4; x++) {
            for (int y = 16; y < 19; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
       
                if (count<100) {
                    heightsForShapeDisplay[xy] = 120; // 0 - 255 // 40 - 200
                } else {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                }
                
            }
        }
        
        //top border
        for (int x = 0 ; x < 4; x++) {
            for (int y = 15; y < 16; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
       
                if (count<100) {
                    heightsForShapeDisplay[xy] = 235; // 0 - 255 // 40 - 200
                } else {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                }
                
            }
        }
        
        //left border
        for (int x = 0 ; x < 1; x++) {
            for (int y = 16; y < 19; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
       
                if (count<100) {
                    heightsForShapeDisplay[xy] = 235; // 0 - 255 // 40 - 200
                } else {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                }
                
            }
        }
        
        //bottom border
        for (int x = 0 ; x < 4; x++) {
            for (int y = 19; y < 20; y++) {
        int xy = heightsForShapeDisplay.getPixelIndex(x, y);
       
                if (count<100) {
                    heightsForShapeDisplay[xy] = 235; // 0 - 255 // 40 - 200
                } else {
                    heightsForShapeDisplay[xy] = 20; // 0 - 255 // 40 - 200
                }
                
            }
        }
        
        
    }

        
        }
        
        

        
        
    


void AxisCheckerApp::drawGraphicsForShapeDisplay(int x, int y, int width, int height) {
    color.setHsb(fmod(normalizedPhase * 180, 180), 255, 255);
    ofSetColor(color);
    ofImage(heightsForShapeDisplay).draw(x, y, width, height);
}

string AxisCheckerApp::appInstructionsText() {
    string instructions = (string) "" +
        "This can be used to identify axis orientation. There are two modes,\n" +
        "scan lines and checkerboard.\n" +
        "\n" +
        "  'a' : switch to " + (checkerboard ? "scan lines" : "checkerboard") + " mode" +
        "";

    return instructions;
}

void AxisCheckerApp::keyPressed(int key) {
    if (key == 'a') {
        checkerboard = !checkerboard;
        tally = 0;
    }
}
