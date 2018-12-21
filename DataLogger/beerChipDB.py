import sqlite3

class beerChipDB:
    def connect(self, connInfo):
        raise NotImplementedError("Should have implemented this")

    def query(self, queryString ):
        raise NotImplementedError("Should have implemented this")

    def execute(self, sqlString):
        raise NotImplementedError("Should have implemented this")

    def close(self):
        raise NotImplementedError("Should have implemented this")

class beerChipSQLiteDB( beerChipDB ):
    def __init__(self):
        self.conn = None

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
        if (self.conn == None):
            print("ERROR: Database NOT connected")
            return

        cur = None
        try:
            cur = self.conn.cursor()
            cur.execute(sqlString)
            self.conn.commit()
        except:
            print( 'ERROR: Error executing %s' % (sqlString) )

        if( cur != None ):
            cur.close()

    def close(self):
        if( self.conn != None ):
            self.conn.close()
        self.conn = None





