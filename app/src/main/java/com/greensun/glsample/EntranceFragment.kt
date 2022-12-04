package com.greensun.glsample

import android.os.Bundle
import androidx.fragment.app.Fragment
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.os.bundleOf
import androidx.navigation.fragment.findNavController
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import androidx.recyclerview.widget.RecyclerView.ViewHolder
import com.example.glsample.R
import com.example.glsample.databinding.FragmentEntranceBinding
import com.example.glsample.databinding.ItemListBinding
import com.greensun.glsample.render.BaseRenderer

class EntranceFragment : Fragment() {

    private val mViewBinding by viewBinding<FragmentEntranceBinding>()

    val data = listOf(
        "三角形" to BaseRenderer.RENDER_TRIANGLE,
        "水波纹" to BaseRenderer.RENDER_RIPPLE,
        "爱心" to BaseRenderer.RENDER_HEART,
        "文字" to BaseRenderer.RENDER_TEXT
    )

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View {
        return mViewBinding.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)
        mViewBinding.recyclerView.adapter = object : RecyclerView.Adapter<Holder>() {
            override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): Holder {
                val itemBinding = ItemListBinding.inflate(layoutInflater, parent, false)
                return Holder(itemBinding)
            }

            override fun onBindViewHolder(holder: Holder, position: Int) {
                val item = data[position]
                holder.binding.run {
                    tv.text = item.first
                    tv.setOnClickListener {
                        findNavController()
                            .navigate(
                                R.id.action_entranceFragment_to_main_fragment,
                                bundleOf(
                                    "type" to item.second
                                )
                            )
                    }
                }
            }

            override fun getItemCount(): Int {
                return data.size
            }

        }

        mViewBinding.recyclerView.layoutManager = LinearLayoutManager(requireContext())
    }

    class Holder(val binding: ItemListBinding) : ViewHolder(binding.root)
}