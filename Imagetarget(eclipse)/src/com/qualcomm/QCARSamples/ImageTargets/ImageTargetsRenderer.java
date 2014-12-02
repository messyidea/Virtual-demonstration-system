/*==============================================================================
Copyright (c) 2010-2013 QUALCOMM Austria Research Center GmbH.
All Rights Reserved.

@file
    ImageTargetsRenderer.java

@brief
    Sample for ImageTargets

==============================================================================*/


package com.qualcomm.QCARSamples.ImageTargets;

import java.io.File;
import java.io.FileOutputStream;
import java.nio.IntBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.graphics.Bitmap;
import android.graphics.Matrix;
import android.graphics.Bitmap.CompressFormat;
import android.opengl.GLES20;
import android.opengl.GLSurfaceView;
import android.os.Environment;

import com.qualcomm.QCAR.QCAR;


/** The renderer class for the ImageTargets sample. */
public class ImageTargetsRenderer implements GLSurfaceView.Renderer
{
    public boolean mIsActive = false;
    private int mViewWidth = 0;
    private int mViewHeight = 0;
    public int numm = 0;

    /** Reference to main activity **/
    public ImageTargets mActivity;

    /** Native function for initializing the renderer. */
    public native void initRendering();

    /** Native function to update the renderer. */
    public native void updateRendering(int width, int height);
    
    public void saveScreenShot(int x, int y, int w, int h, String filename) {
        Bitmap bmp = grabPixels(x, y, w, h);
        try {
            String path = Environment.getExternalStorageDirectory() + "/tmp/" + filename;
            DebugLog.LOGD(path);
            File file = new File(path);
            file.createNewFile();
             
            FileOutputStream fos = new FileOutputStream(file);
            bmp.compress(CompressFormat.PNG, 100, fos);
  
            fos.flush();
             
            fos.close();
             
        } catch (Exception e) {
            DebugLog.LOGD(e.getStackTrace().toString());
        }
    }
  
    private Bitmap grabPixels(int x, int y, int w, int h) {
        int b[] = new int[w * (y + h)];
        int bt[] = new int[w * h];
        IntBuffer ib = IntBuffer.wrap(b);
        ib.position(0);
         
        GLES20.glReadPixels(x, 0, w, y + h, 
                   GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, ib);
  
        for (int i = 0, k = 0; i < h; i++, k++) {
            for (int j = 0; j < w; j++) {
                int pix = b[i * w + j];
                int pb = (pix >> 16) & 0xff;
                int pr = (pix << 16) & 0x00ff0000;
                int pix1 = (pix & 0xff00ff00) | pr | pb;
                bt[(h - k - 1) * w + j] = pix1;
            }
        }
  
        Bitmap sb = Bitmap.createBitmap(bt, w, h, Bitmap.Config.ARGB_8888);
        return sb;
    }


    /** Called when the surface is created or recreated. */
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        DebugLog.LOGD("GLRenderer::onSurfaceCreated");

        // Call native function to initialize rendering:
        initRendering();

        // Call QCAR function to (re)initialize rendering after first use
        // or after OpenGL ES context was lost (e.g. after onPause/onResume):
        QCAR.onSurfaceCreated();
    }


    /** Called when the surface changed size. */
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        DebugLog.LOGD("GLRenderer::onSurfaceChanged");

        // Call native function to update rendering when render surface
        // parameters have changed:
        updateRendering(width, height);
        mViewWidth = width;
        mViewHeight = height;

        // Call QCAR function to handle render surface size changes:
        QCAR.onSurfaceChanged(width, height);
    }
    public void setnumm(int a){
    	numm = a;
    }


    /** The native render function. */
    public native void renderFrame();
    
    public static String Number() {  
        String str="";  
        for (int i = 1; i <= 4; i++) {  
            char ca=(char) (Math.random()*9+48);  
            str +=ca+"";  
        }  
        return str;  
    }  
  
    /** Called to draw the current frame. */
    public void onDrawFrame(GL10 gl)
    {
        if (!mIsActive)
            return;

        // Update render view (projection matrix and viewport) if needed:
        mActivity.updateRenderView();
       
        // Call our native function to render content
        renderFrame();
        GLES20.glFinish();
        
        if ( numm == 1 ) {
            saveScreenShot(0, 0, mViewWidth, mViewHeight, Number()+".png");
            System.out.println("aaa");
            numm = numm -1;
        }
    }
}
