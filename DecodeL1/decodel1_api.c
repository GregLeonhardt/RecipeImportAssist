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
#include <decodel2_api.h>       //  API for all decodel2_*          PUBLIC
#include <decode_html_api.h>    //  API for all decode_html_*       PUBLIC
#include <email_api.h>          //  API for all email_*             PUBLIC
#include <decode_txt_api.h>     //  API for all decode_txt*         PUBLIC
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
    /**
     * @param content_type      e-Mail content type                         */
    enum    content_type_e          content_type;
    /**
     * @param encoding_type     e-Mail encoding type                        */
    enum    encoding_type_e         encoding_type;
    /**
     * @param boundary_type     e-Mail boundary type                        */
    enum    boundary_type_e         boundary_type;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decodel1_parse",
                  "Entering with %6d lines of text\n",
                  list_query_count( level1_list_p ) );

    //  By default we are NOT doing e-Mail.
    email_flag = false;

    //  At startup we are NOT looking for a boundary= tag.
    boundary_type = BT_NOT_SEARCHING;

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

        //  Reset the content types.
        content_type   = CT_NONE;
        encoding_type  = CTE_NONE;

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
        /**
         * @param tmp_c_type        e-Mail content type                     */
        enum    content_type_e          tmp_c_type;
        /**
         * @param tmp_e_type        e-Mail encoding type                    */
        enum    encoding_type_e         tmp_e_type;

        //  Remove the data from the level 1 list
        list_fdelete( level1_list_p, list_data_p, list_lock_key );

        tmp_c_type = email_find_content( list_data_p );
        tmp_e_type = email_find_encoding( list_data_p );

        /********************************************************************/
        //  Content-Type: text/plain; charset="utf-8"
        /********************************************************************/

        //  Do we have a new content type ?
        if (    ( email_flag == true    )
             && ( tmp_c_type != CT_NONE ) )
        {
            //  YES:    Set the new encoding type
            content_type = tmp_c_type;

            //  Just in case it is on, turn the boundary search flag off
            boundary_type = BT_NOT_SEARCHING;

            //  YES:    Were we decoding HTML ?
            if ( content_type == CT_TEXT_HTML  )
            {
                //  YES:    Insert a '<html>' tag so the decoder will work.
                list_put_last( level2_list_p, text_copy_to_new( "<html>\0" ) );

                //  The input data will interfere with HTML decode.
                list_data_p[ 0 ] = '\0';
            }

            //  YES:    Is this the content type 'multipart/alternative' ?
            if ( strcasestr( list_data_p, "multipart/alternative" ) != NULL )
            {
                //  YES:    Set the searching for boundary flag.
                boundary_type = BT_SEARCHING;
            }
        }

        /********************************************************************/
        //  Content-Transfer-Encoding: base64
        /********************************************************************/

        //  Do we have a new encoding type ?
        else
        if (    ( email_flag == true     )
             && ( tmp_e_type != CTE_NONE ) )
        {
            //  YES:    Set the new encoding type
            encoding_type = tmp_e_type;

            //  Are we decoding HTML ?
            if ( content_type == CT_TEXT_HTML  )
            {
                //  YES:    The input data will interfere with HTML decode.
                list_data_p[ 0 ] = '\0';
            }
        }

        /********************************************************************/
        //  -- End of Boundary-Content
        /********************************************************************/

        //  Is this the end of a multipart message ?
        else
        if (    ( email_flag                              == true     )
             && ( content_type                            != CT_NONE  )
             && ( encoding_type                           != CTE_NONE )
             && ( email_is_multipart_break( list_data_p ) == true     ) )
        {
            //  YES:    Were we decoding text/html ?
            if ( content_type == CT_TEXT_HTML  )
            {
                //  YES:    HACK:   End html
                list_put_last( level2_list_p, text_copy_to_new( "</html>\0" ) );

                //  Convert HTML to TEXT
                decode_html( level2_list_p, source_info_p );
            }
            //  Were we decoding content type text/plain ?
            if (    ( content_type  == CT_TEXT    )
                 && ( encoding_type == CTE_BASE64 ) )
            {
                //  YES:    Clean up the text.
                level2_list_p = decode_txt_cleanup( level2_list_p );
            }

            //  Reset the content types.
            content_type   = CT_NONE;
            encoding_type  = CTE_NONE;
            boundary_type  = BT_NOT_SEARCHING;
        }

        /********************************************************************/
        //  Decode BASE64 data
        /********************************************************************/

        //  Is it time to decode base64 data ?
        else
        if ( encoding_type == CTE_BASE64 )
        {
            //  YES:    Is this plain text or html ?
            if (    ( content_type == CT_TEXT )
                 || ( content_type == CT_TEXT_HTML  ) )
            {
                /**
                 * @param tmp_data_p        Pointer to decoded data         */
                char                        *   tmp_data_p;

                //  YES:    Decode the current text string
                tmp_data_p = base64_decode( list_data_p );

                //  Do we have a good decode ?
                if ( tmp_data_p != NULL )
                {
                    //  YES:    Swap the read data for the decoded data.
                    mem_free( list_data_p );
                    list_data_p = tmp_data_p;
                }
            }
        }

        /********************************************************************/
        //  Group-From:
        /********************************************************************/

        else
        //  Did we locate e-Mail source data ?
        if (    ( email_flag                                        == true )
             && ( ( tmp_data_p = email_find_source( list_data_p ) ) != NULL )
             && ( strlen( source_info_p->g_from )                   ==    0 ) )
        {
            //  YES:    Log what we found
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "Source:    '%.60s'\n", tmp_data_p );

            //  Is there already something here ?
            if ( strlen( source_info_p->g_from ) == 0 )
            {
                //  NO:     Will the e-Mail source information fit in the buffer ?
                if ( sizeof( source_info_p->g_from ) > strlen( tmp_data_p ) )
                {
                    //  YES:    Overwrite the e-Mail 'source' data
                    memset( source_info_p->g_from, '\0',
                            sizeof( source_info_p->g_from ) );
                    strncpy( source_info_p->g_from, tmp_data_p, SOURCE_L );
                }
            }
        }

        /********************************************************************/
        //  Email-From:
        /********************************************************************/

        //  Did we locate e-Mail from data ?
        else
        if (    ( email_flag                                      == true )
             && ( ( tmp_data_p = email_find_from( list_data_p ) ) != NULL )
             && ( strlen( source_info_p->e_from )                 ==    0 ) )
        {
            //  YES:    Log what we found
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "From:      '%.60s'\n", tmp_data_p );

            //  Is there already something here ?
            if ( strlen( source_info_p->e_from ) == 0 )
            {
                //  NO:     Will the e-Mail from information fit in the buffer ?
                if ( sizeof( source_info_p->e_from ) > strlen( tmp_data_p ) )
                {
                    //  YES:    Overwrite the e-Mail 'from' data
                    memset( source_info_p->e_from, '\0',
                            sizeof( source_info_p->e_from ) );
                    strncpy( source_info_p->e_from, tmp_data_p, FROM_L );
                }
            }
        }

        /********************************************************************/
        //  Group-DateTime:
        /********************************************************************/

        //  Did we locate e-Mail from data ?
        else
        if (    ( email_flag                                           == true )
             && ( ( tmp_data_p = email_find_datetime( list_data_p ) )  != NULL ) )
        {
            //  YES:    Log what we found
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "DataTime   '%.60s'\n", tmp_data_p );

            //  Is there already something here ?
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
            //  Will the e-Mail date/time information fit in the buffer ?
            if ( sizeof( source_info_p->g_datetime ) > strlen( tmp_data_p ) )
            {
                //  YES:    Write the e-Mail 'date/time' data
                memset( source_info_p->e_datetime, '\0',
                        sizeof( source_info_p->e_datetime ) );
                strncpy( source_info_p->e_datetime, tmp_data_p, DATETIME_L );
            }
        }

        /********************************************************************/
        //  Group-Subject:
        /********************************************************************/

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
            //  Will the e-Mail subject information fit in the buffer ?
            if ( sizeof( source_info_p->e_subject ) > strlen( tmp_data_p ) )
            {
                //  YES:    Write the e-Mail subject data
                memset( source_info_p->e_subject, '\0',
                        sizeof( source_info_p->e_subject ) );
                strncpy( source_info_p->e_subject, tmp_data_p, SUBJECT_L );
            }
        }

        /********************************************************************/
        //  Are we looking for the boundary identifier ?
        if ( boundary_type == BT_NO_IDENTIFIER )
        {
            //  See if this is the boundary identifier.
            boundary_type = email_is_boundary_identifier( list_data_p );
        }

        /********************************************************************/
        //  Are we looking for boundary= ?
        if ( boundary_type == BT_SEARCHING )
        {
            //  We are looking for a multi-part boundary
            boundary_type = email_is_boundary( list_data_p );
        }

        /********************************************************************/
        //  Is this the start of a new e-Mail message ?
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

            if ( list_query_count( level2_list_p ) != 0 )
            {
                log_write( MID_FATAL, "decodel1_parse",
                           "There is still something on the list\n" );
            }
        }

        /********************************************************************/
        else
        {
            log_write( MID_DEBUG_0, "decodel1_parse",
                          "Else       %p - %4d - '%.60s'\n",
                          list_data_p, list_query_count( level2_list_p ), list_data_p );
        }
        //  Add the line to the level 2 list
        list_put_last( level2_list_p, list_data_p );
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
