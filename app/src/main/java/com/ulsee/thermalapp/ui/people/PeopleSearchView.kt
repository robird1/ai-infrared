package com.ulsee.thermalapp.ui.people

import android.content.Context
import android.util.Log
import android.view.KeyEvent
import android.view.MenuItem
import androidx.appcompat.widget.SearchView

private val TAG = "PeopleSearchView"

class PeopleSearchView(context: Context?) : SearchView(context),
    SearchView.OnQueryTextListener, MenuItem.OnActionExpandListener {

    override fun onQueryTextSubmit(query: String?): Boolean {
        Log.d(TAG, "[Enter] onQueryTextSubmit")

        return false
    }

    override fun onQueryTextChange(newText: String?): Boolean {
        Log.d(TAG, "[Enter] onQueryTextChange")

        return false
    }

    override fun dispatchKeyEventPreIme(event: KeyEvent): Boolean {
        Log.d(TAG, "[Enter] dispatchKeyEventPreIme")

        if (event.keyCode == KeyEvent.KEYCODE_BACK && event.action == KeyEvent.ACTION_UP) {
            if (!isIconified) {
                onActionViewCollapsed()
            }
        }
        return super.dispatchKeyEventPreIme(event)
    }

    override fun onMenuItemActionExpand(item: MenuItem?): Boolean {
        Log.d(TAG, "[Enter] onMenuItemActionExpand")

        return true
    }

    override fun onMenuItemActionCollapse(item: MenuItem?): Boolean {
        Log.d(TAG, "[Enter] onMenuItemActionCollapse")

        return true
    }

}