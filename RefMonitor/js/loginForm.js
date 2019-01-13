/*
** APIs
*/

function toggleLoginDisplay() {
    div = document.getElementById( "loginFormDiv" )
    if( (div.style.display == "none") || (div.style.display == "") ){
        div.style.display = "block"
    }
    else {
        div.style.display = "none"
    }

}

function handleSessInfo( sess ) {
    console.log( "Handling sess info" );
    logp = document.getElementById( "loginDisplay" );
    if( sess.hasOwnProperty( "privLevel" ) ) {
        userSelections.privLevel = sess.privLevel;
        if( sess.privLevel > 0 ) {
            userSelections.loggedIn = true;
            if( sess.hasOwnProperty( "username" ) ) {
                if( sess.hasOwnProperty( "greeting" ) ) {
                    logp.innerHTML = sess.greeting + sess.username;
                }
                else {
                    logp.innerHTML = sess.username;
                }
            }
            else {
                if( logp.innerHTML == "Login" ) {
                    logp.innerHTML = "Logged In";
                }
            }
        }
        else {
            userSelections.loggedIn = false;
            logp.innerHTML = "Login";
        }
    }
    else {
        userSelections.loggedIn = false;
        logp.innerHTML = "Login"
    }
}

function loginOnClick() {
    console.log( "Login()" );

    if( (document.getElementById( "loginUserName" ).value != "") ) {
        document.getElementById( "loginUserName" ).setAttribute( "class", "inputBorder" );
        if( (document.getElementById( "loginPasswd" ).value != "")  ) {
            document.getElementById( "loginPasswd" ).setAttribute( "class", "inputBorder" );
            console.log( "Got un/pw" );

            $.ajax( {
                type: "POST",
                url: "/api/login",
                dataType: 'json',
                contentType: 'application/json',
                data: JSON.stringify( { username: document.getElementById( "loginUserName" ).value,
                                        password: document.getElementById( "loginPasswd" ).value
                                      } ),
                success: function( data, status, xhr ) {
                    console.log( "login() Success!" );
                    handleSessInfo( data );
                    if( data.result == "OK" ) {
                        toggleLoginDisplay();
                    }
                },
                error: function( xhr, textStatus, errorMsg ) {
                    console.log( "Error: Login return - " + errorMsg );
	            }
            });
            document.getElementById( "loginUserName" ).value = "";
            document.getElementById( "loginPasswd" ).value = "";
        }
        else {
            document.getElementById( "loginPasswd" ).setAttribute( "class", "errorInputBorder" );
        }
    }
    else {
        document.getElementById( "loginUserName" ).setAttribute( "class", "errorInputBorder" );
    }
}

function logoutOnClick() {
    console.log( "Logging out" );
    userSelections.loggedIn = false;
    document.getElementById( "loginDisplay" ).innerHTML = "Login";

    $.ajax( {
        type: "POST",
        url: "/api/logout",
        dataType: 'json',
        contentType: 'application/json',
        data: JSON.stringify( {} ),
        error: function( xhr, textStatus, errorMsg ) {
            console.log( "Error: Logout return - " + errorMsg );
        }
    });
    toggleLoginDisplay();
    changeMode();
}

function changeMode() {

    if( (userSelections.cntlMode == "MONITOR") && (userSelections.privLevel >= 3) ) {
        userSelections.cntlMode = "CONTROL";
        $( ".controlEnabled" ).prop( "disabled", false );
    }
    else
    {
        userSelections.cntlMode = "MONITOR";
        $( ".controlEnabled" ).prop( "disabled", true );
    }
    document.getElementById( "modeDisplay" ).innerHTML = userSelections.cntlMode
}