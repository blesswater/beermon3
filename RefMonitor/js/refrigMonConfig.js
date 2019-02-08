
/*
** Globals
*/
userSelections = {
    selDatasetValue: -1,
    activeProbes: {},
    tempDataTimer: null,
    loggedIn: false,
    privLevel: 0,
    cntlMode: "MONITOR",
    activated: false,
    setpointControlKnob : {}
};

stateLed = {
    "Switched Out" : "led-off",
    "Off" : "led-red",
    "On Debounce" : "led-red-blink",
    "On" : "led-blue",
    "Off Debounce" : "led-blue-blink",
    "Ext Cntl" : "led-off"
}

/*
** Local functions
*/

function uptimeToHMS( uptime ) {
    var hr = Math.floor( uptime / 3600 );
    var min = Math.floor( (uptime - (hr * 3600)) / 60 );
    var sec = Math.floor( uptime - (hr*3600) - (min*60) );
    return { hr: hr, min: min, sec: sec };
}

function updateUptime( sec ) {
    var t = uptimeToHMS( sec );
    var td = document.getElementById( "uptimeInfo" );
    td.innerHTML = "Hrs: " + t.hr + " Min: " + t.min + " Sec " + t.sec
}

function handleConfigInfo( data ) {
    var td = document.getElementById( "versionInfo" );
    td.innerHTML = data.version;

    updateUptime( data.uptime );

    var sel = document.getElementById( "datasetSelect" );

    data.dataset.forEach( function(ds) {
        opt = document.createElement( "option" );
        opt.setAttribute( 'value', ds.id );
        opt.innerHTML = ds.name;
        sel.appendChild( opt );

        if( ds.hasOwnProperty( "setpoints" ) && ds["setpoints"].length > 0 ) {
            console.log( "Got Setpoints" );
            userSelections.setpointControlKnob[ds.id] = {
                state : 0,
                value : ds["setpoints"][0].value,
                setting : ds["setpoints"][0].value,
                minRange : ds["setpoints"][0].min_range,
                maxRange : ds["setpoints"][0].max_range,
            }
            initControlKnob( userSelections.setpointControlKnob[ds.id] );
            processKnobEvent( 0 ); /* SwitchOut */
        }
    });

    processKnobEvent( 0 ); /* SwitchOut */
    userSelections.selDatasetValue = sel.value;
    if( userSelections.cntlMode == 'MONITOR') {
        processKnobEvent( 1 ); /* SwitchInMonitor */
    }
    else {
        processKnobEvent( 2 ); /* SwitchInControl */
    }


    /* TODO: Store control knob data here */

    /* Create setpoint control knob */
    /*
    td = document.getElementById( "setpointControlTd" );
    knob = createKnob();
    if( userSelections.cntlMode == "MONITOR" ) {
        knob.setAttribute( "data-readonly", "readonly" );
    }
    td.appendChild( knob );
    $(".knob").knob();
    */

}

function _addProbeRadioInputNew( formElement, prb ) {
    cntlProbeTd = document.getElementById( "controlProbeTd" );

    formDiv = document.createElement( "div" );
    formDiv.setAttribute( "class", "form-check" );

    probeInput = document.createElement( "input" );
    probeInput.setAttribute("type", "radio" );
    probeInput.setAttribute( "name", "controlProbeSelect" );
    probeInput.setAttribute( "id", "controlProbeSelect_" + prb.id );
    probeInput.setAttribute( "value", prb.id );
    probeInput.setAttribute( "class", "form-check-input controlEnabled" );
    probeInput.setAttribute( "onchange", "setControlProbe(" + prb.id + ")" );
    if( prb.cntl ) {
        probeInput.setAttribute( "checked", "checked")
    }

    probeLabel = document.createElement( "label" );
    probeLabel.setAttribute( "class", "form-check-label" );
    probeLabel.setAttribute( "for", "controlProbeSelect_" + prb.id );
    probeLabel.innerHTML = "<table><tr><td>" + prb.name + "</td><td align=\"left\"><div id=\"controlProbeSelectLed_" + prb.id + "\"></div></td></tr></table>"

    formDiv.appendChild( probeInput );
    formDiv.appendChild( probeLabel );

    cntlProbeTd.appendChild( formDiv );

    if( userSelections.cntlMode == "CONTORL" ) {
        $( ".controlEnabled" ).prop( "disabled", false );
    }
    else {
        $( ".controlEnabled" ).prop( "disabled", true );
    }
}

