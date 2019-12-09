/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017,2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'encode_rxf' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ENCODE_RXF        ( "ALLOCATE STORAGE FOR ENCODE_RXF" )

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
#include <encode_api.h>         //  API for all encode_*            PUBLIC
                                //*******************************************
#include <encode_rxf_api.h>     //  API for all encode_rxf_*        PUBLIC
#include "encode_rxf_lib.h"     //  API for all ENCODE_RXF__*       PRIVATE
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
#define DIR_FORMATTED           "Recipes_Formatted"
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

#if 0
/****************************************************************************/
/**
 *  Encode the recipe in 'RXF' format
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
encode_rxf(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param out_name          Encoded output file name                    */
    char                            out_name[ FILE_NAME_L + 1 ];
    /**
     * @param input_file_fp     Output File pointer                         */
    FILE                        *   out_file_fp;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                        *   data_p;
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t              *   auip_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Build the source file path & name
    snprintf( out_name, sizeof( out_name ),
            "%s/%s", recipe_p->dir_name, recipe_p->file_name );

    //  Change the file extention
    data_p = strrchr( out_name, '.' );
    if ( data_p != NULL ) data_p[ 0 ] = '\0';
    strncat( out_name, ".rxf", sizeof( out_name ) );

    //  Open the input file
    out_file_fp = file_open_append( out_name );

    /************************************************************************
     *  Defaults
     ************************************************************************/

    //-----------------------------------------------------------------------
    if ( recipe_p->name == NULL )
    {
        //  Create a default string
        recipe_p->name = text_copy_to_new( "No Recipe Name Found" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->recipe_id == NULL )
    {
        //  Create a default string
        recipe_p->recipe_id = text_copy_to_new( "000000000000000000000" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
   }
    //-----------------------------------------------------------------------
    if ( recipe_p->file_name == NULL )
    {
        //  Create a default string
        recipe_p->file_name = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
  }
    //-----------------------------------------------------------------------
    if ( recipe_p->file_date_time == NULL )
    {
        //  Create a default string
        recipe_p->file_date_time = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
 }
    //-----------------------------------------------------------------------
    if ( recipe_p->file_size == NULL )
    {
        //  Create a default string
        recipe_p->file_size = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->author == NULL )
    {
        //  Create a default string
        recipe_p->author = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->group_from == NULL )
    {
        //  Create a default string
        recipe_p->group_from = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
   }
    //-----------------------------------------------------------------------
    if ( recipe_p->copyright == NULL )
    {
        //  Create a default string
        recipe_p->copyright = text_copy_to_new( "Copyright (c) and related "
                "rights reserved via Creative Commons [CC BY-SA 4.0]\n\t"
                "https://creativecommons.org/licenses/by-sa/4.0/" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->imported_from == NULL )
    {
        //  Create a default string
        recipe_p->imported_from = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->posted_by == NULL )
    {
        //  Create a default string
        recipe_p->posted_by = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
   }
    //-----------------------------------------------------------------------
    if ( recipe_p->posted_date == NULL )
    {
        //  Create a default string
        recipe_p->posted_date = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
  }
    //-----------------------------------------------------------------------
    if ( recipe_p->posted_subject == NULL )
    {
        //  Create a default string
        recipe_p->posted_subject = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->edited_by == NULL )
    {
        //  Create a default string
        recipe_p->edited_by = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->formatted_by == NULL )
    {
        //  Create a default string
        recipe_p->formatted_by = text_copy_to_new( "UNKNOWN" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
   }
    //-----------------------------------------------------------------------
    if ( recipe_p->serves == NULL )
    {
        //  Create a default string
        recipe_p->serves = text_copy_to_new( "0" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->servings_size == NULL )
    {
        //  Create a default string
        recipe_p->servings_size = text_copy_to_new( "Servings" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->makes == NULL )
    {
        //  Create a default string
        recipe_p->makes = text_copy_to_new( "0" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->makes_unit == NULL )
    {
        //  Create a default string
        recipe_p->makes_unit = text_copy_to_new( " " );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->time_prep == NULL )
    {
        //  Create a default string
        recipe_p->time_prep = text_copy_to_new( "0:00" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->time_cook == NULL )
    {
        //  Create a default string
        recipe_p->time_cook = text_copy_to_new( "0:00" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->time_resting == NULL )
    {
        //  Create a default string
        recipe_p->time_resting = text_copy_to_new( "0:00" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->time_total == NULL )
    {
        //  Create a default string
        recipe_p->time_total = text_copy_to_new( "0:00" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->description == NULL )
    {
        //  Create a default string
        recipe_p->description = text_copy_to_new( " " );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->based_on == NULL )
    {
        //  Create a default string
        recipe_p->based_on = text_copy_to_new( " " );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->serve_with == NULL )
    {
        //  Create a default string
        recipe_p->serve_with = text_copy_to_new( " " );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->rating == NULL )
    {
        //  Create a default string
        recipe_p->rating = text_copy_to_new( "0" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->skill == NULL )
    {
        //  Create a default string
        recipe_p->skill = text_copy_to_new( "0" );

        log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------

    /************************************************************************
     *  Function
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  Start of recipe marker
    fputs( "#! Recipe eXchange Format - START\n", out_file_fp );
    //-----------------------------------------------------------------------
    //  recipe_p->name
    fprintf( out_file_fp,
             "NAME:\n\t%s\n", recipe_p->name );
    //-----------------------------------------------------------------------
    //  recipe_p->recipe_id
    fprintf( out_file_fp,
             "RECIPE-ID:\n\t%s\n", recipe_p->recipe_id );
    //-----------------------------------------------------------------------
    //  recipe_p->dir_name
    fprintf( out_file_fp,
             "IMPORT_FILE_INFO:\n\t%s\n\t%s\n\t%s\n",
                recipe_p->file_name,
                recipe_p->file_date_time,
                recipe_p->file_size );
    //-----------------------------------------------------------------------
    //  recipe_p->source_format
    fprintf( out_file_fp,
             "SOURCE-FORMAT:\n\t%s\n", recipe_p->source_format );
    //-----------------------------------------------------------------------
    //  recipe_p->author
    fprintf( out_file_fp,
             "AUTHOR:\n\t%s\n", recipe_p->author );
    //-----------------------------------------------------------------------
    //  recipe_p->group_from
    fprintf( out_file_fp,
             "SOURCE:\n\t%s\n", recipe_p->group_from );
    //-----------------------------------------------------------------------
    //  recipe_p->copyright
    fprintf( out_file_fp,
             "COPYRIGHT:\n\t%s\n", recipe_p->copyright );
    //-----------------------------------------------------------------------
    //  recipe_p->imported_from
    fprintf( out_file_fp,
             "IMPORTED FROM:\n\t%s\n", recipe_p->imported_from );
    //-----------------------------------------------------------------------
    //  recipe_p->posted_by
    fprintf( out_file_fp,
             "POSTED-BY:\n\t%s\n", recipe_p->posted_by );
    //-----------------------------------------------------------------------
    //  recipe_p->posted_date
    fprintf( out_file_fp,
             "POSTED-DATE:\n\t%s\n", recipe_p->posted_date );
    //-----------------------------------------------------------------------
    //  recipe_p->posted_subject
    fprintf( out_file_fp,
             "POSTED-SUBJECT:\n\t%s\n", recipe_p->posted_subject );
    //-----------------------------------------------------------------------
    //  recipe_p->edited_by
    fprintf( out_file_fp,
             "EDITED-BY:\n\t%s\n", recipe_p->edited_by );
    //-----------------------------------------------------------------------
    //  recipe_p->formatted_by
    fprintf( out_file_fp,
             "FORMATTED-BY:\n\t%s\n", recipe_p->formatted_by );
    //-----------------------------------------------------------------------
    //  recipe_p->serves
    fprintf( out_file_fp,
             "SERVES:\n\t%s\n", recipe_p->serves );
    //-----------------------------------------------------------------------
    //  recipe_p->servings_size
    fprintf( out_file_fp,
             "SERVINGS-SIZE:\n\t%s\n", recipe_p->servings_size );
    //-----------------------------------------------------------------------
    //  recipe_p->makes
    fprintf( out_file_fp,
             "MAKES:\n\t%s\n", recipe_p->makes );
    //-----------------------------------------------------------------------
    //  recipe_p->makes_unit
    fprintf( out_file_fp,
             "MAKES-UNIT:\n\t%s\n", recipe_p->makes_unit );
    //-----------------------------------------------------------------------
    //  recipe_p->time_prep
    fprintf( out_file_fp,
             "TIME-PREPERATION:\n\t%s\n", recipe_p->time_prep );
    //-----------------------------------------------------------------------
    //  recipe_p->time_cook
    fprintf( out_file_fp,
             "TIME-COOK:\n\t%s\n", recipe_p->time_cook );
    //-----------------------------------------------------------------------
    //  recipe_p->time_resting
    fprintf( out_file_fp,
             "TIME-RESTING:\n\t%s\n", recipe_p->time_resting );
    //-----------------------------------------------------------------------
    //  recipe_p->time_total
    fprintf( out_file_fp,
             "TIME-TOTAL:\n\t%s\n", recipe_p->time_total );
    //-----------------------------------------------------------------------
    //  recipe_p->description
    fprintf( out_file_fp,
             "DESCRIPTION:\n\t%s\n", recipe_p->description );
    //-----------------------------------------------------------------------
    //  recipe_p->based_on
    fprintf( out_file_fp,
             "BASED-ON:\n\t%s\n", recipe_p->based_on );
    //-----------------------------------------------------------------------
    //  recipe_p->serve_with
    fprintf( out_file_fp,
             "SERVE-WITH:\n\t%s\n", recipe_p->serve_with );
    //-----------------------------------------------------------------------
    //  recipe_p->rating
    fprintf( out_file_fp,
             "RATING:\n\t%s\n", recipe_p->rating );
    //-----------------------------------------------------------------------
    //  recipe_p->skill
    fprintf( out_file_fp,
             "SKILL:\n\t%s\n", recipe_p->skill );
    //-----------------------------------------------------------------------
    //  recipe_p->appliance
    fprintf( out_file_fp, "APPLIANCE:\n" );
    if ( list_query_count( recipe_p->appliance ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->appliance );
             data_p != NULL;
             data_p = list_get_next( recipe_p->appliance, data_p ) )
        {
            fprintf( out_file_fp, "\t%s\n", data_p );
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  recipe_p->diet
    fprintf( out_file_fp, "DIET:\n" );
    if ( list_query_count( recipe_p->diet ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->diet );
             data_p != NULL;
             data_p = list_get_next( recipe_p->diet, data_p ) )
        {
            fprintf( out_file_fp, "\t%s\n", data_p );
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  recipe_p->course
    fprintf( out_file_fp, "COURSE:\n" );
    if ( list_query_count( recipe_p->course ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->course );
             data_p != NULL;
             data_p = list_get_next( recipe_p->course, data_p ) )
        {
            fprintf( out_file_fp, "\t%s\n", data_p );
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  recipe_p->cuisine
    fprintf( out_file_fp, "CUISINE:\n" );
    if ( list_query_count( recipe_p->cuisine ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->cuisine );
             data_p != NULL;
             data_p = list_get_next( recipe_p->cuisine, data_p ) )
        {
            fprintf( out_file_fp, "\t%s\n", data_p );
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  recipe_p->occasion
    fprintf( out_file_fp, "OCCASION:\n" );
    if ( list_query_count( recipe_p->occasion ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->occasion );
             data_p != NULL;
             data_p = list_get_next( recipe_p->occasion, data_p ) )
        {
            fprintf( out_file_fp, "\t%s\n", data_p );
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  recipe_p->chapter
    fprintf( out_file_fp, "CHAPTER:\n" );
    if ( list_query_count( recipe_p->chapter ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->chapter );
             data_p != NULL;
             data_p = list_get_next( recipe_p->chapter, data_p ) )
        {
            fprintf( out_file_fp, "\t%s\n", data_p );
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  recipe_p->ingredient
    fprintf( out_file_fp, "INGREDIENTS:\n" );
    if ( list_query_count( recipe_p->ingredient ) > 0 )
    {
        for( auip_p = list_get_first( recipe_p->ingredient );
             auip_p != NULL;
             auip_p = list_get_next( recipe_p->ingredient, auip_p ) )
        {
            //  Is there an amount ?
            if ( auip_p->amount_p == NULL )
            {
                //  NO:     Use the filler
                auip_p->amount_p = text_copy_to_new( " " );

                log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
            }
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p == NULL )
            {
                //  NO:     Use the filler
                auip_p->unit_p = text_copy_to_new( " " );

                log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p == NULL )
            {
                //  NO:     Use the filler
                auip_p->ingredient_p = text_copy_to_new( " " );

                log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
            }
            //  Is there a preparation method ?
            if ( auip_p->preparation_p == NULL )
            {
                //  NO:     Use the filler
                auip_p->preparation_p = text_copy_to_new( " " );

                log_write( MID_DEBUG_1, "encode_rxf_lib.c", "Line: %d\n", __LINE__ );
            }
            //  Is there a preparation method here ?
            if ( text_is_blank_line( auip_p->preparation_p ) == false )
            {
                //  YES:
                fprintf( out_file_fp, "\t%-8s %-32s %s %s\n",
                         auip_p->amount_p,
                         auip_p->unit_p,
                         auip_p->ingredient_p,
                         auip_p->preparation_p );
            }
            else
            {
                //  NO:
                fprintf( out_file_fp, "\t%-8s %-32s %s\n",
                         auip_p->amount_p,
                         auip_p->unit_p,
                         auip_p->ingredient_p );
            }
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  recipe_p->directions
    fprintf( out_file_fp, "DIRECTIONS:\n" );
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->directions );
             data_p != NULL;
             data_p = list_get_next( recipe_p->directions, data_p ) )
        {
            fprintf( out_file_fp, "\t%s\n", data_p );
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  recipe_p->notes
    fprintf( out_file_fp, "NOTES:\n" );
    if ( list_query_count( recipe_p->notes ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->notes );
             data_p != NULL;
             data_p = list_get_next( recipe_p->notes, data_p ) )
        {
            fprintf( out_file_fp, "\t%s\n", data_p );
        }
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  End of recipe marker
    fputs( "#! Recipe eXchange Format - END\n\n", out_file_fp );

    //  Close the output file.
    file_close( out_file_fp );

    //  Free all storage consumed by the recipe.
    recipe_kill( recipe_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/
}
#else
/****************************************************************************/
/**
 *  Encode the recipe in 'RXF' format
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
encode_rxf(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param out_name          Encoded output file name                    */
    char                            out_name[ FILE_NAME_L + 1 ];
    /**
     * @param subdirectory      Sub Directory name based on the Recipe-ID   */
    char                            subdirectory[ SUBDIRECTORY_L + 1 ];
    /**
     * @param input_file_fp     Output File pointer                         */
    FILE                        *   out_file_fp;
    /**
     * @param list_data_p       Pointer to the read data                    */
    char                        *   data_p;
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t              *   auip_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Build the subdirectory name
    snprintf( subdirectory, sizeof( subdirectory ),
              "%s", recipe_p->recipe_id );

    /************************************************************************
     *  Open the output file
     ************************************************************************/

    //  Start building the output name
    snprintf( out_name, sizeof( out_name ),
              "%s", out_dir_name_p );

    //  If the directory does not already exist, create it.
    file_dir_exist( out_name, true );

    //  Append DIR_FORMATTED to the output name
    snprintf( out_name, sizeof( out_name ),
              "%s/%s", out_dir_name_p, DIR_FORMATTED );

    //  If the directory does not already exist, create it.
    file_dir_exist( out_name, true );

    //  Append the subdirectory to the output name
    snprintf( out_name, sizeof( out_name ),
              "%s/%s/%s", out_dir_name_p, DIR_FORMATTED, subdirectory );

    //  If the directory does not already exist, create it.
    file_dir_exist( out_name, true );

    //  Append the file name to the output name
    snprintf( out_name, sizeof( out_name ),
              "%s/%s/%s/%s.rxf", out_dir_name_p, DIR_FORMATTED,
              subdirectory, recipe_p->recipe_id );

    /************************************************************************
     *  Open the output file
     ************************************************************************/

    //  Is this a recipe with no instructions ?
    if ( strncmp( recipe_p->recipe_id, "0000000000", 10 ) == 0 )
    {
        //  YES:    Append to whatever is already there
        out_file_fp = file_open_append( out_name );
    }
    else
    {
        //  NO:     Open a unique file for the output.
        out_file_fp = file_open_write_no_dup( out_name, "duplicate_" );
    }

    /************************************************************************
     *  Write RXF-XML to the file
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  DOCTYPE
    fprintf( out_file_fp, "<!DOCTYPE rxf SYSTEM rxf.dtd [\n" );
    //-----------------------------------------------------------------------
    //  <RXF>
    fprintf( out_file_fp, "<RXF Version=\"1.0.0\">\n" );
    //-----------------------------------------------------------------------
    //  <NAME>
    fprintf( out_file_fp, "  <Name>        %s  </Name>\n",
             recipe_p->name );
    //-----------------------------------------------------------------------
    //  <AUTHOR>
    fprintf( out_file_fp, "  <Author>      %s  </Author>\n",
             recipe_p->author );
    //-----------------------------------------------------------------------
    //  <ID>
    fprintf( out_file_fp, "  <ID>          %s  </ID>\n",
             recipe_p->recipe_id );
    //-----------------------------------------------------------------------
    //  <COPYRIGHT>
    fprintf( out_file_fp, "  <Copyright>   %s  </Copyright>\n",
             recipe_p->copyright );
    //-----------------------------------------------------------------------
    //  <OUTPUT>
    if (    ( recipe_p->serves     != NULL )
         || ( recipe_p->makes      != NULL )
         || ( recipe_p->makes_unit != NULL ) )
    {
        fprintf( out_file_fp, "  <Output>\n" );
        if ( recipe_p->serves != NULL )
        {
            fprintf( out_file_fp, "    <Serves>             %s  </Serves>\n",
                     recipe_p->serves );
        }
        if ( recipe_p->makes  != NULL )
        {
            fprintf( out_file_fp, "    <Makes>              %s  </Makes>\n",
                     recipe_p->makes );
        }
        if ( recipe_p->makes_unit != NULL )
        {
            fprintf( out_file_fp, "    <MakesUnit>          %s  </MakesUnit>\n",
                     recipe_p->makes_unit );
        }
        fprintf( out_file_fp, "  </output>\n" );
    }
    //-----------------------------------------------------------------------
    //  <CATEGORIES type=APPLIANCE>
    if ( list_query_count( recipe_p->appliance ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->appliance );
             data_p != NULL;
             data_p = list_get_next( recipe_p->appliance, data_p ) )
        {
            fprintf( out_file_fp, "  <Categories type=\"Appliance\"  >"
                                  "  %s  </Categories>\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <CATEGORIES type=CUISINE>
    if ( list_query_count( recipe_p->cuisine ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->cuisine );
             data_p != NULL;
             data_p = list_get_next( recipe_p->cuisine, data_p ) )
        {
            fprintf( out_file_fp, "  <Categories type=\"Cuisine\"    >"
                                  "  %s  </Categories>\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <CATEGORIES type=OCCASION>
    if ( list_query_count( recipe_p->occasion ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->occasion );
             data_p != NULL;
             data_p = list_get_next( recipe_p->occasion, data_p ) )
        {
            fprintf( out_file_fp, "  <Categories type=\"Occasion\"   >"
                                  "  %s  </Categories>\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <CATEGORIES type=COURSE>
    if ( list_query_count( recipe_p->course ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->course );
             data_p != NULL;
             data_p = list_get_next( recipe_p->course, data_p ) )
        {
            fprintf( out_file_fp, "  <Categories type=\"Course\"     >"
                                  "  %s  </Categories>\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <CATEGORIES type=DIET>
    if ( list_query_count( recipe_p->diet ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->diet );
             data_p != NULL;
             data_p = list_get_next( recipe_p->diet, data_p ) )
        {
            fprintf( out_file_fp, "  <Categories type=\"Diet\"       >"
                                  "  %s  </Categories>\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <CATEGORIES type=GROUP>
    if ( list_query_count( recipe_p->chapter ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->chapter );
             data_p != NULL;
             data_p = list_get_next( recipe_p->chapter, data_p ) )
        {
            fprintf( out_file_fp, "  <Categories type=\"Group\"      >"
                                  "  %s  </Categories>\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <DESCRIPTION>
    if ( recipe_p->description != NULL )
    {
        fprintf( out_file_fp, "  <Description> %s  </Description>\n",
                 recipe_p->description );
    }
    //-----------------------------------------------------------------------
    //  <TIME type=PREP>
    if ( recipe_p->time_prep != NULL )
    {
        fprintf( out_file_fp, "  <Time type= \"Prep\">      %s  </Time>\n",
                 recipe_p->time_prep );
    }
    //-----------------------------------------------------------------------
    //  <TIME type=PRE-COOK>
    if ( recipe_p->time_rest != NULL )
    {
        fprintf( out_file_fp, "  <Time type= \"PreCook\">   %s  </Time>\n",
                 recipe_p->time_rest );
    }
    //-----------------------------------------------------------------------
    //  <TIME type=COOK>
    if ( recipe_p->time_cook != NULL )
    {
        fprintf( out_file_fp, "  <Time type= \"Cook\">      %s  </Time>\n",
                 recipe_p->time_cook );
    }
    //-----------------------------------------------------------------------
    //  <TIME type=POST-COOK>
    if ( recipe_p->time_rest != NULL )
    {
        fprintf( out_file_fp, "  <Time type= \"PostCook\">  %s  </Time>\n",
                 recipe_p->time_rest );
    }
    //-----------------------------------------------------------------------
    //  <RATING type=SKILL>
    if ( recipe_p->skill != NULL )
    {
        fprintf( out_file_fp, "  <Rating type= \"Skill\">   %s  </Rating>\n",
                 recipe_p->skill );
    }
    //-----------------------------------------------------------------------
    //  <RATING type=RATING>
    if ( recipe_p->rating != NULL )
    {
        fprintf( out_file_fp, "  <Rating type= \"Taste\">   %s  </Rating>\n",
                 recipe_p->rating );
    }
    //-----------------------------------------------------------------------
    //  <PUBLISHED format=(MXP|MMF|MX2|TXT)>
    if (    ( recipe_p->dir_name         != NULL )
         || ( recipe_p->file_name        != NULL )
         || ( recipe_p->file_size        != NULL )
         || ( recipe_p->file_date_time   != NULL )

         || ( recipe_p->group_from       != NULL )
//       || ( recipe_p->group_subject    != NULL )
         || ( recipe_p->posted_by        != NULL )
         || ( recipe_p->posted_subject   != NULL )

         || ( recipe_p->formatted_by     != NULL )
//       || ( recipe_p->formatted_date   != NULL )
         || ( recipe_p->edited_by        != NULL )
#if 0
         || ( recipe_p->edited_date      != NULL ) )
#else
                                                   )
#endif
    {
        if ( recipe_p->source_format == NULL )
        {
            fprintf( out_file_fp, "  <Published format=\"TXT\" >\n" );
        }
        else
        {
            fprintf( out_file_fp, "  <Published format=\"%s\">\n",
                     recipe_p->source_format );
        }
        //-------------------------------------------------------------------
        //  <IMPORTED_FROM>
        if (    ( recipe_p->dir_name         != NULL )
             || ( recipe_p->file_name        != NULL )
             || ( recipe_p->file_size        != NULL )
             || ( recipe_p->file_date_time   != NULL ) )
        {
            fprintf( out_file_fp, "    <ImportedFrom>\n" );
            if ( recipe_p->dir_name != NULL )
            {
                fprintf( out_file_fp, "      <FileDirectory>    %s  </FileDirectory>\n",
                         recipe_p->dir_name );
            }
            if ( recipe_p->file_name != NULL )
            {
                fprintf( out_file_fp, "      <FileName>         %s  </FileName>\n",
                         recipe_p->file_name );
            }
            if ( recipe_p->file_date_time != NULL )
            {
                fprintf( out_file_fp, "      <FileSize>         %s  </FileSize>\n",
                         recipe_p->file_size );
            }
            {
                fprintf( out_file_fp, "      <FileDateTime>     %s  </FileDateTime>\n",
                         recipe_p->file_date_time );
            }
            fprintf( out_file_fp, "    </ImportedFrom>\n" );
        }
        //-------------------------------------------------------------------
        //  <E-MAIL>
        if (    ( recipe_p->group_from       != NULL )
             || ( recipe_p->group_subject    != NULL )
             || ( recipe_p->posted_by        != NULL )
             || ( recipe_p->posted_subject   != NULL ) )
        {
            fprintf( out_file_fp, "    <EMail>\n" );
            //---------------------------------------------------------------
            //  <GROUPNAME>
            if ( recipe_p->group_from != NULL )
            {
                fprintf( out_file_fp, "      <GroupName>        %s  </GroupName>\n",
                         recipe_p->group_from );
            }
            //---------------------------------------------------------------
            //  <GROUPSUBJECT>
            if ( recipe_p->group_subject != NULL )
            {
                fprintf( out_file_fp, "      <GroupSubject>     %s  </GroupSubject>\n",
                         recipe_p->group_subject );
            }
            //---------------------------------------------------------------
            //  <GROUPDATE>
            if ( recipe_p->group_date != NULL )
            {
                fprintf( out_file_fp, "      <GroupDate>        %s  </GroupDate>\n",
                         recipe_p->group_date );
            }
            //---------------------------------------------------------------
            //  <POSTEDBY>
            if ( recipe_p->posted_by != NULL )
            {
                fprintf( out_file_fp, "      <PostedBy>         %s  </PostedBy>\n",
                         recipe_p->posted_by );
            }
            //---------------------------------------------------------------
            //  <POSTEDSUBJECT>
            if ( recipe_p->posted_subject != NULL )
            {
                fprintf( out_file_fp, "      <PostedSubject>    %s  </PostedSubject>\n",
                         recipe_p->posted_subject );
            }
            //---------------------------------------------------------------
            //  <POSTEDDATE>
            if ( recipe_p->posted_date != NULL )
            {
                fprintf( out_file_fp, "      <PostedDate>       %s  </PostedDate>\n",
                         recipe_p->posted_date );
            }
            //---------------------------------------------------------------
            fprintf( out_file_fp, "    </EMail>\n" );
        }

        //-------------------------------------------------------------------
        //  <EDITING>
        if (    ( recipe_p->formatted_by     != NULL )
//           || ( recipe_p->formatted_date   != NULL )
             || ( recipe_p->edited_by        != NULL )
#if 0
             || ( recipe_p->edited_date      != NULL ) )
#else
                                                      )
#endif

        {
            fprintf( out_file_fp, "    <Editing>\n" );
            if ( recipe_p->formatted_by != NULL )
            {
                fprintf( out_file_fp, "      <FormattedBy>      %s  </FormattedBy>\n",
                         recipe_p->formatted_by );
            }
#if 0
            if ( recipe_p->formatted_date != NULL )
            {
                fprintf( out_file_fp, "      <FormattedDate>    %s  </FormattedDate>\n",
                         recipe_p->formatted_date );
            }
#endif
            if ( recipe_p->edited_by != NULL )
            {
                fprintf( out_file_fp, "      <EditedBy>         %s  </EditedBy>\n",
                         recipe_p->edited_by );
            }
#if 0
            if ( recipe_p->edited_date != NULL )
            {
                fprintf( out_file_fp, "      <EditedDate>       %s  </EditedDate>\n",
                         recipe_p->edited_date );
            }
#endif
            fprintf( out_file_fp, "    </Editing>\n" );
        }
        //-------------------------------------------------------------------
        fprintf( out_file_fp, "  </published>\n" );
    }
    //-----------------------------------------------------------------------
    //  <AUIP>
    if ( list_query_count( recipe_p->ingredient ) > 0 )
    {
        for( auip_p = list_get_first( recipe_p->ingredient );
             auip_p != NULL;
             auip_p = list_get_next( recipe_p->ingredient, auip_p ) )
        {
            if ( auip_p->type_p == NULL )
            {
                fprintf( out_file_fp, "  <AUIP type=\"Ingredient\" >\n" );
            }
            else
            {
                fprintf( out_file_fp, "  <AUIP type=\"%s\" >\n",
                        auip_p->type_p );
            }
            //  Is there an amount ?
            if ( auip_p->amount_p != NULL )
            {
                fprintf( out_file_fp, "    <Amount>         %s  </Amount>\n",
                         auip_p->amount_p );
            }
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p != NULL )
            {
                fprintf( out_file_fp, "    <Unit>           %s  </Unit>\n",
                         auip_p->unit_p );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p != NULL )
            {
                fprintf( out_file_fp, "    <Ingredient>     %s  </Ingredient>\n",
                         auip_p->ingredient_p );
            }
            //  Is there a preparation method ?
            if ( auip_p->preparation_p != NULL )
            {
                fprintf( out_file_fp, "    <Preperation>    %s  </Preperation>\n",
                         auip_p->preparation_p );
            }
            fprintf( out_file_fp, "  </AUIP>\n" );
        }
    }
    //-----------------------------------------------------------------------
    //  <DIRECTIONS>
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->directions );
             data_p != NULL;
             data_p = list_get_next( recipe_p->directions, data_p ) )
        {
            fprintf( out_file_fp, "  <Directions>\n"
                                  "    %s\n"
                                  "  </Directions>\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <NOTES>
    if ( list_query_count( recipe_p->notes ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->notes );
             data_p != NULL;
             data_p = list_get_next( recipe_p->notes, data_p ) )
        {
            fprintf( out_file_fp, "  <Notes>\n"
                                  "    %s\n"
                                  "  </Notes>\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  </RXF>
    fprintf( out_file_fp, "</RXF>\n\n<!-- ######## ######## ######## -->\n\n" );
    //-----------------------------------------------------------------------

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Close the output file.
    file_close( out_file_fp );

    //  Free all storage consumed by the recipe.
    recipe_kill( recipe_p );

    //  DONE!
}
#endif

/****************************************************************************/
