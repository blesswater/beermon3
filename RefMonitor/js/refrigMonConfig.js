
/*
** Globals
*/
userSelections = {
    selDatasetValue: -1,
    activeProbes: {},
    tempDataTimer: null,
    loggedIn: false,
    privLevel: 0,
    cntlMode: "MONITOR"
};

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
    });

    userSelections.selDatasetValue = sel.value;

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
        if( !userSelections.activeProbes.hasOwnProperty( prb.id) ) {
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
        }
        else {
            therm = userSelections.activeProbes[prb.id].therm;
            therm.setCurrentValue( prb.currentTemp );
            therm.updateMarker( "sdevHi", "", prb.avgTemp + prb.sdevTemp );
            therm.updateMarker( "sdevLo", "", prb.avgTemp - prb.sdevTemp );
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
                // userSelections.activeProbes[prbKey].therm.destroy();
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
        userSelections.selDatasetValue = sel.value;
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
            handleSessInfo( data.sessInfo );
        },
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: " + errorMsg );
	    }
    });
}
