//
//  AppManager.cpp
//  emptyExample
//
//  Created by Daniel Levine on 5/19/21.
//

#include "AppManager.hpp"

void AppManager::setup(){
    
    ofSetFrameRate(30);

    // inFORM screen setup
    #if SHAPE_DISPLAY_IN_USE==INFORM_DISPLAY
    ofSetWindowShape(MONITOR_SCREEN_RESOLUTION_X + SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_X, SHAPE_DISPLAY_PROJECTOR_RAW_RESOLUTION_Y);
    projectorOffsetX = MONITOR_SCREEN_RESOLUTION_X + SHAPE_DISPLAY_PROJECTOR_OFFSET_X;
    ofSetWindowPosition(0, 0);
    #endif
    //Lauren end
    
    printf("Setting up Shape Display\n");

    
    // initialize shape display and set up helper objects
    setupShapeDisplayManagement();
    
    // zero timeOfLastUpdate tracker
    timeOfLastUpdate = elapsedTimeInSeconds();
    
    // set up applications
    mqttApp = new MqttTransmissionApp();
    applications["mqttTransmission"] = mqttApp;
    
    // set up debugging application
    // and the debugging apps, too
    axisCheckerApp = new AxisCheckerApp();
    applications["axisChecker"] = axisCheckerApp;
    
    // give applications read access to input data
    for (map<string, Application *>::iterator iter = applications.begin(); iter != applications.end(); iter++) {
        Application *app = iter->second;

        // shape display heights, if they are accessible
        if (shapeIOManager->heightsFromShapeDisplayAvailable) {
            app->setHeightsFromShapeDisplayRef(&heightPixelsFromShapeDisplay);
        }
    }
    
    // set default application
    setCurrentApplication("axisChecker");
}

// initialize the shape display and set up shape display helper objects
void AppManager::setupShapeDisplayManagement() {
    // initialize communication with the shape display
    
    //Lauren start
    switch (SHAPE_DISPLAY_IN_USE) {
        case INFORM_DISPLAY:
            shapeIOManager = new InformIOManager();
            break;
        case TRANSFORM_DISPLAY:
            shapeIOManager = new TransformIOManager();
            break;
        case COOPERFORM_DISPLAY:
            shapeIOManager = new CooperformIOManager();
            break;
        default:
            throw "unknown setting for `SHAPE_DISPLAY_IN_USE`";
    }
    //Lauren end
    
    
    
    printf("Setting up Shape Display Management\n");


    // initialize shape display pin configs
    PinConfigs pinConfigs;
    pinConfigs.timeOfUpdate = elapsedTimeInSeconds();
    pinConfigs.gainP = DEFAULT_GAIN_P;
    pinConfigs.gainI = DEFAULT_GAIN_I;
    pinConfigs.maxI = DEFAULT_MAX_I;
    pinConfigs.deadZone = DEFAULT_DEAD_ZONE;
    pinConfigs.maxSpeed = DEFAULT_MAX_SPEED;
    shapeIOManager->setGlobalPinConfigs(pinConfigs);
    timeOfLastPinConfigsUpdate = elapsedTimeInSeconds();

    // clear height and pin config buffers
    for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
        for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
            heightsForShapeDisplay[x][y] = 0;
            heightsFromShapeDisplay[x][y] = 0;
            pinConfigsForShapeDisplay[x][y] = pinConfigs;
        }
    }

    // allocate height pixels objects and clear contents
    heightPixelsForShapeDisplay.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    heightPixelsForShapeDisplay.set(0);
    heightPixelsFromShapeDisplay.allocate(SHAPE_DISPLAY_SIZE_X, SHAPE_DISPLAY_SIZE_Y, 1);
    heightPixelsFromShapeDisplay.set(0);

    // allocate shape display graphics container and clear contents
    
    //Lauren start
    graphicsForShapeDisplay.allocate(SHAPE_DISPLAY_PROJECTOR_SIZE_X, SHAPE_DISPLAY_PROJECTOR_SIZE_X, GL_RGBA);
    //Lauren end
    
    graphicsForShapeDisplay.begin();
    ofClear(0);
    graphicsForShapeDisplay.end();
}

