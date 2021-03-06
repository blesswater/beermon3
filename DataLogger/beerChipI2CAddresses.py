

beerChipI2CCmdAddr = {
    'BEERCHIP_I2C_PROD_INDEX' : 0x00,
    'BEERCHIP_I2C_MAJ_VER_INDEX' : 0x01,
    'BEERCHIP_I2C_MIN_VER_INDEX' : 0x02,
    'BEERCHIP_I2C_PROD_BUILD_INDEX' : 0x03,
    'BEERCHIP_BEERMON_CFG_CNTL_PROBE' : 0x3A,
    'BEERCHIP_I2C_UPTIME_BYTE0' : 0x04,
    'BEERCHIP_BEERMON_CFG_SETPT' : 0x30,
    'BEERCHIP_BEERMON_CNTL_CMD' : 0x4F,
    'BEERCHIP_BEERMON_STATE_STATE' : 0x4E,
    'BEERCHIP_BEERMON_CFG_UPDATE_STATE' : 0x3B,
    'BEERCHIP_BEERMON_BLOOPDET_STATE' : 0x50,
    'BEERCHIP_BEERMON_BLOOPDET_CNT' : 0x52,
    'BEERCHIP_BEERMON_BLOOPDET_ACK_CNT' : 0x54,
}

beerChipI2CConstants = {
    'BEERCHIP_PRODUCT_ID' : 0x0B,
}

beerChipTemperatureProbe = [
    {'Trigger' : 0x10, 'TempIndex' : 0x12, 'D2AIndex' : 0x14},
    {'Trigger' : 0x10, 'TempIndex' : 0x18, 'D2AIndex' : 0x1A},
    {'Trigger' : 0x10, 'TempIndex' : 0x22, 'D2AIndex' : 0x24},
    {'Trigger' : 0x10, 'TempIndex' : 0x28, 'D2AIndex' : 0x2A},
]
