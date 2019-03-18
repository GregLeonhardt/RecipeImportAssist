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
#define GRF_END_1               "]]]]]"
#define GRF_END_1_L             strlen( GRF_END_1 )
//----------------------------------------------------------------------------
#define GRF_END_2               "-----"
#define GRF_END_2_L             strlen( GRF_END_2 )
//----------------------------------------------------------------------------
#define GRF_C                   "C:"
#define GRF_C_L                 strlen( GRF_C )
//----------------------------------------------------------------------------
#define GRF_S                   "S:"
#define GRF_S_L                 strlen( GRF_S )
//----------------------------------------------------------------------------
#define GRF_B                   "B:"
#define GRF_B_L                 strlen( GRF_B )
//----------------------------------------------------------------------------
#define GRF_N                   "N:"
#define GRF_N_L                 strlen( GRF_N )
//----------------------------------------------------------------------------
#define GRF_F                   "F:"
#define GRF_F_L                 strlen( GRF_F )
//----------------------------------------------------------------------------
#define GRF_D                   "D:"
#define GRF_D_L                 strlen( GRF_D )
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
    if ( grf_rc == false )
    {
        //  Does the string start with "[[[[[" ?
        if ( strncmp( tmp_data_p, GRF_START, GRF_START_L ) == 0 )
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
    if (    ( strncmp( tmp_data_p, GRF_END_1, GRF_END_1_L  ) == 0 )
         || ( strncmp( tmp_data_p, GRF_END_2, GRF_END_2_L  ) == 0 ) )
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
 *  Search for and process the 'C:' (category), 'S:' (servings),
 *  'B:' (recipe by), 'N:' (note), 'F:' (from), 'D:' (description)
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
DECODE_GRF__csbnfd(
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
    /**
     * @param chapter_p         Pointer to the translated chapter name      */
    char    *                       chapter_p;
    /**
     *  @param  raw_chapter     A temporary holding buffer for a chapter    */
    char                            raw_chapter[ MAX_LINE_L ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is a one of the following.
    grf_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  ---------------------------------------------------------------------
    //  'C:'    category
    if ( strncmp( data_p, GRF_C, GRF_C_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GRF_C_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {

            //  Should be pointing to the start of a chapter or
            //  the end of the line.
            while ( tmp_p[ 0 ] != '\0' )
            {
                //  Wipe the temporary chapter buffer clean
                memset( &raw_chapter,
                        '\0',
                        sizeof ( raw_chapter ) );

                tmp_p = text_skip_past_whitespace( tmp_p );

                //  Copy the new chapter to the temp buffer
                for (  ;
                      (    ( tmp_p[ 0 ] != ',' )
                        && ( tmp_p[ 0 ] != '\0' ) );
                      tmp_p ++ )
                {
                    strncat( raw_chapter, tmp_p, 1 );
                }
                //  Skip past the ','
                tmp_p += 1;

                //  Translate the chapter
                chapter_p = xlate_chapter( raw_chapter );

                //  Do we have a chapter to save ?
                if ( chapter_p != NULL )
                {
                    //  YES:    Save it
                    decode_save_chapter( chapter_p, recipe_p );
                }
            }
        }
    }
    //  ---------------------------------------------------------------------
    //  'S:'    serving / makes
    else
    if ( strncmp( data_p, GRF_S, GRF_S_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GRF_S_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  QTY="..."
            recipe_p->serves = text_copy_to_new( tmp_p );
        }
    }
    //  ---------------------------------------------------------------------
    //  'B:'    recipe by
    else
    if ( strncmp( data_p, GRF_B, GRF_B_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GRF_B_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  Posted By:
            recipe_p->posted_by = text_copy_to_new( tmp_p );
        }
    }
    //  ---------------------------------------------------------------------
    //  'N:'    note
    else
    if ( strncmp( data_p, GRF_N, GRF_N_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GRF_N_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  Add it to the instructions and parse it out later
            recipe_fmt_notes( recipe_p, tmp_p );
            recipe_fmt_notes( recipe_p, " " );
        }
    }
    //  ---------------------------------------------------------------------
    //  'F:'    recipe from
    else
    if ( strncmp( data_p, GRF_F, GRF_F_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GRF_F_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {
            //  Author:
            recipe_p->author = text_copy_to_new( tmp_p );
        }
    }
    //  ---------------------------------------------------------------------
    //  'D:'    description
    else
    if ( strncmp( data_p, GRF_D, GRF_D_L  ) == 0 )
    {
        //  Skip past the tag
        tmp_p = data_p + GRF_D_L;
        tmp_p = text_skip_past_whitespace( tmp_p );

        //  Save the information
        if ( text_is_blank_line( tmp_p ) != true )
        {

            //  Add it to the Directions
            recipe_p->description = text_copy_to_new( tmp_p );
        }
    }
    //  ---------------------------------------------------------------------
    else
    {
        //  Didn't find one of the above.
        grf_rc = false;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
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
    if ( text_is_blank_line( title_p ) != true )
    {
        title_p = text_skip_past_whitespace( title_p );

        //  Cleanup the recipe name before saving it.
        recipe_name_cleanup( title_p, strlen( title_p ) );

        //  Save the recipe title (name)
        recipe_p->name = text_copy_to_new( title_p );
log_write( MID_DEBUG_0, "DECODE_GRF__title", "251\n" );

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
    if ( text_is_blank_line( in_buffer_p ) != true )
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
    if ( DECODE_GRF__end( in_buffer_p ) == false )
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
