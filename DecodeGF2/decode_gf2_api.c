/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_gf2' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_GF2          ( "ALLOCATE STORAGE FOR DECODE_GF2" )

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
#include <decode_gf2_api.h>     //  API for all decode_gf2_*        PUBLIC
#include "decode_gf2_lib.h"     //  API for all DECODE_GF2__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    gf2_decode_state_e
{
    GF2_DS_START                =   0,
    GF2_DS_TITLE                =   1,
    GF2_DS_BREAK                =   2,
    GF2_DS_NOTE                 =   3,
    GF2_DS_AUIP                 =   4,
    GF2_DS_DIRECTIONS           =   5,
    GF2_DS_END                  =  99
};
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
 *  Decode a MealMaster formatted recipe
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return                     void
 *
 *  @note
 *
 ****************************************************************************/

void
decode_gf2(
    struct  list_base_t         *   gf2_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    gf2_decode_state_e      gf2_state;
    /**
     * @param list_lock_key     File list key                               */
    int                             list_lock_key;
    /**
     * @param cookn_format      Recipe is using the Cook'n file format      */
    int                             cookn_format;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                        *   list_data_p;
    /**
     *  @param recipe_t         Primary structure for a recipe              */
    struct   recipe_t           *   recipe_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decode_gf2",
                  "Entering with %6d lines of text\n",
                  list_query_count( gf2_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( gf2_list_p );

    //  Change state to looking for the recipe title.
    gf2_state = GF2_DS_START;

    //  Not using the Cook'n format style until it is detected
    cookn_format = false;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    recipe_p = recipe_new( RECIPE_FORMAT_GF2 );

    //  Copy source information to the recipe structure
    decode_copy_info_to_recipe( recipe_p, source_info_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( gf2_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( gf2_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( gf2_list_p, list_data_p, list_lock_key );

        //  Debug log output
        log_write( MID_DEBUG_0, "decode_gf2",
                      "'%.60s'\n", list_data_p );

        //  Process the new data
        switch( gf2_state )
        {

            /****************************************************************
             *  Search for the start of a recipe
             ****************************************************************/

            case GF2_DS_START:
            {
                //  Locate and process the recipe title
                if ( DECODE_GF2__start( list_data_p ) == true )
                {
                    //  The next line should be the recipe title.
                    gf2_state = GF2_DS_TITLE;
                }
            }   break;

            /****************************************************************
             *  Search for the recipe title
             ****************************************************************/

            case GF2_DS_TITLE:
            {
                //  Locate and process the recipe title
                if ( DECODE_GF2__title( recipe_p, list_data_p ) == true )
                {
                    //  Change to Title Case
                    text_title_case( recipe_p->name, recipe_p->name );

                    //  Log the new title
                    log_write( MID_INFO, "decode_gf2",
                               "'%s'\n", recipe_p->name );

                    //  Next look for a blank line or a Cook'n segment change.
                    gf2_state = GF2_DS_BREAK;
                }
            }   break;

            /****************************************************************
             *  This line should be a segment break '|'
             ****************************************************************/

            case GF2_DS_BREAK:
            {
                //  Is this a blank line ?
                if ( text_is_blank_line( list_data_p ) == true )
                {
                    //  YES:    This is the old format style.
                    cookn_format = false;

                    //  Set the next field
                    gf2_state = GF2_DS_NOTE;
                }
                //  Is this a section break marker ?
                else
                if ( ( list_data_p[ 0 ] ) == '|' )
                {
                    //  This is present in the Cook'n format style.
                    cookn_format = true;

                    //  Set the next field
                    gf2_state = GF2_DS_NOTE;
                }
                else
                {
                    //  ???:    None of the above
                    gf2_state = GF2_DS_AUIP;

                    //  Process the AUIP data.
                    DECODE_GF2__auip( recipe_p, list_data_p );
                }
            }   break;

            /****************************************************************
             *  NOTE: segment
             ****************************************************************/

            case GF2_DS_NOTE:
            {
                //  Are we using the Cook'n format style ?
                if ( cookn_format == false )
                {
                    //  YES:    Process the note.
                    DECODE_GF2__note( recipe_p, list_data_p );

                    //  Is this the end of the note ?
                    if ( text_is_blank_line( list_data_p ) == true )
                    {
                        //  YES:    Set the next field
                        gf2_state = GF2_DS_AUIP;
                    }
                }
                //  Is this a section break marker ?
                else
                {
                    if ( ( list_data_p[ 0 ] ) == '|' )
                    {
                        //  YES:    Set the next field
                        gf2_state = GF2_DS_AUIP;

                        //  There may be some data in the notes processing
                        //  buffer. This call will flush it out.
                        DECODE_GF2__note( recipe_p, "   " );
                    }
                    else
                    {
                        //  Process the data
                        DECODE_GF2__aspcy( recipe_p, list_data_p );
                    }
                }
            }   break;

            /****************************************************************
             *  Search for the start of AUIP
             *      Amount, Unit, Ingredient, Preparation
             ****************************************************************/

            case GF2_DS_AUIP:
            {
                //  Are we using the Cook'n format style ?
                if ( cookn_format == false )
                {
                    //  YES:    Process the AUIP data.
                    DECODE_GF2__auip( recipe_p, list_data_p );

                    //  Is this the end of the note ?
                    if ( text_is_blank_line( list_data_p ) == true )
                    {
                        //  YES:    Set the next field
                        gf2_state = GF2_DS_DIRECTIONS;
                    }
                }
                //  Is this a section break marker ?
                else
                {
                    if ( ( list_data_p[ 0 ] ) == '|' )
                    {
                        //  YES:    Set the next field
                        gf2_state = GF2_DS_DIRECTIONS;
                    }
                    else
                    {
                        //  Process the AUIP data.
                        DECODE_GF2__auip( recipe_p, list_data_p );
                    }
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case GF2_DS_DIRECTIONS:
            {
                //  Is this the end-of-recipe marker ?
                if ( decode_gf2_end( list_data_p ) == true )
                {
                    //  YES:    The-End of this recipe
                    gf2_state = GF2_DS_END;

                    //  There may be some data in the directions
                    //  processing buffer. This call will flush it out.
                    DECODE_GF2__directions( recipe_p, "   " );
                }
                else
                {
                    //  Process a line of directions information
                    DECODE_GF2__directions( recipe_p, list_data_p );
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
    list_user_unlock( gf2_list_p, list_lock_key );

    //  Is there anything still on the list ?
    if ( list_query_count( gf2_list_p ) != 0 )
    {
        //  YES:    There shouldn't be!
        log_write( MID_FATAL, "decode_gf2",
                   "There is still something on the list\n" );
    }

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook GF2 formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return gf2_rc              Passed through from DECODE_GF2__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_gf2_start(
    char                        *   data_p
    )
{
    /**
     * @param gf2_rc            Return Code                                 */
    int                             gf2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    gf2_rc = DECODE_GF2__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
#if 1   //  @ToDo   [DEBUG]     Disable GF2
    return( gf2_rc );
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
 *  @return gf2_rc              Passed through from DECODE_GF2__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_gf2_end(
    char                        *   data_p
    )
{
    /**
     * @param gf2_rc            Return Code                                 */
    int                             gf2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    gf2_rc = DECODE_GF2__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( gf2_rc );
}

/****************************************************************************/
