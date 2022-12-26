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
import androidx.fragment.app.FragmentManager
import androidx.lifecycle.DefaultLifecycleObserver
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleOwner
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
}

interface LayoutInflaterProvider {
    fun get(): LayoutInflater
}

class FragmentViewBindingLazy<VB : ViewBinding>(
    private val viewBingClass: Class<VB>,
    private val fragment: Fragment,
    private val layoutInflaterProvider: LayoutInflaterProvider
) : Lazy<VB> {
    private var cached: VB? = null

    init {
        val listener = object :
            FragmentManager.FragmentLifecycleCallbacks() {
            override fun onFragmentViewDestroyed(fm: FragmentManager, f: Fragment) {
                super.onFragmentViewDestroyed(fm, f)
                if (f == fragment) {
                    cached = null
                }
            }
        }
        fragment.lifecycle.addObserver(object : DefaultLifecycleObserver {
            override fun onCreate(owner: LifecycleOwner) {
                super.onCreate(owner)
                fragment.parentFragmentManager.registerFragmentLifecycleCallbacks(listener, false)
            }

            override fun onDestroy(owner: LifecycleOwner) {
                super.onDestroy(owner)
                fragment.parentFragmentManager.unregisterFragmentLifecycleCallbacks(listener)
            }
        })
    }

    @Suppress("UNCHECKED_CAST")
    override val value: VB
        get() {
            check(fragment.viewLifecycleOwner.lifecycle.currentState.isAtLeast(Lifecycle.State.INITIALIZED)) {
                "cannot use binding in before onCreateView() or after onDestroyView() from 1.1.4. about [issue](https://github.com/hi-dhl/Binding/issues/31#issuecomment-1109733307)"
            }
            return cached ?: kotlin.run {
                val inflateMethod: Method = viewBingClass.getDeclaredMethod("inflate", LayoutInflater::class.java)
                (inflateMethod.invoke(null, layoutInflaterProvider.get()) as VB).also {
                    cached = it
                }
            }
        }

    override fun isInitialized() = cached != null
}

@MainThread
inline fun <reified VB : ViewBinding> Fragment.viewBinding(): Lazy<VB> {
    return FragmentViewBindingLazy(VB::class.java, this, object : LayoutInflaterProvider {
        override fun get(): LayoutInflater {
            return layoutInflater
        }
    })
}

@MainThread
inline fun <reified VB : ViewBinding> Activity.viewBinding(): Lazy<VB> {
    return ViewBindingLazy(VB::class.java, object : LayoutInflaterProvider {
        override fun get(): LayoutInflater {
            return layoutInflater
        }
    })
}