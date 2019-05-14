/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_erd' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define _GNU_SOURCE

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
#include <recipe_api.h>         //  API for all recipe_*            PUBLIC
#include <xlate_api.h>          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include <decode_erd_api.h>     //  API for all decode_erd_*        PUBLIC
#include "decode_erd_lib.h"     //  API for all DECODE_ERD__*       PRIVATE
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
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return erd_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_ERD__start(
    char                        *   data_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                        *   start_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain a 'start' marker.
    erd_rc = false;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Start of recipe detection
     ************************************************************************/

    if (    ( start_p != NULL )                           //  Data is present
         && ( start_p[ 0 ] != '>' ) )                     //  Not forwarded e-mail
    {
        //  Is this the start of a Easy Recipe Deluxe recipe ?
        if ( strncmp( start_p, ERD_PREFIX,  ERD_PREFIX_L  ) == 0 )
        {
                if ( strstr( start_p, ERD_START ) != NULL )
                {
                    //  YES:    Change the return code
                    erd_rc = true;
                }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( erd_rc );
}

/****************************************************************************/
/**
 *  Test the string for the recipe end marker.
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
DECODE_ERD__end(
    char                        *   data_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param start_p           Pointer to a temp data buffer               */
    char                        *   start_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    erd_rc = false;

    //  Locate the first character in the buffer
    start_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Test for a valid recipe end string
     ************************************************************************/

    if ( start_p != NULL )
    {
        //  Is this the start of a Easy Recipe Deluxe recipe ?
        if ( strncmp( start_p, ERD_SUFFIX,  ERD_SUFFIX_L  ) == 0 )
        {
                if ( strstr( start_p, ERD_END ) != NULL )
                {
                    //  YES:    Change the return code
                    erd_rc = true;
                }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the recipe 'Title:' tag.
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
DECODE_ERD__title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this isn't the title
    erd_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( title_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( title_p );

        //  Look for a TITLE tag in the line
        if ( strncmp( tmp_p, ERD_TITLE, ERD_TITLE_L ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            tmp_p += ERD_TITLE_L;
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Is there something to save here ?
            if( strlen( tmp_p ) > 0 )
            {
                //  Cleanup the recipe name before saving it.
                recipe_name_cleanup( tmp_p, strlen( tmp_p ) );

                //  Save the recipe title (name)
                recipe_p->name = text_copy_to_new( tmp_p );
                log_write( MID_DEBUG_1, "decode_erd_lib.c", "Line: %d\n", __LINE__ );
            }

            // Change the pass_fail flag to PASS
            erd_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the 'Recipe By:' tag.
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
DECODE_ERD__recipe_by(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this isn't the title
    erd_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( title_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( title_p );

        //  Look for a "Recipe By:" tag
        if ( strncmp( tmp_p, ERD_RECIPE_BY, ERD_RECIPE_BY_L ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            tmp_p += ERD_RECIPE_BY_L;
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Is there something to save here ?
            if( strlen( tmp_p ) > 0 )
            {
                //  Save the recipe title (name)
                recipe_p->author = text_copy_to_new( tmp_p );
                log_write( MID_DEBUG_1, "decode_erd_lib.c", "Line: %d\n", __LINE__ );
            }

            // Change the pass_fail flag to PASS
            erd_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the 'Category:' tag.
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
DECODE_ERD__category(
    struct  recipe_t            *   recipe_p,
    char                        *   categories_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;
    /**
     * @param raw_chapter       A temporary holding buffer for a chapter    */
    char                            raw_chapter[ MAX_LINE_L ];
    /**
     * @param xlated_chapter_p  A pointer to the translated chapter         */
    char                         *  xlated_chapter_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT a categories line
    erd_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( categories_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( categories_p );

        //  Look for a "Recipe By:" tag
        if ( strncmp( tmp_p, ERD_CATEGORY, ERD_CATEGORY_L ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            tmp_p += ERD_CATEGORY_L;
            tmp_p = text_skip_past_whitespace( tmp_p );

            /****************************************************************
             *  Parse and save the recipe categories.
             ****************************************************************/

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
                xlated_chapter_p = xlate_chapter( raw_chapter );

                //  Do we have a chapter to save ?
                if ( xlated_chapter_p != NULL )
                {
                    //  YES:    Save it
                    decode_save_chapter( xlated_chapter_p, recipe_p );
                }
            }
        }

        //  Change the pass_fail flag to PASS
        erd_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the 'Main Ingredient:' tag.
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
DECODE_ERD__main_ingred(
    struct  recipe_t            *   recipe_p,
    char                        *   main_ingred_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;
    /**
     * @param main_ingred       A temporary holding buffer for a chapter    */
    char                            main_ingred[ MAX_LINE_L ];
    /**
     * @param xlated_ingred_p    A pointer to the translated chapter         */
    char                         *  xlated_ingred_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT a categories line
    erd_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( main_ingred_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( main_ingred_p );

        //  Look for a "Recipe By:" tag
        if ( strncmp( tmp_p, ERD_MAIN_INGRED, ERD_MAIN_INGRED_L ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            tmp_p += ERD_MAIN_INGRED_L;
            tmp_p = text_skip_past_whitespace( tmp_p );

            /****************************************************************
             *  Parse and save the recipe categories.
             ****************************************************************/

            while ( tmp_p[ 0 ] != '\0' )
            {
                //  Wipe the temporary chapter buffer clean
                memset( &main_ingred,
                        '\0',
                        sizeof ( main_ingred ) );

                tmp_p = text_skip_past_whitespace( tmp_p );

                //  Copy the new chapter to the temp buffer
                for (  ;
                      (    ( tmp_p[ 0 ] != ',' )
                        && ( tmp_p[ 0 ] != '\0' ) );
                      tmp_p ++ )
                {
                    strncat( main_ingred, tmp_p, 1 );
                }
                //  Skip past the ','
                tmp_p += 1;

                //  Translate the chapter
                xlated_ingred_p = xlate_chapter( main_ingred );

                //  Do we have a chapter to save ?
                if ( xlated_ingred_p != NULL )
                {
                    //  YES:    Save it
                    decode_save_chapter( xlated_ingred_p, recipe_p );
                }
            }
        }

        //  Change the pass_fail flag to PASS
        erd_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the 'Cuisine Style:' tag.
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
DECODE_ERD__quisine(
    struct  recipe_t            *   recipe_p,
    char                        *   main_ingred_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;
    /**
     * @param main_ingred       A temporary holding buffer for a chapter    */
    char                            main_ingred[ MAX_LINE_L ];
    /**
     * @param quisine_ingred_p  A pointer to the translated chapter         */
    char                        *  quisine_ingred_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT a categories line
    erd_rc = true;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( main_ingred_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( main_ingred_p );

        //  Look for a "Recipe By:" tag
        if ( strncmp( tmp_p, ERD_QUISINE, ERD_QUISINE_L ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            tmp_p += ERD_QUISINE_L;
            tmp_p = text_skip_past_whitespace( tmp_p );

            /****************************************************************
             *  Parse and save the recipe categories.
             ****************************************************************/

            while ( tmp_p[ 0 ] != '\0' )
            {
                //  Wipe the temporary chapter buffer clean
                memset( &main_ingred,
                        '\0',
                        sizeof ( main_ingred ) );

                tmp_p = text_skip_past_whitespace( tmp_p );

                //  Copy the new chapter to the temp buffer
                for (  ;
                      (    ( tmp_p[ 0 ] != ',' )
                        && ( tmp_p[ 0 ] != '\0' ) );
                      tmp_p ++ )
                {
                    strncat( main_ingred, tmp_p, 1 );
                }
                //  Skip past the ','
                tmp_p += 1;

                //  Translate the chapter
                quisine_ingred_p = xlate_chapter( main_ingred );

                //  Do we have a chapter to save ?
                if ( quisine_ingred_p != NULL )
                {
                    //  YES:    Save it
                    decode_save_chapter( quisine_ingred_p, recipe_p );
                }
            }
        }

        //  Change the pass_fail flag to PASS
        erd_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the 'Yield:' tag.
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
DECODE_ERD__yield(
    struct  recipe_t            *   recipe_p,
    char                        *   yield_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;
    /**
     * @param local_amount      Local buffer for AMOUNT                     */
    char                            local_amount[ MAX_LINE_L ];
    /**
     * @param local_unit        Local buffer for UNIT                       */
    char                            local_unit[ MAX_LINE_L ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT a categories line
    erd_rc = true;

    // Wipe clean the local buffers
    memset( local_amount, '\0', sizeof ( local_amount ) );
    memset( local_unit,   '\0', sizeof ( local_unit   ) );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( yield_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( yield_p );

        //  Look for a "Yield:" tag
        if ( strncmp( tmp_p, ERD_YIELD, ERD_YIELD_L ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            tmp_p += ERD_YIELD_L;
            tmp_p = text_skip_past_whitespace( tmp_p );


            //  The first part should be an amount
            for (;
                  ( ( isspace( tmp_p[ 0 ] ) == 0 )
                    && ( tmp_p[ 0 ] != '\0' ) );
                  tmp_p++ )
            {
                strncat( local_amount, &tmp_p[ 0 ], 1 );
            }

            //  Skip over spaces and or tabs between the
            //  amount and unit fields
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Copy the units field
            for (;
                  tmp_p[ 0 ] != '\0';
                  tmp_p++ )
            {
                strncat( local_unit, &tmp_p[ 0 ], 1 );
            }

            //  Change the pass_fail flag to PASS
            erd_rc = true;
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

            //  To compare this field, I have to make it UPPER case
            //  Do this is a temp buffer
            memset( tmp_unit, '\0', sizeof ( tmp_unit ) );
            //  Is the units field empty
            if ( strlen( tmp_unit ) == 0 )
            {
                //  YES:    This is a serves amount, not a MAKES amount
                recipe_p->serves = text_copy_to_new( local_amount );
                log_write( MID_DEBUG_1, "decode_erd_lib.c", "Line: %d\n", __LINE__ );
            }
            else
            {
                //  NO:     This is a serves amount, not a MAKES amount
                recipe_p->makes      = text_copy_to_new( local_amount );
                log_write( MID_DEBUG_1, "decode_erd_lib.c", "Line: %d\n", __LINE__ );

                recipe_p->makes_unit = text_copy_to_new( tmp_unit );
                log_write( MID_DEBUG_1, "decode_erd_lib.c", "Line: %d\n", __LINE__ );
            }
        }
        else
        {
            //  This is a serves amount, not a MAKES amount
            recipe_p->serves = text_copy_to_new( local_amount );
            log_write( MID_DEBUG_1, "decode_erd_lib.c", "Line: %d\n", __LINE__ );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the 'Preparation Time:' tag.
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
DECODE_ERD__prep_time(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this isn't the title
    erd_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( title_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( title_p );

        //  Look for a "Recipe By:" tag
        if ( strncmp( tmp_p, ERD_PREP_TIME, ERD_PREP_TIME_L ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            tmp_p += ERD_PREP_TIME_L;
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Is there something to save here ?
            if( strlen( tmp_p ) > 0 )
            {
                //  Save the recipe title (name)
                recipe_p->time_prep = text_copy_to_new( tmp_p );
                log_write( MID_DEBUG_1, "decode_erd_lib.c", "Line: %d\n", __LINE__ );
            }

            // Change the pass_fail flag to PASS
            erd_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the 'Cook Time:' tag.
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
DECODE_ERD__cook_time(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this isn't the title
    erd_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( title_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( title_p );

        //  Look for a "Recipe By:" tag
        if ( strncmp( tmp_p, ERD_COOK_TIME, ERD_COOK_TIME_L ) == 0 )
        {
            //  This is the title!  Move the pointer past the title tag.
            tmp_p += ERD_COOK_TIME_L;
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Is there something to save here ?
            if( strlen( tmp_p ) > 0 )
            {
                //  Save the recipe title (name)
                recipe_p->time_cook = text_copy_to_new( tmp_p );
                log_write( MID_DEBUG_1, "decode_erd_lib.c", "Line: %d\n", __LINE__ );
            }

            // Change the pass_fail flag to PASS
            erd_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    // DONE with this line
    return ( erd_rc );
}

/****************************************************************************/
/**
 *  Search for and process the '[Amount] [Measure] ...' tag.
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
DECODE_ERD__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   auip_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             erd_rc;
    /**
     * @param tmp_p             Temporary data pointer                      */
    char                        *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    erd_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Skip everything if this is a blank line
    if ( text_is_blank_line( auip_p ) != true )
    {
        tmp_p = text_skip_past_whitespace( auip_p );

        //  Is this the start of the directions ?
        if ( strncmp( tmp_p, ERD_PREPARATION, ERD_PREPARATION_L ) == 0 )
        {
            //  YES:    Change decode state
            erd_rc = true;
        }
        //  Is this the AUIP tag line 1 ?
        else
        if (    ( strstr( tmp_p, ERD_AMOUNT  ) != NULL )
             && ( strstr( tmp_p, ERD_MEASURE ) != NULL )
             && ( strstr( tmp_p, ERD_INGRED  ) != NULL )
             && ( strstr( tmp_p, ERD_PREP    ) != NULL ) )
        {
            //  YES:    Ignore this line
        }
        //  Is this the AUIP tag line 2 ?
        else
        if ( strstr( tmp_p, "----------------------------------------------"  ) != NULL )
        {
            //  YES:    Ignore this line
        }
        //  This should be the AUIP information
        else
        {
            //  Process the first half (or the entire line)
            tmp_p = text_skip_past_whitespace( tmp_p );

            // Format the AUIP line
            recipe_fmt_auip( recipe_p, tmp_p, RECIPE_FORMAT_ERD );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( erd_rc );
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
DECODE_ERD__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   in_buffer_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start of a Meal-Master recipe
    erd_rc = false;

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Is this the end of the recipe ?
    if ( DECODE_ERD__end( in_buffer_p ) == true )
    {
        //  YES:    Change the return code
        erd_rc = true;
    }
    else
    {
        //  Format the Directions line
        recipe_add_instructions( recipe_p, in_buffer_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( erd_rc );
}

/****************************************************************************/