void AppManager::update(){
    
    //cout << "Update in App Manager\n";
    
    // time elapsed since last update
    float currentTime = elapsedTimeInSeconds();
    double dt = currentTime - timeOfLastUpdate;
    timeOfLastUpdate = currentTime;

    // copy heights from shape display to pixels object
    if (shapeIOManager->heightsFromShapeDisplayAvailable) {
        shapeIOManager->getHeightsFromShapeDisplay(heightsFromShapeDisplay);

        // note: manually looping over all pixels is important! the underlying
        // ofPixels char array is stored as unsigned char[y][x], while the
        // shape display heights are stored as unsigned char[x][y]
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = heightPixelsFromShapeDisplay.getPixelIndex(x, y);
                heightPixelsFromShapeDisplay[xy] = heightsFromShapeDisplay[x][y];
            }
        }
    }

    // copy heights and pin configs from application
    bool pinConfigsAreStale;
    if (!paused) {
        currentApplication->update(dt);
        currentApplication->getHeightsForShapeDisplay(heightPixelsForShapeDisplay);

        // note: manually looping over all pixels is important! the underlying
        // ofPixels char array is stored as unsigned char[y][x], while the
        // shape display heights are stored as unsigned char[x][y]
        for (int x = 0; x < SHAPE_DISPLAY_SIZE_X; x++) {
            for (int y = 0; y < SHAPE_DISPLAY_SIZE_Y; y++) {
                int xy = heightPixelsForShapeDisplay.getPixelIndex(x, y);
                heightsForShapeDisplay[x][y] = heightPixelsForShapeDisplay[xy];
            }
        }

        pinConfigsAreStale = timeOfLastPinConfigsUpdate < currentApplication->timeOfLastPinConfigsUpdate;
        if (pinConfigsAreStale) {
            currentApplication->getPinConfigsForShapeDisplay(pinConfigsForShapeDisplay);
        }
    }

    // render graphics
   graphicsForShapeDisplay.begin();
    ofBackground(0);
    ofSetColor(255);
    
    //Lauren start
    currentApplication->drawGraphicsForShapeDisplay(0, 0, SHAPE_DISPLAY_PROJECTOR_SIZE_X, SHAPE_DISPLAY_PROJECTOR_SIZE_X);
    //Lauren end
    
    graphicsForShapeDisplay.end();
    
    shapeIOManager->sendHeightsToShapeDisplay(heightsForShapeDisplay);
    if (pinConfigsAreStale) {
        shapeIOManager->setPinConfigs(pinConfigsForShapeDisplay);
        timeOfLastPinConfigsUpdate = elapsedTimeInSeconds();
    }
}

void AppManager::draw(){
    ofBackground(0,0,0);
    ofSetColor(255);
    
    // draw shape and color I/O images

    ofRect(1, 1, 302, 302);
    ofImage(heightPixelsFromShapeDisplay).draw(2, 2, 300, 300);
    
    ofRect(305, 1, 302, 302);
    ofImage(heightPixelsForShapeDisplay).draw(306, 2, 300, 300);
    
    ofRect(609, 1, 302, 302);
    graphicsForShapeDisplay.draw(610, 2, 300, 300);
    
    ofRect(913, 1, 302, 302);
    ofImage(colorPixels).draw(914, 2, 300, 300);

    // draw this app's debugging gui, if selected
    if (showDebugGui) {
        currentApplication->drawDebugGui(1, 305);
    }

    // draw text
    int menuLeftCoordinate = 21;
    int menuHeight = 350;
    string title = currentApplication->getName() + (showDebugGui ? " - Debug" : "");
    ofDrawBitmapString(title, menuLeftCoordinate, menuHeight);
    menuHeight += 30;
    ofDrawBitmapString((string) "  '?' : " + (showGlobalGuiInstructions ? "hide" : "show") + " instructions", menuLeftCoordinate, menuHeight);
    if (showGlobalGuiInstructions) {
        menuHeight += 20;
        ofDrawBitmapString((string) "  '1' - '9' : select application", menuLeftCoordinate, menuHeight);
        menuHeight += 20;
        ofDrawBitmapString((string) "  '.' : turn debug gui " + (showDebugGui ? "off" : "on"), menuLeftCoordinate, menuHeight);
        menuHeight += 20;
        ofDrawBitmapString((string) "  ' ' : " + (paused ? "play" : "pause"), menuLeftCoordinate, menuHeight);
    }
    menuHeight += 30;

    // if there isn't already a debug gui, draw some more information
    if (!showDebugGui || currentApplication == applications["water"] || currentApplication == applications["stretchy"]) {
        ofRect(913, 305, 302, 302);
        ofImage(depthPixels).draw(914, 306, 300, 300);

        ofDrawBitmapString(currentApplication->appInstructionsText(), menuLeftCoordinate, menuHeight);
        menuHeight += 20;
    }
    


//Lauren start
// draw graphics onto projector
bool shouldDrawProjectorGraphics = SHAPE_DISPLAY_IN_USE == INFORM_DISPLAY;
if (shouldDrawProjectorGraphics) {
    graphicsForShapeDisplay.draw(projectorOffsetX, SHAPE_DISPLAY_PROJECTOR_OFFSET_Y, SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_X, SHAPE_DISPLAY_PROJECTOR_SCALED_SIZE_Y);
};
}
//Lauren end


