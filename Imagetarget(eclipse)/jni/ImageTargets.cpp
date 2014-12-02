/*==============================================================================
Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
All Rights Reserved.

@file
    ImageTargets.cpp

@brief
    Sample for ImageTargets

==============================================================================*/


#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/time.h>

#ifdef USE_OPENGL_ES_1_1
#include <GLES/gl.h>
#include <GLES/glext.h>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include <QCAR/QCAR.h>
#include <QCAR/CameraDevice.h>
#include <QCAR/Renderer.h>
#include <QCAR/VideoBackgroundConfig.h>
#include <QCAR/Trackable.h>
#include <QCAR/TrackableResult.h>
#include <QCAR/Tool.h>
#include <QCAR/Tracker.h>
#include <QCAR/TrackerManager.h>
#include <QCAR/ImageTracker.h>
#include <QCAR/CameraCalibration.h>
#include <QCAR/UpdateCallback.h>
#include <QCAR/DataSet.h>


#include "SampleUtils.h"
#include "Texture.h"
#include "CubeShaders.h"
#include "Teapot.h"
#include "Heroin.h"
#include "chhhh.h"
#include "coo.h"
#include "hh.h"

#ifdef __cplusplus
extern "C"
{
#endif

// Textures:
    int textureCount                = 0;
    Texture** textures              = 0;
    int modeltype = 0;

// OpenGL ES 2.0 specific:
#ifdef USE_OPENGL_ES_2_0
    unsigned int shaderProgramID    = 0;
    GLint vertexHandle              = 0;
    GLint normalHandle              = 0;
    GLint textureCoordHandle        = 0;
    GLint mvpMatrixHandle           = 0;
    GLint texSampler2DHandle        = 0;
#endif

// Screen dimensions:
    unsigned int screenWidth        = 0;
    unsigned int screenHeight       = 0;
    bool isRotate = true;

// Indicates whether screen is in portrait (true) or landscape (false) mode
    bool isActivityInPortraitMode   = false;

// The projection matrix used for rendering virtual objects:
    QCAR::Matrix44F projectionMatrix;

// Constants:
//static const float kObjectScale = 3.f;
    float kObjectScale = 120.f;
    float objectx = -90.0f;
    float objecty = 1.0f;



    void animateteapot(QCAR::Matrix44F& modelViewMatrix);

    QCAR::DataSet* dataSetStonesAndChips    = 0;
    QCAR::DataSet* dataSetTarmac            = 0;
    QCAR::DataSet* dataSethuitailang		= 0;
    QCAR::DataSet* dataSetheroin		= 0;
    QCAR::DataSet* dataSetchhhh		= 0;
    QCAR::DataSet* dataSethh		= 0;
    QCAR::DataSet* dataSetcoo		= 0;
    bool switchDataSetAsap  = false;

// Object to receive update callbacks from QCAR SDK
    class ImageTargets_UpdateCallback : public QCAR::UpdateCallback
    {
        virtual void QCAR_onUpdate(QCAR::State& /*state*/)
        {
            if (switchDataSetAsap)
            {
                switchDataSetAsap = false;

                // Get the image tracker:
                QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
                QCAR::ImageTracker* imageTracker = static_cast<QCAR::ImageTracker*>(
                                                       trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER));
                if (imageTracker == 0 || dataSetStonesAndChips == 0 || dataSetTarmac == 0 || dataSethuitailang == 0||dataSetchhhh == 0||dataSethh == 0||dataSetcoo == 0||dataSetheroin == 0||
                        imageTracker->getActiveDataSet() == 0)
                {
                    LOG("Failed to switch data set.");
                    return;
                }

                if (imageTracker->getActiveDataSet() == dataSetStonesAndChips)
                {
                    imageTracker->deactivateDataSet(dataSetStonesAndChips);
                    imageTracker->activateDataSet(dataSetTarmac);
                    imageTracker->activateDataSet(dataSethuitailang);
                    imageTracker->activateDataSet(dataSetheroin);
                    imageTracker->activateDataSet(dataSetchhhh);
                    imageTracker->activateDataSet(dataSetcoo);
                    imageTracker->activateDataSet(dataSethh);
                }
                else
                {
                    imageTracker->activateDataSet(dataSetTarmac);
                    imageTracker->activateDataSet(dataSetStonesAndChips);
                    imageTracker->activateDataSet(dataSethuitailang);
                    imageTracker->activateDataSet(dataSetheroin);
                    imageTracker->activateDataSet(dataSetchhhh);
                    imageTracker->activateDataSet(dataSetcoo);
                    imageTracker->activateDataSet(dataSethh);
                }
            }
        }
    };

    ImageTargets_UpdateCallback updateCallback;

    JNIEXPORT int JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_getOpenGlEsVersionNative(JNIEnv *, jobject)
    {
#ifdef USE_OPENGL_ES_1_1
        return 1;
#else
        return 2;
#endif
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_setActivityPortraitMode(JNIEnv *, jobject, jboolean isPortrait)
    {
        isActivityInPortraitMode = isPortrait;
    }



    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_switchDatasetAsap(JNIEnv *, jobject)
    {
        switchDataSetAsap = true;
    }


    JNIEXPORT int JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_initTracker(JNIEnv *, jobject)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_initTracker");

        // Initialize the image tracker:
        QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
        QCAR::Tracker* tracker = trackerManager.initTracker(QCAR::Tracker::IMAGE_TRACKER);
        if (tracker == NULL)
        {
            LOG("Failed to initialize ImageTracker.");
            return 0;
        }

        LOG("Successfully initialized ImageTracker.");
        return 1;
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_deinitTracker(JNIEnv *, jobject)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_deinitTracker");

        // Deinit the image tracker:
        QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
        trackerManager.deinitTracker(QCAR::Tracker::IMAGE_TRACKER);
    }


    JNIEXPORT int JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_loadTrackerData(JNIEnv *, jobject)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_loadTrackerData");

        // Get the image tracker:
        QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
        QCAR::ImageTracker* imageTracker = static_cast<QCAR::ImageTracker*>(
                                               trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER));
        if (imageTracker == NULL)
        {
            LOG("Failed to load tracking data set because the ImageTracker has not"
                " been initialized.");
            return 0;
        }

        // Create the data sets:
        dataSetStonesAndChips = imageTracker->createDataSet();
        if (dataSetStonesAndChips == 0)
        {
            LOG("Failed to create a new tracking data.");
            return 0;
        }

        dataSetTarmac = imageTracker->createDataSet();
        if (dataSetTarmac == 0)
        {
            LOG("Failed to create a new tracking data.");
            return 0;
        }

        dataSethuitailang = imageTracker->createDataSet();
        if (dataSethuitailang == 0)
        {
            LOG("Failed to create a new tracking data.");
            return 0;
        }

        dataSetheroin = imageTracker->createDataSet();
        if (dataSetheroin == 0)
        {
            LOG("Failed to create a new tracking data.");
            return 0;
        }

        dataSethh = imageTracker->createDataSet();
        if (dataSethh == 0)
        {
            LOG("Failed to create a new tracking data.");
            return 0;
        }

        dataSetchhhh = imageTracker->createDataSet();
        if (dataSetchhhh == 0)
        {
            LOG("Failed to create a new tracking data.");
            return 0;
        }
        dataSetcoo = imageTracker->createDataSet();
        if (dataSetcoo == 0)
        {
            LOG("Failed to create a new tracking data.");
            return 0;
        }

        // Load the data sets:
        if (!dataSetStonesAndChips->load("StonesAndChips.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
        {
            LOG("Failed to load data set1.");
            return 0;
        }

        if (!dataSetTarmac->load("Tarmac.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
        {
            LOG("Failed to load data set2.");
            return 0;
        }

        if (!dataSethuitailang->load("huitailang.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
        {
            LOG("Failed to load data set3.");
            return 0;
        }

        if (!dataSetheroin->load("heroin.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
        {
            LOG("Failed to load data set4.");
            return 0;
        }

        if (!dataSetchhhh->load("chhhh.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
        {
            LOG("Failed to load data set5.");
            return 0;
        }
        if (!dataSetcoo->load("coo.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
        {
            LOG("Failed to load data set6.");
            return 0;
        }
        if (!dataSethh->load("hh.xml", QCAR::DataSet::STORAGE_APPRESOURCE))
        {
            LOG("Failed to load data set7.");
            return 0;
        }

        // Activate the data set:
        if (!imageTracker->activateDataSet(dataSetStonesAndChips))
        {
            LOG("Failed to activate data set.");
            return 0;
        }

        imageTracker->activateDataSet(dataSethuitailang);
        imageTracker->activateDataSet(dataSetheroin);
        imageTracker->activateDataSet(dataSetStonesAndChips);
        imageTracker->activateDataSet(dataSetchhhh);
        imageTracker->activateDataSet(dataSethh);
        imageTracker->activateDataSet(dataSetcoo);
        LOG("Successfully loaded and activated data set.");
        return 1;
    }


    JNIEXPORT int JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_destroyTrackerData(JNIEnv *, jobject)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_destroyTrackerData");

        // Get the image tracker:
        QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
        QCAR::ImageTracker* imageTracker = static_cast<QCAR::ImageTracker*>(
                                               trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER));
        if (imageTracker == NULL)
        {
            LOG("Failed to destroy the tracking data set because the ImageTracker has not"
                " been initialized.");
            return 0;
        }

        if (dataSetStonesAndChips != 0)
        {
            if (imageTracker->getActiveDataSet() == dataSetStonesAndChips &&
                    !imageTracker->deactivateDataSet(dataSetStonesAndChips))
            {
                LOG("Failed to destroy the tracking data set StonesAndChips because the data set "
                    "could not be deactivated.");
                return 0;
            }

            if (!imageTracker->destroyDataSet(dataSetStonesAndChips))
            {
                LOG("Failed to destroy the tracking data set StonesAndChips.");
                return 0;
            }

            LOG("Successfully destroyed the data set StonesAndChips.");
            dataSetStonesAndChips = 0;
        }

        if (dataSetTarmac != 0)
        {
            if (imageTracker->getActiveDataSet() == dataSetTarmac &&
                    !imageTracker->deactivateDataSet(dataSetTarmac))
            {
                LOG("Failed to destroy the tracking data set Tarmac because the data set "
                    "could not be deactivated.");
                return 0;
            }

            if (!imageTracker->destroyDataSet(dataSetTarmac))
            {
                LOG("Failed to destroy the tracking data set Tarmac.");
                return 0;
            }

            LOG("Successfully destroyed the data set Tarmac.");
            dataSetTarmac = 0;
        }

        if (dataSethuitailang != 0)
        {
            if (imageTracker->getActiveDataSet() == dataSethuitailang &&
                    !imageTracker->deactivateDataSet(dataSethuitailang))
            {
                LOG("Failed to destroy the tracking data set huitailang because the data set "
                    "could not be deactivated.");
                return 0;
            }

            if (!imageTracker->destroyDataSet(dataSethuitailang))
            {
                LOG("Failed to destroy the tracking data set huitailang.");
                return 0;
            }

            LOG("Successfully destroyed the data set huitailang.");
            dataSethuitailang = 0;
        }


        if (dataSetchhhh != 0)
        {
            if (imageTracker->getActiveDataSet() == dataSetchhhh &&
                    !imageTracker->deactivateDataSet(dataSetchhhh))
            {
                LOG("Failed to destroy the tracking data set chhhh because the data set "
                    "could not be deactivated.");
                return 0;
            }

            if (!imageTracker->destroyDataSet(dataSetchhhh))
            {
                LOG("Failed to destroy the tracking data set chhhh.");
                return 0;
            }

            LOG("Successfully destroyed the data set chhhh.");
            dataSetchhhh = 0;
        }

        if (dataSethh != 0)
        {
            if (imageTracker->getActiveDataSet() == dataSethh &&
                    !imageTracker->deactivateDataSet(dataSethh))
            {
                LOG("Failed to destroy the tracking data set hh because the data set "
                    "could not be deactivated.");
                return 0;
            }

            if (!imageTracker->destroyDataSet(dataSethh))
            {
                LOG("Failed to destroy the tracking data set hh.");
                return 0;
            }

            LOG("Successfully destroyed the data set hh.");
            dataSethh = 0;
        }

        if (dataSetcoo != 0)
        {
            if (imageTracker->getActiveDataSet() == dataSetcoo &&
                    !imageTracker->deactivateDataSet(dataSetcoo))
            {
                LOG("Failed to destroy the tracking data set coo because the data set "
                    "could not be deactivated.");
                return 0;
            }

            if (!imageTracker->destroyDataSet(dataSetcoo))
            {
                LOG("Failed to destroy the tracking data set coo.");
                return 0;
            }

            LOG("Successfully destroyed the data set coo.");
            dataSetcoo = 0;
        }

        if (dataSetheroin != 0)
        {
            if (imageTracker->getActiveDataSet() == dataSetheroin &&
                    !imageTracker->deactivateDataSet(dataSetheroin))
            {
                LOG("Failed to destroy the tracking data set heroin because the data set "
                    "could not be deactivated.");
                return 0;
            }

            if (!imageTracker->destroyDataSet(dataSetheroin))
            {
                LOG("Failed to destroy the tracking data set heroin.");
                return 0;
            }

            LOG("Successfully destroyed the data set heroin.");
            dataSetheroin = 0;
        }

        return 1;
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_onQCARInitializedNative(JNIEnv *, jobject)
    {
        // Register the update callback where we handle the data set swap:
        QCAR::registerCallback(&updateCallback);

        // Comment in to enable tracking of up to 2 targets simultaneously and
        // split the work over multiple frames:
        QCAR::setHint(QCAR::HINT_MAX_SIMULTANEOUS_IMAGE_TARGETS, 2);
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_stopRotate(JNIEnv *, jobject)
    {
        isRotate = !isRotate;
    }

    JNIEXPORT int JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_getmodelmessage(JNIEnv *, jobject)
    {
        /*if(modeltype == 0){

        	return "none";
        }
        else if(modeltype == 1){
        	return "this is heroin";
        }
        else if(modeltype == 2) {
        	return "this is huitailang";
        }
        else return "none";*/
        return modeltype;
    }

    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_QCARSampleGLView_bigger(JNIEnv *, jobject)
    {
        if (kObjectScale<7600)
            kObjectScale = kObjectScale + 8;
    }

    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_QCARSampleGLView_Rotatex(JNIEnv *, jobject,jfloat bx)
    {
        objectx += bx* 180.0f/3.1415f;
    }

    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_QCARSampleGLView_Rotatey(JNIEnv *, jobject,jfloat by)
    {
        objecty += by* 180.0f/3.1415f;
    }

    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_QCARSampleGLView_small(JNIEnv *, jobject)
    {
        if (kObjectScale>=8.5)
            kObjectScale = kObjectScale - 8;
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_renderFrame(JNIEnv *, jobject)
    {
        //LOG("Java_com_qualcomm_QCARSamples_ImageTargets_GLRenderer_renderFrame");

        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Get the state from QCAR and mark the beginning of a rendering section
        QCAR::State state = QCAR::Renderer::getInstance().begin();

        // Explicitly render the Video Background
        QCAR::Renderer::getInstance().drawVideoBackground();

        glEnable(GL_DEPTH_TEST);

        // We must detect if background reflection is active and adjust the culling direction.
        // If the reflection is active, this means the post matrix has been reflected as well,
        // therefore standard counter clockwise face culling will result in "inside out" models.
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        if (QCAR::Renderer::getInstance().getVideoBackgroundConfig().mReflection == QCAR::VIDEO_BACKGROUND_REFLECTION_ON)
            glFrontFace(GL_CW);  //Front camera
        else
            glFrontFace(GL_CCW);   //Back camera


        // Did we find any trackables this frame?
        for (int tIdx = 0; tIdx < state.getNumTrackableResults(); tIdx++)
        {
            // Get the trackable:
            const QCAR::TrackableResult* result = state.getTrackableResult(tIdx);
            const QCAR::Trackable& trackable = result->getTrackable();
            QCAR::Matrix44F modelViewMatrix =
                QCAR::Tool::convertPose2GLMatrix(result->getPose());

            // Choose the texture based on the target name:
            int textureIndex;
            modeltype = 11;
            if (strcmp(trackable.getName(), "huitailang") == 0||strcmp(trackable.getName(), "stones") == 0||strcmp(trackable.getName(), "chips") == 0)
            {
                modeltype = 2;
                textureIndex = 0;

                const Texture* const thisTexture = textures[textureIndex];

                QCAR::Matrix44F modelViewProjection;

                modelViewMatrix = QCAR::Tool::convertPose2GLMatrix(result->getPose());
                animateteapot(modelViewMatrix);
                //1.35f*120.0f
                SampleUtils::translatePoseMatrix(0.0f,-0.50f*120.0f,1.35f*120.0f,
                                                 &modelViewMatrix.data[0]);
                //-90.0f
                SampleUtils::rotatePoseMatrix(objectx,  objecty,0.0f, 0,
                                              &modelViewMatrix.data[0]);
                SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, kObjectScale,
                                             &modelViewMatrix.data[0]);
                SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
                                            &modelViewMatrix.data[0] ,
                                            &modelViewProjection.data[0]);




                glUseProgram(shaderProgramID);

                glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &teapotVerts[0]);
                glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &teapotNormals[0]);
                glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &teapotTexCoords[0]);

                glEnableVertexAttribArray(vertexHandle);
                glEnableVertexAttribArray(normalHandle);
                glEnableVertexAttribArray(textureCoordHandle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, thisTexture->mTextureID);
                glUniform1i(texSampler2DHandle, 0 );
                glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                                   (GLfloat*)&modelViewProjection.data[0] );
                glDrawArrays(GL_TRIANGLES, 0,teapotNumVerts);

                glDisableVertexAttribArray(vertexHandle);
                glDisableVertexAttribArray(normalHandle);
                glDisableVertexAttribArray(textureCoordHandle);

                SampleUtils::checkGlError("ImageTargets renderFrame");
                glDisable(GL_DEPTH_TEST);

                QCAR::Renderer::getInstance().end();
            }
            else if (strcmp(trackable.getName(), "heroin") == 0)
            {
                textureIndex = 0;
                modeltype = 1;

                const Texture* const thisTexture = textures[textureIndex];

                QCAR::Matrix44F modelViewProjection;

                modelViewMatrix = QCAR::Tool::convertPose2GLMatrix(result->getPose());
                animateteapot(modelViewMatrix);
                //1.35f*120.0f
                SampleUtils::translatePoseMatrix(0.0f,-0.50f*120.0f,1.35f*120.0f,
                                                 &modelViewMatrix.data[0]);
                //-90.0f
                SampleUtils::rotatePoseMatrix(objectx,  objecty,0.0f, 0,
                                              &modelViewMatrix.data[0]);
                SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, kObjectScale,
                                             &modelViewMatrix.data[0]);
                SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
                                            &modelViewMatrix.data[0] ,
                                            &modelViewProjection.data[0]);




                glUseProgram(shaderProgramID);

                glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &HeroinVerts[0]);
                glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &HeroinNormals[0]);
                glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &HeroinTexCoords[0]);

                glEnableVertexAttribArray(vertexHandle);
                glEnableVertexAttribArray(normalHandle);
                glEnableVertexAttribArray(textureCoordHandle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, thisTexture->mTextureID);
                glUniform1i(texSampler2DHandle, 0 );
                glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                                   (GLfloat*)&modelViewProjection.data[0] );
                glDrawArrays(GL_TRIANGLES, 0,HeroinNumVerts);

                glDisableVertexAttribArray(vertexHandle);
                glDisableVertexAttribArray(normalHandle);
                glDisableVertexAttribArray(textureCoordHandle);

                SampleUtils::checkGlError("ImageTargets renderFrame");
                glDisable(GL_DEPTH_TEST);

                QCAR::Renderer::getInstance().end();
            }
            else if (strcmp(trackable.getName(), "stones") == 0)
            {
                textureIndex = 2;
            }
            else if (strcmp(trackable.getName(), "chips") == 0)
            {
                ;
            }
            else if (strcmp(trackable.getName(), "chhhh") == 0)
            {
                textureIndex = 0;
                modeltype = 3;

                const Texture* const thisTexture = textures[textureIndex];

                QCAR::Matrix44F modelViewProjection;

                modelViewMatrix = QCAR::Tool::convertPose2GLMatrix(result->getPose());
                animateteapot(modelViewMatrix);
                //1.35f*120.0f
                SampleUtils::translatePoseMatrix(0.0f,-0.50f*120.0f,1.35f*120.0f,
                                                 &modelViewMatrix.data[0]);
                //-90.0f
                SampleUtils::rotatePoseMatrix(objectx,  objecty,0.0f, 0,
                                              &modelViewMatrix.data[0]);
                SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, kObjectScale,
                                             &modelViewMatrix.data[0]);
                SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
                                            &modelViewMatrix.data[0] ,
                                            &modelViewProjection.data[0]);




                glUseProgram(shaderProgramID);

                glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &chhhhVerts[0]);
                glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &chhhhNormals[0]);
                glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &chhhhTexCoords[0]);

                glEnableVertexAttribArray(vertexHandle);
                glEnableVertexAttribArray(normalHandle);
                glEnableVertexAttribArray(textureCoordHandle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, thisTexture->mTextureID);
                glUniform1i(texSampler2DHandle, 0 );
                glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                                   (GLfloat*)&modelViewProjection.data[0] );
                glDrawArrays(GL_TRIANGLES, 0,chhhhNumVerts);

                glDisableVertexAttribArray(vertexHandle);
                glDisableVertexAttribArray(normalHandle);
                glDisableVertexAttribArray(textureCoordHandle);

                SampleUtils::checkGlError("ImageTargets renderFrame");
                glDisable(GL_DEPTH_TEST);

                QCAR::Renderer::getInstance().end();
            }
            else if (strcmp(trackable.getName(), "hh") == 0)
            {
                textureIndex = 0;
                modeltype = 4;

                const Texture* const thisTexture = textures[textureIndex];

                QCAR::Matrix44F modelViewProjection;

                modelViewMatrix = QCAR::Tool::convertPose2GLMatrix(result->getPose());
                animateteapot(modelViewMatrix);
                //1.35f*120.0f
                SampleUtils::translatePoseMatrix(0.0f,-0.50f*120.0f,1.35f*120.0f,
                                                 &modelViewMatrix.data[0]);
                //-90.0f
                SampleUtils::rotatePoseMatrix(objectx,  objecty,0.0f, 0,
                                              &modelViewMatrix.data[0]);
                SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, kObjectScale,
                                             &modelViewMatrix.data[0]);
                SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
                                            &modelViewMatrix.data[0] ,
                                            &modelViewProjection.data[0]);




                glUseProgram(shaderProgramID);

                glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &hhVerts[0]);
                glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &hhNormals[0]);
                glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &hhTexCoords[0]);

                glEnableVertexAttribArray(vertexHandle);
                glEnableVertexAttribArray(normalHandle);
                glEnableVertexAttribArray(textureCoordHandle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, thisTexture->mTextureID);
                glUniform1i(texSampler2DHandle, 0 );
                glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                                   (GLfloat*)&modelViewProjection.data[0] );
                glDrawArrays(GL_TRIANGLES, 0,hhNumVerts);

                glDisableVertexAttribArray(vertexHandle);
                glDisableVertexAttribArray(normalHandle);
                glDisableVertexAttribArray(textureCoordHandle);

                SampleUtils::checkGlError("ImageTargets renderFrame");
                glDisable(GL_DEPTH_TEST);

                QCAR::Renderer::getInstance().end();
            }
            else if (strcmp(trackable.getName(), "coo") == 0)
            {
                textureIndex = 0;
                modeltype = 5;

                const Texture* const thisTexture = textures[textureIndex];

                QCAR::Matrix44F modelViewProjection;

                modelViewMatrix = QCAR::Tool::convertPose2GLMatrix(result->getPose());
                animateteapot(modelViewMatrix);
                //1.35f*120.0f
                SampleUtils::translatePoseMatrix(0.0f,-0.50f*120.0f,1.35f*120.0f,
                                                 &modelViewMatrix.data[0]);
                //-90.0f
                SampleUtils::rotatePoseMatrix(objectx,  objecty,0.0f, 0,
                                              &modelViewMatrix.data[0]);
                SampleUtils::scalePoseMatrix(kObjectScale, kObjectScale, kObjectScale,
                                             &modelViewMatrix.data[0]);
                SampleUtils::multiplyMatrix(&projectionMatrix.data[0],
                                            &modelViewMatrix.data[0] ,
                                            &modelViewProjection.data[0]);




                glUseProgram(shaderProgramID);

                glVertexAttribPointer(vertexHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &cooVerts[0]);
                glVertexAttribPointer(normalHandle, 3, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &cooNormals[0]);
                glVertexAttribPointer(textureCoordHandle, 2, GL_FLOAT, GL_FALSE, 0,
                                      (const GLvoid*) &cooTexCoords[0]);

                glEnableVertexAttribArray(vertexHandle);
                glEnableVertexAttribArray(normalHandle);
                glEnableVertexAttribArray(textureCoordHandle);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, thisTexture->mTextureID);
                glUniform1i(texSampler2DHandle, 0 );
                glUniformMatrix4fv(mvpMatrixHandle, 1, GL_FALSE,
                                   (GLfloat*)&modelViewProjection.data[0] );
                glDrawArrays(GL_TRIANGLES, 0,cooNumVerts);

                glDisableVertexAttribArray(vertexHandle);
                glDisableVertexAttribArray(normalHandle);
                glDisableVertexAttribArray(textureCoordHandle);

                SampleUtils::checkGlError("ImageTargets renderFrame");
                glDisable(GL_DEPTH_TEST);

                QCAR::Renderer::getInstance().end();
            }
            else
            {
                textureIndex = 6;
            }

        }

        /*glDisable(GL_DEPTH_TEST);

        QCAR::Renderer::getInstance().end();*/
    }


    void
    configureVideoBackground()
    {
        // Get the default video mode:
        QCAR::CameraDevice& cameraDevice = QCAR::CameraDevice::getInstance();
        QCAR::VideoMode videoMode = cameraDevice.
                                    getVideoMode(QCAR::CameraDevice::MODE_DEFAULT);


        // Configure the video background
        QCAR::VideoBackgroundConfig config;
        config.mEnabled = true;
        config.mSynchronous = true;
        config.mPosition.data[0] = 0.0f;
        config.mPosition.data[1] = 0.0f;

        if (isActivityInPortraitMode)
        {
            //LOG("configureVideoBackground PORTRAIT");
            config.mSize.data[0] = videoMode.mHeight
                                   * (screenHeight / (float)videoMode.mWidth);
            config.mSize.data[1] = screenHeight;

            if (config.mSize.data[0] < screenWidth)
            {
                LOG("Correcting rendering background size to handle missmatch between screen and video aspect ratios.");
                config.mSize.data[0] = screenWidth;
                config.mSize.data[1] = screenWidth *
                                       (videoMode.mWidth / (float)videoMode.mHeight);
            }
        }
        else
        {
            //LOG("configureVideoBackground LANDSCAPE");
            config.mSize.data[0] = screenWidth;
            config.mSize.data[1] = videoMode.mHeight
                                   * (screenWidth / (float)videoMode.mWidth);

            if (config.mSize.data[1] < screenHeight)
            {
                LOG("Correcting rendering background size to handle missmatch between screen and video aspect ratios.");
                config.mSize.data[0] = screenHeight
                                       * (videoMode.mWidth / (float)videoMode.mHeight);
                config.mSize.data[1] = screenHeight;
            }
        }

        LOG("Configure Video Background : Video (%d,%d), Screen (%d,%d), mSize (%d,%d)", videoMode.mWidth, videoMode.mHeight, screenWidth, screenHeight, config.mSize.data[0], config.mSize.data[1]);

        // Set the config:
        QCAR::Renderer::getInstance().setVideoBackgroundConfig(config);
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_initApplicationNative(
        JNIEnv* env, jobject obj, jint width, jint height)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_initApplicationNative");

        // Store screen dimensions
        screenWidth = width;
        screenHeight = height;

        // Handle to the activity class:
        jclass activityClass = env->GetObjectClass(obj);

        jmethodID getTextureCountMethodID = env->GetMethodID(activityClass,
                                            "getTextureCount", "()I");
        if (getTextureCountMethodID == 0)
        {
            LOG("Function getTextureCount() not found.");
            return;
        }

        textureCount = env->CallIntMethod(obj, getTextureCountMethodID);
        if (!textureCount)
        {
            LOG("getTextureCount() returned zero.");
            return;
        }

        textures = new Texture*[textureCount];

        jmethodID getTextureMethodID = env->GetMethodID(activityClass,
                                       "getTexture", "(I)Lcom/qualcomm/QCARSamples/ImageTargets/Texture;");

        if (getTextureMethodID == 0)
        {
            LOG("Function getTexture() not found.");
            return;
        }

        // Register the textures
        for (int i = 0; i < textureCount; ++i)
        {

            jobject textureObject = env->CallObjectMethod(obj, getTextureMethodID, i);
            if (textureObject == NULL)
            {
                LOG("GetTexture() returned zero pointer");
                return;
            }

            textures[i] = Texture::create(env, textureObject);
        }
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_initApplicationNative finished");
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_deinitApplicationNative(
        JNIEnv* env, jobject obj)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_deinitApplicationNative");

        // Release texture resources
        if (textures != 0)
        {
            for (int i = 0; i < textureCount; ++i)
            {
                delete textures[i];
                textures[i] = NULL;
            }

            delete[]textures;
            textures = NULL;

            textureCount = 0;
        }
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_startCamera(JNIEnv *,
            jobject)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_startCamera");

        // Select the camera to open, set this to QCAR::CameraDevice::CAMERA_FRONT
        // to activate the front camera instead.
        QCAR::CameraDevice::CAMERA camera = QCAR::CameraDevice::CAMERA_DEFAULT;

        // Initialize the camera:
        if (!QCAR::CameraDevice::getInstance().init(camera))
            return;

        // Configure the video background
        configureVideoBackground();

        // Select the default mode:
        if (!QCAR::CameraDevice::getInstance().selectVideoMode(
                    QCAR::CameraDevice::MODE_DEFAULT))
            return;

        // Start the camera:
        if (!QCAR::CameraDevice::getInstance().start())
            return;

        // Uncomment to enable flash
        //if(QCAR::CameraDevice::getInstance().setFlashTorchMode(true))
        //    LOG("IMAGE TARGETS : enabled torch");

        // Uncomment to enable infinity focus mode, or any other supported focus mode
        // See CameraDevice.h for supported focus modes
        //if(QCAR::CameraDevice::getInstance().setFocusMode(QCAR::CameraDevice::FOCUS_MODE_INFINITY))
        //    LOG("IMAGE TARGETS : enabled infinity focus");

        // Start the tracker:
        QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
        QCAR::Tracker* imageTracker = trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER);
        if (imageTracker != 0)
            imageTracker->start();
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_stopCamera(JNIEnv *, jobject)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_stopCamera");

        // Stop the tracker:
        QCAR::TrackerManager& trackerManager = QCAR::TrackerManager::getInstance();
        QCAR::Tracker* imageTracker = trackerManager.getTracker(QCAR::Tracker::IMAGE_TRACKER);
        if (imageTracker != 0)
            imageTracker->stop();

        QCAR::CameraDevice::getInstance().stop();
        QCAR::CameraDevice::getInstance().deinit();
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_setProjectionMatrix(JNIEnv *, jobject)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_setProjectionMatrix");

        // Cache the projection matrix:
        const QCAR::CameraCalibration& cameraCalibration =
            QCAR::CameraDevice::getInstance().getCameraCalibration();
        projectionMatrix = QCAR::Tool::getProjectionGL(cameraCalibration, 2.0f, 2500.0f);
    }

