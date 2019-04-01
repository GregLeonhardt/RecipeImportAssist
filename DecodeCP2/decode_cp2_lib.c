/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_cp2' library.
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
#include <decode_cp2_api.h>     //  API for all decode_cp2_*        PUBLIC
#include "decode_cp2_lib.h"     //  API for all DECODE_CP2__*       PRIVATE
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
 *  Test the string for a recipe START marker.
 *      FORMATS:
 *          1   |CookenPro 2.0|

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
DECODE_CP2__start (
    char                        *   data_p
    )
{
    /**
     * @param cp2_rc            Return Code                                 */
    int                             cp2_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe start tag
    cp2_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );


    /************************************************************************
     *  Anything that starts with "-= Exported from BigOven =-"
     ************************************************************************/

    //  Skip this test if a previous test was TRUE
    if ( cp2_rc == false )
    {
        //  Does the string start with "-= Exported from BigOven =-" ?
        if ( strncmp( tmp_data_p, CP2_START, CP2_START_L ) == 0 )
        {
            //  YES:    Change the return code
            cp2_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( cp2_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |CookenPro End-of-Recipe|
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
DECODE_CP2__end(
    char                        *   data_p
    )
{
    /**
     * @param cp2_rc            Return Code                                 */
    int                             cp2_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a recipe end tag
    cp2_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    //  Is this the start of a Big-Oven CP2 recipe ?
    if (    ( strncmp( tmp_data_p, CP2_END_1, CP2_END_1_L  ) == 0 )
         || ( strncmp( tmp_data_p, CP2_END_2, CP2_END_2_L  ) == 0 ) )
    {
        //  YES:    Change the return code
        cp2_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( cp2_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe title.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  title_p             Pointer to a a line of text to be scanned.
 *
 *  @return                     No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_CP2__title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param tmp_title_p       Pointer to the first character              */
    char                        *   tmp_title_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Set the pointer to the first character of the recipe title.
    tmp_title_p = title_p + CP2_TITLE_L;
    tmp_title_p = text_skip_past_whitespace( tmp_title_p );
    
    //  Cleanup the recipe name before saving it.
    recipe_name_cleanup( tmp_title_p, strlen( tmp_title_p ) );

    //  Save the recipe title (name)
    recipe_p->name = text_copy_to_new( tmp_title_p );
    log_write( MID_DEBUG_1, "decode_cp2_lib.c", "Line: %d\n", __LINE__ );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
}

/****************************************************************************/
/**
 *  Copy 'serving size' information to the recipe structure.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return                     No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_CP2__srv_size(
    struct   recipe_t           *   recipe_p,
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Serving Size  :
     ************************************************************************/

    //  Jump past the search string
    tmp_data_p = data_p + CP2_SERVINGS_L;

    //  Skip past any leading whitespace.
    tmp_data_p = text_skip_past_whitespace( tmp_data_p );

    //  Save the recipe title (name)
    recipe_p->serves = text_copy_to_new( tmp_data_p );
    log_write( MID_DEBUG_1, "decode_cp2_lib.c", "Line: %d\n", __LINE__ );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Everything from here to the next blank line is AUIP.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  in_buffer_p         Pointer to a a line of text to be scanned.
 *
 *  @return                     true when the input buffer is NOT a blank
 *                              line, else false.
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_CP2__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param cp2_rc            Return Code                                 */
    int                             cp2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT an AUIP
    cp2_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( in_buffer_p ) != true )
    {
        //  Process the first half (or the entire line)
        in_buffer_p = text_skip_past_whitespace( in_buffer_p );

        // Format the AUIP line
        recipe_fmt_auip( recipe_p, in_buffer_p, RECIPE_FORMAT_CP2 );

        //  Set the return code
        cp2_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( cp2_rc );
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
DECODE_CP2__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param cp2_rc            Return Code                                 */
    int                             cp2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a DIRECTION
    cp2_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Is this the end-of-recipe tag ?
    if ( strncmp( in_buffer_p, CP2_DIR_END, CP2_DIR_END_L  ) != 0 )
    {
        //  Format the Directions line
        recipe_add_instructions( recipe_p, in_buffer_p );

        //  This is a directions line of text
        cp2_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( cp2_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe categories.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  categories_p        Pointer to a a line of text to be scanned.
 *
 *  @return                     No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_CP2__categories(
    struct  recipe_t            *   recipe_p,
    char                        *   categories_p
    )
{
    /**
     *  @param  raw_chapter     A temporary holding buffer for a chapter    */
    char                            raw_category[ MAX_LINE_L ];
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;
    /**
     * @param found_category_p  Pointer to a temp data buffer               */
    char                        *   found_category_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Jump past the search string
    tmp_data_p = categories_p + CP2_SERVINGS_L;

    //  Skip past any leading whitespace.
    tmp_data_p = text_skip_past_whitespace( tmp_data_p );

    //  Should be pointing to the start of a chapter or
    //  the end of the line.
    while ( tmp_data_p[ 0 ] != '\0' )
    {
        //  Wipe the temporary chapter buffer clean
        memset( &(raw_category[ 0 ]),
                '\0',
                sizeof ( raw_category ) );

        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Copy the new chapter to the temp buffer
        for (  ;
              (    ( tmp_data_p[ 0 ] != ',' )
                && ( tmp_data_p[ 0 ] != '\0' ) );
              tmp_data_p ++ )
        {
            strncat( raw_category, tmp_data_p, 1 );
        }
        //  Skip past the ','
        tmp_data_p += 1;

        //  Translate the chapter
        found_category_p = xlate_chapter( raw_category );

        //  Do we have a chapter to save ?
        if ( found_category_p != NULL )
        {
            //  YES:    Save it
            decode_save_chapter( found_category_p, recipe_p );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Process Vegetarian information.
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  categories_p        Pointer to a a line of text to be scanned.
 *
 *  @return                     No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_CP2__vegetarian(
    struct  recipe_t            *   recipe_p,
    char                        *   categories_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Jump past the search string
    tmp_data_p = categories_p + CP2_VEGETARIAN_L;

    //  Skip past any leading whitespace.
    tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    
    //  Yes or No
    if ( strncasecmp( tmp_data_p, "YES", 3 ) != 0 )
    {
        //  YES:    Add vegetarian to the categories list
        decode_save_chapter( "Vegetarian", recipe_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
