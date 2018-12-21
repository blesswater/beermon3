
/*
** Globals
*/
userSelections = {
    selDatasetValue: -1,
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

function handleConfigInfo( data ) {
    var td = document.getElementById( "versionInfo" );
    td.innerHTML = data.version;

    var t = uptimeToHMS( data.uptime );
    td = document.getElementById( "uptimeInfo" );
    td.innerHTML = "Hrs: " + t.hr + " Min: " + t.min + " Sec: " + t.sec;

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
            console.log( "Sent stat information")
        },
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: " + errorMsg );
        }
    });
}

/*
** APIs
*/

function initConfig() {
}

function datasetOnchange() {
    var sel = document.getElementById( "datasetSelect" );
    userSelections.selDatasetValue = sel.value;

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
        },
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: " + errorMsg );
	}
    });
}
