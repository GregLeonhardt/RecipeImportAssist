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

#if 0   //  Why is this here
    //  Assume there is NOT an AUIP line
    auip_found = false;
#endif

    //  Clean the Text Message Array
    memset( (char*)txt_msg_array, 0x00,
            ( sizeof( struct txt_msg_t ) * MAX_LINE_COUNT ) );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Will everything fit in the structure
    if( line_count < MAX_LINE_COUNT )
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

#if 0       //  @ToDo   Why was this code written ?  Is it needed ?
            //  Do we have three consecutive blank lines ?
            if(    (                line_number                 >=               1 )
                && ( txt_msg_array[ line_number     ].line_type == LINE_TYPE_BLANK )
                && ( txt_msg_array[ line_number - 1 ].line_type == LINE_TYPE_BLANK )
                && ( txt_msg_array[ line_number - 2 ].line_type == LINE_TYPE_BLANK ) )
            {
                //  YES:    Go process a text recipe
                DECODE_TXT__decode( source_info_p );

                //  Clean the Text Message Array
                memset( (char*)txt_msg_array, 0x00,
                        ( sizeof( struct txt_msg_t ) * MAX_LINE_COUNT ) );

                //  Reset the starting line number
                line_number = -1;
            }
#endif
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
