import json

import configInfo
import temperatureData
from beerSession import sessionId, sessionHandler


def application( env, start_response ):
    sessMgr =  sessionHandler()
    if( 'HTTP_COOKIE' in env ):
        sess = sessMgr.getSession( env['HTTP_COOKIE'], env['REMOTE_ADDR'] )
    else:
        sess = sessMgr.getSession(None, env['REMOTE_ADDR'])
        
    headers = [('Content-Type', 'application/json'),
               ('Set-Cookie', str(sess.sessId))]
    start_response( '200 OK', headers )

    result = {}
    data = {}
    length = int( env.get( 'CONTENT_LENGTH', 0 ) )
    if( length > 0 ):
        data = json.loads( env['wsgi.input'].read( length ).decode() )

    cmd = None
    if( env['PATH_INFO'] == '/api/getConfig' ):
        result = configInfo.getConfigInfo( data )

    elif( env['PATH_INFO'] == '/api/datasetStat' ):
        result = temperatureData.getTempStat( data )

    elif( env['PATH_INFO'] == '/api/login' ):
        if( ('username' in data) and ('password' in data) ):
            result = sessMgr.login( sess, data['username'], data['password'] )
        else:
            result = { 'result' : 'ERROR: Invalid Username/Password' }

    elif( env['PATH_INFO'] == '/api/logout' ):
        result = sessMgr.logout( sess )

    elif( env['PATH_INFO'] == '/api/setConfig' ):
        if( sess.privLevel >= 3 ):
            result = configInfo.setConfigInfo( data )
        else:
            result = { 'result' : 'ERROR: Must be logged in!' }

    else:
        result = { 'cmd' : 'No command' }

    # Add session data to response
    sessData = {
        'info' : sess.info,
        'privLevel' : sess.privLevel
    }
    if( len( sess.username ) > 0 ):
        sessData['username'] = sess.username
        sessData['greeting'] = result['greeting']
    result['sessInfo'] = sessData

    resultJsonStr = json.dumps( result ).encode( 'utf-8' )

    return [resultJsonStr]
    
