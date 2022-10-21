package com.greensun.glsample

/**
 * @author greensun
 * @since 2022/9/29
 * blog https://juejin.cn/user/3263006244363095
 */

import android.app.Activity
import android.view.LayoutInflater
import androidx.annotation.MainThread
import androidx.fragment.app.Fragment
import androidx.viewbinding.ViewBinding
import java.lang.reflect.Method

class ViewBindingLazy<VB : ViewBinding>(
    private val viewBingClass: Class<VB>,
    private val layoutInflaterProvider: LayoutInflaterProvider
) : Lazy<VB> {
    private var cached: VB? = null

    @Suppress("UNCHECKED_CAST")
    override val value: VB
        get() {
            return cached ?: kotlin.run {
                val inflateMethod: Method = viewBingClass.getDeclaredMethod("inflate", LayoutInflater::class.java)
                (inflateMethod.invoke(null, layoutInflaterProvider.get()) as VB).also {
                    cached = it
                }
            }
        }

    override fun isInitialized() = cached != null

    interface LayoutInflaterProvider {
        fun get(): LayoutInflater
    }
}

@MainThread
inline fun <reified VB : ViewBinding> Fragment.viewBinding(): Lazy<VB> {
    return ViewBindingLazy(VB::class.java, object : ViewBindingLazy.LayoutInflaterProvider {
        override fun get(): LayoutInflater {
            return layoutInflater
        }
    })
}

@MainThread
inline fun <reified VB : ViewBinding> Activity.viewBinding(): Lazy<VB> {
    return ViewBindingLazy(VB::class.java, object : ViewBindingLazy.LayoutInflaterProvider {
        override fun get(): LayoutInflater {
            return layoutInflater
        }
    })
}