// ----------------------------------------------------------------------------
// Activates Camera Flash
// ----------------------------------------------------------------------------
    JNIEXPORT jboolean JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_activateFlash(JNIEnv*, jobject, jboolean flash)
    {
        return QCAR::CameraDevice::getInstance().setFlashTorchMode((flash==JNI_TRUE)) ? JNI_TRUE : JNI_FALSE;
    }

    JNIEXPORT jboolean JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_autofocus(JNIEnv*, jobject)
    {
        return QCAR::CameraDevice::getInstance().setFocusMode(QCAR::CameraDevice::FOCUS_MODE_TRIGGERAUTO) ? JNI_TRUE : JNI_FALSE;
    }


    JNIEXPORT jboolean JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargets_setFocusMode(JNIEnv*, jobject, jint mode)
    {
        int qcarFocusMode;

        switch ((int)mode)
        {
        case 0:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_NORMAL;
            break;

        case 1:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_CONTINUOUSAUTO;
            break;

        case 2:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_INFINITY;
            break;

        case 3:
            qcarFocusMode = QCAR::CameraDevice::FOCUS_MODE_MACRO;
            break;

        default:
            return JNI_FALSE;
        }

        return QCAR::CameraDevice::getInstance().setFocusMode(qcarFocusMode) ? JNI_TRUE : JNI_FALSE;
    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_initRendering(
        JNIEnv* env, jobject obj)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_initRendering");

        // Define clear color
        glClearColor(0.0f, 0.0f, 0.0f, QCAR::requiresAlpha() ? 0.0f : 1.0f);

        // Now generate the OpenGL texture objects and add settings
        for (int i = 0; i < textureCount; ++i)
        {
            glGenTextures(1, &(textures[i]->mTextureID));
            glBindTexture(GL_TEXTURE_2D, textures[i]->mTextureID);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textures[i]->mWidth,
                         textures[i]->mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                         (GLvoid*)  textures[i]->mData);
        }
