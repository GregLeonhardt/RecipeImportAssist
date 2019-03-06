/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_bof' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_BOF          ( "ALLOCATE STORAGE FOR DECODE_BOF" )

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
#include <decode_bof_api.h>     //  API for all decode_bof_*        PUBLIC
#include "decode_bof_lib.h"     //  API for all decode_bof__*       PRIVATE
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
decode_bof(
    struct  list_base_t         *   bof_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param state             Current decode state                        */
    enum    bof_decode_state_e      bof_state;
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
    log_write( MID_DEBUG_0, "decode_bof",
                  "Entering with %6d lines of text\n",
                  list_query_count( bof_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( bof_list_p );

    //  Change state to looking for the recipe title.
    bof_state = BOF_DS_START;

    /************************************************************************
     *  Copy e-Mail information
     ************************************************************************/

    //  Allocate a new recipe data structure
    recipe_p = recipe_new( RECIPE_FORMAT_BOF );

    //  Copy source information to the recipe structure
    decode_copy_info_to_recipe( recipe_p, source_info_p );

    /************************************************************************
     *  Function Body
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( bof_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( bof_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( bof_list_p, list_data_p, list_lock_key );

        //  Debug log output
        log_write( MID_DEBUG_0, "decode_bof",
                      "'%.60s'\n", list_data_p );

        /*********************************************************************
         *  Look for a segment change
         *********************************************************************/

        //  Locate the first character in the buffer
        tmp_data_p = text_skip_past_whitespace( list_data_p );

        //--------------------------------------------------------------------
        //  -= Exported from BigOven =- ?
        if( DECODE_BOF__start( list_data_p ) == true )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_START;
        }
        //--------------------------------------------------------------------
        //  Recipe By: ?
        if( strncmp( tmp_data_p, BOF_BY, BOF_BY_L  ) == 0 )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_BY;
        }
        //--------------------------------------------------------------------
        //  Serving Size: ?
        else
        if( strncmp( tmp_data_p, BOF_SERVING, BOF_SERVING_L  ) == 0 )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_SERVING;
        }
        //--------------------------------------------------------------------
        //  Cuisine: ?
        else
        if( strncmp( tmp_data_p, BOF_CUISINE, BOF_CUISINE_L  ) == 0 )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_CUISINE;
        }
        //--------------------------------------------------------------------
        //  Main Ingredient: ?
        else
        if( strncmp( tmp_data_p, BOF_MAIN, BOF_MAIN_L  ) == 0 )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_MAIN;
        }
        //--------------------------------------------------------------------
        //  Categories: ?
        else
        if( strncmp( tmp_data_p, BOF_CATEGORY, BOF_CATEGORY_L  ) == 0 )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_CATEGORY;
        }
        //--------------------------------------------------------------------
        //  -= Ingredients =- ?
        else
        if( strncmp( tmp_data_p, BOF_INGREDIENTS, BOF_INGREDIENTS_L  ) == 0 )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_INGREDIENTS;
        }
        //--------------------------------------------------------------------
        //  -= Instructions =- ?
        else
        if( strncmp( tmp_data_p, BOF_INSTRUCTIONS, BOF_INSTRUCTIONS_L  ) == 0 )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_INSTRUCTIONS;
        }
        //--------------------------------------------------------------------
        //  End-of-Recipe ?
        else
        if( DECODE_BOF__pre_end( tmp_data_p ) == true )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_END;
        }
        //--------------------------------------------------------------------
        //  End-of-Recipe ?
        else
        if( DECODE_BOF__end( tmp_data_p ) == true )
        {
            //  YES:    Change the decode state
            bof_state = BOF_DS_NEXT_SEGMENT;
        }
        //--------------------------------------------------------------------

        //  Process the new data
        switch( bof_state )
        {

            /****************************************************************
             *  Search for the Start of a recipe
             ****************************************************************/

            case BOF_DS_START:
            {
                //  Change recipe Decode State
                bof_state = BOF_DS_TITLE;

            }   break;

            /****************************************************************
             *  Process the Recipe Title
             ****************************************************************/

            case BOF_DS_TITLE:
            {
                //  Locate and process the recipe title
                if( DECODE_BOF__title( recipe_p, list_data_p ) == true )
                {
                    log_write( MID_INFO, "decode_bof",
                               "'%s'\n", recipe_p->name );

                    //  Change recipe Decode State
                    bof_state = BOF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  Process Recipe By:
             ****************************************************************/

            case BOF_DS_BY:
            {
                //  Locate and process the recipe title
                if( DECODE_BOF__recipe_by( recipe_p, list_data_p ) == true )
                {
                    //  Change recipe Decode State
                    bof_state = BOF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  Process Servings:
             ****************************************************************/

            case BOF_DS_SERVING:
            {
                //  Locate and process the recipe title
                if( DECODE_BOF__srv_size( recipe_p, list_data_p ) == true )
                {
                    //  Change recipe Decode State
                    bof_state = BOF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  Process the Cuisine:
             *              Main Ingredient:
             *              Categories: information
             ****************************************************************/

            case BOF_DS_CUISINE:
            case BOF_DS_MAIN:
            case BOF_DS_CATEGORY:
            {
                //  Locate and process the recipe title
                if( DECODE_BOF__categories( recipe_p, list_data_p ) == true )
                {
                    //  Change recipe Decode State
                    bof_state = BOF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  -= Ingredients =-
             ****************************************************************/

            case BOF_DS_INGREDIENTS:
            {
                //  Change recipe Decode State
                bof_state = BOF_DS_AUIP;

            }   break;

            /****************************************************************
             *  Process AUIP
             ****************************************************************/

            case BOF_DS_AUIP:
            {
                //  Locate and process the recipe title
                if( DECODE_BOF__auip( recipe_p, list_data_p ) != true )
                {
                    //  Change recipe Decode State
                    bof_state = BOF_DS_NEXT_SEGMENT;
                }
            }   break;

            /****************************************************************
             *  -= Instructions =-
             ****************************************************************/

            case BOF_DS_INSTRUCTIONS:
            {
                //  Change recipe Decode State
                bof_state = BOF_DS_DIRECTIONS;

            }   break;

            /****************************************************************
             *  Process Directions
             ****************************************************************/

            case BOF_DS_DIRECTIONS:
            {
                //  Locate and process the recipe title
                if( DECODE_BOF__directions( recipe_p, list_data_p ) != true )
                {
                    //  Change recipe Decode State
                    bof_state = BOF_DS_NEXT_SEGMENT;

                    //  There may be some data in the directions processing buffer.
                    //  This call will flush it out.
                    DECODE_BOF__directions( recipe_p, "   " );
                }
            }   break;

            /****************************************************************
             *  Process End-of-Recipe
             ****************************************************************/

            case BOF_DS_END:
            {
                //  There may be some data in the directions processing buffer.
                //  This call will flush it out.
                DECODE_BOF__directions( recipe_p, "   " );

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
    list_user_unlock( bof_list_p, list_lock_key );

    //  Is there anything still on the list ?
    if( list_query_count( bof_list_p ) != 0 )
    {
        //  YES:    There shouldn't be!
        log_write( MID_FATAL, "decode_bof",
                   "There is still something on the list\n" );
    }

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook BOF formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return bof_rc              Passed through from DECODE_BOF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_bof_start(
    char                        *   data_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    bof_rc = DECODE_BOF__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
#if 1   //  @ToDo   [DEBUG]     Disable BOF
    return( bof_rc );
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
 *  @return bof_rc              Passed through from DECODE_BOF__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_bof_end(
    char                        *   data_p
    )
{
    /**
     * @param bof_rc            Return Code                                 */
    int                             bof_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    bof_rc = DECODE_BOF__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( bof_rc );
}

/****************************************************************************/
