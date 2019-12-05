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

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Just a shortened version of the pointer
    directions_p = recipe_p->instructions;

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

    //  Copy the recipe title to the local buffer
    memcpy( name_p, recipe_p->name, strlen( recipe_p->name ) );

    /************************************************************************
     *  Function
     ************************************************************************/

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

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release the recipe name buffer
    mem_free( name_p );

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

                //  Move past any leading whitespace
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
/**
 *  Scan the recipe directions for the recipe source
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
     *  @param  source_p        Pointer to a "SOURCE:" string               */
    char                        *   source_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Look for the keyword "From:"
            if ( ( source_p = strcasestr( directions_p, "From:" ) ) != NULL )
            {
                //  Remove the "FROM:" tag
                text_remove( source_p, 0, 5 );
            }
            //  Look for the keyword "SentBy:"
            else
            if ( ( source_p = strcasestr( directions_p, "SentBy:" ) ) != NULL )
            {
                //  Remove the "SENTBY:" tag
                text_remove( source_p, 0, 7 );
            }
            //  Look for the keyword "Source:"
            else
            if ( ( source_p = strcasestr( directions_p, "Source:" ) ) != NULL )
            {
                //  Remove the "Source:" tag
                text_remove( source_p, 0, 7 );
            }
            //  Look for the keyword "PostedBy:"
            else
            if ( ( source_p = strcasestr( directions_p, "PostedBy:" ) ) != NULL )
            {
                //  Remove the "POSTEDBY:" tag
                text_remove( source_p, 0, 9 );
            }
            //  Did we find "SOURCE:" ?
            if ( source_p != NULL )
            {
                //  YES:    Move past any leading white space
                source_p = text_skip_past_whitespace( source_p );

                //  Does a source string already exist ?
                if ( recipe_p->source != NULL )
                {
                    //  YES:    Dump the old for the new
                    mem_free( recipe_p->source );
                }

                //  Move the remaining string to the "AUTHOR: string
                recipe_p->source = text_copy_to_new( source_p );
                source_p[ 0 ] = '\0';
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
 *  Scan the recipe directions for a recipe copyright
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
     *  @param  copyright_p     Pointer to a "COPYRIGHT:" string            */
    char                        *   copyright_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Look for the keyword "Copyright:"
            if ( ( copyright_p = strcasestr( directions_p, "Copyright:" ) ) != NULL )
            {
                //  Remove the "COPYRIGHT:" tag
                text_remove( copyright_p, 0, 10 );
            }

            //  Did we find "COPYRIGHT:" ?
            if ( copyright_p != NULL )
            {
                //  YES:    Move past any leading whitespace
                copyright_p = text_skip_past_whitespace( copyright_p );

                //  Does a copyright string already exist ?
                if ( recipe_p->copyright != NULL )
                {
                    //  YES:    Dump the old for the new
                    mem_free( recipe_p->copyright );
                }

                //  Move the remaining string to the "COPYRIGHT:" string
                recipe_p->copyright = text_copy_to_new( copyright_p );
                copyright_p[ 0 ] = '\0';
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
 *  Scan the recipe directions for a recipe author.
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE__directions_author(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  directions_p    Pointer to a line of the directions         */
    char                        *   directions_p;
    /**
     *  @param  author_p        Pointer to a "AUTHOR:" string               */
    char                        *   author_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        for( directions_p = list_get_first( recipe_p->directions );
             directions_p != NULL;
             directions_p = list_get_next( recipe_p->directions, directions_p ) )
        {
            //  Look for the keyword "Copyright:"
            if ( ( author_p = strcasestr( directions_p, "Author:" ) ) != NULL )
            {
                //  Remove the "AUTHOR:" tag
                text_remove( author_p, 0, 10 );
            }
            //  Look for the keyword "RecipeBy:"
            else
            if ( ( author_p = strcasestr( directions_p, "RecipeBy:" ) ) != NULL )
            {
                //  Remove the "RECIPEBY:" tag
                text_remove( author_p, 0, 9 );
            }

            //  Did we find "AUTHOR:" ?
            if ( author_p != NULL )
            {
                //  YES:    Move past any leading whitespace
                author_p = text_skip_past_whitespace( author_p );

                //  Does a author string already exist ?
                if ( recipe_p->author != NULL )
                {
                    //  YES:    Dump the old for the new
                    mem_free( recipe_p->author );
                }

                //  Move the remaining string to the "AUTHOR:" string
                recipe_p->author = text_copy_to_new( author_p );
                author_p[ 0 ] = '\0';
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
