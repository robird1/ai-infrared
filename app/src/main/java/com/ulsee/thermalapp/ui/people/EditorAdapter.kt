package com.ulsee.thermalapp.ui.people

import android.content.Context
import android.text.Editable
import android.text.TextWatcher
import android.util.Log
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ArrayAdapter
import android.widget.AutoCompleteTextView
import androidx.recyclerview.widget.RecyclerView
import com.google.android.material.datepicker.MaterialDatePicker
import com.google.android.material.textfield.TextInputLayout
import com.ulsee.thermalapp.R
import java.text.SimpleDateFormat
import java.util.*


private val TAG = EditorAdapter::class.java.simpleName

class EditorAdapter(private val context: Context, private val isEditingMode: Boolean): RecyclerView.Adapter<RecyclerView.ViewHolder>() {

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): RecyclerView.ViewHolder {
        val holder: RecyclerView.ViewHolder
        val layoutInflater = LayoutInflater.from(parent.context)
        holder = when (viewType) {
//            AttributeType.FACE.viewType -> {
//                FaceViewHolder(layoutInflater.inflate(AttributeType.FACE.viewType, parent, false))
//            }
            AttributeType.GENDER.viewType -> {
                GenderViewHolder(
                    layoutInflater.inflate(
                        AttributeType.GENDER.viewType,
                        parent,
                        false
                    )
                )
            }
            else -> {
                TextViewHolder(layoutInflater.inflate(AttributeType.NAME.viewType, parent, false))
            }
        }
        return holder
    }

    override fun getItemCount(): Int = AttributeType.values().size

    override fun onBindViewHolder(holder: RecyclerView.ViewHolder, position: Int) {
//        if (position == AttributeType.FACE.position) {
//            (holder as FaceViewHolder).bind(position)
//        }
        if (position == AttributeType.GENDER.position) {
            (holder as GenderViewHolder).bind(position)
        } else {
            (holder as TextViewHolder).bind(position)
        }
    }

    override fun getItemViewType(position: Int): Int {
        return AttributeType.fromPosition(position).viewType
    }


    inner class TextViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        val textInputLayout = itemView.findViewById<TextInputLayout>(R.id.textInputLayout_name)

        fun bind(position: Int) {
            textInputLayout.hint = itemView.context.getString(AttributeType.fromPosition(position).fieldName)

            if (position == AttributeType.NAME.position) {
                textInputLayout.helperText = itemView.context.getString(R.string.activity_editor_required)

            } else if (position == AttributeType.BIRTHDAY.position) {
                textInputLayout.editText?.isCursorVisible = false
                textInputLayout.editText?.isFocusableInTouchMode = false
                textInputLayout.editText?.setOnClickListener {
                    showDatePicker(position)
                }
            }

            if (isEditingMode) {
                textInputLayout.editText?.setText(AttributeType.fromPosition(position).inputValue)
                if (position == AttributeType.AGE.position) {
                    if (AttributeType.AGE.inputValue == "0") {
                        textInputLayout.editText?.setText("")
                    }
                }
            }

            textInputLayout.editText?.addTextChangedListener(object : TextWatcher {
                override fun beforeTextChanged(
                    charSequence: CharSequence,
                    i: Int,
                    i1: Int,
                    i2: Int
                ) {
                }

                override fun onTextChanged(charSequence: CharSequence, i: Int, i1: Int, i2: Int) {}
                override fun afterTextChanged(editable: Editable) {
                    Log.d(TAG, "[Enter] afterTextChanged")
                    AttributeType.fromPosition(position).inputValue = editable.toString()

                    if (position == AttributeType.NAME.position) {
                        AttributeType.fromPosition(position).isInputValid =
                            editable.toString().isNotEmpty()

                    } else if (position == AttributeType.AGE.position) {
                        if (!isAgeValid()) {
                            textInputLayout.error = itemView.context.getString(R.string.activity_editor_invalid_age)
                            AttributeType.fromPosition(position).isInputValid = false

                        } else {
                            textInputLayout.isErrorEnabled = false
                            AttributeType.fromPosition(position).isInputValid = true
                        }

                    } else {
                        AttributeType.fromPosition(position).isInputValid = true
                    }
                }
            })
        }

        private fun showDatePicker(position: Int) {
            val builder = MaterialDatePicker.Builder.datePicker()
//            if (isEditingMode) {
//                var date: Date
//                val pattern = "yyyy/MM/dd"
//                val simpleDateFormat = SimpleDateFormat(pattern,  Locale.getDefault())
//                date = try {
//                    simpleDateFormat.parse(AttributeType.fromPosition(position).inputValue)
//                } catch (e: Exception) {
//                    simpleDateFormat.parse("1970/01/01")
//                }
////
//                val constraintsBuilder = CalendarConstraints.Builder()//
//                val cal = Calendar.getInstance(TimeZone.getTimeZone("UTC"))
//                cal.time = date
//                val openAt = cal.time.time
////                val local = LocalDateTime.of(2020, 11, 1, 0, 0)
////                val openAt =
////                    local.atZone(ZoneId.ofOffset("UTC", ZoneOffset.UTC)).toInstant().toEpochMilli()
//                constraintsBuilder.setOpenAt(openAt)
//                builder.setCalendarConstraints(constraintsBuilder.build())
//                builder.setSelection(cal.time.time)
//            }
            val picker = builder.build()
            picker.show((context as EditorActivity).supportFragmentManager, "date_picker_tag")
            picker.addOnPositiveButtonClickListener {
                Log.d(TAG, "Date String = ${picker.headerText}:: Date epoch value = ${it}")

                val dateString = SimpleDateFormat("yyyy/MM/dd").format(Date(it))
                textInputLayout.editText?.setText(dateString)
            }
        }

        private fun isAgeValid(): Boolean {
            val inputString = textInputLayout.editText?.text.toString().trim()
            if (inputString.isEmpty())
                return true

            try {
                inputString.toInt()
            } catch (e: Exception) {
                return false
            }
            return inputString.toInt() in 1..115
        }

    }


    inner class GenderViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
        private val textInputLayout =
            itemView.findViewById<TextInputLayout>(R.id.textInputLayout4_gender)

        fun bind(position: Int) {

            if (isEditingMode) {
                textInputLayout.editText?.setText(AttributeType.fromPosition(position).inputValue)
            }

            setGenderAdapter()


            textInputLayout.editText?.addTextChangedListener(object : TextWatcher {
                override fun beforeTextChanged(
                    charSequence: CharSequence,
                    i: Int,
                    i1: Int,
                    i2: Int
                ) {
                }

                override fun onTextChanged(charSequence: CharSequence, i: Int, i1: Int, i2: Int) {}
                override fun afterTextChanged(editable: Editable) {
                    AttributeType.fromPosition(position).inputValue = editable.toString()
                }
            })
        }

        private fun setGenderAdapter() {
            val items = listOf(itemView.context.getString(R.string.activity_editor_male), itemView.context.getString(R.string.activity_editor_female))
            val adapter = ArrayAdapter(context, R.layout.list_item_sex, items)
            (textInputLayout.editText as? AutoCompleteTextView)?.setAdapter(adapter)
        }
    }

//    inner class FaceViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
//        private val faceImage = itemView.findViewById<ImageView>(R.id.face_image)
//        private val faceEdit = itemView.findViewById<ImageView>(R.id.button_edit)
//
//
//        fun bind(position: Int) {
////            if (isEditingMode) {
//            val bm = Base64.decode(
//                AttributeType2.fromPosition(position).inputValue,
//                Base64.DEFAULT
//            )
//            Glide.with(context).load(bm).into(faceImage)
////            }
//            faceImage.setOnClickListener {
//                (context as EditorActivity2).pickImageFromTakePhoto()
//            }
//        }
//    }


}
