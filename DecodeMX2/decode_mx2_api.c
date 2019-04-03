/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode_mx2' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE_MX2          ( "ALLOCATE STORAGE FOR DECODE_MX2" )

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
#include <decode_mx2_api.h>     //  API for all decode_mx2_*        PUBLIC
#include "decode_mx2_lib.h"     //  API for all DECODE_MX2__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Private API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    mx2_decode_state_e
{
    MX2_DS_INITIAL             =    0,  // Starting the recipe scan
    MX2_DS_NEXT                =    1,  // Searching for recipe title
    MX2_DS_SRV_SIZE            =    2,  // Search for 'Serving Size:
    MX2_DS_PREP_TIME           =    3,  // Search for 'Recipe By:'
    MX2_DS_CATEGORY            =    4,  // Search for 'Categories:'
    MX2_DS_CAT_INPUT           =    5,  // Search for 'Categories:'
    MX2_DS_AUIP                =    6,  // Processing AUIP
    MX2_DS_PREPARATION         =    7,  // Processing Directions
    MX2_DS_DIRECTIONS          =    8,  // Processing Directions
    MX2_DS_NOTES               =    9,  // Processing Notes
    MX2_DS_XML                 =   10,  // Parsing generic XML
    MX2_DS_END                 =   11,  // Recipe END detected
    MX2_DS_PRINT               =   12   // Print recipe file
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
 *  Decode a Mastercook eXport ver. 2 formatted recipe.
 *
 *  @param  void                No parameters are passed in.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_mx2(
    struct  list_base_t         *   mx2_list_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                        *   list_data_p;
    /**
     *  @param  decode_p        Pointer to the HTML decode buffer           */
    char                        *   decode_p;
    /**
     *  @param  mx2_active      Flag showing an MX2 recipe is active        */
    int                             mx2_active;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Debug log the current activity.
    log_write( MID_DEBUG_0, "decode_mx2",
                  "Entering with %6d lines of text\n",
                  list_query_count( mx2_list_p ) );

    //  Allocate storage for the decode buffer
    decode_p = mem_malloc( DECODE_BUF_L );

    //  Initialize the flag(s)
    mx2_active = false;

    /************************************************************************
     *  Move the list data into the decode buffer
     ************************************************************************/

    //  Scan the list
    for( list_data_p = list_get_first( mx2_list_p );
         list_data_p != NULL;
         list_data_p = list_get_next( mx2_list_p, list_data_p ) )
    {
        //  Remove the line from the list.
        list_delete( mx2_list_p, list_data_p );

        //  Is this the start of a new recipe ?
        if ( DECODE_MX2__start( list_data_p ) == true )
        {
            //  YES:    Set the flag ON
            mx2_active = true;
        }

        //  Are we actively copying a MX2 recipe ?
        if ( mx2_active == true )
        {
            //  Will the new buffer fit into the HTML decode buffer ?
            if ( ( strlen( decode_p ) + strlen( list_data_p ) ) < ( DECODE_BUF_L + 10 ) )
            {
                //  YES:    Append the new to the existing
                strncat( decode_p, " ", ( DECODE_BUF_L - 1 ) );
                strncat( decode_p, list_data_p, ( DECODE_BUF_L - strlen( decode_p ) ) );
            }
            else
            {
                //  NO:     We need a bigger buffer.
                log_write( MID_WARNING, "decode_mx2",
                              "The HTML decode buffer is not big enough.  Please\n" );
                log_write( MID_FATAL, "decode_mx2",
                              "increase the size of 'DECODE_BUF_L' and recompile.\n" );
            }

            //  Is this the end of the active MX2 recipe ?
            if ( DECODE_MX2__end( list_data_p ) == true )
            {
                //  YES:    Set the flag OFF
                mx2_active = false;

                //  Decode the recipe
                DECODE_MX2__decode( decode_p, source_info_p );

                //  Clean out the decode buffer
                memset( decode_p, '\0', DECODE_BUF_L );
            }
        }
        //  Release the input data buffer
        mem_free( list_data_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release storage for the decode buffer
    mem_free( decode_p );

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the data for the start of a MasterCook MX2 formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mx2_rc              Passed through from DECODE_MX2__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_mx2_start(
    char                        *   data_p
    )
{
    /**
     * @param mx2_rc            Return Code                                 */
    int                             mx2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    mx2_rc = DECODE_MX2__start( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mx2_rc );
}

/****************************************************************************/
/**
 *  Test the data for the end of a MasterCook MX2 formatted recipe.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mx2_rc              Passed through from DECODE_MX2__start.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_mx2_end(
    char                        *   data_p
    )
{
    /**
     * @param mx2_rc            Return Code                                 */
    int                             mx2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Call the private function
    mx2_rc = DECODE_MX2__end( data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mx2_rc );
}

/****************************************************************************/