#ifndef USE_OPENGL_ES_1_1

        shaderProgramID     = SampleUtils::createProgramFromBuffer(cubeMeshVertexShader,
                              cubeFragmentShader);

        vertexHandle        = glGetAttribLocation(shaderProgramID,
                              "vertexPosition");
        normalHandle        = glGetAttribLocation(shaderProgramID,
                              "vertexNormal");
        textureCoordHandle  = glGetAttribLocation(shaderProgramID,
                              "vertexTexCoord");
        mvpMatrixHandle     = glGetUniformLocation(shaderProgramID,
                              "modelViewProjectionMatrix");
        texSampler2DHandle  = glGetUniformLocation(shaderProgramID,
                              "texSampler2D");

#endif

    }


    JNIEXPORT void JNICALL
    Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_updateRendering(
        JNIEnv* env, jobject obj, jint width, jint height)
    {
        LOG("Java_com_qualcomm_QCARSamples_ImageTargets_ImageTargetsRenderer_updateRendering");

        // Update screen dimensions
        screenWidth = width;
        screenHeight = height;

        // Reconfigure the video background
        configureVideoBackground();
    }

    double
    getCurrentTime()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        double t = tv.tv_sec + tv.tv_usec/1000000.0;
        return t;
    }

    void
    animateteapot(QCAR::Matrix44F& modelViewMatrix)
    {
        static float rotateBowlAngle = 0.0f;

        static double prevTime = getCurrentTime();
        double time = getCurrentTime();             // Get real time difference
        float dt = (float)(time-prevTime);          // from frame to frame

        rotateBowlAngle += dt * 180.0f/3.1415f;     // Animate angle based on time

        if (isRotate == true)
            //第二个参数为半径

            SampleUtils::rotatePoseMatrix(rotateBowlAngle, 0.0f, 1.0f, 0.0f,
                                          &modelViewMatrix.data[0]);

        prevTime = time;
    }

    /*void
    animateteapotx(QCAR::Matrix44F& modelViewMatrix,float x){
    	SampleUtils::rotatePoseMatrix(x, 0.0f, 1.0f, 0.0f,
    	                                  &modelViewMatrix.data[0]);
    }

    void
    animateteapoty(QCAR::Matrix44F& modelViewMatrix,float y){
    	SampleUtils::rotatePoseMatrix(0.0f, y, 1.0f, 0.0f,
    	                                  &modelViewMatrix.data[0]);
    }*/


#ifdef __cplusplus
}
#endif
