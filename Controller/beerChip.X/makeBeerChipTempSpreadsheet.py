
Vcc = 5.33
R = 24.9

Rf = 10.0
R1 = 47.5
R2 = 48.7

tempTable = [
    {'F' : -40, 'Rt' : 277.2},
    {'F' : -38.2, 'Rt' : 263.6},
    {'F' : -36.4, 'Rt' : 250.1},
    {'F' : -34.6, 'Rt' : 236.8},
    {'F' : -32.8, 'Rt' : 224},
    {'F' : -31, 'Rt' : 211.5},
    {'F' : -29.2, 'Rt' : 199.6},
    {'F' : -27.4, 'Rt' : 188.1},
    {'F' : -25.6, 'Rt' : 177.3},
    {'F' : -23.8, 'Rt' : 167},
    {'F' : -22, 'Rt' : 157.2},
    {'F' : -20.2, 'Rt' : 148.1},
    {'F' : -18.4, 'Rt' : 139.4},
    {'F' : -16.6, 'Rt' : 131.3},
    {'F' : -14.8, 'Rt' : 123.7},
    {'F' : -13, 'Rt' : 116.6},
    {'F' : -11.2, 'Rt' : 110},
    {'F' : -9.4, 'Rt' : 103.7},
    {'F' : -7.6, 'Rt' : 97.9},
    {'F' : -5.8, 'Rt' : 92.5},
    {'F' : -4, 'Rt' : 87.43},
    {'F' : -2.2, 'Rt' : 82.79},
    {'F' : -0.4, 'Rt' : 78.44},
    {'F' : 1.4, 'Rt' : 74.36},
    {'F' : 3.2, 'Rt' : 70.53},
    {'F' : 5, 'Rt' : 66.92},
    {'F' : 6.8, 'Rt' : 63.54},
    {'F' : 8.6, 'Rt' : 60.34},
    {'F' : 10.4, 'Rt' : 57.33},
    {'F' : 12.2, 'Rt' : 54.5},
    {'F' : 14, 'Rt' : 51.82},
    {'F' : 15.8, 'Rt' : 49.28},
    {'F' : 17.6, 'Rt' : 46.89},
    {'F' : 19.4, 'Rt' : 44.62},
    {'F' : 21.2, 'Rt' : 42.48},
    {'F' : 23, 'Rt' : 40.45},
    {'F' : 24.8, 'Rt' : 38.53},
    {'F' : 26.6, 'Rt' : 36.7},
    {'F' : 28.4, 'Rt' : 34.97},
    {'F' : 30.2, 'Rt' : 33.33},
    {'F' : 32, 'Rt' : 31.77},
    {'F' : 33.8, 'Rt' : 30.25},
    {'F' : 35.6, 'Rt' : 28.82},
    {'F' : 37.4, 'Rt' : 27.45},
    {'F' : 39.2, 'Rt' : 26.16},
    {'F' : 41, 'Rt' : 24.94},
    {'F' : 42.8, 'Rt' : 23.77},
    {'F' : 44.6, 'Rt' : 22.67},
    {'F' : 46.4, 'Rt' : 21.62},
    {'F' : 48.2, 'Rt' : 20.63},
    {'F' : 50, 'Rt' : 19.68},
    {'F' : 51.8, 'Rt' : 18.87},
    {'F' : 53.6, 'Rt' : 17.93},
    {'F' : 55.4, 'Rt' : 17.12},
    {'F' : 57.2, 'Rt' : 16.35},
    {'F' : 59, 'Rt' : 15.62},
    {'F' : 60.8, 'Rt' : 14.93},
    {'F' : 62.6, 'Rt' : 14.26},
    {'F' : 64.4, 'Rt' : 13.63},
    {'F' : 66.2, 'Rt' : 13.04},
    {'F' : 68, 'Rt' : 12.47},
    {'F' : 69.8, 'Rt' : 11.92},
    {'F' : 71.6, 'Rt' : 11.41},
    {'F' : 73.4, 'Rt' : 10.91},
    {'F' : 75.2, 'Rt' : 10.45},
    {'F' : 77, 'Rt' : 10},
    {'F' : 78.8, 'Rt' : 9.575},
    {'F' : 80.6, 'Rt' : 9.17},
    {'F' : 82.4, 'Rt' : 8.784},
    {'F' : 84.2, 'Rt' : 8.416},
    {'F' : 86, 'Rt' : 8.064},
    {'F' : 87.8, 'Rt' : 7.73},
    {'F' : 89.6, 'Rt' : 7.41},
    {'F' : 91.4, 'Rt' : 7.016},
    {'F' : 93.2, 'Rt' : 6.815},
    {'F' : 95, 'Rt' : 6.538},
    {'F' : 96.8, 'Rt' : 6.273},
    {'F' : 98.6, 'Rt' : 6.02},
    {'F' : 100.4, 'Rt' : 5.778},
    {'F' : 102.2, 'Rt' : 5.548},
    {'F' : 104, 'Rt' : 5.327},
    {'F' : 105.8, 'Rt' : 5.117},
    {'F' : 107.6, 'Rt' : 4.915},
    {'F' : 109.4, 'Rt' : 4.723},
    {'F' : 111.2, 'Rt' : 4.539},
    {'F' : 113, 'Rt' : 4.363},
    {'F' : 114.8, 'Rt' : 4.195},
    {'F' : 116.6, 'Rt' : 4.034},
    {'F' : 118.4, 'Rt' : 3.88},
    {'F' : 120.2, 'Rt' : 3.733},
    {'F' : 122, 'Rt' : 3.592},
    {'F' : 123.8, 'Rt' : 3.457},
    {'F' : 125.6, 'Rt' : 3.328},
    {'F' : 127.4, 'Rt' : 3.204},
    {'F' : 129.2, 'Rt' : 3.086},
    {'F' : 131, 'Rt' : 2.972},
    {'F' : 132.8, 'Rt' : 2.863},
    {'F' : 134.6, 'Rt' : 2.759},
    {'F' : 136.4, 'Rt' : 2.659},
    {'F' : 138.2, 'Rt' : 2.564},
    {'F' : 140, 'Rt' : 2.472},
    {'F' : 141.8, 'Rt' : 2.384},
    {'F' : 143.6, 'Rt' : 2.299},
    {'F' : 145.4, 'Rt' : 2.218},
    {'F' : 147.2, 'Rt' : 2.141},
    {'F' : 149, 'Rt' : 2.066},
    {'F' : 150.8, 'Rt' : 1.994},
    {'F' : 152.6, 'Rt' : 1.926},
    {'F' : 154.4, 'Rt' : 1.86},
    {'F' : 156.2, 'Rt' : 1.796},
    {'F' : 158, 'Rt' : 1.735},
    {'F' : 159.8, 'Rt' : 1.677},
    {'F' : 161.6, 'Rt' : 1.621},
    {'F' : 163.4, 'Rt' : 1.567},
    {'F' : 165.2, 'Rt' : 1.515},
    {'F' : 167, 'Rt' : 1.465},
    {'F' : 168.8, 'Rt' : 1.717},
    {'F' : 170.6, 'Rt' : 1.371},
    {'F' : 172.4, 'Rt' : 1.326},
    {'F' : 174.2, 'Rt' : 1.284},
    {'F' : 176, 'Rt' : 1.243},
    {'F' : 177.8, 'Rt' : 1.203},
    {'F' : 179.6, 'Rt' : 1.165},
    {'F' : 181.4, 'Rt' : 1.128},
    {'F' : 183.2, 'Rt' : 1.093},
    {'F' : 185, 'Rt' : 1.059},
    {'F' : 186.8, 'Rt' : 1.027},
    {'F' : 188.6, 'Rt' : 0.9955},
    {'F' : 190.4, 'Rt' : 0.9654},
    {'F' : 192.2, 'Rt' : 0.9363},
    {'F' : 194, 'Rt' : 0.9083},
    {'F' : 195.8, 'Rt' : 0.8812},
    {'F' : 197.6, 'Rt' : 0.855},
    {'F' : 199.4, 'Rt' : 0.8297},
    {'F' : 201.2, 'Rt' : 0.8052},
    {'F' : 203, 'Rt' : 0.7816},
    {'F' : 204.8, 'Rt' : 0.7587},
    {'F' : 206.6, 'Rt' : 0.7366},
    {'F' : 208.4, 'Rt' : 0.7152},
    {'F' : 210.2, 'Rt' : 0.6945},
    {'F' : 212, 'Rt' : 0.6744},
]

