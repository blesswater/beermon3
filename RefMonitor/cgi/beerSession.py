import hashlib, uuid
from datetime import datetime, timedelta
import re

import sqlite3

from beermonConfig import beermonConfig

class sessionId:

    def __init__(self, id, sessId, privLevel):
        self.sessId = sessId
        self.id = id
        self.privLevel = privLevel
        self.username = ''
        self.info = 'initial session'

class sessionHandler:
    sessIdLen = 20
    sessIdChkLen = 5
    sessExpireSeconds = 1800
    greeting = 'Hello, '

    def __init__(self):
        pass

    def _sessIdIsGood(self, sessId):
        result = False
        if( re.match( '[0-9a-fA-F]{%d}' % (self.sessIdLen + self.sessIdChkLen), sessId ) ):
            sessChk = sessId[0:self.sessIdChkLen]
            chk = hashlib.sha512(sessId[self.sessIdChkLen:(self.sessIdLen + self.sessIdChkLen)].encode('utf-8')).hexdigest()[5:10]
            if( str(chk) == sessChk ):
                result = True
            else:
                result = False

        return result

    def _validUsername(self, username ):
        if( re.match('[0-9a-zA-Z_.]{8,20}', username) ):
            if( not re.match('^[_.][_.]$', username) ):
                return True
        return False

    def getSession(self, sessId, ipAddr ):
        conn = None
        cur = None
        makeNew = True
        rtnSess = None
        try:
            conn = sqlite3.connect(beermonConfig['dbSessionLocation'])
            cur = conn.cursor()

            if( ipAddr == '127.0.0.1' ):
                # This is the kiosk, always let it in.
                rtnSess = sessionId( 0, '000000000', 3 )
                makeNew = False

            elif( (sessId != None) and (self._sessIdIsGood( sessId )) ):
                sql =  "SELECT id, ipAddr, datetime(last_time), privledge "
                sql += "FROM Session "
                sql += "WHERE sessId = ?"

                cur.execute( sql, (sessId,) )
                row = cur.fetchone()
                if( row ):
                    sessTime = datetime.strptime( row[2], '%Y-%m-%d %H:%M:%S' )
                    if( (sessTime + timedelta( seconds = self.sessExpireSeconds )) > datetime.utcnow() ):
                        if( row[1] == ipAddr ):
                            # Good Session - Update it
                            sql  = "UPDATE Session SET "
                            sql += "last_time = datetime( 'now' ) "
                            sql += "WHERE id = %d" % (row[0])
                            cur.execute( sql )
                            conn.commit()

                            rtnSess = sessionId( row[0], sessId, row[3] )
                            makeNew = False
            else:
                pass

            if( makeNew ):
                sid = uuid.uuid4().hex
                sid = sid[0:self.sessIdLen].encode( 'utf-8' )
                chk = str( hashlib.sha512(sid).hexdigest()[5:10] )

                sql =  "INSERT INTO Session (sessId, ipAddr, begin_time, last_time, privledge) "
                sql += "VALUES ('%s', '%s', datetime( 'now' ), datetime( 'now' ), 0)" % (chk + sid.decode('utf-8'), ipAddr)

                cur.execute( sql )
                conn.commit()

                rtnSess = sessionId( cur.lastrowid, chk + sid.decode('utf-8'), 0 )

        except:
            rtnSess = None

        finally:
            if( cur != None ):
                cur.close()
            if( conn != None ):
                conn.close()

        return rtnSess

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
                row = cur.fetchone()
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

    def login( self, sess, username, passwd ):
        conn = None
        cur = None
        result = { 'result' : 'ERROR: Invalid Username/Password' }

        if( sess.sessId == 0 ):
                # This is kiosk
                sess.username = 'kiosk'
                result['username'] = sess.username
                result['result'] = 'OK'
                result['greeting'] = self.greeting
                result['privLevel'] = sess.privLevel

                return result

        try:
            conn = sqlite3.connect(beermonConfig['dbSessionLocation'])
            cur = conn.cursor()

            sql =  "SELECT id, username, pwhash, priv "
            sql += "FROM User "
            sql += "WHERE username=?"

            cur.execute( sql, (username,) )
            for row in cur.fetchall():
                if( result['result'] != 'OK' ):
                    pwSalt = re.search( '.*\$(.*?)\$', row[2] ).group(1)
                    pwHash = hashlib.sha512( (passwd + pwSalt).encode( 'utf-8' ) ).hexdigest() + '$' + pwSalt + '$'
                    if( pwHash == row[2] ):
                        # Found it
                        sql =  "UPDATE Session SET "
                        sql += "last_time = datetime( 'now' ), "
                        sql += "privledge = %d, " % (row[3])
                        sql += "userId = %d " % (row[0])
                        sql += "WHERE id = %d" % (sess.id)

                        cur.execute( sql )
                        conn.commit()

                        sess.privLevel = row[3]
                        result['username'] = row[1]
                        result['result'] = 'OK'
                        result['greeting'] = self.greeting
                        result['privLevel'] = sess.privLevel
        except:
            result['result'] = 'ERROR: User database problem'

        finally:
            if( cur != None ):
                cur.close()
            if( conn != None):
                conn.close()

        return( result )

    def logout(self, sess ):
        conn = None
        cur = None
        result = {'result': 'ERROR: Invalid Username/Password'}

        if (sess.sessId == 0):
            # This is kiosk - cannot logout the kiosk
            sess.username = 'kiosk'
            result['username'] = sess.username;
            result['result'] = 'OK';
            result['greeting'] = self.greeting;
            result['privLevel'] = sess.privLevel;

            return result

        try:
            conn = sqlite3.connect(beermonConfig['dbSessionLocation'])
            cur = conn.cursor()

            sql =  "UPDATE Session SET privledge = 0 WHERE id = %d" % (sess.id);
            cur.execute( sql );
            conn.commit();
        finally:
            if( cur != None ):
                cur.close()
            if( conn != None ):
                conn.close()

        sess.privLevel = 0;
        sess.info = 'logged Out'
        sess.username = ''
        result['privLevel'] = 0;
        result['username'] = '';
        result['result'] = 'OK';
        result['greeting'] = '';

        return result;

if( __name__ == '__main__' ):
    sessMgr = sessionHandler()
    sess = sessMgr.getSession( None, '123.3.3.22' )
    sess2 = sessMgr.getSession( sess.sessId, '123.3.3.22' )
    sess3 = sessMgr.getSession( sess.sessId[0:24] + '5', '123.3.3.22' )

    sessMgr.login( sess, 'testuser0', 'dontc4r3' )
    sessMgr.logout(sess)
    # sess.createSession( 'testuser0', 'dontc4r3', '192.168.166.34')
    # sess.createSession('testuser0', 'dontcar3', '192.168.166.34')
