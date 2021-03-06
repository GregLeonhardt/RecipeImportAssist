/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_txt' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_TXT          ( "ALLOCATE STORAGE FOR DECODE_TXT" )

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
#include <email_api.h>          //  API for all email_*             PUBLIC
#include <recipe_api.h>         //  API for all recipe_*            PUBLIC
                                //*******************************************
#include <decode_txt_api.h>     //  API for all decode_txt_*        PUBLIC
#include "decode_txt_lib.h"     //  API for all DECODE_TXT__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    txt_decode_state_e
{
    TXT_STATE_IDLE              =    0,
    TXT_STATE_TITLE             =    1,
    TXT_STATE_RECIPE_BY         =    2,
    TXT_STATE_SRV_SIZE          =    3,
    TXT_STATE_PREP_TIME         =    4,
    TXT_STATE_CATEGORIES        =    5,
    TXT_STATE_AUIP              =    6,
    TXT_STATE_DIRECTIONS        =    7,
    TXT_STATE_NOTES             =    8,
    TXT_STATE_END               =    9,
    TXT_STATE_COMPLETE          =   10,

    TXT_STATE_FORMAT_1_A        =  100,
    TXT_STATE_FORMAT_1_B        =  101,
    TXT_STATE_FORMAT_1_C        =  102,
    TXT_STATE_FORMAT_1_D        =  103,
    TXT_STATE_FORMAT_1_E        =  104,
    TXT_STATE_FORMAT_1_F        =  105,
    TXT_STATE_FORMAT_1_G        =  106,
    TXT_STATE_FORMAT_1_H        =  107,
    TXT_STATE_FORMAT_1_I        =  108,
    TXT_STATE_FORMAT_1_J        =  109
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
 *  The decode_txt function is called bla bla bla.
 *
 *  @param  txt_list_p          Pointer to the source data list
 *
 *  @return new_list_p          Pointer to the list with cleaned data
 *
 *  @note
 *
 ****************************************************************************/

struct  list_base_t         *
decode_txt_cleanup(
    struct  list_base_t         *   txt_list_p
    )
{
    /**
     *  @param  new_list_p      Pointer to the list with cleaned data       */
    struct  list_base_t         *   new_list_p;
    /**
     *  @param  src_data_p      Pointer to the source data                  */
    char                        *   src_data_p;
    /**
     *  @param  data_p          Pointer to the data buffer                   */
    char                        *   data_p;
    /**
     *  @param  complete        Flag showing the state of the buffer scan.   */
    int                             complete;
    /**
     *  @param  line_start_p    Pointer to the first character of input line */
    char                        *   line_start_p;
    /**
     *  @param  line_end_p      Pointer to the last character of input line  */
    char                        *   line_end_p;
    /**
     *  @param  ndx             Index into an input line.                    */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Create the new list
    new_list_p = list_new( );

    //  Allocate storage for the decode buffer
    data_p = mem_malloc( DECODE_BUF_L );

    /************************************************************************
     *  Input the entire list
     ************************************************************************/

    //  The whole list
    for( src_data_p = list_get_first( txt_list_p );
         src_data_p != NULL;
         src_data_p = list_get_next( txt_list_p, src_data_p ) )
    {
        //  Remove the line from the list.
        list_delete( txt_list_p, src_data_p );

        //  Will the new buffer fit into the HTML decode buffer ?
        if ( ( strlen( src_data_p ) + strlen( data_p ) ) < ( DECODE_BUF_L + 10 ) )
        {
            //  YES:    Append the new to the existing
            strcat( data_p, src_data_p );
        }
        else
        {
            //  NO:     We need a bigger buffer.
            log_write( MID_WARNING, "decode_txt_cleanup",
                          "The data buffer is not big enough.  Please\n" );
            log_write( MID_FATAL, "decode_txt_cleanup",
                          "increase the size of 'DECODE_BUF_L' and recompile.\n" );
        }
        //  Release the input data buffer
        mem_free( src_data_p );
    }

    //  We are done with this list.
    list_kill( txt_list_p );

    /************************************************************************
     *  Split source data into individual output lines
     ************************************************************************/

    //  Set the beginning of the first input line.
    line_start_p = data_p;

    //  The whole string
    do
    {
        //  The assumption is that we are done.
        complete = true;

        //  Look for the end of another input line
        line_end_p = strchr( line_start_p, '\n' );

        //  Was there another line of data in the buffer ?
        if ( line_end_p != NULL )
        {
            //  YES:    Change the flag to show we are not done yet.
            complete = false;

            //  YES:    Terminate the current input line.
            line_end_p[ 0 ] = '\0';

            //  Scan the line for any control characters.
            for ( ndx = 0;
                  line_start_p[ ndx ] != '\0';
                  ndx += 1 )
            {
                //  Is this a control character ?
                if ( iscntrl( line_start_p[ ndx ] ) != 0 )
                {
                    //  Yes:    Just make it a space
                    line_start_p[ ndx ] = ' ';
                }

            }
            //  Is there something to add to the output list ?
//          if ( strlen( line_start_p ) != 0 )
            {
                //  YES:    Save the new formatted data in the new list
                list_put_last( new_list_p, text_copy_to_new( line_start_p ) );
            }

            //  Set the start pointer to the start of the next line of data
            line_start_p = line_end_p + 1;
        }

    }   while( complete == false );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release the temporary data buffer
    mem_free( data_p );
    
    //  DONE!
    return( new_list_p );
}

/****************************************************************************/
/**
 *  The decode_txt function is called bla bla bla.
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_txt(
    struct  list_base_t         *   txt_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     *  @param  line_count      Number of lines of text on the list         */
    int                             line_count;
    /**
     *  @param  line_number     Depth into input list                       */
    int                             line_number;
    /**
     *  @param  tmp_p           Pointer to a temporary data buffer          */
    char                        *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decode_txt",
                  "Entering with %6d lines of text\n",
                  list_query_count( txt_list_p ) );

    //  Count the number of text lines on the list
    line_count = list_query_count( txt_list_p );

    //  Clean the Text Message Array
    memset( (char*)txt_msg_array, 0x00,
            ( sizeof( struct txt_msg_t ) * MAX_LINE_COUNT ) );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  @TODO   Should there also be a minimum line count to reduce the
    //          amount of trash we accumulate.


    //  Will everything fit in the structure
    if ( line_count < MAX_LINE_COUNT )
    {
        //  Set the starting line number
        line_number = 0;

        //  Scan the list
        for( tmp_p = list_get_first( txt_list_p );
             tmp_p != NULL;
             tmp_p = list_get_next( txt_list_p, tmp_p ) )
        {
            //  Remove it from the input list
            list_delete( txt_list_p, tmp_p );

            //  Transfer to the array
            txt_msg_array[ line_number ].line_p    = tmp_p;
            txt_msg_array[ line_number ].line_type = LINE_TYPE_UNKNOWN;

            //  Identify the line type
            txt_msg_array[ line_number ].line_type
                    = DECODE_TXT__line_type( txt_msg_array[ line_number ].line_p );

            //  Advance to the next array cell
            line_number += 1;
        }

        //  Process anything in the list.
        DECODE_TXT__decode( source_info_p );
    }
    else
    {
        //  NO:     Write the data to an output file
        DECODE_TXT__dump( txt_list_p, source_info_p, FILE_DISCARD );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    log_write( MID_DEBUG_0, "decode_txt",
                  "Exit:\n" );

    //  DONE!
}

/****************************************************************************/