beginCFile = """
#include <stdint.h>

"""

endCFile = """

typedef uint8_t (*tempLookup_ptr_t)(uint8_t);
tempLookup_ptr_t tempLookupMsb[] = {tempLookupMsb0,
                                    tempLookupMsb1,
                                    tempLookupMsb2, 
                                    tempLookupMsb3};

tempLookup_ptr_t tempLookupLsb[] = {tempLookupLsb0,
                                    tempLookupLsb1};


int16_t tempLookup( uint16_t index )
{
    uint8_t msb, lsb;
    uint16_t result = 0x0000;

    msb = tempLookupMsb[(index & 0x0300) >> 8]((uint8_t)(index & 0x00FF));
    
    lsb = tempLookupLsb[(index & 0x0200) >> 9]((uint8_t)((index & 0x01FF) >> 1));
    if( !(index & 0x0001) )
    {
        result = (uint16_t)msb << 8 | (uint16_t)((lsb & 0x0F) << 4);
    }
    else
    {
        result = (uint16_t)msb << 8 | (uint16_t)(lsb & 0xF0);
    }

    return *(int16_t *)&result;
}
"""

def applyPoly( x, coef ):
    y = 0.0
    for pow in range( 0, len(coef) ):
        y += x**pow * coef[pow]
    return y

