#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=beerChipA2D.c beerChipBeermon.c beerChipBlinker.c beerChipI2C.c beerChipISR.c beerChipLed.c beerChipMain.c beerChipTempLookup.c beerChipUserTimer.c beerLock.c beerchipRelay.c beerChipBloopDet.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/beerChipA2D.p1 ${OBJECTDIR}/beerChipBeermon.p1 ${OBJECTDIR}/beerChipBlinker.p1 ${OBJECTDIR}/beerChipI2C.p1 ${OBJECTDIR}/beerChipISR.p1 ${OBJECTDIR}/beerChipLed.p1 ${OBJECTDIR}/beerChipMain.p1 ${OBJECTDIR}/beerChipTempLookup.p1 ${OBJECTDIR}/beerChipUserTimer.p1 ${OBJECTDIR}/beerLock.p1 ${OBJECTDIR}/beerchipRelay.p1 ${OBJECTDIR}/beerChipBloopDet.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/beerChipA2D.p1.d ${OBJECTDIR}/beerChipBeermon.p1.d ${OBJECTDIR}/beerChipBlinker.p1.d ${OBJECTDIR}/beerChipI2C.p1.d ${OBJECTDIR}/beerChipISR.p1.d ${OBJECTDIR}/beerChipLed.p1.d ${OBJECTDIR}/beerChipMain.p1.d ${OBJECTDIR}/beerChipTempLookup.p1.d ${OBJECTDIR}/beerChipUserTimer.p1.d ${OBJECTDIR}/beerLock.p1.d ${OBJECTDIR}/beerchipRelay.p1.d ${OBJECTDIR}/beerChipBloopDet.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/beerChipA2D.p1 ${OBJECTDIR}/beerChipBeermon.p1 ${OBJECTDIR}/beerChipBlinker.p1 ${OBJECTDIR}/beerChipI2C.p1 ${OBJECTDIR}/beerChipISR.p1 ${OBJECTDIR}/beerChipLed.p1 ${OBJECTDIR}/beerChipMain.p1 ${OBJECTDIR}/beerChipTempLookup.p1 ${OBJECTDIR}/beerChipUserTimer.p1 ${OBJECTDIR}/beerLock.p1 ${OBJECTDIR}/beerchipRelay.p1 ${OBJECTDIR}/beerChipBloopDet.p1

