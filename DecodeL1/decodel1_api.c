/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decodel1' library.
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
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <decode_api.h>         //  API for all decode_*            PUBLIC
#include <decodel2_api.h>       //  API for all decodel2_*          PUBLIC
#include <email_api.h>          //  API for all email_*             PUBLIC
                                //*******************************************
#include "decodel1_lib.h"       //  API for all DECODEL1__*         PRIVATE
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
 *  When this function is called a file is being scanned.  It is unknown at
 *  this point if it is a complete file with formatted recipes or an e-mail
 *  with none to some number of recipes embedded in it.
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
decodel1_parse(
    struct  list_base_t         *   level1_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param list_lock_key     File list key                               */
    int                             list_lock_key;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                        *   list_data_p;
    /**
     * @param list_data_p       Pointer to temporary data                   */
    char                        *   tmp_data_p;
    /**
     * @param email_flag        A mark on the wall if wr are doing an e-Mail*/
    int                             email_flag;
    /**
     * @param file_ll_p         Pointer to the level 2 list                 */
    struct  list_base_t         *   level2_list_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decodel1_parse",
                  "Entering with %6d lines of text\n",
                  list_query_count( level1_list_p ) );

    //  By default we are NOT doing e-Mail.
    email_flag = false;

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( level1_list_p );

    //  Create the level 2 list
    level2_list_p = list_new( );

    log_write( MID_DEBUG_0, "decodel1_parse",
                  "Allocate a new list structure 'level2_list_p' [%p].\n",
                  level2_list_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the start of a new e-Mail ?
    list_data_p = list_fget_first( level1_list_p, list_lock_key );

    //  YES:    Log what we found
    log_write( MID_DEBUG_0, "decodel1_parse",
                  "'%s'\n", list_data_p );

    if ( email_is_start( list_data_p ) == true )
    {
        //  YES:    Set a flag so we can track it.
        email_flag = true;

        log_write( MID_DEBUG_0, "decodel1_parse",
                      "Start      %p - '%.80s'\n", list_data_p, list_data_p );

        //  Done with this text line
        list_fdelete( level1_list_p, list_data_p, list_lock_key );
        mem_free( list_data_p );
    }

    //  Scan the list
    for( list_data_p = list_fget_first( level1_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( level1_list_p, list_data_p, list_lock_key ) )
    {

        /********************************************************************/
        //  Mailing-List: list MC_recipes@onelist.com; contact MC_recipes-owner@onelist.com
        //  Sender: bread-bakers-errors@lists.best.com

        //  Did we locate e-Mail source data ?
        if (    ( email_flag                                        == true )
             && ( ( tmp_data_p = email_find_source( list_data_p ) ) != NULL )
             && ( strlen( source_info_p->g_from )                   ==    0 ) )
        {
            //  YES:    Log what we found
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "Source:    '%.60s'\n", tmp_data_p );

            //  Will the e-Mail source information fit in the buffer ?
            if ( sizeof( source_info_p->g_from ) > strlen( tmp_data_p ) )
            {
                //  YES:    Overwrite the e-Mail 'source' data
                memset( source_info_p->g_from, '\0',
                        sizeof( source_info_p->g_from ) );
                strncpy( source_info_p->g_from, tmp_data_p, SOURCE_L );

                //  Done with this text line
                list_fdelete( level1_list_p, list_data_p, list_lock_key );

                //  Add the line to the level 2 list
                list_put_last( level2_list_p, list_data_p );
            }
        }

        /********************************************************************/
        //  From: 'Jane Doe' jane.doe@server.mail (Jane Doe)

        //  Did we locate e-Mail from data ?
        else
        if (    ( email_flag                                      == true )
             && ( ( tmp_data_p = email_find_from( list_data_p ) ) != NULL )
             && ( strlen( source_info_p->e_from )                 ==    0 ) )
        {
            //  YES:    Log what we found
                log_write( MID_DEBUG_0, "decodel1_parse",
                              "From:      '%.60s'\n", tmp_data_p );

            //  Will the e-Mail from information fit in the buffer ?
            if ( sizeof( source_info_p->e_from ) > strlen( tmp_data_p ) )
            {
                //  YES:    Overwrite the e-Mail 'from' data
                memset( source_info_p->e_from, '\0',
                        sizeof( source_info_p->e_from ) );
                strncpy( source_info_p->e_from, tmp_data_p, FROM_L );

                //  Done with this text line
                list_fdelete( level1_list_p, list_data_p, list_lock_key );

                //  Add the line to the level 2 list
                list_put_last( level2_list_p, list_data_p );
            }
        }

        /********************************************************************/
        //  Date: Tue, 7 Mar 2000 10:18:45 -0800 (PST)

        //  Did we locate e-Mail from data ?
        else
        if (    ( email_flag                                           == true )
             && ( ( tmp_data_p = email_find_datetime( list_data_p ) )  != NULL ) )
        {
            //  YES:    Log what we found
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "DataTime   '%.60s'\n", tmp_data_p );

            //  Is there something is the group date/time field ?
            if ( strlen( source_info_p->g_datetime ) == 0 )
            {
                //  NO:     Will the e-Mail date/time information fit in the buffer ?
                if ( sizeof( source_info_p->g_datetime ) > strlen( tmp_data_p ) )
                {
                    //  YES:    Write the e-Mail 'group date/time' data
                    memset( source_info_p->g_datetime, '\0',
                            sizeof( source_info_p->g_datetime ) );
                    strncpy( source_info_p->g_datetime, tmp_data_p, DATETIME_L );
                }
            }
            //  Is there something is the subject field ?
            else
            if ( strlen( source_info_p->e_datetime ) == 0 )
            {
                //  NO:     Will the e-Mail date/time information fit in the buffer ?
                if ( sizeof( source_info_p->g_datetime ) > strlen( tmp_data_p ) )
                {
                    //  YES:    Write the e-Mail 'date/time' data
                    memset( source_info_p->e_datetime, '\0',
                            sizeof( source_info_p->e_datetime ) );
                    strncpy( source_info_p->e_datetime, tmp_data_p, DATETIME_L );
                }
            }

            //  Done with this text line
            list_fdelete( level1_list_p, list_data_p, list_lock_key );

            //  Add the line to the level 2 list
            list_put_last( level2_list_p, list_data_p );
        }

        /********************************************************************/
        //  Subject: Crockpot Cherry Pie Filling

        //  Did we locate the e-Mail subject ?
        else
        if (    ( email_flag                                         == true )
             && ( ( tmp_data_p = email_find_subject( list_data_p ) ) != NULL ) )
        {
            //  YES:    Log what we found
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "Subject:   '%.60s'\n", tmp_data_p );

            //  Is there something is the group subject field ?
            if ( strlen( source_info_p->g_subject ) == 0 )
            {
                //  NO:     Will the e-Mail subject information fit in the buffer ?
                if ( sizeof( source_info_p->g_subject ) > strlen( tmp_data_p ) )
                {
                    //  YES:    Write the e-Mail group subject data
                    memset( source_info_p->g_subject, '\0',
                            sizeof( source_info_p->g_subject ) );
                    strncpy( source_info_p->g_subject, tmp_data_p, SUBJECT_L );
                }
            }
            //  Is there something is the subject field ?
            else
            if ( strlen( source_info_p->e_subject ) == 0 )
            {
                //  YES:    Will the e-Mail subject information fit in the buffer ?
                if ( sizeof( source_info_p->e_subject ) > strlen( tmp_data_p ) )
                {
                    //  YES:    Write the e-Mail subject data
                    memset( source_info_p->e_subject, '\0',
                            sizeof( source_info_p->e_subject ) );
                    strncpy( source_info_p->e_subject, tmp_data_p, SUBJECT_L );
                }
            }

            //  Done with this text line
            list_fdelete( level1_list_p, list_data_p, list_lock_key );

            //  Add the line to the level 2 list
            list_put_last( level2_list_p, list_data_p );
        }

        /********************************************************************/
        //  Is this the start of a new e-Mail message ?
        else
        if (    ( email_flag                          == true )
             && ( email_is_group_break( list_data_p ) == true ) )
        {
            //  YES:    Log what we found
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "GroupBreak'\n" );

            //  Pass the data to level 2 processing
            decodel2_parse( level2_list_p, source_info_p );

            //  Wipe clean the old information
            memset( source_info_p->e_from,     '\0', sizeof( source_info_p->e_from     ) );
            memset( source_info_p->e_datetime, '\0', sizeof( source_info_p->e_datetime ) );
            memset( source_info_p->e_subject,  '\0', sizeof( source_info_p->e_subject  ) );

            //  Done with this text line
            list_fdelete( level1_list_p, list_data_p, list_lock_key );

            if ( list_query_count( level2_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel1_parse",
                           "There is still something on the list\n" );
            }

            //  Add the line to the level 2 list
            list_put_last( level2_list_p, list_data_p );
        }

        /********************************************************************/
        else
        {
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "Else       %p - %4d - '%.60s'\n",
                          list_data_p, list_query_count( level2_list_p ), list_data_p );

            //  Remove the line from the level 1 list
            list_fdelete( level1_list_p, list_data_p, list_lock_key );

            //  Add the line to the level 2 list
            list_put_last( level2_list_p, list_data_p );
        }
    }

    /************************************************************************
     *  Individual e-mails distributed from Yahoo Groups don't have message
     *  breaks.  If this is the case [or this isn't from an e-Mail] we will
     *  get here without sending stuff to the level 2 processor.
     ************************************************************************/

    //  Is there still something on the list ?
    if ( list_query_count( level2_list_p ) != 0 )
    {
        //  Pass the data to level 2 processing
        decodel2_parse( level2_list_p, source_info_p );

        if ( list_query_count( level2_list_p ) != 0 )
        {
            log_write( MID_FATAL, "decodel1_parse",
                       "There is still something on the list\n" );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

#if ( MEM_DUMP == true )
    //  Watch for false triggers because of threading when the encode thread
    //  takes a little longer then normal.
    mem_dump( );
#endif

    //  Close the level 2 list
    if ( list_kill( level2_list_p ) != true )
    {
        log_write( MID_FATAL, "decodel1_parse",
                      "list_kill( level2_list_p ) failed\n" );
    }
    else
    {
        log_write( MID_DEBUG_0, "decodel1_parse",
                      "list_kill( [%p] level2_list_p ) complete.\n",
                      level2_list_p );
    }

    //  Unlock the list
    list_user_unlock( level1_list_p, list_lock_key );

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decodel1_parse",
                  "Exiting  with %6d lines of text\n",
                  list_query_count( level1_list_p ) );

    //  Sanity check.  The list should be empty, make sure it is.
    if ( list_query_count( level1_list_p ) != 0 )
    {
        //  Debug dump whatever is on the list
        for( list_data_p = list_get_first( level1_list_p );
             list_data_p != NULL;
             list_data_p = list_get_next( level1_list_p, list_data_p ) )
        {
            //  Write the data to the log
            log_write( MID_WARNING, "decodel1_parse",
                          "WARNING: %p - '%.60s'\n", list_data_p, list_data_p );
        }

        //  Kill the program
        log_write( MID_FATAL, "decodel1_parse",
                      "Stop to figure out what happened\n" );
    }
}

/****************************************************************************/
