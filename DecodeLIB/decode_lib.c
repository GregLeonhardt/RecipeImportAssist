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


/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
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
     *  @param  lowercase       The current directions data in lowercase    */
    char                            lowercase[ INSTRUCTIONS_L ];
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

        //  Clean out the lowercase buffer
        memset( lowercase, '\0', sizeof( lowercase ) );

        //  Copy the formatted text buffer to the lowercase buffer
        strncpy( lowercase, directions_p, sizeof( lowercase ) );

        //  Make everything in the buffer lowercase
        text_to_lowercase( lowercase );

        //--------------------------------------------------------------------
        //  Date :
        compare_p = strstr( lowercase, "date :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Date: ",       6 );
            found = true;
        }
        //====================================================================
        //  From :
        compare_p = strstr( lowercase, "from :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "From: ",       6 );
            found = true;
        }
        //====================================================================
        //  Makes :
        compare_p = strstr( lowercase, "makes :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Makes: ",      7 );
            found = true;
        }
#if 0
        //  @NOTE:  Many instances of corrupting the directions.
        //--------------------------------------------------------------------
        //  Makes
        compare_p = strstr( lowercase, "makes " );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Makes:",       6 );
            found = true;
        }
#endif
        //====================================================================
        //  Note :
        compare_p = strstr( lowercase, "note :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Notes: ",      7 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Notes :
        compare_p = strstr( lowercase, "notes :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Notes: ",      7 );
            found = true;
        }
        //====================================================================
        //  Per Serving:
        compare_p = strstr( lowercase, "per serving:" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "PerServing: ",12 );
            found = true;
        }
        //====================================================================
        //  Posted By :
        compare_p = strstr( lowercase, "posted by :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentBy:    ", 11 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Posted By:
        compare_p = strstr( lowercase, "posted by:" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentBy:   ",  10 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Posted By
        compare_p = strstr( lowercase, "posted by" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentBy:   ",  10 );
            found = true;
        }
        //====================================================================
        //  Posted To :
        compare_p = strstr( lowercase, "posted To :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentTo:    ", 11 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Posted To:
        compare_p = strstr( lowercase, "posted to:" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentTo:   ",  10 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Posted To
        compare_p = strstr( lowercase, "posted to" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentTo:   ",  10 );
            found = true;
        }
        //====================================================================
        //  Recipe By :
        compare_p = strstr( lowercase, "recipe by :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "RecipeBy:  ", 11 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Recipe By:
        compare_p = strstr( lowercase, "recipe by:" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "RecipeBy: ",  10 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Recipe By
        compare_p = strstr( lowercase, "recipe by" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "RecipeBy: ",  10 );
            found = true;
        }
        //====================================================================
        //  Sent By :
        compare_p = strstr( lowercase, "sent by :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentBy:  ",     9 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Sent By:
        compare_p = strstr( lowercase, "sent by:" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentBy: ",      8 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Sent By
        compare_p = strstr( lowercase, "sent by " );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentBy: ",      8 );
            found = true;
        }
        //====================================================================
        //  Sent To :
        compare_p = strstr( lowercase, "sent to :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentTo:  ",     9 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Sent To:
        compare_p = strstr( lowercase, "sent to:" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentTo: ",      8 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Sent To
        compare_p = strstr( lowercase, "sent to " );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "SentTo: ",      8 );
            found = true;
        }
        //====================================================================
        //  Serves :
        compare_p = strstr( lowercase, "serves :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Serves: ",     8 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Servings:
        compare_p = strstr( lowercase, "servings:" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Serves:   ",  10 );
            found = true;
        }
        //--------------------------------------------------------------------
        //  Servings :
        compare_p = strstr( lowercase, "servings :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Serves:    ", 11 );
            found = true;
        }
        //====================================================================
        //  Source :
        compare_p = strstr( lowercase, "source :" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "Source: ",     8 );
            found = true;
        }
        //====================================================================
        //  To Plate:
        compare_p = strstr( lowercase, "to plate:" );
        if ( compare_p != NULL )
        {
            memcpy( &directions_p[ compare_p - lowercase ], "ToPlate: ",     9 );
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
                        // YES: Release the storage buffer.
                        mem_free( directions_p );
                    }
                }
                else
                {

                    //  NO:     Are we supposed to free the buffer ?
                    if ( free_note == true )
                    {
                        //  YES:    Release the storage buffer.
                        mem_free( directions_p );
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
