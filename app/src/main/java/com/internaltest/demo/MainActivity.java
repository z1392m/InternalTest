package com.internaltest.demo;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class MainActivity extends Activity {
    static { System.loadLibrary("internaltest"); }
    private static native void init();
    private static native void resize(int width, int height);
    private static native void frame();
    private static native void touch(float x, float y, int action);
    private GLSurfaceView surface;
    private int pointer = -1;

    @Override public void onCreate(Bundle state) {
        super.onCreate(state);
        surface = new GLSurfaceView(this);
        surface.setEGLContextClientVersion(3);
        surface.setPreserveEGLContextOnPause(true);
        surface.setRenderer(new GLSurfaceView.Renderer() {
            public void onSurfaceCreated(GL10 gl, EGLConfig config) { init(); }
            public void onSurfaceChanged(GL10 gl, int width, int height) { resize(width, height); }
            public void onDrawFrame(GL10 gl) { frame(); }
        });
        surface.setOnTouchListener((v, event) -> {
            final int action = event.getActionMasked();
            final int index = event.getActionIndex();
            if (action == MotionEvent.ACTION_DOWN) {
                pointer = event.getPointerId(index);
            }
            if (action == MotionEvent.ACTION_CANCEL) {
                pointer = -1;
                surface.queueEvent(() -> touch(0, 0, 3));
                return true;
            }
            final int active = event.findPointerIndex(pointer);
            if (active < 0) return true;
            final float x = event.getX(active), y = event.getY(active);
            if (action == MotionEvent.ACTION_DOWN || action == MotionEvent.ACTION_MOVE) {
                surface.queueEvent(() -> touch(x, y, action));
            } else if (action == MotionEvent.ACTION_UP ||
                      (action == MotionEvent.ACTION_POINTER_UP && index == active)) {
                pointer = -1;
                surface.queueEvent(() -> touch(x, y, 1));
            }
            return true;
        });
        setContentView(surface);
    }

    @Override protected void onResume() { super.onResume(); surface.onResume(); }
    @Override protected void onPause() {
        pointer = -1;
        surface.queueEvent(() -> touch(0, 0, 3));
        surface.onPause();
        super.onPause();
    }
}
