//
//  LaurenApp.hpp
//  emptyExample
//
//  Created by Daniel Levine on 5/19/21.
//

#ifndef LaurenApp_hpp
#define LaurenApp_hpp

#include <stdio.h>
#include "Application.hpp"


class LaurenApp : public Application {
public:
    void update(float dt);
    void drawGraphicsForShapeDisplay(int x, int y, int width, int height);
    string appInstructionsText();
    void keyPressed(int key);

    string getName() {return "Axis Checker";};

private:
    void updateHeights();
    
    int count = 0;

    int tally = 0;
    bool checkerboard = false;

    float normalizedPhase = 0;
    ofColor color;
};

#endif /* LaurenApp_hpp */
