/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017,2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define _GNU_SOURCE             //  See feature_test_macros(7)

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <ctype.h>              //  Testing and mapping characters
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <xlate_api.h>          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include <decode_api.h>         //  API for all decode_*            PUBLIC
#include "decode_lib.h"         //  API for all DECODE__*           PRIVATE
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
#define INSTRUCTIONS_L          ( 1024 * 1024 )
//----------------------------------------------------------------------------
#define TIME_L                  ( 5 + 1 )
//----------------------------------------------------------------------------
#define MAKES_AMOUNT            ( 10 + 1 )
//----------------------------------------------------------------------------
#define MAKES_UNIT              ( 25 + 1 )
//----------------------------------------------------------------------------
#define LINE_L                  ( 255 + 1 )
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
 *  Scan the recipe title for information that can categorize the recipe.
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

static
char    *
DECODE__get_tag_data(
    char                        *   source_data_p,
    char                        *   search_string_p
    )
{
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  string_beg_p    Beginning of the source string              */
    char                        *   string_beg_p;
    /**
     *  @param  string_end_p    End of the source string                    */
    char                        *   string_end_p;
    /**
     *  @param  string_l        Length of the source string                 */
    int                             string_l;
    /**
     *  @param  data_p          Pointer to a data buffer with the tag data  */
    char                        *   data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the return data pointer.
    data_p = NULL;

    /************************************************************************
     *  Process a [FROM: "xxxx"] tag
     ************************************************************************/

    //  NO:     Is there a FROM: in this directions line ?
    if ( ( temp_p = strcasestr( source_data_p, search_string_p ) ) != NULL )
    {
        //  YES:    Move past the tag and any white space that follows
        string_beg_p = &temp_p[ strlen( search_string_p ) ];
        string_beg_p = text_skip_past_whitespace( string_beg_p );

        //  Is the next character a quotation ["]
        if (    ( string_beg_p[ 0 ] == '"'  )
             && ( string_beg_p[ 1 ] != '"'  )
             && ( string_beg_p[ 1 ] != '\0' ) )
        {
            //  YES:    Look for the ending quotation mark
            string_end_p = strchr( &string_beg_p[ 1 ], '"' );

            //  Is it a well formed source tag ?
            if ( string_end_p != NULL )
            {
                //  YES:    Allocate a data buffer for the tag data
                data_p = mem_malloc( MAX_LINE_L );

                //  Set the length of the data to copy out
                string_l = ( string_end_p - string_beg_p ) - 1;

                //  Copy the source data string
                memcpy( data_p, &string_beg_p[ 1 ], string_l );

                //  Now remove the FROM: string from the directions line.
                text_remove( temp_p, 0, ( string_end_p - temp_p ) + 1 );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( data_p );
}

/****************************************************************************/
/**
 *  Some recipes have embedded content in the recipe directions.  Such as
 *  the original poster, number of people it serves, etc.
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *      Be careful here.  If you try to use this to simply change case the
 *      function will go into an infinite loop.
 *
 ****************************************************************************/

void
DECODE__directions_cleanup(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  compare_p       Pointer to a compare match string           */
    char                        *   compare_p;
    /**
     *  @param  replace         A flag showing that something was replace   */
    int                             replace;
    /**
     *  @param  found           A flag showing that a tag was found         */
    int                             found;
    /**
     *  @param  directions_l    Initial length of the directions string     */
    int                             directions_l;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the found flag;
    found = false;

    //  @NOTE - Some substitutions will take more space then the original text
    //          so the source buffer size is being increased by an arbitrary
    //          number.

    //  Set the initial size of the buffer
    directions_l = strlen( recipe_p->instructions ) + LINE_L;

    //  Allocate a new (larger) buffer.
    directions_p = mem_malloc( directions_l );

    //  Copy the instructions data to the new buffer.
    memcpy( directions_p, recipe_p->instructions, ( directions_l - LINE_L ) );

    //  Release the old buffer
    mem_free( recipe_p->instructions );

    //  And finally, replace the pointer with the new buffer
    recipe_p->instructions = directions_p;

    /************************************************************************
     *  Cleanup some common formatting problems
     ************************************************************************/

    do
    {
        //  Set the flag
        replace = false;

        //--------------------------------------------------------------------
        //  "Date :"
        compare_p = strcasestr( directions_p, "date :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Date:");

            replace = true;
        }
        //====================================================================
        //  "From ::
        compare_p = strcasestr( directions_p, "from :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "From:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  "From The:"
        compare_p = strcasestr( directions_p, "from the:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "From:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  ">From:"
        compare_p = strcasestr( directions_p, ">from:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "From:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  "From The "
        //  Can't use this because "from the" occurs in normal recipe directions.
        //====================================================================
        //  Makes :
        compare_p = strcasestr( directions_p, "makes :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Makes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Makes
        compare_p = strcasestr( directions_p, "makes " );
        if ( compare_p != NULL )
        {
            /**
             *  @param  tmp_p           Temporary pointer                   */
            char                        *   tmp_p;

            //  Is the next thing numeric ?
            tmp_p = compare_p + 6;
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Is it numeric ?
            if ( isdigit( tmp_p[ 0 ] ) != 0 )
            {
                memcpy( compare_p, "Makes:",       6 );
                replace = true;
            }
        }
        //====================================================================
        //  Notes :
        compare_p = strcasestr( directions_p, "notes :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 7 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Notes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Note :
        compare_p = strcasestr( directions_p, "note :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Notes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Notes
        compare_p = strcasestr( directions_p, "notes " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Notes:");

            replace = true;
        }
        //====================================================================
        //  Per Serving:
        compare_p = strcasestr( directions_p, "per serving:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 12 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "PerServing:");

            replace = true;
        }
        //====================================================================
        //  Posted By :
        compare_p = strcasestr( directions_p, "posted by :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 11 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Posted By:
        compare_p = strcasestr( directions_p, "posted by:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Posted By
        compare_p = strcasestr( directions_p, "posted by " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //====================================================================
        //  Posted To :
        compare_p = strcasestr( directions_p, "posted to :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 11 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Posted To:
        compare_p = strcasestr( directions_p, "posted to:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Posted To
        compare_p = strcasestr( directions_p, "posted to " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //====================================================================
        //  Recipe By :
        compare_p = strcasestr( directions_p, "recipe by :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 11 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "RecipeBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Recipe By:
        compare_p = strcasestr( directions_p, "recipe by:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "RecipeBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Recipe By
        compare_p = strcasestr( directions_p, "recipe by " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "RecipeBy:");

            replace = true;
        }
        //====================================================================
        //  Sent By :
        compare_p = strcasestr( directions_p, "sent by :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Sent By:
        compare_p = strcasestr( directions_p, "sent by:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Sent By
        compare_p = strcasestr( directions_p, "sent by " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentBy:");

            replace = true;
        }
        //====================================================================
        //  Sent To :
        compare_p = strcasestr( directions_p, "sent to :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Sent To:
        compare_p = strcasestr( directions_p, "sent to:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Sent To
        compare_p = strcasestr( directions_p, "sent to " );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "SentTo:");

            replace = true;
        }
        //====================================================================
        //  Servings :
        compare_p = strcasestr( directions_p, "servings :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Serves:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Servings:
        compare_p = strcasestr( directions_p, "servings:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Serves:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Serves :
        compare_p = strcasestr( directions_p, "serves :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Serves:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Serves
        //  NOTE:   This code gets confused by compound words such as
        //          preserves.  They get translated into 'preServes:'
#if 0
        compare_p = strcasestr( directions_p, "serves " );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Serves:",     7 );
            replace = true;
        }
#endif
        //====================================================================
        //  Source :
        compare_p = strcasestr( directions_p, "source :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 8 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Source:");

            replace = true;
        }
        //====================================================================
        //  To Plate:
        compare_p = strcasestr( directions_p, "to plate:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 9 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "ToPlate:");

            replace = true;
        }
        //====================================================================
        //  Yield :
        compare_p = strcasestr( directions_p, "yield :" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 7 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Makes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Yield:
        compare_p = strcasestr( directions_p, "yield:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 6 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Makes:");

            replace = true;
        }
        //--------------------------------------------------------------------
        //  Yield
        compare_p = strcasestr( directions_p, "yield " );
        if ( compare_p != NULL )
        {
            /**
             *  @param  tmp_p           Temporary pointer                   */
            char                        *   tmp_p;

            //  Is the next thing numeric ?
            tmp_p = compare_p + 6;
            tmp_p = text_skip_past_whitespace( tmp_p );

            //  Is it numeric ?
            if ( isdigit( tmp_p[ 0 ] ) != 0 )
            {
                memcpy( compare_p, "Makes:",       6 );
                replace = true;
            }
        }
        //====================================================================
        //  Copyright,
        compare_p = strcasestr( directions_p, "copyright," );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 10 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Copyright:");

            replace = true;
        }
        //====================================================================
        //  Imported-From:
        //====================================================================
        compare_p = strcasestr( directions_p, "S(Imported From):" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 17 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Imported-From:");

            replace = true;
        }
        //====================================================================
        //  TIME-COOK:
        //====================================================================
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cook time on High):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 21 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cooking Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 16 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Baking Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 15 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cook Time:):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 13 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Bake Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 13 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cook Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 13 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cooking):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 11 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Baking):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 10 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Grill):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 9 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Cook):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 8 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Bake):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 8 );

                //  Set the found flag
                found = true;
            }
        }

        //  Did we find a tag ?
        if ( found == true )
        {
            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Time-Cook:");

            //  Something was replace and changed.
            replace = true;

            //  Reset the found flag
            found = false;
        }
        //====================================================================
        //  TIME-WAIT:
        //====================================================================
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Marinating time:):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 20 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Standing Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 17 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Standing Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 17 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Chilling):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 12 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Chill:):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 10 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Chill):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 9 );

                //  Set the found flag
                found = true;
            }
        }

        //  Did we find a tag ?
        if ( found == true )
        {
            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Time-Wait:");

            //  Something was replace and changed.
            replace = true;

            //  Reset the found flag
            found = false;
        }
        //====================================================================
        //  TIME-TOTAL:
        //====================================================================
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "Start to Finish Time:" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 21 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Prep & Cook Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 20 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Total Time):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 14 );

                //  Set the found flag
                found = true;
            }
        }
        if ( found == false )
        {
            compare_p = strcasestr( directions_p, "T(Ready in):" );
            if ( compare_p != NULL )
            {
                //  Remove the old text
                text_remove( compare_p, 0, 12 );

                //  Set the found flag
                found = true;
            }
        }

        //  Did we find a tag ?
        if ( found == true )
        {
            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Time-Total:");

            //  Something was replace and changed.
            replace = true;

            //  Reset the found flag
            found = false;
        }
        //--------------------------------------------------------------------

    }   while( replace == true );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe title for information that can categorize the recipe.
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__title_information(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  name_p          Pointer to a local copy of the recipe title */
    char                        *   name_p;
    /**
     *  @param  buffer_p        A local pointer to the input buffer         */
    char                        *   buffer_p;
    /**
     *  @param  next_word       A local bucket for the next directions word */
    char                            next_word[ MAX_LINE_L ];
    /**
     * @param chapter_p         Pointer to a temp data buffer               */
    char                        *   chapter_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Allocate storage for the local copy of the recipe title
    name_p = mem_malloc( ( strlen( recipe_p->name ) + 1 ) );
    log_write( MID_DEBUG_1, "decode_lib.c", "Line: %d\n", __LINE__ );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is there a recipe title to analyze ?
    if ( recipe_p->name != NULL )
    {
        //  YES:    Copy the recipe title to the local buffer
        memcpy( name_p, recipe_p->name, strlen( recipe_p->name ) );

        //  Clean the next word buffer
        memset( next_word, '\0', sizeof( next_word ) );

        //  Process all words in the input buffer
        for( buffer_p = text_next_word( name_p, next_word );
             buffer_p != NULL;
             buffer_p = text_next_word( buffer_p, next_word ) )
        {
            //  Lookup to see if this word has meaning
            chapter_p = xlate_chapter( next_word );

            //  Do we have a chapter to save ?
            if ( chapter_p != NULL )
            {
                //  YES:    Save it
                decode_save_chapter( chapter_p, recipe_p );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release the recipe name buffer
    mem_free( name_p );

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [FROM: "wxyz"]
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_from(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [FROM: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "From:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Format the output data
                memset( temp_data, '\0', sizeof( temp_data ) );
                snprintf( temp_data, sizeof( temp_data ),
                          "FROM: \"%s\"", temp_p );

                //  Free storage used by the old buffer
                mem_free( temp_p );


                //  Add it to [NOTES :].
                list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [SOURCE: "wxyz"]
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_source(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                             saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [SOURCE: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Source:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a source ?
                if ( recipe_p->source == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < LINE_L )
                    {
                        //  YES:    Save it
                        recipe_p->source = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Source: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [COPYRIGHT: "wxyz"]
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_copyright(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                             saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [COPYRIGHT: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Copyright:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a copyright ?
                if ( recipe_p->copyright == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < LINE_L )
                    {
                        //  YES:    Save it
                        recipe_p->copyright = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Copyright: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [Description: "wxyz"]
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_description(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                             saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [DESCRIPTION: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Description:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( recipe_p->description == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < LINE_L )
                    {
                        //  YES:    Save it
                        recipe_p->description = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Description: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [MAKES: "0 0/0 wxyz"]
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_makes(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                             saved;
    /**
     *  @param  makes_amount    How many of something will be made          */
    char                        *   makes_amount;
    /**
     *  @param  makes_unit      What is being made                          */
    char                        *   makes_unit;
    /**
     *  @param  ndx             Index into a buffer                         */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [MAKES: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Makes:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Allocate storage for the two parts
                makes_amount = mem_malloc( MAKES_AMOUNT );
                makes_unit   = mem_malloc( MAKES_UNIT );

                //  Copy the makes_amount
                for ( ndx = 0;
                      ndx < MAKES_AMOUNT;
                      ndx += 1 )
                {
                    //  Is this a valid amount character ?
                    if (    ( isdigit( temp_p[ ndx ] ) !=  0  )
                         || ( temp_p[ ndx ]            == '/' )
                         || ( temp_p[ ndx ]            == ' ' ) )
                    {
                        //  YES:    Copy the character
                        strncat( makes_amount, &temp_p[ ndx ], 1 );
                    }
                    else
                    {
                        //  NO:     Done with makes_amount
                        break;
                    }
                }

                //  Copy the makes_unit
                for ( ;
                      ndx < MAKES_UNIT;
                      ndx += 1 )
                {
                    //  Is this a valid amount character ?
                    if ( temp_p[ ndx ] != '\0' )
                    {
                        //  YES:    Copy the character
                        strncat( makes_unit, &temp_p[ ndx ], 1 );
                    }
                    else
                    {
                        //  NO:     Done with makes_unit
                        break;
                    }
                }

                //  Release the storage used by the temporary buffer.
                mem_free( temp_p );

                //  Is there already a makes amount or unit ?
                if (    ( recipe_p->makes      == NULL )
                     && ( recipe_p->makes_unit == NULL ) )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if (    ( strlen( makes_amount ) < MAKES_AMOUNT - 1 )
                         && ( strlen( makes_unit   ) < MAKES_UNIT   - 1 ) )
                    {
                        //  YES:    Save it
                        recipe_p->makes      = makes_amount;
                        recipe_p->makes_unit = makes_unit;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Yield: \"%s %s\"", makes_amount, makes_unit );

                    //  Free storage used by the old buffer
                    mem_free( makes_amount );
                    mem_free( makes_unit );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [TIME-wxyz: "00:00"]
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_time(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                             saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [TIME-wxyz: "00:00"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //----------------------------------------------------------------
            //  TIME-PREP:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Time-Prep:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( recipe_p->time_prep == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        recipe_p->time_prep = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-PREP: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }

            //----------------------------------------------------------------
            //  TIME-WAIT:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Time-Wait:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( recipe_p->time_wait == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        recipe_p->time_wait = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-WAIT: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }

            //----------------------------------------------------------------
            //  TIME-COOK:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Time-Cook:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( recipe_p->time_cook == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        recipe_p->time_cook = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-COOK: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }

            //----------------------------------------------------------------
            //  TIME-REST:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Time-Rest:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( recipe_p->time_rest == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        recipe_p->time_rest = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-REST: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }

            //----------------------------------------------------------------
            //  TIME-TOTAL:
            //----------------------------------------------------------------
            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Time-Total:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( recipe_p->time_total == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < TIME_L )
                    {
                        //  YES:    Save it
                        recipe_p->time_total = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "TIME-TOTAL: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe directions for [S(Imported From): "wxyz"]
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_import_from(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  temp_p          Temporary string pointer                    */
    char                        *   temp_p;
    /**
     *  @param  temp_data       Temporary data buffer                       */
    char                            temp_data[ MAX_LINE_L ];
    /**
     *  @param  saved           The information has been saved              */
    int                             saved;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Process a [Imported-From: "xxxx"] tag
     ************************************************************************/

    //  Are there any directions for this recipe ?
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        //  YES:    Scan the whole thing.
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Initialize the saved flag
            saved = false;

            //  Look for the tag
            temp_p = DECODE__get_tag_data( directions_p, "Imported-From:" );

            //  Did we find it ?
            if ( temp_p != NULL )
            {
                //  YES:    Is there already a description ?
                if ( recipe_p->import_from == NULL )
                {
                    //  NO:     Will the data fit into the MasterCook Buffer ?
                    if ( strlen( temp_p ) < LINE_L )
                    {
                        //  YES:    Save it
                        recipe_p->import_from = temp_p;

                        //  Set the saved flag
                        saved = true;
                    }
                }
                //  Was the information saved ?
                if ( saved == false )
                {
                    //  NO:     Format the output data
                    memset( temp_data, '\0', sizeof( temp_data ) );
                    snprintf( temp_data, sizeof( temp_data ),
                              "Imported-From: \"%s\"", temp_p );

                    //  Free storage used by the old buffer
                    mem_free( temp_p );

                    //  Add it to [NOTES :].
                    list_put_last( recipe_p->notes, text_copy_to_new( " "       ) );
                    list_put_last( recipe_p->notes, text_copy_to_new( temp_data ) );
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the recipe title for information that can categorize the recipe.
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_notes(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  free_note       Flag to free memory for the buffer          */
    int                             free_note;
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  notes_p         Pointer to the start of a notes string      */
    char                        *   notes_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that the buffer will not be free'd when we are done
    free_note = false;

    /************************************************************************
     *  Function
     ************************************************************************/

    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Look for the keyword "NOTES:" or "NOTE:"
            if (    ( ( notes_p = strstr( directions_p, "NOTES:" ) ) != NULL )
                 || ( ( notes_p = strstr( directions_p, "Notes:" ) ) != NULL )
                 || ( ( notes_p = strstr( directions_p, "Note:"  ) ) != NULL )
                 || ( ( notes_p = strstr( directions_p, "Note:"  ) ) != NULL ) )
            {
                //  Is this a Notes: ONLY directions line ?
                if ( notes_p == directions_p )
                {
                    //  YES:    Remove it from the directions
                    list_delete( recipe_p->directions, directions_p );

                    //  We are go to have to free this buffer later.
                    free_note = true;
                }

                //  Remove the remaining text from directions by NULL terminating
                notes_p[ 0 ] = '\0';

                //  Locate the end of the notes tag
                notes_p = strchr( ++notes_p, ':' );

                //  Move past any leading white space
                notes_p = text_skip_past_whitespace( ++notes_p );

                //  Is there anything else on this line ?
                if ( text_is_blank_line( notes_p ) == false )
                {
                    //  YES:    Add the remaining text to the notes.
                    recipe_fmt_notes( recipe_p, notes_p );

                    //  Are we supposed to free the buffer ?
                    if ( free_note == true )
                    {
                        //  YES:    Release the storage buffer.
                        mem_free( directions_p );

                        //  And clear the flag
                        free_note = false;
                    }
                }
                else
                {

                    //  NO:     Are we supposed to free the buffer ?
                    if ( free_note == true )
                    {
                        //  YES:    Release the storage buffer.
                        mem_free( directions_p );

                        //  And clear the flag
                        free_note = false;
                    }

                    //  Add the next line to the notes:
                    directions_p = list_get_next( recipe_p->directions, directions_p );

                    //  Sanity check! Did we get something
                    if ( directions_p != NULL )
                    {
                        //  YES:    Remove the entire line from the directions
                        list_delete( recipe_p->directions, directions_p );

                        //  Now add it to the notes
                        recipe_fmt_notes( recipe_p, directions_p );

                        //  Release the storage buffer.
                        mem_free( directions_p );
                    }
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