def findTempFromRtUsingPoly( Rt, lowRange, hiRange ):
    coef = [77.3759745285,
            -2.37495975063,
            0.0445001891604,
            -0.000561417684203,
            0.00000252632267812,
            0.0000000392996719646,
            -0.000000000693695706632,
            0.00000000000453286511074,
            -0.0000000000000139685794136,
            1.68495137249E-017]

    inc = 0.01
    leastErr = float( "inf" )
    best = lowRange

    x = lowRange
    while( x <= hiRange ):
        val = applyPoly( x, coef )
        if( (val - Rt)**2 < leastErr**2 ):
            best = x
            leastErr = (val - Rt)
        x += inc
        # print( "x = %f" % (x) )

    return best

def findTempFromRtUsingTable( Rt, lowRange, hiRange ):
    for i, tableDat in enumerate( tempTable ):
        if( Rt <= tableDat['Rt'] ):
            datIdx = i

    m = (tempTable[datIdx+1]['F'] - tempTable[datIdx]['F']) / (tempTable[datIdx+1]['Rt'] - tempTable[datIdx]['Rt'])
    b = tempTable[datIdx]['F'] - m * tempTable[datIdx]['Rt']
    temp = m * Rt + b
    return temp

if __name__ == "__main__":

    Ri = R1 * R2 / (R1 + R2)
    A = Rf/Ri + 1.0
    Voff = Vcc * (R2 / (R1 + R2)) * Rf / Ri

    print( 'Sample,Vrt,Rt,F,8.8' )

    batch256 = 0
    for sampleIndex in range(0, 1024):
        AmpOut = Vcc * float(sampleIndex) / 1024.0
        Vrt = (AmpOut + Voff) / A
        Rt = Vrt * R / (Vcc - Vrt)
        # F = findTempFromRtUsingPoly( Rt, -20.0, 115.0 )
        F = findTempFromRtUsingTable( Rt, -20.0, 115.0 )
        EDotE = int(F * 256.0)
        EDotE = EDotE & 0x0000FFFF
        if( (sampleIndex % 256) == 0 ):
            batch256 += 1

        cmtStr = "%d,%f,%f,%f,0x%04X" % (sampleIndex, Vrt, Rt, F, EDotE)
        print( '%s' % (cmtStr) )
        if( ((sampleIndex + 1) % 256) == 0 ):
            pass

    # Do lower (LSB) btyes
    batch256 = 0
    thisWord = 0x00
    for sampleIndex in range(0, 1024):
        AmpOut = Vcc * float(sampleIndex) / 1024.0
        Vrt = (AmpOut + Voff) / A
        Rt = Vrt * R / (Vcc - Vrt)
        # F = findTempFromRtUsingPoly( Rt, -20.0, 115.0 )
        F = findTempFromRtUsingTable( Rt, -20.0, 115.0 )
        EDotE = int(F * 256.0)
        EDotE = EDotE & 0x0000FFFF
        thisIndex = sampleIndex >> 1
        if( (sampleIndex % 512) == 0 ):
            batch256 += 1

        cmtStr = "Sample = %d,%f,%f,%f,0x%04X" % (sampleIndex, Vrt, Rt, F, EDotE)
        # print( '%s' % (cmtStr) )
        if( sampleIndex & 0x0001 ):
            thisWord = (EDotE & 0x00F0) | thisWord
            thisWord = 0x00
        else:
            thisWord = (EDotE & 0xF0) >> 4

        if( ((sampleIndex + 1) % 512) == 0 ):
            pass


