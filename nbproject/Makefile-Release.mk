#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/DecodeBOF/decode_bof_api.o \
	${OBJECTDIR}/DecodeBOF/decode_bof_lib.o \
	${OBJECTDIR}/DecodeCP2/decode_cp2_api.o \
	${OBJECTDIR}/DecodeCP2/decode_cp2_lib.o \
	${OBJECTDIR}/DecodeERD/decode_erd_api.o \
	${OBJECTDIR}/DecodeERD/decode_erd_lib.o \
	${OBJECTDIR}/DecodeGF2/decode_gf2_api.o \
	${OBJECTDIR}/DecodeGF2/decode_gf2_lib.o \
	${OBJECTDIR}/DecodeGRF/decode_grf_api.o \
	${OBJECTDIR}/DecodeGRF/decode_grf_lib.o \
	${OBJECTDIR}/DecodeHTML/decode_html_api.o \
	${OBJECTDIR}/DecodeHTML/decode_html_lib.o \
	${OBJECTDIR}/DecodeL1/decodel1_api.o \
	${OBJECTDIR}/DecodeL1/decodel1_lib.o \
	${OBJECTDIR}/DecodeL2/decodel2_api.o \
	${OBJECTDIR}/DecodeL2/decodel2_lib.o \
	${OBJECTDIR}/DecodeLIB/decode_api.o \
	${OBJECTDIR}/DecodeLIB/decode_lib.o \
	${OBJECTDIR}/DecodeMMF/decode_mmf_api.o \
	${OBJECTDIR}/DecodeMMF/decode_mmf_lib.o \
	${OBJECTDIR}/DecodeMX2/decode_mx2_api.o \
	${OBJECTDIR}/DecodeMX2/decode_mx2_lib.o \
	${OBJECTDIR}/DecodeMXP/decode_mxp_api.o \
	${OBJECTDIR}/DecodeMXP/decode_mxp_lib.o \
	${OBJECTDIR}/DecodeNYC/decode_nyc_api.o \
	${OBJECTDIR}/DecodeNYC/decode_nyc_lib.o \
	${OBJECTDIR}/DecodeTXT/decode_txt_api.o \
	${OBJECTDIR}/DecodeTXT/decode_txt_lib.o \
	${OBJECTDIR}/Encode/encode_api.o \
	${OBJECTDIR}/Encode/encode_lib.o \
	${OBJECTDIR}/EncodeRIA/encode_ria_api.o \
	${OBJECTDIR}/EncodeRIA/encode_ria_lib.o \
	${OBJECTDIR}/EncodeRXF/encode_rxf_api.o \
	${OBJECTDIR}/EncodeRXF/encode_rxf_lib.o \
	${OBJECTDIR}/FileScan/filescan_api.o \
	${OBJECTDIR}/FileScan/filescan_lib.o \
	${OBJECTDIR}/Recipe/recipe_api.o \
	${OBJECTDIR}/Recipe/recipe_lib.o \
	${OBJECTDIR}/Xlate/xlate_api.o \
	${OBJECTDIR}/Xlate/xlate_lib.o \
	${OBJECTDIR}/eMail/email_api.o \
	${OBJECTDIR}/eMail/email_lib.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/recipeimportassist

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/recipeimportassist: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/recipeimportassist ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/DecodeBOF/decode_bof_api.o: DecodeBOF/decode_bof_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeBOF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeBOF/decode_bof_api.o DecodeBOF/decode_bof_api.c

${OBJECTDIR}/DecodeBOF/decode_bof_lib.o: DecodeBOF/decode_bof_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeBOF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeBOF/decode_bof_lib.o DecodeBOF/decode_bof_lib.c

${OBJECTDIR}/DecodeCP2/decode_cp2_api.o: DecodeCP2/decode_cp2_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeCP2
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeCP2/decode_cp2_api.o DecodeCP2/decode_cp2_api.c

${OBJECTDIR}/DecodeCP2/decode_cp2_lib.o: DecodeCP2/decode_cp2_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeCP2
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeCP2/decode_cp2_lib.o DecodeCP2/decode_cp2_lib.c

${OBJECTDIR}/DecodeERD/decode_erd_api.o: DecodeERD/decode_erd_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeERD
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeERD/decode_erd_api.o DecodeERD/decode_erd_api.c

