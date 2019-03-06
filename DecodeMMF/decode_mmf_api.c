/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_mmf' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_MMF          ( "ALLOCATE STORAGE FOR DECODE_MMF" )

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
#include <decode_mmf_api.h>     //  API for all decode_mmf_*        PUBLIC
#include "decode_mmf_lib.h"     //  API for all DECODE_MMF__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    mmf_decode_state_e
{
    MMF_DS_IDLE                 =   0,
    MMF_DS_TITLE                =   1,
    MMF_DS_CATEGORIES           =   2,
    MMF_DS_YIELD                =   3,
    MMF_DS_AUIP_0               =   4,
    MMF_DS_AUIP_1               =   5,
    MMF_DS_DIRECTIONS           =   6,
    MMF_DS_END                  =   7
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
decode_mmf(
    struct  list_base_t         *   mmf_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    mmf_decode_state_e      mmf_state;
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
    log_write( MID_DEBUG_0, "decode_mmf",
                  "Entering with %6d lines of text\n",
                  list_query_count( mmf_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( mmf_list_p );

    //  Change state to looking for the recipe title.
    mmf_state = MMF_DS_TITLE;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    recipe_p = recipe_new( RECIPE_FORMAT_MMF );

    //  Copy source information to the recipe structure
    decode_copy_info_to_recipe( recipe_p, source_info_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( mmf_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( mmf_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( mmf_list_p, list_data_p, list_lock_key );

        //  Debug log output
        log_write( MID_DEBUG_0, "decode_mmf",
                      "'%.60s'\n", list_data_p );

        //  Process the new data
        switch( mmf_state )
        {

            /****************************************************************
             *  Search for the recipe title
             ****************************************************************/

            case MMF_DS_TITLE:
            {
                //  Locate and process the recipe title
                if( DECODE_MMF__title( recipe_p, list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe categories.
                    mmf_state = MMF_DS_CATEGORIES;
                }
            }   break;

            /****************************************************************
             *  Search for recipe categories
             ****************************************************************/

            case MMF_DS_CATEGORIES:
            {
                //  Locate and process the recipe title
                if( DECODE_MMF__categories( recipe_p, list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe yield.
                    mmf_state = MMF_DS_YIELD;
                }
            }   break;

            /****************************************************************
             *  Search for recipe yield
             ****************************************************************/

            case MMF_DS_YIELD:
            {
                //  Locate and process the recipe title
                if( DECODE_MMF__yield( recipe_p, list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the AUIP section.
                    mmf_state = MMF_DS_AUIP_0;
                }
            }   break;

            /****************************************************************
             *  Search for the start of AUIP
             *      Amount, Unit, Ingredient, Preparation
             ****************************************************************/

            case MMF_DS_AUIP_0:
            {
                //  Change state to processing AUIP section.
                mmf_state = MMF_DS_AUIP_1;
            }   break;

            /****************************************************************
             *  Process AUIP
             ****************************************************************/

            case MMF_DS_AUIP_1:
            {
                //  Locate and process the recipe title
                if( DECODE_MMF__auip( recipe_p, list_data_p ) == true )
                {
                    log_write( MID_INFO, "decode_mmf",
                               "'%s'\n", recipe_p->name );

                    //  When rc == true, the title search is complete.
                    //  Change state to looking for the recipe directions.
                    mmf_state = MMF_DS_DIRECTIONS;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case MMF_DS_DIRECTIONS:
            {
                //  Locate and process the recipe title
                if( DECODE_MMF__directions( recipe_p, list_data_p ) == true )
                {
                    //  When rc == true, the title search is complete.
                    //  Change state to recipe decode complete.
                    mmf_state = MMF_DS_END;
                }
            }   break;

            /****************************************************************
             *  End of recipe decode.
             ****************************************************************/

            case MMF_DS_END:
            {
                //  There may be some data in the directions processing buffer.
                //  This call will flush it out.
                DECODE_MMF__directions( recipe_p, "   " );
            }   break;


            /****************************************************************
             *  Here to catch the unused case
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
    list_user_unlock( mmf_list_p, list_lock_key );

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook MMF formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mmf_rc              Passed through from DECODE_MMF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_mmf_start(
    char                        *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mmf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    mmf_rc = DECODE_MMF__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
#if 1   //  @ToDo   [DEBUG]     Disable MMF
    return( mmf_rc );
#else
    return( false );
#endif
}

/****************************************************************************/
/**
 *  Test the data for the end of a MasterCook MMF formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mmf_rc              Passed through from DECODE_MMF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_mmf_end(
    char                        *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mmf_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    mmf_rc = DECODE_MMF__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mmf_rc );
}

/****************************************************************************/