function _handleControlState( stateData ) {
    if( stateData.hasOwnProperty( "controlState" ) ) {
        stateTd = document.getElementById( "controlStateTd" )

        led = "led-off"
        if( stateLed.hasOwnProperty( stateData.controlState ) ) {
            led = stateLed[stateData.controlState]
        }

        stateTd.innerHTML = "<table><tr><td>" + stateData.controlState + "</td>" +
                            "<td><div class=\"" + led + "\"></div></td></tr></table>";

        if( stateData.controlState == "Switched Out") {
            userSelections.activated = false;
        }
        else {
            userSelections.activated = true;
        }
    }

    if( stateData.hasOwnProperty( "setpoint" ) ) {
        processKnobEvent( 8, stateData.setpoint );
    }
}

function getTempStat( dsid ) {

    statInfo = { datasetId: dsid,
                 statInterval: 86400,
                 // startTime: new Date(),
               };
    $.ajax( {
        type: "POST",
        url: "/api/datasetStat",
        dataType: 'json',
        contentType: 'application/json',
        data: JSON.stringify( statInfo ),
        success: function( data, status, xhr ) {
            console.log( "Sent stat information");
            updateUptime( data.uptime );
            handleTempStatData( data );
            _handleControlState( data.state );
            handleSessInfo( data.sessInfo );
        },
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: " + errorMsg );
        }
    });
}

/*
** Handle temperature Stat data
*/

function handleTempStatData( data ) {
    console.log( "Handle temp stat data" );
    if( userSelections.tempDataTimer != null ) {
        window.clearTimeout( userSelections.tempDataTimer );
        userSelections.tempDataTimer = null;
    }
    var thermRow = document.getElementById( "thermometerRow" );
    var thermNameRow = document.getElementById( "thermometerNameRow" );

    updateUptime( data.uptime );

    data.probes.forEach( function(prb) {
        console.log( "Looking at probe " + prb.name );
        if( !userSelections.activeProbes.hasOwnProperty( prb.id ) ) {
            /* Name Thermometer Name Row */
            thermNameCol = document.createElement( "td" );
            thermNameCol.setAttribute( "id", "thermNameCol_" + prb.id );
            thermNameCol.setAttribute( "class", "thermNameCol" );
            thermNameCol.innerHTML = prb.name;
            thermNameRow.appendChild( thermNameCol );

            /* Create Thermometer */
            thermCol = document.createElement( "td" );
            thermCol.setAttribute( "id", "thermCol_" + prb.id )
            thermDiv = document.createElement( "div" );
            thermDiv.setAttribute( "id", "thermDiv_" + prb.id );
            thermCol.appendChild( thermDiv );
            thermRow.appendChild( thermCol );
            therm = new Thermometer({
                mercuryColor: "rgb(220, 150, 0)"
            });

            therm.render( thermDiv, prb.currentTemp, prb.min_range, prb.max_range );
            therm.createMarker( "sdevHi", "", prb.avgTemp + prb.sdevTemp );
            // therm.setMarkerValue( "sdevHi", prb.avgTemp + prb.sdevTemp );
            therm.createMarker( "sdevLo", "", prb.avgTemp - prb.sdevTemp );
            // therm.setMarkerValue( "sdevLo", prb.avgTemp - prb.sdevTemp );

            userSelections.activeProbes[prb.id] = { therm: therm };

            /* Figure out which probe is in control and which is selected */
            controlForm = document.getElementById( "controlProbeTd" );
            if( prb.cntl_able ) {
                _addProbeRadioInputNew( controlForm, prb );
            }

        }
        else {
            therm = userSelections.activeProbes[prb.id].therm;
            therm.setCurrentValue( prb.currentTemp );
            therm.updateMarker( "sdevHi", "", prb.avgTemp + prb.sdevTemp );
            therm.updateMarker( "sdevLo", "", prb.avgTemp - prb.sdevTemp );
        }

        cntlLed = document.getElementById( "controlProbeSelectLed_" + prb.id );
        if( cntlLed && prb.cntl_able ) {
            if( prb.cntl ) {
                cntlLed.setAttribute( "class", "led-green" );
            }
            else {
                cntlLed.setAttribute( "class", "led-off" );
            }
        }

    });

    userSelections.tempDataTimer = window.setTimeout( function() {
        console.log( "Data time expire" );
        getTempStat( userSelections.selDatasetValue );
    }, 5000 );
}

