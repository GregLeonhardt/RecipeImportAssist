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
#define GF2_END_1               "_____"
#define GF2_END_1_L             strlen( GF2_END_1 )
//----------------------------------------------------------------------------
#define GF2_END_2               "-----"
#define GF2_END_2_L             strlen( GF2_END_2 )
//----------------------------------------------------------------------------
#define GF2_A                   "Author:"
#define GF2_A_L                 strlen( GF2_A )
//----------------------------------------------------------------------------
#define GF2_S                   "Serves:"
#define GF2_S_L                 strlen( GF2_S )
//----------------------------------------------------------------------------
#define GF2_PT                  "Prep Time:"
#define GF2_PT_L                strlen( GF2_PT )
//----------------------------------------------------------------------------
#define GF2_CT                  "Cook Time:"
#define GF2_CT_L                strlen( GF2_CT )
//----------------------------------------------------------------------------
#define GF2_Y                   "Yield:"
#define GF2_Y_L                 strlen( GF2_Y )
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
    if (    ( strncmp( tmp_data_p, GF2_END_1, GF2_END_1_L  ) == 0 )
         || ( strncmp( tmp_data_p, GF2_END_2, GF2_END_2_L  ) == 0 ) )
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
log_write( MID_DEBUG_0, "DECODE_GF2__title", "253\n" );

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
 *  Search for and process the 'Author:', 'Serves:', 'Prep Time:',
 *                             'Cook Time:', 'Yield:'
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  data_p              Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a the recipe title is located and
 *                              processed else false.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_GF2__aspcy(
    struct  recipe_t            *   recipe_p,
    char                        *   data_p
    )
{
    /**
     * @param grf_rc            Return Code                                 */
    int                             grf_rc;
    /**
     * @param tmp_p             Local pointer to data                       */
    char    *                       tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is a one of the following.
    grf_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  ---------------------------------------------------------------------
    //  'Author:'
    if ( strncmp( data_p, GF2_A, GF2_A_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GF2_A_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  Is there something already there ?
            if ( recipe_p->author != NULL )
            {
                //  YES:    Release it
                mem_free( recipe_p->author );
            }
            //  Now add the new information
            recipe_p->author = text_copy_to_new( tmp_p );
        }
    }
    //  ---------------------------------------------------------------------
    //  'Serves:'
    else
    if ( strncmp( data_p, GF2_S, GF2_S_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GF2_S_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  Is there something already there ?
            if ( recipe_p->serves != NULL )
            {
                //  YES:    Release it
                mem_free( recipe_p->serves );
            }
            //  Now add the new information
            recipe_p->serves = text_copy_to_new( tmp_p );
        }
    }
    //  ---------------------------------------------------------------------
    //  'Prep Time:'
    else
    if ( strncmp( data_p, GF2_PT, GF2_PT_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GF2_PT_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  Is there something already there ?
            if ( recipe_p->time_prep != NULL )
            {
                //  YES:    Release it
                mem_free( recipe_p->time_prep );
            }
            //  Now add the new information
            recipe_p->time_prep = text_copy_to_new( tmp_p );
        }
    }
    //  ---------------------------------------------------------------------
    //  'Cook Time:'
    else
    if ( strncmp( data_p, GF2_CT, GF2_CT_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GF2_CT_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  Is there something already there ?
            if ( recipe_p->time_cook != NULL )
            {
                //  YES:    Release it
                mem_free( recipe_p->time_cook );
            }
            //  Now add the new information
            recipe_p->time_cook = text_copy_to_new( tmp_p );
        }
    }
    //  ---------------------------------------------------------------------
    //  'Yield:'
    else
    if ( strncmp( data_p, GF2_Y, GF2_Y_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GF2_Y_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  Is there something already there ?
            if ( recipe_p->makes != NULL )
            {
                //  YES:    Release it
                mem_free( recipe_p->makes );
            }
            //  @ToDo   Before saving the yield information it is supposed to
            //          be split into unit and amount fields.  Code is needed..

            //  Now add the new information
            recipe_p->makes = text_copy_to_new( tmp_p );

            //  Now add the new information
            recipe_p->makes_unit = text_copy_to_new( " " );
        }
    }
    //  ---------------------------------------------------------------------
    else
    {
        //  If it isn't any of the above it must be a note.
        DECODE_GF2__note( recipe_p, data_p );
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