${OBJECTDIR}/DecodeERD/decode_erd_lib.o: DecodeERD/decode_erd_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeERD
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeERD/decode_erd_lib.o DecodeERD/decode_erd_lib.c

${OBJECTDIR}/DecodeGF2/decode_gf2_api.o: DecodeGF2/decode_gf2_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeGF2
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeGF2/decode_gf2_api.o DecodeGF2/decode_gf2_api.c

${OBJECTDIR}/DecodeGF2/decode_gf2_lib.o: DecodeGF2/decode_gf2_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeGF2
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeGF2/decode_gf2_lib.o DecodeGF2/decode_gf2_lib.c

${OBJECTDIR}/DecodeGRF/decode_grf_api.o: DecodeGRF/decode_grf_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeGRF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeGRF/decode_grf_api.o DecodeGRF/decode_grf_api.c

${OBJECTDIR}/DecodeGRF/decode_grf_lib.o: DecodeGRF/decode_grf_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeGRF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeGRF/decode_grf_lib.o DecodeGRF/decode_grf_lib.c

${OBJECTDIR}/DecodeHTML/decode_html_api.o: DecodeHTML/decode_html_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeHTML
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeHTML/decode_html_api.o DecodeHTML/decode_html_api.c

${OBJECTDIR}/DecodeHTML/decode_html_lib.o: DecodeHTML/decode_html_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeHTML
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeHTML/decode_html_lib.o DecodeHTML/decode_html_lib.c

${OBJECTDIR}/DecodeL1/decodel1_api.o: DecodeL1/decodel1_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeL1
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeL1/decodel1_api.o DecodeL1/decodel1_api.c

${OBJECTDIR}/DecodeL1/decodel1_lib.o: DecodeL1/decodel1_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeL1
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeL1/decodel1_lib.o DecodeL1/decodel1_lib.c

${OBJECTDIR}/DecodeL2/decodel2_api.o: DecodeL2/decodel2_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeL2
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeL2/decodel2_api.o DecodeL2/decodel2_api.c

${OBJECTDIR}/DecodeL2/decodel2_lib.o: DecodeL2/decodel2_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeL2
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeL2/decodel2_lib.o DecodeL2/decodel2_lib.c

${OBJECTDIR}/DecodeLIB/decode_api.o: DecodeLIB/decode_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeLIB
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeLIB/decode_api.o DecodeLIB/decode_api.c

${OBJECTDIR}/DecodeLIB/decode_lib.o: DecodeLIB/decode_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeLIB
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeLIB/decode_lib.o DecodeLIB/decode_lib.c

${OBJECTDIR}/DecodeMMF/decode_mmf_api.o: DecodeMMF/decode_mmf_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeMMF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeMMF/decode_mmf_api.o DecodeMMF/decode_mmf_api.c

${OBJECTDIR}/DecodeMMF/decode_mmf_lib.o: DecodeMMF/decode_mmf_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeMMF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeMMF/decode_mmf_lib.o DecodeMMF/decode_mmf_lib.c

${OBJECTDIR}/DecodeMX2/decode_mx2_api.o: DecodeMX2/decode_mx2_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeMX2
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeMX2/decode_mx2_api.o DecodeMX2/decode_mx2_api.c

${OBJECTDIR}/DecodeMX2/decode_mx2_lib.o: DecodeMX2/decode_mx2_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeMX2
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeMX2/decode_mx2_lib.o DecodeMX2/decode_mx2_lib.c

${OBJECTDIR}/DecodeMXP/decode_mxp_api.o: DecodeMXP/decode_mxp_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeMXP
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeMXP/decode_mxp_api.o DecodeMXP/decode_mxp_api.c

${OBJECTDIR}/DecodeMXP/decode_mxp_lib.o: DecodeMXP/decode_mxp_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeMXP
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeMXP/decode_mxp_lib.o DecodeMXP/decode_mxp_lib.c

${OBJECTDIR}/DecodeNYC/decode_nyc_api.o: DecodeNYC/decode_nyc_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeNYC
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeNYC/decode_nyc_api.o DecodeNYC/decode_nyc_api.c

