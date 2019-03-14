/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_gf2' library.
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
#include <decode_gf2_api.h>     //  API for all decode_gf2_*        PUBLIC
#include "decode_gf2_lib.h"     //  API for all DECODE_GF2__*       PRIVATE
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
#define GF2_START               "@@@@@"
#define GF2_START_L             strlen( GF2_START )
//----------------------------------------------------------------------------
#define GF2_END                 "_____"
#define GF2_END_L               strlen( GF2_END )
//----------------------------------------------------------------------------
#define GF2_NEW_FIELD           '|'
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
 *          1   |@@@@@|
 *          2   |_____|
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
DECODE_GF2__start (
    char                        *   data_p
    )
{
    /**
     * @param gf2_rc            Return Code                                 */
    int                             gf2_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe start tag
    gf2_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );


    /************************************************************************
     *  Anything that starts with "@@@@@"
     ************************************************************************/

    //  Skip this test if a previous test was TRUE
    if ( gf2_rc == false )
    {
        //  Does the string start with "@@@@@" ?
        if ( strncmp( tmp_data_p, GF2_START, GF2_START_L ) == 0 )
        {
            //  YES:    Change the return code
            gf2_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( gf2_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |_____|
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
DECODE_GF2__end(
    char                        *   data_p
    )
{
    /**
     * @param gf2_rc            Return Code                                 */
    int                             gf2_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    gf2_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Meal-Master GF2 recipe ?
    if ( strncmp( tmp_data_p, GF2_END,   GF2_END_L    ) == 0 )
    {
        //  YES:    Change the return code
        gf2_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( gf2_rc );
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
DECODE_GF2__title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param gf2_rc            Return Code                                 */
    int                             gf2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a title
    gf2_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( title_p ) != true )
    {
        title_p = text_skip_past_whitespace( title_p );

        //  Cleanup the recipe name before saving it.
        recipe_name_cleanup( title_p, strlen( title_p ) );

        //  Save the recipe title (name)
        recipe_p->name = text_copy_to_new( title_p );

        // Change the pass_fail flag to PASS
        gf2_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( gf2_rc );
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
DECODE_GF2__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param gf2_rc            Return Code                                 */
    int                             gf2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT an AUIP
    gf2_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( in_buffer_p ) != true )
    {
        //  Process the first half (or the entire line)
        in_buffer_p = text_skip_past_whitespace( in_buffer_p );

        // Format the AUIP line
        recipe_fmt_auip( recipe_p, in_buffer_p, RECIPE_FORMAT_GF2 );

        //  Set the return code
        gf2_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( gf2_rc );
}

/****************************************************************************/
/**
 *  Add another note.
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
DECODE_GF2__note(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param gf2_rc            Return Code                                 */
    int                             gf2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DIRECTION
    gf2_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Format another note
    recipe_fmt_notes( recipe_p, in_buffer_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( gf2_rc );
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
DECODE_GF2__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param gf2_rc            Return Code                                 */
    int                             gf2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DIRECTION
    gf2_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Format another instruction line
    recipe_add_instructions( recipe_p, in_buffer_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( gf2_rc );
}

/****************************************************************************/
