# IoT Reticulation Controller

This is a standalone code for a 4 channel reticulation controller, the controller already works with Home Assistants ESPhome.  This is repo is if you would like to use it stand alone if you are not running home assistant or if you would just like to have it stand alone so that it is not network dependent.
To use this code you will need to have a RTC module installed such as the DS3231 or MAX31328 the later which is available from my website www.egglec.com.au

## Overview

The front end GUI, that is viewed in a web browser to control and setup the Easy Retic Controller is made from Java Script and HTML, the site is loaded from the ESP8285 using ESPasyncwebserver once into the browser and once loaded communicates with the logic controller running on the ESP8285 via a websocket connection.

The websocket connection will also communicate with other devices on the saem network, so that it could be connected to 3rd party devices in this way. The command system operates on JSON formatted messages to operate stations, and receive updates.  This is a local push type of device, the best type.

First a user connects their phone to the AP the device creates. Then on connecting to the device, the device's browser will send the current time and date to the Easy Reticulation Controller automatically and set the clock. Once set it should be keep time well for a long period without further updates.

## Command System

- sId : Fixed Station ID
- SName : Station display name, can be changed
- sTime : Station run time in minutes
- isON : the station is on or off flag (true:false)

- pId : Program ID - there can be 2 programs
- pHour : Program start time hour of day 24HRS
- pMinute : Program start time minute of day
- pStations : Array of 4 stations if true the station with run if false the station will not run in this program
- pDays : Array of 7 days of the week **first item is monday** true the program will run and false the program will not run on that day
- pDisable : Disable the program true:false
- isRunning : False or 0 if the program is not running, total seconds remaining in the program run time

### Set time of day
This is generally not required
>`SEND:{"hour": 7,"minute": 15,"second": 10,"day": 21,"month": 6,"year":22,"week":1}`

### Toggle a station
Toggle the status of station 1:  
>`SEND:{"command": "toggle", "sId": 1}`  
Response is station was toggled to on:  
>`RESPONSE:{"sId": 1, "sName": "Station 1", "sTime": 10, "isOn": true}`  
As you can see it includes the station name, run time in minutes and is ON flag.

### Update the station runtime
Send a new run time in minutes:  

>`SEND:{"command": "time", "sId": 1, "sTime": 15}`   

>`RESPONSE:{"sId": 1, "sName": "Station 1", "sTime": 10, "isOn": true}`  


