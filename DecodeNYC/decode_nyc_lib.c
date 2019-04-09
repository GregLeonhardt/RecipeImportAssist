/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_nyc' library.
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
#include <decode_nyc_api.h>     //  API for all decode_nyc_*        PUBLIC
#include "decode_nyc_lib.h"     //  API for all DECODE_NYC__*       PRIVATE
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
#define NYC_START_1             "@@@@@ Now You're Cooking! Export Format"
#define NYC_START_1_L           strlen( NYC_START_1 )
//----------------------------------------------------------------------------
#define NYC_END_1               "** Exported from Now You're Cooking!"
#define NYC_END_1_L             strlen( NYC_END_1 )
//----------------------------------------------------------------------------
#define NYC_END_2               "-+-"
#define NYC_END_2_L             strlen( NYC_END_2 )
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
 *          1   |@@@@@ Now You're Cooking! Export Format|

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
DECODE_NYC__start (
    char                        *   data_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                        *   start_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    nyc_rc = false;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  |Recipe via Meal-Master|
     ************************************************************************/

    //  Is the a CP2 start tag ?
    if (    ( start_p != NULL )                           //  Data is present
         && ( start_p[ 0 ] != '>' )                       //  Not forwarded e-mail
         && ( strncmp( start_p, NYC_START_1,  NYC_START_1_L  ) == 0 ) )
    {
        //  YES:    Change the return code
        nyc_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( nyc_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |** Exported from Now You're Cooking! v5.84 **|
 *      NOTE:
 *          When new end of recipe markers are added here, they should also
 *          be added to rd_input_file:rd_preprocess( )
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
DECODE_NYC__end(
    char                        *   data_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    nyc_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Meal-Master NYC recipe ?
    if (    ( strncmp( tmp_data_p, NYC_END_1,  NYC_END_1_L  ) == 0 )
         || ( strncmp( tmp_data_p, NYC_END_2,  NYC_END_2_L  ) == 0 ) )
    {
        //  YES:    Change the return code
        nyc_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( nyc_rc );
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
DECODE_NYC__title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this isn't the title
    nyc_rc = false;

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

        log_write( MID_DEBUG_1, "decode_nyc_lib.c", "Line: %d\n", __LINE__ );

        // Change the pass_fail flag to PASS
        nyc_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( nyc_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe categories.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  categories_p        Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a the recipe title is located and
 *                              processed else false.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_NYC__categories(
    struct  recipe_t            *   recipe_p,
    char                        *   categories_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;

    // A temporary holding buffer for a chapter
    char                            raw_chapter[ MAX_LINE_L ];

    // A pointer to the translated chapter
    char                         *  xlated_chapter_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT a categories line
    nyc_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( categories_p ) != true )
    {
        categories_p = text_skip_past_whitespace( categories_p );

        while ( categories_p[ 0 ] != '\0' )
        {
            //  Wipe the temporary chapter buffer clean
            memset( &raw_chapter,
                    '\0',
                    sizeof ( raw_chapter ) );

            categories_p = text_skip_past_whitespace( categories_p );

            //  Copy the new chapter to the temp buffer
            for (  ;
                  (    ( categories_p[ 0 ] != ',' )
                    && ( categories_p[ 0 ] != '\0' ) );
                  categories_p ++ )
            {
                strncat( raw_chapter, categories_p, 1 );
            }
            //  Skip past the ','
            categories_p += 1;

            //  Translate the chapter
            xlated_chapter_p = xlate_chapter( raw_chapter );

            //  Do we have a chapter to save ?
            if ( xlated_chapter_p != NULL )
            {
                //  YES:    Save it
                decode_save_chapter( xlated_chapter_p, recipe_p );
            }
        }

        //  Change the pass_fail flag to PASS
        nyc_rc = false;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( nyc_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe yield.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  yield_p             Pointer to a a line of text to be scanned.
 *
 *  @return                     true when a the recipe title is located and
 *                              processed else false.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_NYC__yield(
    struct  recipe_t            *   recipe_p,
    char                        *   yield_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;
    // Local buffer for AMOUNT
    char                            local_amount[ MAX_LINE_L ];

    // Local buffer for UNIT
    char                            local_unit[ MAX_LINE_L ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    // Wipe clean the local buffers
    memset( local_amount,
            '\0',
            sizeof ( local_amount ) );
    memset( local_unit,
            '\0',
            sizeof ( local_unit ) );

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( yield_p ) != true )
    {
        yield_p = text_skip_past_whitespace( yield_p );

        //  Test this line for Meal-Master Servings
        if (    ( strncmp( yield_p, "Servings:", 9 ) == 0 )
             || ( strncmp( yield_p, "Yield:",    6 ) == 0 ) )
        {
            //  Skip past the marker
            for (;
                  ( ( isspace( yield_p[ 0 ] ) == 0 )
                    && ( yield_p[ 0 ] != ':' ) );
                  yield_p++ )
            {
            }
            yield_p = text_skip_past_whitespace( ++yield_p );

            //  The first part should be an amount
            for (;
                  ( ( isspace( yield_p[ 0 ] ) == 0 )
                    && ( yield_p[ 0 ] != '\0' ) );
                  yield_p++ )
            {
                strncat( local_amount, &yield_p[ 0 ], 1 );
            }
            //  Skip over spaces and or tabs between the
            //  amount and unit fields
            yield_p = text_skip_past_whitespace( yield_p );

            //  Copy the units field
            for (;
                  yield_p[ 0 ] != '\0';
                  yield_p++ )
            {
                strncat( local_unit, &yield_p[ 0 ], 1 );
            }
            //  Change the pass_fail flag to PASS
            nyc_rc = true;
        }
    }
    //  Before we go any farther.  Make sure there was something
    //  in the input data.
    if ( strlen( local_amount ) > 0 )
    {
        //  Time to figure out if this is a serves or MAKES field.
        if ( strlen( local_unit ) > 0 )
        {
            char                    tmp_unit[ MAX_LINE_L ];
            char                *   tmp_unit_p;

            //  To compare this field, I have to make it UPPER case
            //  Do this is a temp buffer
            memset( tmp_unit, '\0', sizeof ( tmp_unit ) );
            for ( tmp_unit_p = local_unit;
                  tmp_unit_p[ 0 ] != '\0';
                  tmp_unit_p++ )
            {
                strncat( tmp_unit, tmp_unit_p, 1 );
                tmp_unit[ strlen( tmp_unit ) - 1 ] =
                    toupper( tmp_unit[ strlen( tmp_unit ) - 1 ] );
            }
            //  Is the units field empty
            if ( strlen( tmp_unit ) == 0 )
            {
                //  YES:    This is a serves amount, not a MAKES amount
                recipe_p->serves = text_copy_to_new( local_amount );

                log_write( MID_DEBUG_1, "decode_nyc_lib.c", "Line: %d\n", __LINE__ );
            }
            else
            {
                //  NO:     This is a serves amount, not a MAKES amount
                recipe_p->makes      = text_copy_to_new( local_amount );

                log_write( MID_DEBUG_1, "decode_nyc_lib.c", "Line: %d\n", __LINE__ );

                recipe_p->makes_unit = text_copy_to_new( tmp_unit );

                log_write( MID_DEBUG_1, "decode_nyc_lib.c", "Line: %d\n", __LINE__ );
            }
        }
        else
        {
            //  This is a serves amount, not a MAKES amount
            recipe_p->serves = text_copy_to_new( local_amount );

            log_write( MID_DEBUG_1, "decode_nyc_lib.c", "Line: %d\n", __LINE__ );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( nyc_rc );
}

/****************************************************************************/
/**
 *  Tests for the end of a Meal-Master Formatted recipe.
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
DECODE_NYC__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    nyc_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( in_buffer_p ) != true )
    {
        //  Process the first half (or the entire line)
        in_buffer_p = text_skip_past_whitespace( in_buffer_p );

        // Format the AUIP line
        recipe_fmt_auip( recipe_p, in_buffer_p, RECIPE_FORMAT_NYC );
    }
    else
    {
        //  The blank line marks the end of the AUIP section
        nyc_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( nyc_rc );
}

/****************************************************************************/
/**
 *  Tests for the end of a Meal-Master Formatted recipe.
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
DECODE_NYC__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    nyc_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Is this the end of the recipe ?
    if ( DECODE_NYC__end( in_buffer_p ) == true )
    {
        //  YES:    Change the return code
        nyc_rc = true;
    }
    else
    {
        //  Format the AUIP line
        recipe_add_instructions( recipe_p, in_buffer_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( nyc_rc );
}

/****************************************************************************/
