import hashlib, uuid
import re

import sqlite3

from beermonConfig import beermonConfig

class userSession:
    def __init__(self):
        pass

    def createSession(self, username, pw, ipAddr):
        conn = None
        cur = None
        gotIt = False
        sessId = None
        msg = ''
        (uname, uid ) = ('', '')
        try:
            conn = sqlite3.connect(beermonConfig['dbSessionLocation'])
            cur = conn.cursor()
            if (ipAddr == '127.0.0.1'):
                # This is the kiosk, always let it in.
                sql =  "SELECT id, username, pwhash, priv "
                sql += "FROM User "
                sql += "WHERE username='kiosk"

                cur.execute(sql )
                row = cur.feschone()
                gotIt = True
                (uname, uid, priv) = (row[1], row[0], row[3])

            elif( len( username ) > 0 ):
                # Lookup
                sql =  "SELECT id, username, pwhash, priv "
                sql += "FROM User "
                sql += "WHERE username = ?"

                cur.execute( sql, (username,) )
                for row in cur.fetchall():
                    userSalt = re.search( '[0-9a-fA-f]+\$([0-9a-fA-F]+)\$', row[2] )
                    thisSalt = ''
                    if( userSalt ):
                        thisSalt = userSalt.group(1)
                        phash = hashlib.sha512(pw + str(thisSalt)).hexdigest() + '$' + str(thisSalt) + '$'
                        if( phash == row[2] ):
                            gotIt = True
                            (uname, uid, priv) = (row[1], row[0], row[3])

            else:
                gotIt = False

            if( gotIt ):
                sessId = uuid.uuid4().hex
                sql =  "INSERT INTO Session (sessId, userId, ipAddr, last_time) "
                sql += "VALUES ('%s', %d, ?, datetime('now'))" % (sessId, uid)

                cur.execute( sql, (ipAddr,) )
                conn.commit()

        except:
            msg = 'ERROR: User lookup error'
        finally:
            if( cur != None ):
                cur.close()
            if( conn != None ):
                conn.close()


        return (gotIt, sessId, msg)

if( __name__ == '__main__' ):
    sess = userSession()
    sess.createSession( 'testuser0', 'dontc4r3', '192.168.166.34')
    sess.createSession('testuser0', 'dontcar3', '192.168.166.34')
