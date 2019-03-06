/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decodel2' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <decode_mxp_api.h>     //  API for all decode_mxp_*        PUBLIC
#include <decode_nyc_api.h>     //  API for all decode_nyc_*        PUBLIC
                                //*******************************************
#include <decodel2_api.h>       //  API for all decodel2_*          PUBLIC
#include "decodel2_lib.h"       //  API for all DECODEL2__*         PRIVATE
                                //*******************************************

/****************************************************************************
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Definitions local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Structures local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Storage Allocation local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  When this function is called we we have come to th end of an e-Mail and
 *  there is stuff in the level-3 list.  Find the recipe format we are
 *  working on and give it's decoder a chance to decode it.
 *
 *  @param  level3_list_p       Pointer to a list where the file is stored
 *  @param  source_info_p        Pointer to e-Mail information
 *  @param  recipe_format       The recipe format being decoded
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODEL2__start_decode(
    struct  list_base_t         *   level3_list_p,
    struct  source_info_t       *   source_info_p,
    enum    recipe_format_e         recipe_format
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Select the recipe format that is in use
    switch( recipe_format )
    {
        case    RECIPE_FORMAT_NONE:
        {

        }   break;

        case    RECIPE_FORMAT_MXP:
        {
            //  Process the recipe
            decode_mxp( level3_list_p, source_info_p );
        }   break;

        case    RECIPE_FORMAT_MMF:
        {

        }   break;

        case    RECIPE_FORMAT_MX2:
        {

        }   break;

        case    RECIPE_FORMAT_BOF:
        {

        }   break;

        case    RECIPE_FORMAT_RXF:
        {

        }   break;

        case    RECIPE_FORMAT_NYC:
        {
            //  Process the recipe
            decode_nyc( level3_list_p, source_info_p );
        }   break;

        case    RECIPE_FORMAT_CP2:
        {

        }   break;

        case    RECIPE_FORMAT_GRF:
        {

        }   break;

        case    RECIPE_FORMAT_GF2:
        {

        }   break;

        case    RECIPE_FORMAT_TXT:
        {

        }   break;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

     // DONE!
}
/****************************************************************************/
