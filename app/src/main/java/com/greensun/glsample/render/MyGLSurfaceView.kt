package com.greensun.glsample.render

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent

/**
 * @author greensun
 * @since 2022/10/17
 * blog https://juejin.cn/user/3263006244363095
 */
class MyGLSurfaceView(context: Context?, attrs: AttributeSet?) : GLSurfaceView(context, attrs) {

    private var baseRenderer: BaseRenderer? = null

    private var mRatioWidth = 0
    private var mRatioHeight = 0

    override fun setRenderer(renderer: Renderer) {
        super.setRenderer(renderer)
        baseRenderer = renderer as BaseRenderer
    }

    override fun onTouchEvent(event: MotionEvent): Boolean {
        when(event.action) {
            MotionEvent.ACTION_DOWN -> baseRenderer?.onTouch(event.x, event.y)
        }
        return super.onTouchEvent(event)
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val width = MeasureSpec.getSize(widthMeasureSpec)
        val height = MeasureSpec.getSize(heightMeasureSpec)

        if (0 == mRatioWidth || 0 == mRatioHeight) {
            setMeasuredDimension(width, height)
        } else {
            if (width < height * mRatioWidth / mRatioHeight) {
                setMeasuredDimension(width, width * mRatioHeight / mRatioWidth)
            } else {
                setMeasuredDimension(height * mRatioWidth / mRatioHeight, height)
            }
        }
    }

    fun resetSurfaceSize(width: Int, height: Int) {
        post {
            mRatioWidth = width
            mRatioHeight = height
            requestLayout()
        }
    }
}