/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_cp2' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_CP2          ( "ALLOCATE STORAGE FOR DECODE_CP2" )

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
#include <decode_cp2_api.h>     //  API for all decode_cp2_*        PUBLIC
#include "decode_cp2_lib.h"     //  API for all DECODE_CP2__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private API Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Public API Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Decode a Big-Oven formatted recipe
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return                     void
 *
 *  @note
 *
 ****************************************************************************/

void
decode_cp2(
    struct  list_base_t         *   cp2_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    cp2_decode_state_e      cp2_state;
    /**
     * @param list_lock_key     File list key                               */
    int                             list_lock_key;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                        *   list_data_p;
    /**
     *  @param recipe_t         Primary structure for a recipe              */
    struct   recipe_t           *   recipe_p;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decode_cp2",
                  "Entering with %6d lines of text\n",
                  list_query_count( cp2_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( cp2_list_p );

    //  Change state to looking for the recipe title.
    cp2_state = CP2_DS_LOOKING_FOR_START;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    recipe_p = recipe_new( RECIPE_FORMAT_CP2 );

    //  Copy source information to the recipe structure
    decode_copy_info_to_recipe( recipe_p, source_info_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( cp2_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( cp2_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( cp2_list_p, list_data_p, list_lock_key );

        //  Debug log output
        log_write( MID_DEBUG_0, "decode_cp2",
                      "'%.60s'\n", list_data_p );

        /*********************************************************************
         *  Look for a segment change
         *********************************************************************/

        //  Locate the first character in the buffer
        tmp_data_p = text_skip_past_whitespace( list_data_p );

        //  Process the new data
        switch( cp2_state )
        {

            /****************************************************************
             *  Look for a recipe start tag
             ****************************************************************/

            case CP2_DS_LOOKING_FOR_START:
            {
                //  Is this a recipe start tag ?
                if ( strncmp( tmp_data_p, CP2_START, CP2_START_L  ) == 0 )
                {
                    //  YES:    Change recipe Decode State
                    cp2_state = CP2_DS_HEADER;
                }
                
            }   break;

            /****************************************************************
             *  Process the recipe header tags
             ****************************************************************/

            case CP2_DS_HEADER:
            {
                if ( strncmp( tmp_data_p, CP2_TITLE, CP2_TITLE_L  ) == 0 )
                {
                    //  Locate and process the recipe title
                    DECODE_CP2__title( recipe_p, list_data_p );
                    
                    //  Change to Title Case
                    text_title_case( recipe_p->name, recipe_p->name );

                    //  Log the new title
                    log_write( MID_INFO, "decode_cp2",
                               "'%s'\n", recipe_p->name );
                }
                else
                if ( strncmp( tmp_data_p, CP2_SERVINGS, CP2_SERVINGS_L  ) == 0 )
                {
                    //  Locate and process the recipe title
                    DECODE_CP2__srv_size( recipe_p, list_data_p );
                }
                else
                if ( strncmp( tmp_data_p, CP2_COMMENT, CP2_COMMENT_L  ) == 0 )
                {
                    //  Change recipe Decode State
                    cp2_state = CP2_DS_COMMENT;
                }

            }   break;

            /****************************************************************
             *  Process a Recipe Comment [Note:]
             ****************************************************************/

            case CP2_DS_COMMENT:
            {
                //  Skip everything if this is a blank line
                if ( text_is_blank_line( tmp_data_p ) == true )
                {
                    //  Change recipe Decode State
                    cp2_state = CP2_DS_AUIP;
                }
                else
                {
                    //  Add the text as a note:
                    recipe_fmt_notes( recipe_p, tmp_data_p );
                }
                
            }   break;

            /****************************************************************
             *  Process AUIP
             ****************************************************************/

            case CP2_DS_AUIP:
            {
                //  Locate and process the recipe title
                if ( DECODE_CP2__auip( recipe_p, list_data_p ) != true )
                {
                    //  Change recipe Decode State
                    cp2_state = CP2_DS_DIRECTIONS;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case CP2_DS_DIRECTIONS:
            {
                //  Locate and process the recipe title
                if ( DECODE_CP2__directions( recipe_p, list_data_p ) != true )
                {
                    //  Change recipe Decode State
                    cp2_state = CP2_DS_FOOTER;

                    //  There may be some data in the directions processing buffer.
                    //  This call will flush it out.
                    DECODE_CP2__directions( recipe_p, "   " );
                }
            }   break;

            /****************************************************************
             *  Process the Categories: information
             ****************************************************************/

            case CP2_DS_FOOTER:
            {
                //  Is this the End-of-Recipe tag ?
                if (    ( strncmp( tmp_data_p, CP2_END_1, CP2_END_1_L  ) == 0 )
                     || ( strncmp( tmp_data_p, CP2_END_2, CP2_END_2_L  ) == 0 ) )
                {
                    //  YES:    Change recipe Decode State
                    cp2_state = CP2_DS_LOOKING_FOR_START;
                }
                //  Is this a recipe categories tag ?
                else
                if ( strncmp( tmp_data_p, CP2_CATEGORY, CP2_CATEGORY_L  ) == 0 )
                {
                    //  YES:    Go add recipe categories as needed.
                    DECODE_CP2__categories( recipe_p, tmp_data_p );
                }
                //  Is this a Vegetarian tag ?
                else
                if ( strncmp( tmp_data_p, CP2_VEGETARIAN, CP2_VEGETARIAN_L  ) == 0 )
                {
                    //  YES:    Add the category [if needed].
                    DECODE_CP2__vegetarian( recipe_p, tmp_data_p );
                }
                
            }   break;

            /****************************************************************
             *  End of recipe decode.
             ****************************************************************/

            default:
            {
                //  Shouldn't be here
            }
        }

        //  Free the buffer
        mem_free( list_data_p );
    }

    /************************************************************************
     *  The recipe decode is complete.
     *  Send the recipe to the output encoder.
     ************************************************************************/

    decode_finalize( recipe_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release the lock on the level 3 list
    list_user_unlock( cp2_list_p, list_lock_key );

    //  Is there anything still on the list ?
    if ( list_query_count( cp2_list_p ) != 0 )
    {
        //  YES:    There shouldn't be!
        log_write( MID_FATAL, "decode_cp2",
                   "There is still something on the list\n" );
    }

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook CP2 formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return cp2_rc              Passed through from DECODE_CP2__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_cp2_start(
    char                        *   data_p
    )
{
    /**
     * @param cp2_rc            Return Code                                 */
    int                             cp2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    cp2_rc = DECODE_CP2__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
#if 1   //  @ToDo   [DEBUG]     Disable CP2
    return( cp2_rc );
#else
    return( false );
#endif
}

/****************************************************************************/
/**
 *  Test the data for the end of a Generic Recipe Formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return cp2_rc              Passed through from DECODE_CP2__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_cp2_end(
    char                        *   data_p
    )
{
    /**
     * @param cp2_rc            Return Code                                 */
    int                             cp2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    cp2_rc = DECODE_CP2__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( cp2_rc );
}

/****************************************************************************/
