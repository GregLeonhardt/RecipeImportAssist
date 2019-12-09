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
     *  @param  found           A flag showing that something was found     */
    int                             found;
    /**
     *  @param  directions_l    Initial length of the directions string     */
    int                             directions_l;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Just a shortened version of the pointer
    directions_p = recipe_p->instructions;

    //  Set the initial size of the buffer
    directions_l = strlen( directions_p );

    /************************************************************************
     *  Cleanup some common formatting problems
     ************************************************************************/

    do
    {
        //  Set the flag
        found = false;

        //--------------------------------------------------------------------
        //  "Date :"
        compare_p = strcasestr( directions_p, "date :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Date: ",       6 );
            found = true;
        }
        //====================================================================
        //  "From ::
        compare_p = strcasestr( directions_p, "from :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "From: ",       6 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  "From The:"
        compare_p = strcasestr( directions_p, "from the:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "From:    ",    9 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  ">From:"
        compare_p = strcasestr( directions_p, ">from:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, " From:",    6 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  "From The "
        //  Can't use this because "from the" occurs in normal recipe directions.
        //====================================================================
        //  Makes :
        compare_p = strcasestr( directions_p, "makes :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Makes: ",      7 );
            found = true;
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
                found = true;
            }
        }
        //====================================================================
        //  Notes :
        compare_p = strcasestr( directions_p, "notes :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Notes: ",      7 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Note :
        compare_p = strcasestr( directions_p, "note :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Notes: ",      6 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Notes
        compare_p = strcasestr( directions_p, "notes " );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Notes:",      6 );
            found = true;
        }
        //====================================================================
        //  Per Serving:
        compare_p = strcasestr( directions_p, "per serving:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "PerServing: ",12 );
            found = true;
        }
        //====================================================================
        //  Posted By :
        compare_p = strcasestr( directions_p, "posted by :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentBy:    ", 11 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Posted By:
        compare_p = strcasestr( directions_p, "posted by:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentBy:   ",  10 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Posted By
        compare_p = strcasestr( directions_p, "posted by " );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentBy:   ",  10 );
            found = true;
        }
        //====================================================================
        //  Posted To :
        compare_p = strcasestr( directions_p, "posted to :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentTo:    ", 11 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Posted To:
        compare_p = strcasestr( directions_p, "posted to:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentTo:   ",  10 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Posted To
        compare_p = strcasestr( directions_p, "posted to " );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentTo:   ",  10 );
            found = true;
        }
        //====================================================================
        //  Recipe By :
        compare_p = strcasestr( directions_p, "recipe by :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "RecipeBy:  ", 11 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Recipe By:
        compare_p = strcasestr( directions_p, "recipe by:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "RecipeBy: ",  10 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Recipe By
        compare_p = strcasestr( directions_p, "recipe by " );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "RecipeBy: ",  10 );
            found = true;
        }
        //====================================================================
        //  Sent By :
        compare_p = strcasestr( directions_p, "sent by :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentBy:  ",     9 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Sent By:
        compare_p = strcasestr( directions_p, "sent by:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentBy: ",      8 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Sent By
        compare_p = strcasestr( directions_p, "sent by " );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentBy: ",      8 );
            found = true;
        }
        //====================================================================
        //  Sent To :
        compare_p = strcasestr( directions_p, "sent to :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentTo:  ",     9 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Sent To:
        compare_p = strcasestr( directions_p, "sent to:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentTo: ",      8 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Sent To
        compare_p = strcasestr( directions_p, "sent to " );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "SentTo: ",      8 );
            found = true;
        }
        //====================================================================
        //  Servings :
        compare_p = strcasestr( directions_p, "servings :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Serves:   ", 10 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Servings:
        compare_p = strcasestr( directions_p, "servings:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Serves:  ",    9 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Serves :
        compare_p = strcasestr( directions_p, "serves :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Serves: ",     8 );
            found = true;
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
            found = true;
        }
#endif
        //====================================================================
        //  Source :
        compare_p = strcasestr( directions_p, "source :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Source: ",     8 );
            found = true;
        }
        //====================================================================
        //  To Plate:
        compare_p = strcasestr( directions_p, "to plate:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "ToPlate: ",     9 );
            found = true;
        }
        //====================================================================
        //  Yield :
        compare_p = strcasestr( directions_p, "yield :" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Makes: ",  7 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Yield:
        compare_p = strcasestr( directions_p, "yield:" );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Makes:",  6 );
            found = true;
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
                found = true;
            }
        }
        //====================================================================
        //  Copyright,
        compare_p = strcasestr( directions_p, "copyright," );
        if ( compare_p != NULL )
        {
            memcpy( compare_p, "Copyright:",    10 );
            found = true;
        }
        //====================================================================
        //  Copyright,
        compare_p = strcasestr( directions_p, "start to finish time:" );
        if ( compare_p != NULL )
        {
            //  Remove the old text
            text_remove( compare_p, 0, 21 );

            //  Insert the new text
            text_insert( directions_p, directions_l,
                         (compare_p - directions_p ), "Time-Total:");

            //  Something was found and changed.
            found = true;
        }
        //--------------------------------------------------------------------

    }   while( found == true );

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
