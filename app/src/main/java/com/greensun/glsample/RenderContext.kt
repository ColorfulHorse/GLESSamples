package com.greensun.glsample

import android.content.Context
import android.content.res.AssetManager

/**
 * @author greensun
 * @since 2022/10/8
 * blog https://juejin.cn/user/3263006244363095
 */
object RenderContext {

    @JvmStatic
    fun initialize(context: Context) {
        setAssetManager(context.resources.assets)
    }

    @JvmStatic
    fun release() {
        nativeRelease()
    }

    @JvmStatic
    private external fun setAssetManager(manager: AssetManager)

    @JvmStatic
    private external fun nativeRelease()
}