#include <jni.h>
#include <android/native_activity.h>
#include <android/native_window_jni.h>
#include <Ogre.h>
#include <OgreRTShaderSystem.h>
#include <OgreApplicationContext.h>
#include <android/asset_manager_jni.h>
#include <OgreGLRenderSystemCommon.h>

using namespace Ogre;
using namespace RTShader;
using namespace OgreBites;

ApplicationContext appContext;
SceneManager *scnMgr;
SceneNode *drawNode;
int rotation = 0;
bool destroy = true;
bool wallpaperSet = false;

void createScene();
void renderLoop();

extern "C" {
    JNIEXPORT void JNICALL Java_com_jp_ogrelivewallpaper_Service_00024ServiceEngine_nativeSurfaceCreate(JNIEnv *env, jobject thiz, jobject surface, jobject asset) {
        // create a native window from java surface
        ANativeWindow *window = ANativeWindow_fromSurface(env, surface);
        // check if that window has already been created
        if(appContext.getRenderWindow() == NULL) {
            // create asset manager from java asset
            AAssetManager *assetManager = AAssetManager_fromJava(env, asset);
            // initialize ogre
            appContext.initAppForAndroid(assetManager, window);
            // start the main scene setup
            createScene();
        } else {
            // if the window / surface was already created aka hitting the set wallpaper button
            // we make sure to not destroy our current ogre instance and just set it to the new location (the home screen)
            destroy = false;
            // check if the wallpaper is already set on the home screen
            if(!wallpaperSet) {
                // set the window to the new location (home screen)
                GLRenderSystemCommon::_createInternalResources(appContext.getRenderWindow(), window, NULL);
                // in order to prevent re setting the home screen aka if you go back into the wallpaper
                // settings and select the live wallpaper again
                wallpaperSet = true;
            }
        }
    }

    JNIEXPORT void JNICALL Java_com_jp_ogrelivewallpaper_Service_00024ServiceEngine_nativeDraw(JNIEnv *env, jobject thiz) {
        // main render loop
        renderLoop();
        // render a single frame
        appContext.getRoot()->renderOneFrame();
    }

    JNIEXPORT void JNICALL Java_com_jp_ogrelivewallpaper_Service_00024ServiceEngine_nativeDestroy(JNIEnv *env, jobject thiz) {
        // check if we actually want to destroy the instance
        if(destroy) {
            // destroy it
            appContext.closeApp();
            // wallpaper isn't set anymore
            wallpaperSet = false;
        }
        // set it so that next time only destroy is run without a new surface, we destroy it
        destroy = true;
    }
}

void createScene() {
    // setup our scene manager
    scnMgr = appContext.getRoot()->createSceneManager(ST_GENERIC);
    scnMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    // initialize RTSS
    ShaderGenerator::initialize();
    ShaderGenerator *shaderGen = ShaderGenerator::getSingletonPtr();
    shaderGen->addSceneManager(scnMgr);
    // create a camera node
    SceneNode *camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setPosition(0, 0, 10);
    // create a camera and set properties
    Camera *cam = scnMgr->createCamera("cam");
    cam->setNearClipDistance(0.1);
    cam->setFarClipDistance(1000);
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);
    // create a viewport
    Viewport *vp = appContext.getRenderWindow()->addViewport(cam);
    vp->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f));
    // create a light
    Light *light = scnMgr->createLight("light");
    light->setPosition(0, 0, 10);
    // create cube entity and add it to the scene
    Entity *ent2 = scnMgr->createEntity("cube", "cube.mesh");
    SceneNode *node2 = scnMgr->getRootSceneNode()->createChildSceneNode();
    node2->setOrientation(Quaternion(Degree(rotation), Vector3(1, 1, 1)));
    node2->attachObject(ent2);
    drawNode = node2;
}

void renderLoop() {
    // update rotation
    rotation++;
    // make sure we dont go above 360 degrees
    if(rotation > 360)
        rotation = 0;
    // update the scene node with the new rotation
    drawNode->setOrientation(Quaternion(Degree(rotation), Vector3(1, 0.8, 0.5)));
}