void AppManager::setCurrentApplication(string appName) {
    if (applications.find(appName) == applications.end()) {
        throw "no application exists with name " + appName;
    }

    currentApplication = applications[appName];
    updateDepthInputBoundaries();
}

void AppManager::updateDepthInputBoundaries() {
    pair<int, int> boundaries = currentApplication->getDepthInputBoundaries();
    int near = boundaries.first;
    int far = boundaries.second;

    //no more kinect. this function is useless
}

void AppManager::exit() {
    // delete shapeIOManager to shut down the shape display
    delete shapeIOManager;
}

// handle key presses. keys unused by app manager are forwarded to the current
// application.
void AppManager::keyPressed(int key) {
    // keys used by app manager must be registered as reserved keys
    const int reservedKeysLength = 13;
    const int reservedKeys[reservedKeysLength] = {
        '/', '?', '.', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    const int *reservedKeysEnd = reservedKeys + reservedKeysLength;

    // key press events handled by the application manager
    if (find(reservedKeys, reservedKeysEnd, key) != reservedKeysEnd) {
        if (key == '/' || key == '?') {
            showGlobalGuiInstructions = !showGlobalGuiInstructions;
        } else if (key == '.') {
            showDebugGui = !showDebugGui;
        } else if (key == ' ') {
            paused = !paused;
        } else if (key == '1') {
            setCurrentApplication("mqttTransmission");
            
            //Lauren start
//        } else if (key == '1') {
//            setCurrentApplication("tunableWave");
//        } else if (key == '2') {
//            setCurrentApplication("lever");
//        } else if (key == '3') {
//            setCurrentApplication("BOS");
//        } else if (key == '4') {
//            setCurrentApplication("metaMaterials");
//        } else if (key == '5') {
//            setCurrentApplication("teleoperation");
//        } else if (key == '6') {
//            setCurrentApplication("simpleWave");
//        } else if (key == '7') {
//            setCurrentApplication("touchMaterial");
//        } else if (key == '8') {
//            setCurrentApplication("water");
//        } else if (key == '9') {
//            setCurrentApplication("stretchy");
//        }
            //Lauren end
            
        }

    // forward unreserved keys to the application
    } else {
        currentApplication->keyPressed(key);
    }
}

void AppManager::keyReleased(int key) {};
void AppManager::mouseMoved(int x, int y) {};
void AppManager::mouseDragged(int x, int y, int button) {};
void AppManager::mousePressed(int x, int y, int button) {};
void AppManager::mouseReleased(int x, int y, int button) {};
void AppManager::windowResized(int w, int h) {};
void AppManager::gotMessage(ofMessage msg) {};
void AppManager::dragEvent(ofDragInfo dragInfo) {};
void AppManager::mouseEntered(int x, int y){};
void AppManager::mouseExited(int x, int y){};
