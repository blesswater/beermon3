import sqlite3

class beerChipDB:
    def connect(self, connInfo):
        raise NotImplementedError("Should have implemented this")

    def query(self, queryString ):
        raise NotImplementedError("Should have implemented this")

    def querySafe(self, queryString, valueTuple):
        raise NotImplementedError("Should have implemented this")

    def execute(self, sqlString):
        raise NotImplementedError("Should have implemented this")

    def fetchOne(self, queryString, valueTuple):
        raise NotImplementedError("Should have implemented this")

    #
    # Utility Functions
    #
    def setProject(self, projName ):
        raise NotImplementedError("Should have implemented this")

    def getAvailableProject(self):
        raise NotImplementedError("Should have implemented this")

    def getProbes( self ):
        raise NotImplementedError("Should have implemented this")

    def fetchTemperatures(self):
        raise NotImplementedError("Should have implemented this")

    def close(self):
        raise NotImplementedError("Should have implemented this")

class beerChipSQLiteDB( beerChipDB ):
    def __init__(self):
        self.conn = None
        self.projName = None
        self.projId = None

    def connect(self, dbFilename ):
        try:
            self.conn = sqlite3.connect( dbFilename )
        except:
            print( "ERROR: Cannot connect to SQLite3 DB file %s" % (dbFilename) )

    def query(self, queryString):
        if( self.conn == None ):
            print( "ERROR: Database NOT connected" )
            return

        cur = None
        try:
            cur = self.conn.cursor()
            cur.execute( queryString )
            for row in cur.fetchall():
                yield row
        except:
            print( "ERROR executing %s" % (queryString ))

        if( cur != None ):
            cur.close()

    def querySafe(self, queryString, valueTuple ):
        if( self.conn == None ):
            print("ERROR: Database NOT connected")
            return

        cur = None
        try:
            cur = self.conn.cursor()
            cur.execute(queryString, valueTuple )
            for row in cur.fetchall():
                yield row

        except:
            print("ERROR executing %s" % (queryString))

        if (cur != None):
            cur.close()


    def execute(self, sqlString):
        numRows = 0
        if (self.conn == None):
            print("ERROR: Database NOT connected")
            return

        cur = None
        try:
            cur = self.conn.cursor()
            cur.execute(sqlString)
            self.conn.commit()
            numRows = cur.rowcount
        except:
            print( 'ERROR: Error executing %s' % (sqlString) )

        if( cur != None ):
            cur.close()

        return numRows

    def fetchOne(self, queryString, valueTuple ):
        if (self.conn == None):
            print("ERROR: Database NOT connected")
            return

        cur = None
        try:
            cur = self.conn.cursor()
            cur.execute(queryString, valueTuple )
            row = cur.fetchone()

        except:
            print("ERROR executing %s" % (queryString))

        finally:
            if (cur != None):
                cur.close()

        return row

    def getAvailableProj(self):
        result = []
        if (self.conn == None):
            print("ERROR: Database NOT connected")
            return result
        cur = None
        try:
            cur = self.conn.cursor()
            sql  = "SELECT proj_name FROM Project"
            cur.execute( sql )
            for row in cur.fetchall():
                result.append( row[0] )
        except:
            print( "ERROR finding Projects" )
        finally:
            if( cur != None ):
                cur.close()
        return result

    def setProject(self, projName ):
        sql  = "SELECT id FROM Project WHERE proj_name = ?"
        projId = None
        result = False
        for row in self.querySafe( sql, (projName,) ):
            projId = row[0]
        if( projId != None ):
            self.projId = projId
            self.projName = projName
            result = True
        else:
            print( "ERROR: Cannot find project %s" % (projName) )
        return result

    def getProbes( self ):
        result = []
        if( self.projId == None ):
            print( "ERROR: Must set project" )
            return result

        sql  = "SELECT id, probe_name FROM Probes WHERE proj_id = ?"
        for row in self.querySafe( sql, (self.projId) ):
            result.append( (row[0], row[1]) )
        return result

    def fetchTemperatures(self, interval = 20 ):
        probes = self.getProbes()
        probeUnion = []
        for prb in probes:
            prb[1] = prb[1].replace(' ', '' )
            prbSql  = "SELECT CAST(strftime('%%s', temp_time) / %d AS INTEGER) AS time_group, " % (interval)
            prbSql += "datetime(avg(strftime('%%s', temp_time)),'unixepoch') AS centerTime, "
            for prb2 in probes:
                if( prb2[0] == prb[0] ):
                    prbSql += "AVG(temp) AS %s" % (prb[1].replace(' ', ''))
                else:
                    prbSql += "0.0 AS %s" % (prb[1].replace(' ', '' ))
            prbSql += "FROM Temperature WHERE probe_id=%d " % (prb[0])
            prbSql += "GROUP BY strftime('%%s', temp_time) / %d" % interval
            probeUnion.append( prbSql )
        sql  = "SELECT x.time_group, "
        sql += "DATETIME(AVG(STRFTIME('%s',x.centerTime)),'unixepoch'), "
        sql += [x for x in probes[1]]


    def close(self):
        if( self.conn != None ):
            self.conn.close()
        self.conn = None





