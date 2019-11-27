/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef DECODEL2_LIB_H
#define DECODEL2_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal 
 *  library components of the 'decodel2' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_DECODEL2
   #define DECODEL2_EXT
#else
   #define DECODEL2_EXT         extern
#endif

/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <recipe_api.h>         //  API for all recipe_*            PUBLIC
                                //*******************************************

/****************************************************************************
 * Library Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define URF_START               "<><><> RIA Unformatted Rcipe <><><>"
#define URF_START_L             strlen( URF_START )
//----------------------------------------------------------------------------
#define URF_FDIR                "FILE-DIR:"
#define URF_FDIR_L              strlen( URF_FDIR )
//----------------------------------------------------------------------------
#define URF_FNAME               "FILE-NAME:"
#define URF_FNAME_L             strlen( URF_FNAME )
//----------------------------------------------------------------------------
#define URF_FDATE               "FILE-DATE:"
#define URF_FDATE_L             strlen( URF_FDATE )
//----------------------------------------------------------------------------
#define URF_FSIZE               "FILE-SIZE:"
#define URF_FSIZE_L             strlen( URF_FSIZE )
//----------------------------------------------------------------------------
#define URF_GFROM               "GROUP-FROM:"
#define URF_GFROM_L             strlen( URF_GFROM )
//----------------------------------------------------------------------------
#define URF_GSUB                "GROUP-SUBJECT:"
#define URF_GSUB_L              strlen( URF_GSUB )
//----------------------------------------------------------------------------
#define URF_GDATE               "GROUP-DATE:"
#define URF_GDATE_L             strlen( URF_GDATE )
//----------------------------------------------------------------------------
#define URF_MFROM               "MAIL-FROM:"
#define URF_MFROM_L             strlen( URF_MFROM )
//----------------------------------------------------------------------------
#define URF_MSUB                "MAIL-SUBJECT:"
#define URF_MSUB_L              strlen( URF_MSUB )
//----------------------------------------------------------------------------
#define URF_MDATE               "MAIL-DATE:"
#define URF_MDATE_L             strlen( URF_MDATE )
//----------------------------------------------------------------------------
#define URF_END                 "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
#define URF_END_L               strlen( URF_END )
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
void
DECODEL2__start_decode(
    struct  list_base_t         *   level3_list_p,
    struct  source_info_t       *   source_info_p,
    enum    recipe_format_e         recipe_format
    );
//----------------------------------------------------------------------------
int
DECODEL2__recover_info(
    struct  source_info_t       *   source_info_p,
    char                        *   list_data_p
    );
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DECODEL2_LIB_H
