import json

import configInfo

import pdb

def application( env, start_response ):
    # pdb.set_trace()
    headers = [('Content-Type', 'application/json')]
    start_response( '200 OK', headers )

    result = {}
    data = {}
    length = int( env.get( 'CONTENT_LENGTH', 0 ) )
    if( length > 0 ):
        data = json.loads( env['wsgi.input'].read( length ).decode() )

    cmd = None
    if( env['PATH_INFO'] == '/api/getConfig' ):
        result = configInfo.getConfigInfo( data )
    else:
        result = { 'cmd' : 'None' }

    resultJsonStr = json.dumps( result ).encode( 'utf-8' )

    return [resultJsonStr]
    
