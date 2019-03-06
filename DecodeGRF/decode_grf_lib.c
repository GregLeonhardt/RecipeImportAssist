/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_grf' library.
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
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <ctype.h>              //  Determine the type contained
#include <string.h>             //  Functions for managing strings
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <decode_api.h>         //  API for all decode_*            PUBLIC
#include <recipe_api.h>         //  API for all recipe_*            PUBLIC
#include <xlate_api.h>          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include <decode_grf_api.h>     //  API for all decode_grf_*        PUBLIC
#include "decode_grf_lib.h"     //  API for all DECODE_GRF__*       PRIVATE
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
#define GRF_START               "[[[[["
#define GRF_START_L             strlen( GRF_START )
//----------------------------------------------------------------------------
#define GRF_END                 "]]]]]"
#define GRF_END_L               strlen( GRF_END )
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
 *  Test the string for a recipe START marker.
 *      FORMATS:
 *          1   |[[[[[|

 *
 *  @param  data_p              Pointer to a a line of text to be scanned.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_GRF__start (
    char                        *   data_p
    )
{
    /**
     * @param grf_rc            Return Code                                 */
    int                             grf_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe start tag
    grf_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );


    /************************************************************************
     *  Anything that starts with "[[[[["
     ************************************************************************/

    //  Skip this test if a previous test was TRUE
    if( grf_rc == false )
    {
        //  Does the string start with "[[[[[" ?
        if( strncmp( tmp_data_p, GRF_START, GRF_START_L ) == 0 )
        {
            //  YES:    Change the return code
            grf_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( grf_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |]]]]]|
 *
 *  @param  data_p              Pointer to a a line of text to be scanned.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_GRF__end(
    char                        *   data_p
    )
{
    /**
     * @param grf_rc            Return Code                                 */
    int                             grf_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    grf_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Meal-Master GRF recipe ?
    if( strncmp( tmp_data_p, GRF_END, GRF_END_L  ) == 0 )
    {
        //  YES:    Change the return code
        grf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( grf_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe title.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  title_p             Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a the recipe title is located and
 *                              processed else false.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_GRF__title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param grf_rc            Return Code                                 */
    int                             grf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a title
    grf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if( text_is_blank_line( title_p ) != true )
    {
        title_p = text_skip_past_whitespace( title_p );

        //  Cleanup the recipe name before saving it.
        recipe_name_cleanup( title_p, strlen( title_p ) );

        //  Save the recipe title (name)
        recipe_p->name = text_copy_to_new( title_p );

        // Change the pass_fail flag to PASS
        grf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( grf_rc );
}

/****************************************************************************/
/**
 *  Everything from here to the next blank line is AUIP.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  in_buffer_p         Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a new recipe is detected
 *                              else FRC_FAIL.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_GRF__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param grf_rc            Return Code                                 */
    int                             grf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT an AUIP
    grf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if( text_is_blank_line( in_buffer_p ) != true )
    {
        //  Process the first half (or the entire line)
        in_buffer_p = text_skip_past_whitespace( in_buffer_p );

        // Format the AUIP line
        recipe_fmt_auip( recipe_p, in_buffer_p, RECIPE_FORMAT_GRF );

        //  Set the return code
        grf_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( grf_rc );
}

/****************************************************************************/
/**
 *  Everything from here to END-OF-RECIPE is 'DIRECTIONS'.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  in_buffer_p         Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a new recipe is detected
 *                              else FRC_FAIL.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_GRF__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param grf_rc            Return Code                                 */
    int                             grf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DIRECTION
    grf_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Is this the end-of-recipe tag ?
    if( DECODE_GRF__end( in_buffer_p ) == false )
    {
        //  Format the AUIP line
        recipe_add_instructions( recipe_p, in_buffer_p );

        //  This is a directions line of text
        grf_rc = true;
    }
    else
    {
        //  YES:    NOT a directions line
        grf_rc = false;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( grf_rc );
}

/****************************************************************************/
