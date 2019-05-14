/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_erd' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_ERD          ( "ALLOCATE STORAGE FOR DECODE_ERD" )
#define _GNU_SOURCE

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
#include <decode_api.h>         //  API for all decode_*            PUBLIC
                                //*******************************************
#include <decode_erd_api.h>     //  API for all decode_erd_*        PUBLIC
#include "decode_erd_lib.h"     //  API for all DECODE_ERD__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    erd_decode_state_e
{
    ERD_DS_SCAN             =    0,     //  Scanning for a new tag
    ERD_DS_TITLE            =    1,     //  Title:
    ERD_DS_RECIPE_BY        =    2,     //  Recipe By:
    ERD_DS_CATEGORY         =    3,     //  Category:
    ERD_DS_MAIN_INGRED      =    4,     //  Main Ingredient:
    ERD_DS_QUISINE          =    5,     //  Cuisine Style:
    ERD_DS_YIELD            =    6,     //  Yield:
    ERD_DS_PREP_TIME        =    7,     //  Preparation Time:
    ERD_DS_COOK_TIME        =    8,     //  Cooking Time:
    ERD_DS_AUIP             =    9,     //  [Amount]+[Measure]+[Ingredient (or Header)]+[Preparation]
    ERD_DS_DIRECTIONS       =   10,     //  [Preparation]
    ERD_DS_END              =   99,     //  END-OF-STATE-LIST
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
#define DECODE_BUF_L              ( 1048576 )
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
decode_erd(
    struct  list_base_t         *   erd_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    erd_decode_state_e      erd_state;
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
    log_write( MID_DEBUG_0, "decode_erd",
                  "Entering with %6d lines of text\n",
                  list_query_count( erd_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( erd_list_p );

    //  Change state to looking for the recipe title.
    erd_state = ERD_DS_TITLE;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    recipe_p = recipe_new( RECIPE_FORMAT_ERD );

    //  Copy source information to the recipe structure
    decode_copy_info_to_recipe( recipe_p, source_info_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( erd_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( erd_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( erd_list_p, list_data_p, list_lock_key );

        //  Debug log output
        log_write( MID_DEBUG_0, "decode_erd",
                      "'%.60s'\n", list_data_p );

        //  Process the new data
        switch( erd_state )
        {

            /****************************************************************
             *  Search for and process the 'Title:' tag.
             ****************************************************************/

            case ERD_DS_TITLE:
            {
                //  Locate and process the recipe title
                if ( DECODE_ERD__title( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change to Title Case
                    text_title_case( recipe_p->name, recipe_p->name );

                    //  Log the new title
                    log_write( MID_INFO, "decode_erd",
                               "'%s'\n", recipe_p->name );

                    //  Change the decode state.
                    erd_state = ERD_DS_RECIPE_BY;
                }
            }   break;

            /****************************************************************
             *  Search for and process the 'Recipe By:' tag.
             ****************************************************************/

            case ERD_DS_RECIPE_BY:
            {
                //  Locate and process the recipe title
                if ( DECODE_ERD__recipe_by( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_CATEGORY;
                }
            }   break;

            /****************************************************************
             *  Search for and process the 'Category:' tag.
             *  Search for and process the 'Cuisine Style:' tag.
             ****************************************************************/

            case ERD_DS_CATEGORY:
            {
                //  Locate and process the recipe title
                if ( DECODE_ERD__category( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_MAIN_INGRED;
                }
            }   break;

            /****************************************************************
             *  Search for and process the 'Main Ingredient:' tag.
             ****************************************************************/

            {
            case ERD_DS_MAIN_INGRED:
                //  Locate and process the recipe title
                if ( DECODE_ERD__main_ingred( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_QUISINE;
                }
            }   break;

            /****************************************************************
             *  Search for and process the 'Cuisine Style:' tag.
             ****************************************************************/

            {
            case ERD_DS_QUISINE:
                //  Locate and process the recipe title
                if ( DECODE_ERD__quisine( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_YIELD;
                }
            }   break;

            /****************************************************************
             *  Search for and process the 'Yield:' tag.
             ****************************************************************/

            case ERD_DS_YIELD:
            {
                //  Locate and process the recipe title
                if ( DECODE_ERD__yield( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_PREP_TIME;
                }
            }   break;

            /****************************************************************
             *  Search for and process the 'Preparation Time:' tag.
             ****************************************************************/

            case ERD_DS_PREP_TIME:
            {
                //  Locate and process the recipe title
                if ( DECODE_ERD__prep_time( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_COOK_TIME;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case ERD_DS_COOK_TIME:
            {
                //  Locate and process the recipe title
                if ( DECODE_ERD__cook_time( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_AUIP;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case ERD_DS_AUIP:
            {
                //  Locate and process the recipe title
                if ( DECODE_ERD__auip( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_DIRECTIONS;
                }
            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case ERD_DS_DIRECTIONS:
            {
                //  Locate and process the recipe title
                if ( DECODE_ERD__directions( recipe_p, list_data_p ) == true )
                {
                    //  YES:    Change the decode state.
                    erd_state = ERD_DS_END;
                }
            }   break;

            /****************************************************************
             *  End of recipe decode.
             ****************************************************************/

            case ERD_DS_END:
            {
                //  There may be some data in the directions processing buffer.
                //  This call will flush it out.
                DECODE_ERD__directions( recipe_p, "   " );
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
    list_user_unlock( erd_list_p, list_lock_key );

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook ERD formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return erd_rc              Passed through from DECODE_ERD__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_erd_start(
    char                        *   data_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    erd_rc = DECODE_ERD__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( erd_rc );
}

/****************************************************************************/
/**
 *  Test the data for the end of a MasterCook ERD formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return erd_rc              Passed through from DECODE_ERD__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_erd_end(
    char                        *   data_p
    )
{
    /**
     * @param erd_rc            Return Code                                 */
    int                             erd_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    erd_rc = DECODE_ERD__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( erd_rc );
}

/****************************************************************************/