function _newProbeSetup() {
    var sel = document.getElementById( "datasetSelect" );
    if( sel.value != userSelections.selDatasetValue ) { /* Redundant */
        for( prbKey in userSelections.activeProbes ) {
            if( userSelections.activeProbes.hasOwnProperty( prbKey )) {
                userSelections.activeProbes[prbKey].therm.destroy();
            }
        }

        var thermRow = document.getElementById( "thermometerRow" );
        while( thermRow.firstChild ) {
            thermRow.removeChild( thermRow.firstChild );
        }

        var thermNameRow = document.getElementById( "thermometerNameRow" );
        while( thermNameRow.firstChild ) {
            thermNameRow.removeChild( thermNameRow.firstChild );
        }

        userSelections.activeProbes = {};

        processKnobEvent( 0 ); /* SwitchOut */
        userSelections.selDatasetValue = sel.value;
        if( userSelections.cntlMode == 'MONITOR') {
            processKnobEvent( 1 ); /* SwitchInMonitor */
        }
        else {
            processKnobEvent( 2 ); /* SwitchInControl */
        }

        var formElement = document.getElementById( "controlProbeTd" );
        while( formElement.firstChild ) {
            formElement.removeChild( formElement.firstChild );
        }
    }
}

function _handleProbeData() {
    console.log( "Handle probe data" );
    getTempStat( userSelections.selDatasetValue );
}

/*
** APIs
*/

function initConfig() {
    changeMode();
}

function datasetOnchange() {
    if( userSelections.tempDataTimer != null ) {
        window.clearTimeout( userSelections.tempDataTimer );
        userSelections.tempDataTimer = null;
    }
    _newProbeSetup();
    getTempStat( userSelections.selDatasetValue );
}

function getConfig() {
    console.log( "getConfig() called." );

    $.ajax( {
        type: "POST",
        url: "/api/getConfig",
        dataType: 'json',
        contentType: 'application/json',
        data: JSON.stringify( {} ),
        success: function( data, status, xhr ) {
            console.log( "getConfig() Success!" );
            handleConfigInfo( data );
            datasetOnchange();
            _handleControlState( data.state );
            handleSessInfo( data.sessInfo );
        },
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: " + errorMsg );
	    }
    });
}

function setControlProbe( prbId ){
    console.log( "Got control probe change: " + prbId );

    data = { controlProbe :
               { dataSetId: userSelections.selDatasetValue,
                 probeId: prbId
               }
           }

    $.ajax( {
        type: "POST",
        url: "/api/setConfig",
        dataType: 'json',
        contentType: 'application/json',
        data: JSON.stringify( data ),
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: setControlProbe()" + errorMsg );
	    }
    });
}

function activateButtonOnClick() {
    data = { activate: !userSelections.activated }
     $.ajax( {
        type: "POST",
        url: "/api/setConfig",
        dataType: 'json',
        contentType: 'application/json',
        data: JSON.stringify( data ),
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: setControlProbe()" + errorMsg );
	    }
    });
}

