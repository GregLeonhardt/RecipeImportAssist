/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_grf' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_GRF          ( "ALLOCATE STORAGE FOR DECODE_GRF" )

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
#include <decode_grf_api.h>     //  API for all decode_grf_*        PUBLIC
#include "decode_grf_lib.h"     //  API for all DECODE_GRF__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    grf_decode_state_e
{
    GRF_DS_START                =   0,
    GRF_DS_BLANK_1              =   1,
    GRF_DS_TITLE                =   2,
    GRF_DS_BLANK_2              =   3,
    GRF_DS_AUIP                 =   4,
    GRF_DS_BLANK_3              =   5,
    GRF_DS_DIRECTIONS           =   6,
    GRF_DS_END                  =  99
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
decode_grf(
    struct  list_base_t         *   grf_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    grf_decode_state_e      grf_state;
    /**
     * @param list_lock_key     File list key                               */
    int                             list_lock_key;
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
    log_write( MID_DEBUG_0, "decode_grf",
                  "Entering with %6d lines of text\n",
                  list_query_count( grf_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( grf_list_p );

    //  Change state to looking for the recipe title.
    grf_state = GRF_DS_START;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    recipe_p = recipe_new( RECIPE_FORMAT_GRF );

    //  Copy source information to the recipe structure
    decode_copy_info_to_recipe( recipe_p, source_info_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( grf_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( grf_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( grf_list_p, list_data_p, list_lock_key );

        //  Debug log output
        log_write( MID_DEBUG_0, "decode_grf",
                      "'%.60s'\n", list_data_p );

        //  Process the new data
        switch( grf_state )
        {

            /****************************************************************
             *  Search for the start of a recipe
             ****************************************************************/

            case GRF_DS_START:
            {
                //  Locate and process the recipe title
                if( DECODE_GRF__start( list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe categories.
                    grf_state = GRF_DS_TITLE;
                }
            }   break;

            /****************************************************************
             *  Search for the recipe title
             ****************************************************************/

            case GRF_DS_TITLE:
            {
                //  Locate and process the recipe title
                if( DECODE_GRF__title( recipe_p, list_data_p ) == true )
                {
                    //  Change to Title Case
                    text_title_case( recipe_p->name, recipe_p->name );

                    //  Log the new title
                    log_write( MID_INFO, "decode_grf",
                               "'%s'\n", recipe_p->name );

                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe categories.
                    grf_state = GRF_DS_BLANK_1;
                }
            }   break;

            /****************************************************************
             *  Search for the start of a recipe
             ****************************************************************/

            case GRF_DS_BLANK_1:
            {
                //  Skip everything if this is a blank line
                if( text_is_blank_line( list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe categories.
                    grf_state = GRF_DS_AUIP;
                }
            }   break;

            /****************************************************************
             *  Search for the start of AUIP
             *      Amount, Unit, Ingredient, Preparation
             ****************************************************************/

            case GRF_DS_AUIP:
            {
                //  Locate and process the recipe title
                if( DECODE_GRF__auip( recipe_p, list_data_p ) != true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe directions.
                    grf_state = GRF_DS_DIRECTIONS;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case GRF_DS_DIRECTIONS:
            {
                //  Locate and process the recipe title
                if( DECODE_GRF__directions( recipe_p, list_data_p ) != true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to recipe decode complete.
                    grf_state = GRF_DS_END;

                    //  There may be some data in the directions processing buffer.
                    //  This call will flush it out.
                    DECODE_GRF__directions( recipe_p, "   " );
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
    list_user_unlock( grf_list_p, list_lock_key );

    //  Is there anything still on the list ?
    if( list_query_count( grf_list_p ) != 0 )
    {
        //  YES:    There shouldn't be!
        log_write( MID_FATAL, "decode_grf",
                   "There is still something on the list\n" );
    }

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook GRF formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return grf_rc              Passed through from DECODE_GRF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_grf_start(
    char                        *   data_p
    )
{
    /**
     * @param grf_rc            Return Code                                 */
    int                             grf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    grf_rc = DECODE_GRF__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
#if 1   //  @ToDo   [DEBUG]     Disable GRF
    return( grf_rc );
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
 *  @return grf_rc              Passed through from DECODE_GRF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_grf_end(
    char                        *   data_p
    )
{
    /**
     * @param grf_rc            Return Code                                 */
    int                             grf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    grf_rc = DECODE_GRF__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( grf_rc );
}

/****************************************************************************/