${OBJECTDIR}/DecodeNYC/decode_nyc_lib.o: DecodeNYC/decode_nyc_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeNYC
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeNYC/decode_nyc_lib.o DecodeNYC/decode_nyc_lib.c

${OBJECTDIR}/DecodeTXT/decode_txt_api.o: DecodeTXT/decode_txt_api.c
	${MKDIR} -p ${OBJECTDIR}/DecodeTXT
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeTXT/decode_txt_api.o DecodeTXT/decode_txt_api.c

${OBJECTDIR}/DecodeTXT/decode_txt_lib.o: DecodeTXT/decode_txt_lib.c
	${MKDIR} -p ${OBJECTDIR}/DecodeTXT
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/DecodeTXT/decode_txt_lib.o DecodeTXT/decode_txt_lib.c

${OBJECTDIR}/Encode/encode_api.o: Encode/encode_api.c
	${MKDIR} -p ${OBJECTDIR}/Encode
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Encode/encode_api.o Encode/encode_api.c

${OBJECTDIR}/Encode/encode_lib.o: Encode/encode_lib.c
	${MKDIR} -p ${OBJECTDIR}/Encode
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Encode/encode_lib.o Encode/encode_lib.c

${OBJECTDIR}/EncodeRIA/encode_ria_api.o: EncodeRIA/encode_ria_api.c
	${MKDIR} -p ${OBJECTDIR}/EncodeRIA
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EncodeRIA/encode_ria_api.o EncodeRIA/encode_ria_api.c

${OBJECTDIR}/EncodeRIA/encode_ria_lib.o: EncodeRIA/encode_ria_lib.c
	${MKDIR} -p ${OBJECTDIR}/EncodeRIA
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EncodeRIA/encode_ria_lib.o EncodeRIA/encode_ria_lib.c

${OBJECTDIR}/EncodeRXF/encode_rxf_api.o: EncodeRXF/encode_rxf_api.c
	${MKDIR} -p ${OBJECTDIR}/EncodeRXF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EncodeRXF/encode_rxf_api.o EncodeRXF/encode_rxf_api.c

${OBJECTDIR}/EncodeRXF/encode_rxf_lib.o: EncodeRXF/encode_rxf_lib.c
	${MKDIR} -p ${OBJECTDIR}/EncodeRXF
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/EncodeRXF/encode_rxf_lib.o EncodeRXF/encode_rxf_lib.c

${OBJECTDIR}/FileScan/filescan_api.o: FileScan/filescan_api.c
	${MKDIR} -p ${OBJECTDIR}/FileScan
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileScan/filescan_api.o FileScan/filescan_api.c

${OBJECTDIR}/FileScan/filescan_lib.o: FileScan/filescan_lib.c
	${MKDIR} -p ${OBJECTDIR}/FileScan
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/FileScan/filescan_lib.o FileScan/filescan_lib.c

${OBJECTDIR}/Recipe/recipe_api.o: Recipe/recipe_api.c
	${MKDIR} -p ${OBJECTDIR}/Recipe
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Recipe/recipe_api.o Recipe/recipe_api.c

${OBJECTDIR}/Recipe/recipe_lib.o: Recipe/recipe_lib.c
	${MKDIR} -p ${OBJECTDIR}/Recipe
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Recipe/recipe_lib.o Recipe/recipe_lib.c

${OBJECTDIR}/Xlate/xlate_api.o: Xlate/xlate_api.c
	${MKDIR} -p ${OBJECTDIR}/Xlate
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Xlate/xlate_api.o Xlate/xlate_api.c

${OBJECTDIR}/Xlate/xlate_lib.o: Xlate/xlate_lib.c
	${MKDIR} -p ${OBJECTDIR}/Xlate
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Xlate/xlate_lib.o Xlate/xlate_lib.c

${OBJECTDIR}/eMail/email_api.o: eMail/email_api.c
	${MKDIR} -p ${OBJECTDIR}/eMail
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/eMail/email_api.o eMail/email_api.c

${OBJECTDIR}/eMail/email_lib.o: eMail/email_lib.c
	${MKDIR} -p ${OBJECTDIR}/eMail
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/eMail/email_lib.o eMail/email_lib.c

${OBJECTDIR}/main.o: main.c
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
