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
#include <stdio.h>              //  Standard I/O definitions
#include <string.h>             //  Functions for managing strings
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

        case    RECIPE_FORMAT_CP2:
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
/**
 *  When a recipe format is unknown, source information is written to the
 *  'unformatted_recipe' file.  This function is called when a second
 *  attempt is made at decoding that file.  (With the assumption that
 *  there has been some manual formatting to get recipes into a known format.
 *
 *  @param  source_info_p       Pointer to e-Mail information
 *  @param list_data_p          Pointer to the read data
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODEL2__recover_info(
    struct  source_info_t       *   source_info_p,
    char                        *   list_data_p
    )
{
    /**
     *  @param  un_fmt_output   Flag showing input data from unformatted    */
    int                             un_fmt_output;
    /**
     *  @param list_data_p      Pointer to the read data                    */
    char                        *   data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The flag should be set until the end is detected
    un_fmt_output = true;

    /************************************************************************
     *  Function
     ************************************************************************/


    //  YES:    "FILE-DIR:"
    if ( strncmp( list_data_p, URF_FDIR, URF_FDIR_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_FDIR_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_FDIR_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->f_dir_name, data_p, strlen( data_p ) );
        }
    }
    //  "FILE-NAME:"
    else
    if ( strncmp( list_data_p, URF_FNAME, URF_FNAME_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_FNAME_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_FNAME_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->f_file_name, data_p, strlen( data_p ) );
        }
    }
    //  "FILE-DATE:"
    else
    if ( strncmp( list_data_p, URF_FDATE, URF_FDATE_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_FDATE_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_FDATE_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->f_date_time, data_p, strlen( data_p ) );
        }
    }
    //  "FILE-SIZE:"
    else
    if ( strncmp( list_data_p, URF_FSIZE, URF_FSIZE_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_FSIZE_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_FSIZE_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->f_file_size, data_p, strlen( data_p ) );
        }
    }
    //  "GROUP-FROM:"
    else
    if ( strncmp( list_data_p, URF_GFROM, URF_GFROM_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_GFROM_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_GFROM_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->g_from, data_p, strlen( data_p ) );
        }
    }
    //  "GROUP-SUBJECT:"
    else
    if ( strncmp( list_data_p, URF_GSUB, URF_GSUB_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_GSUB_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_GSUB_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->g_subject, data_p, strlen( data_p ) );
        }
    }
    //  "GROUP-DATE:"
    else
    if ( strncmp( list_data_p, URF_GDATE, URF_GDATE_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_GDATE_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_GDATE_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->g_datetime, data_p, strlen( data_p ) );
        }
    }
    //  "MAIL-FROM:"
    else
    if ( strncmp( list_data_p, URF_MFROM, URF_MFROM_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_MFROM_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_MFROM_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->e_from, data_p, strlen( data_p ) );
        }
    }
    //  "MAIL-SUBJECT:"
    else
    if ( strncmp( list_data_p, URF_MSUB, URF_MSUB_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_MSUB_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_MSUB_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->e_subject, data_p, strlen( data_p ) );
        }
    }
    //  "MAIL-DATE:"
    else
    if ( strncmp( list_data_p, URF_MDATE, URF_MDATE_L ) == 0 )
    {
        //  YES:    Is there anything saved ?
        if ( strlen( list_data_p ) > URF_MDATE_L + 2 )
        {
            //  YES:    Point to the data
            data_p = list_data_p + URF_MDATE_L + 1;

            //  Locate the next character in the buffer
            data_p = text_skip_past_whitespace( data_p );

            strncpy( source_info_p->e_datetime, data_p, strlen( data_p ) );
        }
    }
    //  "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
    else
    if ( strncmp( list_data_p, URF_END, URF_END_L ) == 0 )
    {
        //  YES:    All done, clear the flag
        un_fmt_output = false;
    }
    else
    {
        //  OOPS:   Shouldn't ever get here
        log_write( MID_WARNING, "DECODEL2__recover_info",
                   "Failed to detect a valid file header\n" );
        
        //  All done, clear the flag
        un_fmt_output = false;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

     // DONE!
    return( un_fmt_output );
}
/****************************************************************************/
