/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_nyc' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_NYC          ( "ALLOCATE STORAGE FOR DECODE_NYC" )

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
#include <decode_nyc_api.h>     //  API for all decode_nyc_*        PUBLIC
#include "decode_nyc_lib.h"     //  API for all DECODE_NYC__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    nyc_decode_state_e
{
    NYC_DS_IDLE                 =   0,
    NYC_DS_START                =   1,
    NYC_DS_TITLE                =   2,
    NYC_DS_BLANK_1              =   3,
    NYC_DS_CATEGORIES           =   4,
    NYC_DS_AUIP                 =   5,
    NYC_DS_DIRECTIONS           =   6,
    NYC_DS_END                  =   7
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
decode_nyc(
    struct  list_base_t         *   nyc_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    nyc_decode_state_e      nyc_state;
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
    log_write( MID_DEBUG_0, "decode_nyc",
                  "Entering with %6d lines of text\n",
                  list_query_count( nyc_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( nyc_list_p );

    //  Change state to looking for the recipe title.
    nyc_state = NYC_DS_START;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    recipe_p = recipe_new( RECIPE_FORMAT_NYC );

    //  Copy source information to the recipe structure
    decode_copy_info_to_recipe( recipe_p, source_info_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( nyc_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( nyc_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( nyc_list_p, list_data_p, list_lock_key );

        //  Process the new data
        switch( nyc_state )
        {

            /****************************************************************
             *  Recipe Start Marker
             ****************************************************************/

            case NYC_DS_START:
            {
                //  Locate and process the recipe title
                if ( decode_nyc_start( list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe categories.
                    nyc_state = NYC_DS_TITLE;
                }
            }   break;

            /****************************************************************
             *  Search for the recipe title
             ****************************************************************/

            case NYC_DS_TITLE:
            {
                //  Locate and process the recipe title
                if ( DECODE_NYC__title( recipe_p, list_data_p ) == true )
                {
                    //  Change to Title Case
                    text_title_case( recipe_p->name, recipe_p->name );

                    //  Log the new title
                    log_write( MID_INFO, "decode_nyc",
                               "'%s'\n", recipe_p->name );

                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe categories.
                    nyc_state = NYC_DS_BLANK_1;
                }
            }   break;

            /****************************************************************
             *  Should be a blank line
             ****************************************************************/

            case NYC_DS_BLANK_1:
            {
                //  Locate and process the recipe title
                if ( text_is_blank_line( list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe categories.
                    nyc_state = NYC_DS_CATEGORIES;
                }
            }   break;

            /****************************************************************
             *  Search for recipe categories
             ****************************************************************/

            case NYC_DS_CATEGORIES:
            {
                //  Locate and process the recipe title
                if ( DECODE_NYC__categories( recipe_p, list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe yield.
                    nyc_state = NYC_DS_AUIP;
                }
            }   break;

            /****************************************************************
             *  Process AUIP
             ****************************************************************/

            case NYC_DS_AUIP:
            {
                //  Locate and process the recipe title
                if ( DECODE_NYC__auip( recipe_p, list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe directions.
                    nyc_state = NYC_DS_DIRECTIONS;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case NYC_DS_DIRECTIONS:
            {
                //  Locate and process the recipe title
                if ( DECODE_NYC__directions( recipe_p, list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to recipe decode complete.
                    nyc_state = NYC_DS_END;
                }
            }   break;

            /****************************************************************
             *  End of recipe decode.
             ****************************************************************/

            case NYC_DS_END:
            {
                //  There may be some data in the directions processing buffer.
                //  This call will flush it out.
                DECODE_NYC__directions( recipe_p, "   " );
            }   break;

            /****************************************************************
             *  Just here to catch the unused case types
             ****************************************************************/

            default:
            {
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
    list_user_unlock( nyc_list_p, list_lock_key );

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook NYC formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return nyc_rc              Passed through from DECODE_NYC__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_nyc_start(
    char                        *   data_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    nyc_rc = DECODE_NYC__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
#if 1   //  @ToDo   [DEBUG]     Disable NYC
    return( nyc_rc );
#else
    return( false );
#endif
}

/****************************************************************************/
/**
 *  Test the data for the end of a MasterCook NYC formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return nyc_rc              Passed through from DECODE_NYC__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_nyc_end(
    char                        *   data_p
    )
{
    /**
     * @param nyc_rc            Return Code                                 */
    int                             nyc_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    nyc_rc = DECODE_NYC__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( nyc_rc );
}

/****************************************************************************/
