package com.ulsee.thermalapp.ui.notification

import android.app.TimePickerDialog
import android.os.Bundle
import android.util.Log
import android.widget.*
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import com.google.android.material.chip.ChipGroup
import com.google.android.material.datepicker.MaterialDatePicker
import com.ulsee.thermalapp.R
import com.ulsee.thermalapp.data.model.FilteredRecord
import java.text.SimpleDateFormat
import java.util.*


private val TAG = RecordFilterActivity::class.java.simpleName

class RecordFilterActivity : AppCompatActivity() {
    private lateinit var editName: EditText
    private lateinit var editDate: EditText
    private lateinit var editStartTime: EditText
    private lateinit var editEndTime: EditText
    private lateinit var editTempMin: EditText
    private lateinit var editTempMax: EditText
    private lateinit var groupTempUnit: ChipGroup
    private lateinit var groupMask: ChipGroup
    private val temperatureUnit: TemperatureUnit
        get() {
            return when(groupTempUnit.checkedChipId) {
                R.id.chip_fahrenheit ->
                    TemperatureUnit.FAHRENHEIT
                else ->
                    TemperatureUnit.CELSIUS
            }
        }
    private var startDate = -1L
    private var endDate = -1L
    private var startTime = -1L
    private var endTime = -1L
    private var tempMin = -1F
    private var tempMax = -1F
    private var tempUnit = TemperatureUnit.CELSIUS.apiValue     // 0-> celsius, 1-> Fahrenheit
    private var faceMask = FaceMask.ALL.apiValue                // 0-> No, 1-> Yes, others -> all


    enum class TemperatureUnit(
        val unit: String,
        val pickerMin: Int,
        val pickerMax: Int,
        val apiValue: Int
    ) {
        CELSIUS("°C", 33, 42, 0) ,
        FAHRENHEIT("°F", 92, 108, 1)
    }

