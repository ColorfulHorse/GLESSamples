package com.greensun.glsample.render

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

/**
 * @author greensun
 * @since 2022/9/30
 * blog https://juejin.cn/user/3263006244363095
 */
class BaseRenderer(val type: Int, private val surfaceView: MyGLSurfaceView): GLSurfaceView.Renderer {
    private val mNativePtr: Long

    companion object {
        private external fun nativeInit()

        init {
            nativeInit()
        }

        const val RENDER_TYPE = 100
        const val RENDER_TRIANGLE = RENDER_TYPE + 1
        const val RENDER_RIPPLE = RENDER_TYPE + 2
        const val RENDER_HEART = RENDER_TYPE + 3
        const val RENDER_TEXT = RENDER_TYPE + 4
    }

    init {
        mNativePtr = nativeCreate(type)
    }

    override fun onSurfaceCreated(gl: GL10, config: EGLConfig) {
        nativeOnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10, width: Int, height: Int) {
        nativeOnSurfaceChanged(width, height)
    }

    override fun onDrawFrame(gl: GL10) {
        nativeOnDrawFrame()
    }

    fun onPaused() {
        nativeOnSurfaceDestroyed()
    }

    fun onDestroyed() {
        nativeDestroy()
    }

    fun onTouch(x: Float, y: Float) {
        nativeTouch(x, y)
    }

    fun resetSurfaceSize(width: Int, height: Int) {
        surfaceView.resetSurfaceSize(width, height)
    }

    private external fun nativeCreate(type: Int): Long

    private external fun nativeDestroy()

    private external fun nativeOnSurfaceCreated()

    private external fun nativeOnSurfaceDestroyed()

    private external fun nativeOnSurfaceChanged(width: Int, height: Int)

    private external fun nativeOnDrawFrame()

    private external fun nativeTouch(x: Float, y: Float)
}