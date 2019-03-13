/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'email' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_EMAIL             ( "ALLOCATE STORAGE FOR EMAIL" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <stdlib.h>             //  ANSI standard library.
#include <ctype.h>              //  Determine the type contained
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <email_api.h>          //  API for all cc_email_*          PUBLIC
#include "email_lib.h"          //  API for all EMAIL__*            PRIVATE
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
#define SRCH_SOURCE                 "From - "
#define SRCH_SOURCE_L               strlen( SRCH_SOURCE )
//----------------------------------------------------------------------------
#define SRCH_RETURN                 "Return-Path:"
#define SRCH_RETURN_L               strlen( SRCH_RETURN )
//----------------------------------------------------------------------------
#define SRCH_RXF_BREAK              "........................................"
#define SRCH_RXF_BREAK_L            strlen( SRCH_RXF_BREAK )
//----------------------------------------------------------------------------
#define SRCH_FROM                   "From:"
#define SRCH_FROM_L                 strlen( SRCH_FROM )
//----------------------------------------------------------------------------
#define SRCH_POSTED                 "Posted by:"
#define SRCH_POSTED_L               strlen( SRCH_POSTED )
//----------------------------------------------------------------------------
#define SRCH_MAILINGLIST            "Mailing-List:"
#define SRCH_MAILINGLIST_L          strlen( SRCH_MAILINGLIST )
//----------------------------------------------------------------------------
#define SRCH_SENDER                 "Sender:"
#define SRCH_SENDER_L               strlen( SRCH_SENDER )
//----------------------------------------------------------------------------
#if 0
#define SRCH_DELIVER                "Delivered-To:"
#define SRCH_DELIVER_L              strlen( SRCH_DELIVER )
#endif
//----------------------------------------------------------------------------
#define SRCH_DATETIME               "Date:"
#define SRCH_DATETIME_L             strlen( SRCH_DATETIME )
//----------------------------------------------------------------------------
#define SRCH_SUBJECT                "Subject:"
#define SRCH_SUBJECT_L              strlen( SRCH_SUBJECT )
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
 *  Test the input text line to see if it is the start of a new e-Mail message.
 *
 *  @param  data_p              Pointer to a line of text data.
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *      From - Wed Jun 18 07:08:29 1997
 *
 ****************************************************************************/

int
email_is_start(
    char                        *   data_p
    )
{
    /**
     * @param email_rc          Return code for this function               */
    int                             email_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume this is NOT a new e-Mail message
    email_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the start of a new e-Mail ?
    if( strncmp( tmp_data_p, SRCH_SOURCE, SRCH_SOURCE_L ) == 0 )
    {
        //  YES:    Change the return code
        email_rc = true;
    }
    //  Is this the start of a new e-Mail ?
    else
    if( strncmp( tmp_data_p, SRCH_RETURN, SRCH_RETURN_L ) == 0 )
    {
        //  YES:    Change the return code
        email_rc = true;
    }
    //  Is this the start of a new e-Mail ?
    else
    if( strncmp( tmp_data_p, SRCH_RXF_BREAK, SRCH_RXF_BREAK_L ) == 0 )
    {
        //  YES:    Change the return code
        email_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
/**
 *  Test the text line to see if it contains an e-Mail group break.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *      ------------------------------
 *      --------------- MESSAGE bread-bakers.v096.n027.1 ---------------
 *      --------------- END bread-bakers.v096.n042 ---------------
 *      ________________________________________________________________________
 *
 ****************************************************************************/

int
email_is_group_break(
    char                        *   data_p
    )
{
    /**
     * @param email_flag        A mark on the wall if wr are doing an e-Mail*/
    int                             email_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'from' information.
    email_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the start of a new e-Mail ?
    if(    (    ( strncmp( tmp_data_p, "------------------------------", 30 ) ==  0 )
             && ( strlen( tmp_data_p )                                        == 30 ) )
        || (    ( strncmp( tmp_data_p, "______________________________", 30 ) ==  0 )
             && ( strlen( tmp_data_p )                                        == 30 ) )
        || (    ( strncmp( tmp_data_p, "----------------------------------------------------------------------", 70 ) == 0 )
             && ( strlen( tmp_data_p )                                        == 70 ) )
        || (    ( strncmp( tmp_data_p, "________________________________________________________________________", 72 ) == 0 )
             && ( strlen( tmp_data_p )                                        == 72 ) )
        || (    ( strncmp( tmp_data_p, "-----"                         ,  5 ) ==  0 )
             && ( strlen( tmp_data_p )                                        ==  5 ) )
        || ( strncmp( tmp_data_p, "========================  Arch",      30 ) == 0 )
        || ( strncmp( tmp_data_p, "--------------- MESSAGE bread-",      30 ) == 0 )
        || ( strncmp( tmp_data_p, "--------------- END bread-bake",      30 ) == 0 ) )
    {
        //  YES:    Change the return code
        email_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
/**
 *  Search the text line for Yahoo news group source data.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return tmp_data_p          NULL if no 'source' information, else a
 *                              pointer to the source data.
 *
 *  @note
 *      Mailing-List: list MC_recipes@onelist.com; contact MC_recipes-owner@onelist.com
 *      Sender: bread-bakers-errors@lists.best.com
 *
 ****************************************************************************/

char    *
email_find_source(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  @NOTE:  "Deliver-To:" can be used for some early e-Mails but is
    //          is used as the "To:" later.  This dual use makes it impossible
    //          to use

    //  Mailing-List: ?
    if( strncmp( tmp_data_p, SRCH_MAILINGLIST, SRCH_MAILINGLIST_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_MAILINGLIST_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    //  Sender: ?
    else
    if( strncmp( tmp_data_p, SRCH_SENDER, SRCH_SENDER_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_SENDER_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    else
    {
        //  NO:     Set the pointer to 'NULL'
        tmp_data_p = NULL;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_data_p );
}

/****************************************************************************/
/**
 *  Test the text line to see if it contains where the e-Mail came from.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *      From: 'Jane Doe' jane.doe@server.mail (Jane Doe)
 *
 ****************************************************************************/

char    *
email_find_from(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  From: ?
    if( strncmp( tmp_data_p, SRCH_FROM, SRCH_FROM_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_FROM_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    //  Posted by: ?
    else
    if( strncmp( tmp_data_p, SRCH_POSTED, SRCH_POSTED_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_POSTED_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    else
    {
        //  NO:     Set the pointer to 'NULL'
        tmp_data_p = NULL;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_data_p );
}

/****************************************************************************/
/**
 *  Test the text line to see if it contains when the e-Mail was sent.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *      Date: Tue, 7 Mar 2000 10:18:45 -0800 (PST)
 *
 ****************************************************************************/

char    *
email_find_datetime(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  From: ?
    if( strncmp( tmp_data_p, SRCH_DATETIME, SRCH_DATETIME_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_DATETIME_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    else
    {
        //  NO:     Set the pointer to 'NULL'
        tmp_data_p = NULL;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_data_p );
}

/****************************************************************************/
/**
 *  Test the text line to see if it contains an e-Mail subject.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_rc            TRUE when the text is the start of an e-Mail
 *                              message, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

char    *
email_find_subject(
    char                        *   data_p
    )
{
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  From: ?
    if( strncmp( tmp_data_p, SRCH_SUBJECT, SRCH_SUBJECT_L ) == 0 )
    {
        //  YES:    Move the pointer past the search text
        tmp_data_p += SRCH_SUBJECT_L;

        //  Also move past any whitespace to eventually point to the data
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );
    }
    else
    {
        //  NO:     Set the pointer to 'NULL'
        tmp_data_p = NULL;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_data_p );
}

/****************************************************************************/
/**
 *  Analyze e-Mail messages for stuff that we don't want anything to do with.
 *  When located discard it.
 *
 *  @param  data_p              Pointer to a line of text data
 *
 *  @return email_filtering     TRUE when we are processing stuff that is
 *                              considered to be trash, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

int
email_filter(
    char                        *   data_p
    )
{
    /**
     * @param email_rc          Return code for this function               */
    int                             email_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Was the last line from a reply message ?
    if( email_filter_type == EMAIL_FILTER_REPLY )
    {
        //  YES:    Throw it in the trash
        email_filter_type = EMAIL_FILTER_NONE;
    }

    /************************************************************************
     *  Actively filtering
     ************************************************************************/

    switch( email_filter_type )
    {
        case    EMAIL_FILTER_X_YMAILISG:
        {
            //  Is this part of the current block ?
            if( data_p[ 0 ] != ' ' )
            {
                //  NO:     Set the return code.
                email_filter_type = EMAIL_FILTER_NONE;
            }
        }   break;

        case    EMAIL_FILTER_BASE64:
        {
            //  Is this the end of the BASE-64 data ?
            if( strstr( data_p, "--" ) != NULL )
            {
                //  YES:    Set the return code.
                email_filter_type = EMAIL_FILTER_NONE;
            }
        }   break;

        default:
        {
            /**
             *  @param  tmp_data        Temporary data buffer               */
            char                            tmp_data[ 100 ];

            //  Is the data length too long for a compare match ?
            if( strlen( data_p ) < sizeof( tmp_data ) )
            {
                //  YES:    Copy the data to our temporary buffer
                memset( tmp_data, '\0', sizeof( tmp_data ) );
                strncpy( tmp_data, data_p, sizeof( tmp_data ) - 1 );

                //  Make the temporary data string all lowercase
                text_to_lowercase( tmp_data );

                //  Yahoo Mail  ?
                if( strncmp( tmp_data, "x-ymailisg:", 11 ) == 0 )
                {
                    //  YES:    Set the filter type.
                    email_filter_type = EMAIL_FILTER_X_YMAILISG;
                }
                //  BASE-64:
                else
                if( strncmp( tmp_data, "content-transfer-encoding: base64", 33 ) == 0 )
                {
                    //  YES:    Set the filter type.
                    email_filter_type = EMAIL_FILTER_BASE64;
                }
                //  Is this part of a reply to an e-mail
                else
                if(    ( data_p[ 0 ] == '>' )
//  @ToDo   This was removed because it is a defined part of the GF2
//          recipe format.
//                  || ( data_p[ 0 ] == '|' )
                    || ( data_p[ 0 ] == '!' ) )
                {
                    //  YES:    Throw it in the trash
                    email_filter_type = EMAIL_FILTER_REPLY;
                }
            }
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Set the return code
    if( email_filter_type == EMAIL_FILTER_NONE )
    {
        email_rc = false;
    }
    else
    {
        email_rc = true;
    }

    //  DONE!
    return( email_rc );
}

/****************************************************************************/
