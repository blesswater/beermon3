function getConfig() {
    console.log( "getConfig() called." );

    $.ajax( {
        type: "POST",
        url: "/api/getConfig",
        data: {},
        success: function( data, status, xhr ) {
            console.log( "Success!" );
        },
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: " + errorMsg );
	}
    });
}
