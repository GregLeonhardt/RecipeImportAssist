/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_txt' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


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
#include <encode_api.h>         //  API for all encode_*            PUBLIC
#include <xlate_api.h>          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include <decode_txt_api.h>     //  API for all decode_txt_*          PUBLIC
#include "decode_txt_lib.h"     //  API for all DECODE_TXT__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Definitions local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
#define NOT_AUIP_MAX_COUNT      ( 5 )
//----------------------------------------------------------------------------
#define NOT_A_TITLE_01          "________________________________________________________________________"
#define NOT_A_TITLE_01_L        ( strlen( NOT_A_TITLE_01 ) )
//----------------------------------------------------------------------------
#define NOT_A_TITLE_02          "__________________________________________________________"
#define NOT_A_TITLE_02_L        ( strlen( NOT_A_TITLE_02 ) )
//----------------------------------------------------------------------------
#define NOT_A_TITLE_03          "FooBar"
#define NOT_A_TITLE_03_L        ( strlen( NOT_A_TITLE_03 ) )
//----------------------------------------------------------------------------
#define UNIT_L                  ( 32 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Structures local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Storage Allocation local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/


/****************************************************************************/
/**
 *  Test the buffer passed for an AUIP (Amount, Unit, Ingredient, Preparation
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return decode_rc           TRUE if this is an AUIP; else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE__is_auip(
    char                        *   data_p
    )
{
    /**
     *  @param  decode_rc       Return code                                 */
    int                             decode_rc;
    /**
     *  @param  unit_p          Temporary pointer to a UNIT name            */
    char                        *   unit_p;
    /**
     *  @param  start_numeric   Flag showing the line starts with a number  */
    int                             start_numeric;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize flags and return code
    decode_rc = false;
    start_numeric = false;

    /************************************************************************
     *  Does the line start with a numeric field
     ************************************************************************/

    //  Is it numeric ?
    if( isdigit( data_p[ 0 ] ) != 0 )
    {
        //  YES:    Set the flag
        start_numeric = true;
    }

    /************************************************************************
     *  Is there a UNIT field ?
     ************************************************************************/

    //  Don't bother if it didn't start numeric
    if( start_numeric == true )
    {
        //  @param  tmp_unit        Buffer to hold the units data           */
        char                            tmp_unit[ UNIT_L ];

        //  YES:    Skip past the amount field
        while( 1 )
        {
            //  Is this the end of the amount field ?
            if(    ( isalpha( data_p[ 0 ] ) != 0  )
                || (          data_p[ 0 ] == '\0' ) )
            {
                //  YES:    We are done here.
                break;
            }
            else
            {
                //  NO:     Advance to the next character
                data_p += 1;
            }
        }

        //  Copy the next word to the temporary buffer
        memset( tmp_unit, '\0', sizeof( tmp_unit ) );

        if( strchr( data_p, ' ' ) != NULL )
        {
            //  Will it fit in the buffer ?
            if ( ( strchr( data_p, ' ' ) - data_p )  < UNIT_L )
            {
                //  YES:    Copy it to the temp buffer
                strncpy( tmp_unit, data_p, ( strchr( data_p, ' ' ) - data_p ) );
            }

        }
        else
        {
            //  Will it fit in the buffer ?
            if ( strlen( data_p ) < UNIT_L )
            {
                //  YES:    Copy it to the temp buffer
                strncpy( tmp_unit, data_p, strlen( data_p ) );
            }

        }

        //  Remove any period (.) that may be at the end of the word
        unit_p = strchr( tmp_unit, '.' );
        if( unit_p != NULL )
        {
            unit_p[ 0 ] = '\0';
        }

        //  Look for a UNIT translation
        unit_p = xlate_units( tmp_unit );

        //  Is it a Unit of measurement ?
        if( unit_p != NULL )
        {
            //  YES:    Change the return code
            decode_rc = true;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( decode_rc );
}

/****************************************************************************/
/**
 *  Encode the recipe in 'TXT' format
 *
 *  @param  txt_list_p          Pointer to a list
 *  @param  source_info_p       Pointer to source data information
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *      There isn't any real formatting to do here.  Just dump the data
 *      that wasn't in a recognized recipe format.
 *
 ****************************************************************************/

void
DECODE_TXT__dump(
    struct  list_base_t         *   dump_list_p,
    struct  source_info_t       *   source_info_p,
    int                             file
    )
{
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                        *   list_data_p;
    /**
     * @param list_lock_key     File list key                               */
    int                             list_lock_key;
    /**
     * @param out_name          Encoded output file name                    */
    char                            out_name[ ( FILE_NAME_L * 3 ) ];
    /**
     * @param input_file_fp     Output File pointer                         */
    FILE                        *   out_file_fp;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                        *   data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Build the source file path & name
    snprintf( out_name, sizeof( out_name ),
            "%s/%s", source_info_p->f_dir_name, source_info_p->f_file_name );

    //  Change the file extention
    data_p = strrchr( out_name, '.' );
    if( data_p != NULL ) data_p[ 0 ] = '\0';

    //  Is this an unformatted recipe ?
    if( file == FILE_UNFORMATTED )
    {
        //  YES:
        strncat( out_name, ".unformatted_recipes", sizeof( out_name ) );
    }
    else
    {
        //  NO:
        strncat( out_name, ".discarded_data", sizeof( out_name ) );
    }

    //  Open the input file
    out_file_fp = file_open_append( out_name );

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( dump_list_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Write a header
    fprintf( out_file_fp, "<><><><><><><><><><><><><><><><><><><><>\n" );
    fprintf( out_file_fp, "FILE-DIR:      %s\n", source_info_p->f_dir_name  );
    fprintf( out_file_fp, "FILE-NAME:     %s\n", source_info_p->f_file_name );
    fprintf( out_file_fp, "FILE-DATE:     %s\n", source_info_p->f_date_time );
    fprintf( out_file_fp, "FILE-SIZE:     %s\n", source_info_p->f_file_size );

    fprintf( out_file_fp, "GROUP-FROM:    %s\n", source_info_p->g_from      );
    fprintf( out_file_fp, "GROUP-SUBJECT: %s\n", source_info_p->g_subject   );
    fprintf( out_file_fp, "GROUP-DATE::   %s\n", source_info_p->g_datetime  );

    fprintf( out_file_fp, "MAIL-FROM:     %s\n", source_info_p->e_from      );
    fprintf( out_file_fp, "MAIL-SUBJECT:  %s\n", source_info_p->e_subject   );
    fprintf( out_file_fp, "MAIL-DATE:     %s\n", source_info_p->e_datetime  );
    fprintf( out_file_fp, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n" );

    //  Scan the list
    for( list_data_p = list_fget_first( dump_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( dump_list_p, list_data_p, list_lock_key ) )
    {
        //  Remove the line from the list
        list_fdelete( dump_list_p, list_data_p, list_lock_key );

        //  Write the data to the file
        fprintf( out_file_fp, "%s\n", list_data_p );

        //  Release the buffer
        mem_free( list_data_p );
    }

    fprintf( out_file_fp, "........................................\n" );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Unlock the list
    list_user_unlock( dump_list_p, list_lock_key );

    //  Close the output file.
    file_close( out_file_fp );

    //  Is there anything still on the list ?
    if( list_query_count( dump_list_p ) != 0 )
    {
        //  YES:    There shouldn't be!
        log_write( MID_FATAL, "DECODE_TXT__dump",
                   "There is still something on the list\n" );
    }

    //  DONE!
}

/****************************************************************************/
/**
 *  To get here we have identified what should be an unformatted recipe.
 *
 *  @param  source_info_p       Pointer to source data information
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_TXT__decode(
    struct  source_info_t       *   source_info_p
    )
{
    /**
     *  @param  line_number                                                 */
    int                             line_number;
    /**
     *  @param  auip_found      Flag showing at least one AUIP was found    */
    int                             auip_found;
    /**
     *  @param  grf_list_p      Pointer to a list where the file is stored  */
    struct  list_base_t         *   grf_list_p;
#if 0   //  Alternate code implementation
    /**
     *  @param  title_line                                                  */
    int                             title_line;
    /**
     *  @param  title_found     Flag showing a TITLE was found              */
    int                             title_found;
    /**
     *  @param  not_auip_count  Number of consecutive text or blank lines   */
    int                             not_auip_count;
    /**
     *  @param  tmp_p           Pointer to a temporary data buffer          */
    char                        *   tmp_p;
#endif
    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Variable initialization
#if 0   //  Alternate code implementation
    title_line = 0;
    title_found = false;
#endif
    auip_found = false;

    //  Start a new list for the next phase
    grf_list_p = list_new( );

    /************************************************************************
     *  A quick scan for an AUIP line
     ************************************************************************/

    //  Loop through all lines in the current buffer
    for( line_number = 0;
         txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
         line_number += 1 )
    {
        //  Do we have three AUIP lines ?
        if(    ( txt_msg_array[ line_number     ].line_type == LINE_TYPE_AUIP )
            && ( txt_msg_array[ line_number - 1 ].line_type == LINE_TYPE_AUIP )
            && ( txt_msg_array[ line_number + 1 ].line_type == LINE_TYPE_AUIP ) )
        {
            //  YES:    Set the flag
            auip_found = true;

            //  We are done here
            break;
        }
    }

#if 1
    //  NO:     Loop through all lines in the current buffer
    for( line_number = 0;
         txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
         line_number += 1 )
    {
        //  Is there something in this cell ?
        if( txt_msg_array[ line_number ].line_type != LINE_TYPE_EMPTY )
        {
            //  YES:    Move it to the list
            list_put_last( grf_list_p, txt_msg_array[ line_number ].line_p );
            txt_msg_array[ line_number ].line_p = NULL;
            txt_msg_array[ line_number ].line_type = LINE_TYPE_EMPTY;
        }
    }

    //  Is this possibly a recipe ?
    if( auip_found  == true )
    {
        //  YES:    Put it in the unformatted file
        DECODE_TXT__dump( grf_list_p, source_info_p, FILE_UNFORMATTED );
    }
    else
    {
        //  NO:     Put it in the trash file
        DECODE_TXT__dump( grf_list_p, source_info_p, FILE_DISCARD );
    }
#else
    /************************************************************************
     *  Find the Recipe TITLE
     ************************************************************************/

    //  Do we have a valid recipe title ?
    if( auip_found == true )
    {
        //  Loop through all lines in the current buffer
        for( line_number = 0;
             txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
             line_number += 1 )
        {

            tmp_p = text_skip_past_whitespace( txt_msg_array[ line_number ].line_p );

            //  Is this the first AUIP line ?
            if(    ( txt_msg_array[ line_number ].line_type == LINE_TYPE_TEXT )
                && ( strncmp( tmp_p, NOT_A_TITLE_01, NOT_A_TITLE_01_L ) != 0 )
                && ( strncmp( tmp_p, NOT_A_TITLE_02, NOT_A_TITLE_02_L ) != 0 )
                && ( strncmp( tmp_p, NOT_A_TITLE_03, NOT_A_TITLE_03_L ) != 0 ) )
            {
                //  Identify the recipe title
                if(    (    ( txt_msg_array[ line_number - 1 ].line_type == LINE_TYPE_BLANK )
                         || ( line_number <= 1                                              ) )
                    && ( txt_msg_array[ line_number + 1 ].line_type == LINE_TYPE_BLANK ) )
                {
                    //  Change the line type to TITLE
                    txt_msg_array[ line_number ].line_type = LINE_TYPE_TITLE;

                    //  Save the title index
                    title_line = line_number;

                    //  Set the flag
                    title_found = true;

                    //  We are done here
                    break;
                }
            }
        }
    }

    /************************************************************************
     *  AUIP Fix
     ************************************************************************/

    //  Do we have a valid recipe title ?
    if( title_found == true )
    {
        //  YES:    Reset the number of NOT AUIP lines
        not_auip_count = 0;

        //  Loop through all lines in the current buffer
        for( line_number = ( title_line + 2 );
             txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
             line_number += 1 )
        {
            if( line_number >= MAX_LINE_COUNT )
            {
                log_write( MID_FATAL, "DECODE_TXT__decode",
                              "Working past the array size.\n" );
            }

            //  Is this the first AUIP line ?
            if( txt_msg_array[ line_number ].line_type == LINE_TYPE_AUIP )
            {
                /**
                 *  @param  tmp_ndx                                             */
                int                             tmp_ndx;

                //  Reset the number of NOT AUIP lines
                not_auip_count = 0;

                //  Loop back to the next blank line
                for( tmp_ndx = line_number;
                     txt_msg_array[ tmp_ndx ].line_type != LINE_TYPE_BLANK;
                     tmp_ndx -= 1 )
                {
                    if( tmp_ndx <= title_line )
                    {
                        log_write( MID_INFO, "DECODE_TXT__decode",
                                      "Working before recipe title.\n" );
                        log_write( MID_FATAL, "DECODE_TXT__decode",
                                      "'%s'\n", txt_msg_array[ tmp_ndx ].line_p );
                    }
                    //  Change the line type to TITLE
                    txt_msg_array[ tmp_ndx ].line_type = LINE_TYPE_AUIP;
                }

                //  Loop forward to the next blank line
                for( tmp_ndx = line_number;
                     txt_msg_array[ tmp_ndx ].line_type != LINE_TYPE_BLANK;
                     tmp_ndx += 1 )
                {
                    //  Change the line type to TITLE
                    txt_msg_array[ tmp_ndx ].line_type = LINE_TYPE_AUIP;
                }

                //  Jump to the ending blank line
                line_number = tmp_ndx;
            }
            else
            {
                //  Are we into the directions ?
                if( ++not_auip_count == NOT_AUIP_MAX_COUNT )
                {
                    //  YES:    We are done here.
                    break;
                }
            }
        }
    }

    /************************************************************************
     *  Directions Fix
     ************************************************************************/

    //  Verify that there is a title and AUIP
    if(    ( title_found == true )
        && ( auip_found  == true ) )
    {
        //  YES:    Loop through all lines in the current buffer
        for( ;
             txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
             line_number += 1 )
        {
            //  Is this the first AUIP line ?
            if( txt_msg_array[ line_number ].line_type != LINE_TYPE_BLANK )
            {
                //  Change the line type to TEXT
                txt_msg_array[ line_number ].line_type = LINE_TYPE_TEXT;
            }
        }
    }

    /************************************************************************
     *  Build a Generic Recipe Format list
     ************************************************************************/

    //  Verify that there is a title and AUIP
    if(    ( title_found == true )
        && ( auip_found  == true ) )
    {
        //  YES:    Loop through all lines in the current buffer
        for( line_number = title_line;
             txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
             line_number += 1 )
        {
            //  Is this the recipe title ?
            if( txt_msg_array[ line_number ].line_type == LINE_TYPE_TITLE )
            {
                //  YES:    Insert the recipe start tag into the list
                tmp_p = text_copy_to_new( "[[[[[" );
                list_put_last( grf_list_p, tmp_p );

                //  Move the Recipe title to the list
                list_put_last( grf_list_p, txt_msg_array[ line_number ].line_p );
                txt_msg_array[ line_number ].line_p = NULL;
                txt_msg_array[ line_number ].line_type = LINE_TYPE_EMPTY;

                //  And finally a blank line
                tmp_p = text_copy_to_new( " " );
                list_put_last( grf_list_p, tmp_p );
            }

            //  Is this the recipe title ?
            if( txt_msg_array[ line_number ].line_type == LINE_TYPE_AUIP )
            {
                //  YES:    Move the new ingredient to the list
                list_put_last( grf_list_p, txt_msg_array[ line_number ].line_p );
                txt_msg_array[ line_number ].line_p = NULL;
                txt_msg_array[ line_number ].line_type = LINE_TYPE_EMPTY;
            }
        }

        //  Loop through all lines in the current buffer
        for( line_number = title_line;
             txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
             line_number += 1 )
        {
            //  Is this the recipe title ?
            if(    ( txt_msg_array[ line_number ].line_type == LINE_TYPE_BLANK )
                || ( txt_msg_array[ line_number ].line_type == LINE_TYPE_TEXT  ) )
            {
                //  YES:    Move it to the list
                list_put_last( grf_list_p, txt_msg_array[ line_number ].line_p );
                txt_msg_array[ line_number ].line_p = NULL;
                txt_msg_array[ line_number ].line_type = LINE_TYPE_EMPTY;
            }
        }

        //  Insert the recipe end tag into the list
        tmp_p = text_copy_to_new( "]]]]]" );
        list_put_last( grf_list_p, tmp_p );

        //  Pass the Generic Format Recipe to the decoder.
        decode_xxx( RECIPE_FORMAT_GRF, grf_list_p, source_info_p );

        //  One last time through the list to purge unused data.
        for( line_number = 0;
             txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
             line_number += 1 )
        {
            if( ( txt_msg_array[ line_number ].line_p != NULL ) && ( strncmp( txt_msg_array[ line_number ].line_p, "1. Lime Garlic Shrimp", 21 ) == 0 ) )
            {
                int a = 0;
            }

            //  Is this the recipe title ?
            if( txt_msg_array[ line_number ].line_type != LINE_TYPE_EMPTY )
            {
                //  YES:    Release the storage for this line
                mem_free( txt_msg_array[ line_number ].line_p );
            }
        }
    }
    else
    {
        //  @ToDo   [TEXT] Put this stuff in it's own output file so they
        //          can be reviewed manually.

        //  NO:     Loop through all lines in the current buffer
        for( line_number = 0;
             txt_msg_array[ line_number ].line_type != LINE_TYPE_UNKNOWN;
             line_number += 1 )
        {
            if( ( txt_msg_array[ line_number ].line_p != NULL ) && ( strncmp( txt_msg_array[ line_number ].line_p, "1. Lime Garlic Shrimp", 21 ) == 0 ) )
            {
                int a = 0;
            }
            //  Is there something in this cell ?
            if( txt_msg_array[ line_number ].line_type != LINE_TYPE_EMPTY )
            {
                //  YES:    Move it to the list
                list_put_last( grf_list_p, txt_msg_array[ line_number ].line_p );
                txt_msg_array[ line_number ].line_p = NULL;
                txt_msg_array[ line_number ].line_type = LINE_TYPE_EMPTY;
            }
        }

        //  Is this possibly a recipe ?
        if( auip_found  == true )
        {
            //  YES:    Put it in the unformatted file
            DECODE_TXT__dump( grf_list_p, source_info_p, FILE_UNFORMATTED );
        }
        else
        {
            //  NO:     Put it in the trash file
            DECODE_TXT__dump( grf_list_p, source_info_p, FILE_DISCARD );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/
#endif

    //  Finally, kill the recipe structure
    if( list_kill( grf_list_p ) != true )
    {
        list_debug_dump_list( grf_list_p );
        log_write( MID_FATAL, "DECODE_TXT__decode",
                      "list_kill( grf_list_p ) failed\n" );
    }
    else
    {
        log_write( MID_DEBUG_0, "DECODE_TXT__decode",
                      "list_kill( [%p] grf_list_p ) complete.\n",
                      grf_list_p );
    }

    //  DONE!
}

/****************************************************************************/
/**
 *  Identify the type of line
 *
 *  @param  line_p              Pointer to a line of text
 *
 *  @return line_type           Type of line
 *
 *  @note
 *
 ****************************************************************************/

enum    line_type_e
DECODE_TXT__line_type(
    char                        *   line_p
    )
{
    /**
     *  @param  line_type       Type of line                                */
    enum    line_type_e             line_type;
    /**
     *  @param  tmp_p           Temporary data pointer                      */
    char                        *   tmp_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Set a default value
    line_type = LINE_TYPE_UNKNOWN;

    //  Skip past any leading spaces
    tmp_p = text_skip_past_whitespace( line_p );

    /************************************************************************
     *  LINE_TYPE_@@@@@
     ************************************************************************/

    //  Is this the start marker for "Generic Format" ?
    if(    ( strncmp( tmp_p, "@@@@@", 5 ) == 0 )
       &&  ( strlen(  tmp_p )             == 5 ) )
    {
        //  YES:    Delete it
        memset( tmp_p, '\0', 5 );
    }

    /************************************************************************
     *  LINE_TYPE_BLANK
     ************************************************************************/

    //  Is this line empty ?
    if( text_is_blank_line( tmp_p ) == true )
    {
        //  YES:    Set the line type
        line_type = LINE_TYPE_BLANK;
    }

    /************************************************************************
     *  LINE_TYPE_AUIP
     ************************************************************************/

    //  Does this line contain AUIP (Amount-Unit-Ingredient-Preparation)
    else
    if( DECODE__is_auip( tmp_p ) == true )
    {
        //  YES:    Set the line type
        line_type = LINE_TYPE_AUIP;
    }

    /************************************************************************
     *  LINE_TYPE_TEXT
     ************************************************************************/

    //  There is something there and none of the above
    else
    {
        //  Set the line type
        line_type = LINE_TYPE_TEXT;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( line_type );
}

/****************************************************************************/
