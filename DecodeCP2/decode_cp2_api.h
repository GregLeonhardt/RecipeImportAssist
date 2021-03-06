/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef DECODE_CP2_API_H
#define DECODE_CP2_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external 
 *  library components of the 'decode_cp2' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <decode_api.h>         //  API for all decode_*            PUBLIC
                                //*******************************************
#include <email_api.h>          //  API for all email_*             PUBLIC
#include <recipe_api.h>         //  API for all recipe_*            PUBLIC
                                //*******************************************

/****************************************************************************
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Storage Allocation
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************
 * Library Public Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
void
decode_cp2(
    struct  list_base_t         *   cp2_list_p,
    struct  source_info_t       *   source_info_p
    );
//---------------------------------------------------------------------------
int
decode_cp2_start(
    char                        *   data_p
    );
//---------------------------------------------------------------------------
int
decode_cp2_end(
    char                        *   data_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DECODE_CP2_API_H
