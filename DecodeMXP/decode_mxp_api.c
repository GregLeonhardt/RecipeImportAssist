/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_mxp' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_MXP          ( "ALLOCATE STORAGE FOR DECODE_MXP" )

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
#include <decode_mxp_api.h>     //  API for all decode_mxp_*        PUBLIC
#include "decode_mxp_lib.h"     //  API for all DECODE_MXP__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    mxp_state_e
{
    MXP_STATE_IDLE              =    0,
    MXP_STATE_TITLE             =    1,
    MXP_STATE_RECIPE_BY         =    2,
    MXP_STATE_SRV_SIZE          =    3,
    MXP_STATE_PREP_TIME         =    4,
    MXP_STATE_CATEGORIES        =    5,
    MXP_STATE_AUIP              =    6,
    MXP_STATE_DIRECTIONS        =    7,
    MXP_STATE_NOTES             =    8,
    MXP_STATE_END               =    9,
    MXP_STATE_COMPLETE          =   10
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Manage the MasterCook MXP state decode.
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_mxp(
    struct  list_base_t         *   mxp_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param state             Current decode state                        */
    enum    mxp_state_e             mxp_state;
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
    log_write( MID_DEBUG_0, "decode_mxp",
                  "Entering with %6d lines of text\n",
                  list_query_count( mxp_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( mxp_list_p );

    //  Set the initial decode state
    mxp_state = MXP_STATE_IDLE;

    //  Set the initial note state
    note_state = MXP_NS_IDLE;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    recipe_p = recipe_new( RECIPE_FORMAT_MXP );

    //  Copy source information to the recipe structure
    decode_copy_info_to_recipe( recipe_p, source_info_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( mxp_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( mxp_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( mxp_list_p, list_data_p, list_lock_key );

        //  Debug log output
        log_write( MID_DEBUG_0, "decode_mxp",
                      "'%.60s'\n", list_data_p );

        switch( mxp_state )
        {
            //----------------------------------------------------------------
            case MXP_STATE_IDLE:
            {
                //  Is it the start of a new recipe ?
                if ( ( mxp_rc = DECODE_MXP__start( list_data_p ) ) == true )
                {
                    //  YES:    Change the active state.
                    mxp_state = MXP_STATE_TITLE;
                }
            }   break;
            //----------------------------------------------------------------
            case MXP_STATE_TITLE:
            {
                // Locate and process the recipe title
                if ( DECODE_MXP__title( list_data_p, recipe_p ) == true )
                {
                    //  Change to Title Case
                    text_title_case( recipe_p->name, recipe_p->name );

                    //  Log the new title
                    log_write( MID_INFO, "decode_mxp",
                                  "'%s'\n", recipe_p->name );

                    // When rc == TRUE, the title search is complete.
                    // Change state to looking for the recipe by.
                    mxp_state = MXP_STATE_RECIPE_BY;
                }
            }   break;
            //----------------------------------------------------------------
            case MXP_STATE_RECIPE_BY:
            {
                // Locate and process the recipe by
                if ( DECODE_MXP__recipe_by( list_data_p, recipe_p ) == true )
                {
                    log_write( MID_DEBUG_0, "decode_mxp",
                                  "recipe_p->author: '%s'\n", recipe_p->author );
                    // When rc = TRUE, the recipe by search is complete.
                    // Change state to looking for the serving size section.
                    mxp_state = MXP_STATE_SRV_SIZE;
                }
            }   break;
            //----------------------------------------------------------------
            case MXP_STATE_SRV_SIZE:
            {
                // Locate and process the Serving Size
                if ( DECODE_MXP__srv_size( list_data_p, recipe_p ) == true )
                {
                    log_write( MID_DEBUG_0, "decode_mxp",
                                  "recipe_p->serves: '%s'\n", recipe_p->serves );
                    // When rc == TRUE, the serving size search is complete.
                    // Change state to looking for the preparation time section.
                    mxp_state = MXP_STATE_PREP_TIME;
                }
            }   //  Fall through is intentional
            //----------------------------------------------------------------
            case MXP_STATE_PREP_TIME:
            {
                // Locate and process the Serving Size
                if ( DECODE_MXP__prep_time( list_data_p, recipe_p ) == true )
                {
                    log_write( MID_DEBUG_0, "decode_mxp",
                                  "recipe_p->time_prep: '%s'\n", recipe_p->time_prep );
                    // When rc == TRUE, the preparation time search is complete.
                    // Change state to looking for the categories section.
                    mxp_state = MXP_STATE_CATEGORIES;
                }
            }   break;
            //----------------------------------------------------------------
            case MXP_STATE_CATEGORIES:
            {
                // Locate and process the recipe categories
                if ( DECODE_MXP__categories( list_data_p, recipe_p ) == true )
                {
                    // When rc == TRUE, the categories search is complete.
                    // Change state to looking for the auip section.
                    mxp_state = MXP_STATE_AUIP;
                }
            }   break;
            //----------------------------------------------------------------
            case MXP_STATE_AUIP:
            {
                // Locate and process the recipe AUIP
                if ( DECODE_MXP__auip( list_data_p, recipe_p ) == true )
                {
                    // When rc == TRUE, the AUIP search is complete.
                    // Change state to looking for the recipe directions.
                    mxp_state = MXP_STATE_DIRECTIONS;
                }
            }   break;
            //----------------------------------------------------------------
            case MXP_STATE_DIRECTIONS:
            {
                // Locate and process the recipe title
                if ( DECODE_MXP__directions( list_data_p, recipe_p ) == true )
                {
                    // When rc == TRUE, the directions search is complete.
                    // Change state to recipe decode complete.
                    mxp_state = MXP_STATE_NOTES;
                }
            }   break;
            //----------------------------------------------------------------
            case MXP_STATE_NOTES:
            {
                // Locate and process the recipe title
                if ( DECODE_MXP__notes( list_data_p, recipe_p ) == true )
                {
                    // When rc == TRUE, the title search is complete.
                    // Change state to recipe decode complete.
                    mxp_state = MXP_STATE_END;
                }
            }   break;
            //----------------------------------------------------------------
            case MXP_STATE_END:
            {
                // There may be some data in the directions processing buffer.
                // This call will flush it out.
                DECODE_MXP__directions( list_data_p, recipe_p );

                // Change state to looking for the recipe title.
                mxp_state = MXP_STATE_COMPLETE;
            }   break;
            //----------------------------------------------------------------
            default:
            {
                // Just here to catch the unused case
            }
            //----------------------------------------------------------------
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

    log_write( MID_DEBUG_0, "decode_mxp",
                  "Exit:\n" );

    //  Release the lock on the level 3 list
    list_user_unlock( mxp_list_p, list_lock_key );
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook MXP formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mxp_rc              Passed through from DECODE_MXP__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_mxp_start(
    char                        *   data_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the internal function
    mxp_rc = DECODE_MXP__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
#if 1   //  @ToDo   [DEBUG]     Disable MXP
    return( mxp_rc );
#else
    return( false );
#endif
}

/****************************************************************************/
/**
 *  Test the data for the end of a MasterCook MXP formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mxp_rc              Passed through from DECODE_MXP__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_mxp_end(
    char                        *   data_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the internal function
    mxp_rc = DECODE_MXP__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