    enum class FaceMask(val apiValue: Int) {
        NO(0), YES(1), ALL(2)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_notification_filter)
        initNameField()
        initDatePicker()
        initTimePicker()
        initTemperatureUnit()
        initTemperaturePicker()
        initFaceMask()
        initSaveButton()
    }

    private fun initSaveButton() {
        findViewById<Button>(R.id.save_btn).setOnClickListener {
            if (!isFilterShouldApply()) {
                setResult(RESULT_CANCELED, intent)
                finish()
            }

            val isDateValid = checkDateInput()
            val isTemperatureValid = checkTemperatureInput()
            if (isDateValid && isTemperatureValid) {
                intent.putExtra(ListFragment.KEY_INTENT_FILTER, configBundleData())
                setResult(RESULT_OK, intent)
                finish()
            } else {
                if (!isDateValid) {
                    Toast.makeText(this, getString(R.string.record_filter_error_date), Toast.LENGTH_SHORT)
                        .show()
                }
                if (!isTemperatureValid) {
                    Toast.makeText(this, getString(R.string.record_filter_error_temperature), Toast.LENGTH_SHORT)
                        .show()
                }
            }
        }
    }

    private fun configBundleData(): FilteredRecord {
        val data = FilteredRecord()
        if (editDate.text.toString().isNotEmpty()) {
            data.DateTimeStart = SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(startDate + startTime)
            data.DateTimeEnd = SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(endDate + endTime)
        } else {
            data.DateTimeStart = ""
            data.DateTimeEnd = ""
        }
        data.TempUnit = tempUnit
        data.TempMax = tempMax
        data.TempMin = tempMin
        data.Name = editName.text.toString()
        data.Mask = faceMask
        return data
    }

    private fun isFilterShouldApply(): Boolean {
        return editName.text.toString().isNotEmpty() || editDate.text.toString().isNotEmpty() || editStartTime.text.toString().isNotEmpty() ||
                editEndTime.text.toString().isNotEmpty() || editTempMin.text.toString().isNotEmpty() ||
                editTempMax.text.toString().isNotEmpty() || (faceMask != FaceMask.ALL.apiValue)
    }

    /**
     * 合法的輸入為下列任一者: 1. 所有的日期時間欄位均有填 2. 所有的日期時間欄位均沒填
     */
    private fun checkDateInput(): Boolean {
        val condition1 = (startDate != -1L) && (endDate != -1L) && (startTime != -1L) && (endTime != -1L)
        val condition2 = (startDate == -1L) && (endDate == -1L) && (startTime == -1L) && (endTime == -1L)
        return condition1 || condition2
    }

    private fun checkTemperatureInput(): Boolean {
        if ((tempMin != -1F) && (tempMax != -1F)) {
            return tempMin < tempMax
        }
        return true
    }

    private fun clearNameFieldFocus() {
        editName.clearFocus()
    }

    private fun initNameField() {
        editName = findViewById(R.id.editTextTextPersonName)
    }

    private fun initFaceMask() {
        groupMask = findViewById(R.id.chipGroup_mask)
        groupMask.setOnCheckedChangeListener { _, checkedId ->
            when (checkedId) {
                R.id.mask_no -> faceMask = FaceMask.NO.apiValue
                R.id.mask_yes -> faceMask = FaceMask.YES.apiValue
                R.id.mask_all -> faceMask = FaceMask.ALL.apiValue
            }
            clearNameFieldFocus()
        }
    }

    private fun initTemperatureUnit() {
        groupTempUnit = findViewById(R.id.chipGroup_temp)
        groupTempUnit.setOnCheckedChangeListener { _, checkedId ->
            editTempMin.setText("")
            editTempMax.setText("")
            when (checkedId) {
                R.id.chip_celsius -> tempUnit = TemperatureUnit.CELSIUS.apiValue
                R.id.chip_fahrenheit -> tempUnit = TemperatureUnit.FAHRENHEIT.apiValue
            }
            clearNameFieldFocus()
        }
    }

    private fun initTemperaturePicker() {
        editTempMin = findViewById(R.id.editTextMinTemp)
        editTempMax = findViewById(R.id.editTextMaxTemp)
        editTempMin.setOnClickListener { showNumberPicker(editTempMin, true); clearNameFieldFocus() }
        editTempMax.setOnClickListener { showNumberPicker(editTempMax, false); clearNameFieldFocus() }
    }

    private fun initTimePicker() {
        editStartTime = findViewById(R.id.editText_startTime)
        editEndTime = findViewById(R.id.editText_endTime)
        editStartTime.setOnClickListener { showTimePicker(editStartTime, true); clearNameFieldFocus() }
        editEndTime.setOnClickListener { showTimePicker(editEndTime, false); clearNameFieldFocus() }
    }

    private fun initDatePicker() {
        editDate = findViewById(R.id.editTextDate)
        editDate.setOnClickListener { showDatePicker(); clearNameFieldFocus() }
        findViewById<ImageView>(R.id.date_icon).setOnClickListener { showDatePicker(); clearNameFieldFocus() }
    }

    private fun showTimePicker(editTime: EditText, isStartTime: Boolean) {
        // Get Current Time
        val c = Calendar.getInstance()
        val hour = c[Calendar.HOUR_OF_DAY]
        val currentMinute = c[Calendar.MINUTE]

        val listener =
            TimePickerDialog.OnTimeSetListener { _, hourOfDay, selectedMinute ->
                editTime.setText(String.format("%02d:%02d", hourOfDay, selectedMinute))

                if (isStartTime) {
                    startTime = hourOfDay * 3600000L + selectedMinute * 60000
                } else {
                    endTime = hourOfDay * 3600000L + selectedMinute * 60000
                }
            }
        TimePickerDialog(this, listener, hour, currentMinute, false).show()
    }

    private fun showDatePicker() {
        val builder = MaterialDatePicker.Builder.dateRangePicker()
        val picker = builder.build()
        picker.show(supportFragmentManager, "date_picker_tag")
        picker.addOnPositiveButtonClickListener {
//            Log.d(TAG, "Date String = ${picker.headerText}:: Date epoch value = $it")
            val start = SimpleDateFormat("yyyy/MM/dd").format(Date(it.first!!))
            val end = SimpleDateFormat("yyyy/MM/dd").format(Date(it.second!!))
            editDate.setText("$start - $end")

            // TODO
            startDate = it.first!! - 8*3600000L
            endDate = it.second!! - 8*3600000L
        }
    }

    private fun showNumberPicker(editText: EditText, isTempMin: Boolean) {
        val numberPicker = NumberPicker(this)
        val valuesArray: Array<String> = if (tempUnit == TemperatureUnit.CELSIUS.apiValue) {
            resources.getStringArray(R.array.temperature_range_celsius)
        } else {
            resources.getStringArray(R.array.temperature_range_fahrenheit)
        }
        numberPicker.displayedValues = valuesArray
        numberPicker.minValue = 0
        numberPicker.maxValue = valuesArray.size - 1

        AlertDialog
            .Builder(this)
            .setView(numberPicker)
            .setMessage(getString(R.string.record_filter_picker))
            .setNegativeButton(getString(R.string.cancel)) { dialog, _ ->
                dialog.dismiss()
            }
            .setPositiveButton(getString(R.string.zxing_button_ok)) { _, _ ->
                editText.setText(valuesArray[numberPicker.value])

                if (isTempMin) {
                    tempMin = valuesArray[numberPicker.value].toFloat()
                } else {
                    tempMax = valuesArray[numberPicker.value].toFloat()
                }

            }
            .setCancelable(true)
            .create()
            .show()
    }

}