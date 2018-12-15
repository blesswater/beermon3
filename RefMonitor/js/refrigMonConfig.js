
function uptimeToHMS( uptime ) {
    hr = Math.floor( uptime / 3600 );
    min = Math.floor( (uptime - (hr * 3600)) / 60 );
    sec = Math.floor( uptime - (hr*3600) - (min*60) );
    return { hr: hr, min: min, sec: sec };
}

function handleConfigInfo( data ) {
    td = document.getElementById( "versionInfo" );
    td.innerHTML = data.version;

    t = uptimeToHMS( data.uptime );
    td = document.getElementById( "uptimeInfo" );
    td.innerHTML = "Hrs: " + t.hr + " Min: " + t.min + " Sec: " + t.sec;
}

function getConfig() {
    console.log( "getConfig() called." );

    $.ajax( {
        type: "POST",
        url: "/api/getConfig",
        dataType: 'json',
        contentType: 'application/json',
        data: JSON.stringify( {'bla' : 'blablablablablablablablablablablablablablablabla' } ),
        success: function( data, status, xhr ) {
            console.log( "getConfig() Success!" );
            handleConfigInfo( data );
        },
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: " + errorMsg );
	}
    });
}
