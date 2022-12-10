package com.greensun.glsample

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import com.example.glsample.databinding.FragmentRenderBinding
import com.greensun.glsample.render.BaseRenderer

class RenderFragment : Fragment() {

    val viewBinding by viewBinding<FragmentRenderBinding>()

    private var renderer: BaseRenderer? = null

    private var type = BaseRenderer.RENDER_TRIANGLE

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        arguments?.run {
            type = getInt("type", BaseRenderer.RENDER_TRIANGLE)
        }
        return viewBinding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        viewBinding.glView.run {
            setEGLContextClientVersion(3)
            renderer = BaseRenderer(type, this)
            setRenderer(renderer!!)
        }
    }

    override fun onStart() {
        super.onStart()
        viewBinding.glView.onResume()
    }

    override fun onStop() {
        renderer?.onPaused()
        viewBinding.glView.onPause()
        super.onStop()
    }

    override fun onDestroy() {
        renderer?.onDestroyed()
        renderer = null
        super.onDestroy()
    }
}