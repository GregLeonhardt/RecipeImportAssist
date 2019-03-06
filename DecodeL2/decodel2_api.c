/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decodel2' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_RECIPE          ( "ALLOCATE STORAGE FOR RECIPE" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <ctype.h>              //  Determine the type contained
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <decode_api.h>         //  API for all decode_*            PUBLIC
#include <decode_bof_api.h>     //  API for all decode_bof_*        PUBLIC
#include <decode_grf_api.h>     //  API for all decode_grf_*        PUBLIC
#include <decode_mmf_api.h>     //  API for all decode_mmf_*        PUBLIC
#include <decode_mxp_api.h>     //  API for all decode_mxp_*        PUBLIC
#include <decode_mx2_api.h>     //  API for all decode_mx2_*        PUBLIC
#include <decode_nyc_api.h>     //  API for all decode_nyc_*        PUBLIC
#include <recipe_api.h>         //  API for all recipe_*            PUBLIC
                                //*******************************************
#include <decodel2_api.h>       //  API for all decodel2_*          PUBLIC
#include "decodel2_lib.h"       //  API for all DECODEL2__*         PRIVATE
                                //*******************************************

/****************************************************************************
 * API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
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
 *  When this function is called a file is being scanned.  When we get to
 *  to this point everything being passed in should be from a single e-Mail
 *  message or a single archive file.  It is still unknown if it is a
 *  complete file with formatted recipes or an e-mail with none to some
 *  number of recipes embedded in it.
 *
 *  @param  level1_list_p       Pointer to a list where the file is stored
 *  @param  source_info_p       Pointer to source data information
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decodel2_parse(
    struct  list_base_t         *   level2_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     *  @param list_lock_key    File list key                               */
    int                             list_lock_key;
    /**
     *  @param list_data_p      Pointer to the read data                    */
    char                        *   list_data_p;
    /**
     *  @param recipe_format    The recipe format being decoded             */
    enum    recipe_format_e         recipe_format;
    /**
     *  @param file_ll_p        Pointer to a list where the file is stored  */
    struct  list_base_t         *   level3_list_p;
    /**
     *  @param  l3_data_p       Pointer to level 3 list data                */
    char                        *   l3_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decodel2_parse",
            "Entering with %6d lines of text\n",
                  list_query_count( level2_list_p ) );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( level2_list_p );

    //  Initialize the current recipe format
    recipe_format = RECIPE_FORMAT_NONE;

    //  Create a new list
    level3_list_p = list_new( );

    log_write( MID_DEBUG_0, "decodel2_parse",
            "Allocate a new list structure 'level3_list_p' [%p].\n",
                  level3_list_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_fget_first( level2_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( level2_list_p, list_data_p, list_lock_key ) )
    {

        /********************************************************************
         *  Possible e-Mail Subject
         ********************************************************************/

        //  Is this the first line of a new e-Mail ?
        if( list_query_count( level3_list_p ) == 1 )
        {
            /**
             *  @param subject          This is possibly a subject          */
            int                             subject;

            //  YES:    (0-9).
            if(    ( isdigit( list_data_p[ 0 ] ) !=  0  )
                && (          list_data_p[ 1 ]   == '.' )
                && (          list_data_p[ 2 ]   == ' ' ) )
            {
                subject = 3;
            }
            else    //  (0-9)(a-z).
            if(    ( isdigit( list_data_p[ 0 ] ) !=  0  )
                && ( isalpha( list_data_p[ 1 ] ) !=  0  )
                && (          list_data_p[ 2 ]   == '.' )
                && (          list_data_p[ 3 ]   == ' ' ) )
            {
                subject = 4;
            }
            else    //  (0-9)(0-9).
            if(    ( isdigit( list_data_p[ 0 ] ) !=  0  )
                && ( isdigit( list_data_p[ 1 ] ) !=  0  )
                && (          list_data_p[ 2 ]   == '.' )
                && (          list_data_p[ 3 ]   == ' ' ) )
            {
                subject = 4;
            }
            else    //  (0-9)(0-9)(a-z).
            if(    ( isdigit( list_data_p[ 0 ] ) !=  0  )
                && ( isdigit( list_data_p[ 1 ] ) !=  0  )
                && ( isalpha( list_data_p[ 2 ] ) !=  0  )
                && (          list_data_p[ 3 ]   == '.' )
                && (          list_data_p[ 4 ]   == ' ' ) )
            {
                subject = 5;
            }
            else    //  (0-9).(a-z).
            if(    ( isdigit( list_data_p[ 0 ] ) !=  0  )
                && (          list_data_p[ 1 ]   == '.' )
                && ( isdigit( list_data_p[ 2 ] ) !=  0  )
                && (          list_data_p[ 3 ]   == '.' )
                && (          list_data_p[ 4 ]   == ' ' ) )
            {
                subject = 5;
            }
            else    //  NOT a possible subject
            {
                subject = 0;
            }

            //  Did we find a subject ?
            if( subject != 0 )
            {
                //  Save the e-Mail 'Subject' data
                memset( source_info_p->e_subject, '\0',
                        sizeof( source_info_p->e_subject ) );
                strncpy( source_info_p->e_subject,
                         &(list_data_p[ subject ] ),
                         SUBJECT_L );
            }
        }

        /********************************************************************
         *  Look for a recipe break
         ********************************************************************/

        //  Do we need to process an existing recipe ?
        if(    ( list_query_count( level3_list_p )    >    0 )
            && ( decode_end_of_recipe( list_data_p ) == true ) )
        {
            //  YES:    Go do it.
            decode_xxx( recipe_format, level3_list_p, source_info_p );

            //  Reset the recipe format.
            recipe_format = RECIPE_FORMAT_NONE;

            if( list_query_count( level3_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel2_parse",
                           "There is still something on the list\n" );
            }
        }

        /********************************************************************
         *  Look for the start of a new recipe
         ********************************************************************/

        //  MasterCook MXP
        if(    ( recipe_format == RECIPE_FORMAT_NONE )
            && ( decode_mxp_start( list_data_p ) == true ) )
        {
            //  YES:    Set the format to use
            recipe_format = RECIPE_FORMAT_MXP;
        }
        else
            //  Meal-Master
        if(    ( recipe_format == RECIPE_FORMAT_NONE )
            && ( decode_mmf_start( list_data_p ) == true ) )
        {
            //  YES:    Set the format to use
            recipe_format = RECIPE_FORMAT_MMF;
        }
        else
            //  MasterCook MX2
        if(    ( recipe_format == RECIPE_FORMAT_NONE )
            && ( decode_mx2_start( list_data_p ) == true ) )
        {
            //  YES:    Set the format to use
            recipe_format = RECIPE_FORMAT_MX2;
        }
        else
            //  Big-Oven BOF
        if(    ( recipe_format == RECIPE_FORMAT_NONE )
            && ( decode_bof_start( list_data_p ) == true ) )
        {
            //  YES:    Set the format to use
            recipe_format = RECIPE_FORMAT_BOF;
        }
        else
            //  Now You're Cooking!
        if(    ( recipe_format == RECIPE_FORMAT_NONE )
            && ( decode_nyc_start( list_data_p ) == true ) )
        {
            //  YES:    Set the format to use
            recipe_format = RECIPE_FORMAT_NYC;
        }
        else
            //  Generic-Recipe-Format [[[[[
        if(    ( recipe_format == RECIPE_FORMAT_NONE )
            && ( decode_grf_start( list_data_p ) == true ) )
        {
            //  YES:    Set the format to use
            recipe_format = RECIPE_FORMAT_GRF;
        }

        /********************************************************************
         *  Do something with the current line
         ********************************************************************/

        //  Remove it from the level 2 list
        list_fdelete( level2_list_p, list_data_p, list_lock_key );

        //  Move the data to the level 3 list
        list_put_last( level3_list_p, list_data_p );

        /********************************************************************
         *  Look for the end of the current recipe
         ********************************************************************/

        //--------------------------------------------------------------------
        //  MasterCook MXP
        if(    ( recipe_format == RECIPE_FORMAT_MXP )
            && ( decode_mxp_end( list_data_p ) == true ) )
        {
            //  YES:    Process the recipe
            decode_xxx( recipe_format, level3_list_p, source_info_p );

            //  Done with this recipe
            recipe_format = RECIPE_FORMAT_NONE;

            if( list_query_count( level3_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel2_parse",
                           "There is still something on the list\n" );
            }
        }
        //--------------------------------------------------------------------
        //  MealMaster MMF
        if(    ( recipe_format == RECIPE_FORMAT_MMF )
            && ( decode_mmf_end( list_data_p ) == true ) )
        {
            //  YES:    Process the recipe
            decode_xxx( recipe_format, level3_list_p, source_info_p );

            //  Done with this recipe
            recipe_format = RECIPE_FORMAT_NONE;

            if( list_query_count( level3_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel2_parse",
                           "There is still something on the list\n" );
            }
        }
        //--------------------------------------------------------------------
        //  MasterCook MX2
        if(    ( recipe_format == RECIPE_FORMAT_MX2 )
            && ( decode_mx2_end( list_data_p ) == true ) )
        {
            //  YES:    Process the recipe
            decode_xxx( recipe_format, level3_list_p, source_info_p );

            //  Done with this recipe
            recipe_format = RECIPE_FORMAT_NONE;

            if( list_query_count( level3_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel2_parse",
                           "There is still something on the list\n" );
            }
        }
        //--------------------------------------------------------------------
        //  Big-Oven BOF
        if(    ( recipe_format == RECIPE_FORMAT_BOF )
            && ( decode_bof_end( list_data_p ) == true ) )
        {
            //  YES:    Process the recipe
            decode_xxx( recipe_format, level3_list_p, source_info_p );

            //  Done with this recipe
            recipe_format = RECIPE_FORMAT_NONE;

            if( list_query_count( level3_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel2_parse",
                           "There is still something on the list\n" );
            }
        }
        //--------------------------------------------------------------------
        //  Now You're Cooking!
        if(    ( recipe_format == RECIPE_FORMAT_NYC )
            && ( decode_nyc_end( list_data_p ) == true ) )
        {
            //  YES:    Process the recipe
            decode_xxx( recipe_format, level3_list_p, source_info_p );

            //  Done with this recipe
            recipe_format = RECIPE_FORMAT_NONE;

            if( list_query_count( level3_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel2_parse",
                           "There is still something on the list\n" );
            }
        }
        //--------------------------------------------------------------------
        //  Generic-Recipe-Format
        if(    ( recipe_format == RECIPE_FORMAT_GRF )
            && ( decode_grf_end( list_data_p ) == true ) )
        {
            //  YES:    Process the recipe
            decode_xxx( recipe_format, level3_list_p, source_info_p );

            //  Done with this recipe
            recipe_format = RECIPE_FORMAT_NONE;

            if( list_query_count( level3_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel2_parse",
                           "There is still something on the list\n" );
            }
        }
        //--------------------------------------------------------------------
    }

    //  Is there anything still on the list ?
    if( list_query_count( level3_list_p ) > 0 )
    {
        //  @note   This can be caused by finding a recipe start but the
        //          recipe terminator marker is missing.  THIS HAPPENS A LOT!

        //  YES:    See if it can be decoded.
        decode_xxx( recipe_format, level3_list_p, source_info_p );

        if( list_query_count( level3_list_p ) != 0 )
        {
            log_write( MID_FATAL, "decodel2_parse",
                       "There is still something on the list\n" );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Is there anything on the list ?
    while( ( l3_data_p = list_get_first( level3_list_p ) ) != NULL )
    {
        //  YES:    Purge it.
        log_write( MID_WARNING, "decodel2_parse",
                      "Purging undecoded data: '%s'\n", l3_data_p );
        list_delete( level3_list_p, l3_data_p );

        //  And free the memory buffer
        mem_free( l3_data_p );
    }

    //  All done, delete the list
    if( list_kill( level3_list_p ) != true )
    {
        list_debug_dump_list( level3_list_p );
        log_write( MID_FATAL, "decodel2_parse",
                      "list_kill( level3_list_p ) failed\n" );
    }
    else
    {
        log_write( MID_DEBUG_0, "decodel2_parse",
                "list_kill( [%p] level3_list_p ) complete.\n",
                      level3_list_p );
    }

    //  Unlock the list
    list_user_unlock( level2_list_p, list_lock_key );

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decodel2_parse",
            "Exiting  with %6d lines of text\n",
                  list_query_count( level2_list_p ) );

    //  Did we process everything in the list ?
    if( list_query_count( level2_list_p ) != 0 )
    {
        //  Log what we found
        log_write( MID_FATAL, "decodel2_parse",
                      "level2_list_p is not empty!.\n" );
    }
}

/****************************************************************************/