# Source Files
SOURCEFILES=beerChipA2D.c beerChipBeermon.c beerChipBlinker.c beerChipI2C.c beerChipISR.c beerChipLed.c beerChipMain.c beerChipTempLookup.c beerChipUserTimer.c beerLock.c beerchipRelay.c beerChipBloopDet.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=16F1509
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/beerChipA2D.p1: beerChipA2D.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipA2D.p1.d 
	@${RM} ${OBJECTDIR}/beerChipA2D.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipA2D.p1 beerChipA2D.c 
	@-${MV} ${OBJECTDIR}/beerChipA2D.d ${OBJECTDIR}/beerChipA2D.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipA2D.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipBeermon.p1: beerChipBeermon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipBeermon.p1.d 
	@${RM} ${OBJECTDIR}/beerChipBeermon.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipBeermon.p1 beerChipBeermon.c 
	@-${MV} ${OBJECTDIR}/beerChipBeermon.d ${OBJECTDIR}/beerChipBeermon.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipBeermon.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipBlinker.p1: beerChipBlinker.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipBlinker.p1.d 
	@${RM} ${OBJECTDIR}/beerChipBlinker.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipBlinker.p1 beerChipBlinker.c 
	@-${MV} ${OBJECTDIR}/beerChipBlinker.d ${OBJECTDIR}/beerChipBlinker.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipBlinker.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipI2C.p1: beerChipI2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipI2C.p1.d 
	@${RM} ${OBJECTDIR}/beerChipI2C.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipI2C.p1 beerChipI2C.c 
	@-${MV} ${OBJECTDIR}/beerChipI2C.d ${OBJECTDIR}/beerChipI2C.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipI2C.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipISR.p1: beerChipISR.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipISR.p1.d 
	@${RM} ${OBJECTDIR}/beerChipISR.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipISR.p1 beerChipISR.c 
	@-${MV} ${OBJECTDIR}/beerChipISR.d ${OBJECTDIR}/beerChipISR.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipISR.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipLed.p1: beerChipLed.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipLed.p1.d 
	@${RM} ${OBJECTDIR}/beerChipLed.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipLed.p1 beerChipLed.c 
	@-${MV} ${OBJECTDIR}/beerChipLed.d ${OBJECTDIR}/beerChipLed.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipLed.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipMain.p1: beerChipMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipMain.p1.d 
	@${RM} ${OBJECTDIR}/beerChipMain.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipMain.p1 beerChipMain.c 
	@-${MV} ${OBJECTDIR}/beerChipMain.d ${OBJECTDIR}/beerChipMain.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipMain.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipTempLookup.p1: beerChipTempLookup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipTempLookup.p1.d 
	@${RM} ${OBJECTDIR}/beerChipTempLookup.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipTempLookup.p1 beerChipTempLookup.c 
	@-${MV} ${OBJECTDIR}/beerChipTempLookup.d ${OBJECTDIR}/beerChipTempLookup.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipTempLookup.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipUserTimer.p1: beerChipUserTimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipUserTimer.p1.d 
	@${RM} ${OBJECTDIR}/beerChipUserTimer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipUserTimer.p1 beerChipUserTimer.c 
	@-${MV} ${OBJECTDIR}/beerChipUserTimer.d ${OBJECTDIR}/beerChipUserTimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipUserTimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerLock.p1: beerLock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerLock.p1.d 
	@${RM} ${OBJECTDIR}/beerLock.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerLock.p1 beerLock.c 
	@-${MV} ${OBJECTDIR}/beerLock.d ${OBJECTDIR}/beerLock.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerLock.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerchipRelay.p1: beerchipRelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerchipRelay.p1.d 
	@${RM} ${OBJECTDIR}/beerchipRelay.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerchipRelay.p1 beerchipRelay.c 
	@-${MV} ${OBJECTDIR}/beerchipRelay.d ${OBJECTDIR}/beerchipRelay.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerchipRelay.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipBloopDet.p1: beerChipBloopDet.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipBloopDet.p1.d 
	@${RM} ${OBJECTDIR}/beerChipBloopDet.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipBloopDet.p1 beerChipBloopDet.c 
	@-${MV} ${OBJECTDIR}/beerChipBloopDet.d ${OBJECTDIR}/beerChipBloopDet.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipBloopDet.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/beerChipA2D.p1: beerChipA2D.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipA2D.p1.d 
	@${RM} ${OBJECTDIR}/beerChipA2D.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipA2D.p1 beerChipA2D.c 
	@-${MV} ${OBJECTDIR}/beerChipA2D.d ${OBJECTDIR}/beerChipA2D.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipA2D.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipBeermon.p1: beerChipBeermon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipBeermon.p1.d 
	@${RM} ${OBJECTDIR}/beerChipBeermon.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipBeermon.p1 beerChipBeermon.c 
	@-${MV} ${OBJECTDIR}/beerChipBeermon.d ${OBJECTDIR}/beerChipBeermon.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipBeermon.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipBlinker.p1: beerChipBlinker.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipBlinker.p1.d 
	@${RM} ${OBJECTDIR}/beerChipBlinker.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipBlinker.p1 beerChipBlinker.c 
	@-${MV} ${OBJECTDIR}/beerChipBlinker.d ${OBJECTDIR}/beerChipBlinker.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipBlinker.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipI2C.p1: beerChipI2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipI2C.p1.d 
	@${RM} ${OBJECTDIR}/beerChipI2C.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipI2C.p1 beerChipI2C.c 
	@-${MV} ${OBJECTDIR}/beerChipI2C.d ${OBJECTDIR}/beerChipI2C.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipI2C.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipISR.p1: beerChipISR.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipISR.p1.d 
	@${RM} ${OBJECTDIR}/beerChipISR.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipISR.p1 beerChipISR.c 
	@-${MV} ${OBJECTDIR}/beerChipISR.d ${OBJECTDIR}/beerChipISR.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipISR.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipLed.p1: beerChipLed.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipLed.p1.d 
	@${RM} ${OBJECTDIR}/beerChipLed.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipLed.p1 beerChipLed.c 
	@-${MV} ${OBJECTDIR}/beerChipLed.d ${OBJECTDIR}/beerChipLed.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipLed.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipMain.p1: beerChipMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipMain.p1.d 
	@${RM} ${OBJECTDIR}/beerChipMain.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipMain.p1 beerChipMain.c 
	@-${MV} ${OBJECTDIR}/beerChipMain.d ${OBJECTDIR}/beerChipMain.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipMain.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipTempLookup.p1: beerChipTempLookup.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipTempLookup.p1.d 
	@${RM} ${OBJECTDIR}/beerChipTempLookup.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipTempLookup.p1 beerChipTempLookup.c 
	@-${MV} ${OBJECTDIR}/beerChipTempLookup.d ${OBJECTDIR}/beerChipTempLookup.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipTempLookup.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipUserTimer.p1: beerChipUserTimer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipUserTimer.p1.d 
	@${RM} ${OBJECTDIR}/beerChipUserTimer.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipUserTimer.p1 beerChipUserTimer.c 
	@-${MV} ${OBJECTDIR}/beerChipUserTimer.d ${OBJECTDIR}/beerChipUserTimer.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipUserTimer.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerLock.p1: beerLock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerLock.p1.d 
	@${RM} ${OBJECTDIR}/beerLock.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerLock.p1 beerLock.c 
	@-${MV} ${OBJECTDIR}/beerLock.d ${OBJECTDIR}/beerLock.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerLock.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerchipRelay.p1: beerchipRelay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerchipRelay.p1.d 
	@${RM} ${OBJECTDIR}/beerchipRelay.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerchipRelay.p1 beerchipRelay.c 
	@-${MV} ${OBJECTDIR}/beerchipRelay.d ${OBJECTDIR}/beerchipRelay.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerchipRelay.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/beerChipBloopDet.p1: beerChipBloopDet.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/beerChipBloopDet.p1.d 
	@${RM} ${OBJECTDIR}/beerChipBloopDet.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     -o ${OBJECTDIR}/beerChipBloopDet.p1 beerChipBloopDet.c 
	@-${MV} ${OBJECTDIR}/beerChipBloopDet.d ${OBJECTDIR}/beerChipBloopDet.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/beerChipBloopDet.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -std=c99 -gdwarf-3 -mstack=compiled:auto:auto      -mram=default,-320-32f  $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.hex 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1  -fno-short-double -fno-short-float -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx032 -Wl,--data-init -mno-keep-startup -mno-osccal -mno-resetbits -mno-save-resetbits -mno-download -mno-stackcall -std=c99 -gdwarf-3 -mstack=compiled:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -o dist/${CND_CONF}/${IMAGE_TYPE}/beerChip.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
