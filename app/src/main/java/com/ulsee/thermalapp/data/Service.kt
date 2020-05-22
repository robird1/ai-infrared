package com.ulsee.thermalapp.data

import com.ulsee.thermalapp.data.services.PeopleService
import com.ulsee.thermalapp.data.services.SettingsService

class Service {
    companion object {
        val shared = Service()
    }

    var settings = SettingsService()
    var people = PeopleService()
}