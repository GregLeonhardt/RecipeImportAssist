/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_html' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_HTML          ( "ALLOCATE STORAGE FOR DECODE_HTML" )

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
                                //*******************************************
#include <decode_html_api.h>     //  API for all decode_html_*        PUBLIC
#include "decode_html_lib.h"     //  API for all DECODE_HTML__*       PRIVATE
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
 *  The decode_html function is called bla bla bla.
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_html(
    struct  list_base_t         *   html_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     *  @param  decode_data_p   Pointer to the HTML decode buffer            */
    char                        *   decode_data_p;
    /**
     *  @param  tmp_p           Pointer to a temporary data buffer          */
    char                        *   tmp_p;
    /**
     * @param   list_data_p     Pointer to the read data                    */
    char                        *   list_data_p;
    /**
     *  @param  html_found      Flag showing that an HTML tag was found      */
    int                             html_found;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Allocate storage for the decode buffer
    decode_data_p = mem_malloc( DECODE_BUF_L );

    log_write( MID_DEBUG_1, "decode_html_api.c", "Line: %d\n", __LINE__ );

    //  Set the initial state of the HTML found flag
    html_found = false;


    /************************************************************************
     *  Move the read data into the decode buffer
     ************************************************************************/


    //  Scan the list
    for( list_data_p = list_get_first( html_list_p );
         list_data_p != NULL;
         list_data_p = list_get_next( html_list_p, list_data_p ) )
    {
        //  Will the new buffer fit into the HTML decode buffer ?
        if ( ( strlen( decode_data_p ) + strlen( list_data_p ) ) < ( DECODE_BUF_L + 10 ) )
        {
            //  YES:    Is there an HTML tag here ?
            if ( strstr( list_data_p, "<html>" ) != NULL )
            {
                //  YES:    Set the HTML found flag
                html_found = true;
            }

            //  Is there an HTML end tag here ?
            if ( strstr( list_data_p, "</html>" ) != NULL )
            {
                //  YES:    Clear the HTML found flag
                html_found = false;
            }

            //  YES:    Append the new to the existing
            strcat( decode_data_p, list_data_p );

            //  Have we seen an HTML tag ?
            if ( html_found != true )
            {
                //  NO:     Still in the header; add a carriage return.
                strcat( decode_data_p, "<br>" );
            }

            //  Remove the line from the list.
            list_delete( html_list_p, list_data_p );

            //  Release the input data buffer
            mem_free( list_data_p );
        }
        else
        {
            //  NO:     We need a bigger buffer.
            log_write( MID_WARNING, "decode_html",
                          "The HTML decode buffer is not big enough.  Please\n" );
            log_write( MID_FATAL, "decode_html",
                          "increase the size of 'DECODE_BUF_L' and recompile.\n" );
        }
    }

    /************************************************************************
     *  Convert all HTML data to text
     ************************************************************************/

    //  Strip out consecutive spaces
    list_data_p = html2txt( decode_data_p );

    //  The rendered text is in the changed buffer so delete the source
    mem_free( decode_data_p );

    //  Fix the source pointer.
    decode_data_p = list_data_p;

    /************************************************************************
     *  Rebuild the list with what is now text only
     ************************************************************************/

    //  Go through the entire message
    for( tmp_p = strchr( list_data_p, '\n' );
         tmp_p != NULL;
         tmp_p = strchr( list_data_p, '\n' ) )
    {
        //  Terminate at the carriage return
        tmp_p[ 0 ] = '\0';

        //  Add the new line of text back to the list.
        list_put_last( html_list_p, text_copy_to_new( list_data_p ) );
        
        log_write( MID_DEBUG_1, "decode_html_api.c", "Line: %d\n", __LINE__ );

        //  Advance the buffer pointer to the start of the next line
        list_data_p = tmp_p + 1;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release the HTML decode buffer
    mem_free( decode_data_p );

    //  DONE!
}

/****************************************************************************/
