/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019,2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'encode_mxp' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ENCODE_MXP        ( "ALLOCATE STORAGE FOR ENCODE_MXP" )

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
#include <encode_mxp_api.h>     //  API for all encode_mxp_*        PUBLIC
#include "encode_mxp_lib.h"     //  API for all ENCODE_MXP__*       PRIVATE
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

/****************************************************************************/
/**
 *  Encode the recipe in 'TXT' format
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
encode_mxp(
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
    /**
     * @param category_count    Number of categories written            */
    int                             category_count;

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
              "%s/%s/%s/%s.txt", out_dir_name_p, DIR_FORMATTED,
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
     *  Write TXT-XML to the file
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  TXT Recipe start tag
    //-----------------------------------------------------------------------

    fprintf( out_file_fp, "                 * Exported from MasterCook *\n\n" );

    //-----------------------------------------------------------------------
    //  NAME
    //-----------------------------------------------------------------------

    fprintf( out_file_fp, "                %s\n\n", recipe_p->name );

    //-----------------------------------------------------------------------
    //  AUTHOR:
    //-----------------------------------------------------------------------

    fprintf( out_file_fp, "Recipe By        : " );
    if ( recipe_p->author != NULL )
    {
        fprintf( out_file_fp, "%s\n", recipe_p->author );
    }
    else
    {
        fprintf( out_file_fp, "\n" );
    }

    //-----------------------------------------------------------------------
    //  Serves:
    //-----------------------------------------------------------------------

    fprintf( out_file_fp, "Serving Size     : " );
    if ( recipe_p->serves != NULL )
    {
        fprintf( out_file_fp, "3%s  ", recipe_p->serves );
    }
    else
    {
        fprintf( out_file_fp, "   " );
    }

    //-----------------------------------------------------------------------
    //  TIME PREP:
    //-----------------------------------------------------------------------

    fprintf( out_file_fp, "Preparation Time : " );
    if ( recipe_p->time_prep != NULL )
    {
        fprintf( out_file_fp, "%s\n", recipe_p->time_prep );
    }
    else
    {
        fprintf( out_file_fp, "0:00\n" );
    }

    //-----------------------------------------------------------------------
    //  CATEGORIES
    //-----------------------------------------------------------------------

    fprintf( out_file_fp, "Categories       : " );

    //  Initialize the number of categories written
    category_count = 0;

    //  ####################
    //  type=CUISINE
    //  ####################
    if ( list_query_count( recipe_p->cuisine ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->cuisine );
             data_p != NULL;
             data_p = list_get_next( recipe_p->cuisine, data_p ) )
        {
            //  Do we need to pad this one ?
            if (    ( ( category_count % 2 ) != 0 )
                 || (   category_count       == 0 ) )
            {
                //  NO:
                fprintf( out_file_fp, "%-32s", data_p );
            }
            else
            {
                //  YES:
                fprintf( out_file_fp, "             %-32s", data_p );
            }
            //  Do we need a CR here ?
            if ( ( ( ++category_count ) % 2 ) == 0 )
            {
                //  YES:
                fprintf( out_file_fp, "\n" );
            }
        }
    }
    //  ####################
    //  type=OCCASION
    //  ####################
    if ( list_query_count( recipe_p->occasion ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->occasion );
             data_p != NULL;
             data_p = list_get_next( recipe_p->occasion, data_p ) )
        {
            //  Do we need to pad this one ?
            if (    ( ( category_count % 2 ) != 0 )
                 || (   category_count       == 0 ) )
            {
                //  NO:
                fprintf( out_file_fp, "%-32s", data_p );
            }
            else
            {
                //  YES:
                fprintf( out_file_fp, "                   %-32s", data_p );
            }
            //  Do we need a CR here ?
            if ( ( ( ++category_count ) % 2 ) == 0 )
            {
                //  YES:
                fprintf( out_file_fp, "\n" );
            }
        }
    }

    //  ####################
    //  type=COURSE
    //  ####################
    if ( list_query_count( recipe_p->course ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->course );
             data_p != NULL;
             data_p = list_get_next( recipe_p->course, data_p ) )
        {
            //  Do we need to pad this one ?
            if (    ( ( category_count % 2 ) != 0 )
                 || (   category_count       == 0 ) )
            {
                //  NO:
                fprintf( out_file_fp, "%-32s", data_p );
            }
            else
            {
                //  YES:
                fprintf( out_file_fp, "                   %-32s", data_p );
            }
            //  Do we need a CR here ?
            if ( ( ( ++category_count ) % 2 ) == 0 )
            {
                //  YES:
                fprintf( out_file_fp, "\n" );
            }
        }
    }

    //  ####################
    //  type=DIET
    //  ####################

    if ( list_query_count( recipe_p->diet ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->diet );
             data_p != NULL;
             data_p = list_get_next( recipe_p->diet, data_p ) )
        {
            //  Do we need to pad this one ?
            if (    ( ( category_count % 2 ) != 0 )
                 || (   category_count       == 0 ) )
            {
                //  NO:
                fprintf( out_file_fp, "%-32s", data_p );
            }
            else
            {
                //  YES:
                fprintf( out_file_fp, "                   %-32s", data_p );
            }
            //  Do we need a CR here ?
            if ( ( ( ++category_count ) % 2 ) == 0 )
            {
                //  YES:
                fprintf( out_file_fp, "\n" );
            }
        }
    }

    //  ####################
    //  type=APPLIANCE
    //  ####################

    if ( list_query_count( recipe_p->appliance ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->appliance );
             data_p != NULL;
             data_p = list_get_next( recipe_p->appliance, data_p ) )
        {
            //  Do we need to pad this one ?
            if (    ( ( category_count % 2 ) != 0 )
                 || (   category_count       == 0 ) )
            {
                //  NO:
                fprintf( out_file_fp, "%-32s", data_p );
            }
            else
            {
                //  YES:
                fprintf( out_file_fp, "                   %-32s", data_p );
            }
            //  Do we need a CR here ?
            if ( ( ( ++category_count ) % 2 ) == 0 )
            {
                //  YES:
                fprintf( out_file_fp, "\n" );
            }
        }
    }

    //  ####################
    //  type=CHAPTER
    //  ####################

    if ( list_query_count( recipe_p->chapter ) != 0 )
    {
        for( data_p = list_get_first( recipe_p->chapter );
             data_p != NULL;
             data_p = list_get_next( recipe_p->chapter, data_p ) )
        {
            //  Do we need to pad this one ?
            if (    ( ( category_count % 2 ) != 0 )
                 || (   category_count       == 0 ) )
            {
                //  NO:
                fprintf( out_file_fp, "%-32s", data_p );
            }
            else
            {
                //  YES:
                fprintf( out_file_fp, "                   %-32s", data_p );
            }
            //  Do we need a CR here ?
            if ( ( ( ++category_count ) % 2 ) == 0 )
            {
                //  YES:
                fprintf( out_file_fp, "\n" );
            }
        }
    }
    //  Do we need an extra carrage return ?
    if (    ( ( category_count % 1 ) != 0 )
         || (   category_count       != 0 ) )
    {
        //  YES:
        fprintf( out_file_fp, "\n" );
    }
    //  Add a blank line here
    fprintf( out_file_fp, "\n" );

    //-----------------------------------------------------------------------
    //  <AUIP>
    //-----------------------------------------------------------------------

    //  Write the AUIP header
    fprintf( out_file_fp, "Amount   Measure      Ingredient -- Preparation Method\n" );
    fprintf( out_file_fp, "-------- ------------ --------------------------------\n" );

    if ( list_query_count( recipe_p->ingredient ) > 0 )
    {
        for( auip_p = list_get_first( recipe_p->ingredient );
             auip_p != NULL;
             auip_p = list_get_next( recipe_p->ingredient, auip_p ) )
        {
            //  Is there an amount ?
            if ( auip_p->amount_p != NULL )
            {
                fprintf( out_file_fp, "%-8s",     auip_p->amount_p );
            }
            else
            {
                fprintf( out_file_fp, "        " );
            }
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p != NULL )
            {
                fprintf( out_file_fp, "%-12s",     auip_p->unit_p );
            }
            else
            {
                fprintf( out_file_fp, "            " );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p != NULL )
            {
                fprintf( out_file_fp, "%s",     auip_p->ingredient_p );
            }
            //  Is there a preparation method ?
            if ( auip_p->preparation_p != NULL )
            {
                fprintf( out_file_fp, " %s",     auip_p->preparation_p );
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
        //  Blank Line
        fprintf( out_file_fp, "\n" );
    }
    else
    {
        //  No AUIP
        fprintf( out_file_fp, "** TEXT ONLY - NO INGREDIENTS **\n\n" );
    }

    //-----------------------------------------------------------------------
    //  DIRECTIONS
    //-----------------------------------------------------------------------

    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->directions );
             data_p != NULL;
             data_p = list_get_next( recipe_p->directions, data_p ) )
        {
            //  Is there something to write ?
            if ( text_is_blank_line( data_p ) != true )
            {
                //  YES:    Write the directions text.
                fprintf( out_file_fp, "%s\n\n", data_p );
            }
        }
    }

    //-----------------------------------------------------------------------
    //  DESCRIPTION:
    //-----------------------------------------------------------------------

    if ( recipe_p->description != NULL )
    {
        fprintf( out_file_fp, "Description:\n" );
        fprintf( out_file_fp, "\"%s\"\n", recipe_p->description );
    }

    //-----------------------------------------------------------------------
    //  Source:
    //-----------------------------------------------------------------------

    if ( recipe_p->source != NULL )
    {
        fprintf( out_file_fp, "Source:\n" );
        fprintf( out_file_fp, "\"%s\"\n", recipe_p->source );
    }

    //-----------------------------------------------------------------------
    //  Imported From:
    //-----------------------------------------------------------------------

    fprintf( out_file_fp, "S(Imported From):\n" );

    if ( recipe_p->posted_by != NULL )
    {
        fprintf( out_file_fp, "\"%s | %s | %s | %s\"\n",
                recipe_p->group_from,
                recipe_p->group_subject,
                recipe_p->posted_by,
                recipe_p->posted_date );
    }
    else
    {
        fprintf( out_file_fp, "\"%s | %s\"\n",
                        recipe_p->file_name,
                        recipe_p->file_date_time );
    }

    //-----------------------------------------------------------------------
    //  COPYRIGHT:
    //-----------------------------------------------------------------------

    fprintf( out_file_fp, "Copyright:\n" );
    fprintf( out_file_fp, "\"%s\"\n", recipe_p->copyright );

    //-----------------------------------------------------------------------
    //  Yield:
    //-----------------------------------------------------------------------

    if ( recipe_p->makes != NULL )
    {
        fprintf( out_file_fp, "Yield:\n" );
        fprintf( out_file_fp, "\"%s", recipe_p->makes );
    }
    //  Is there a unit of measurement ?
    if ( recipe_p->makes_unit != NULL )
    {
        //  YES:    Write it
        fprintf( out_file_fp, " %s\"\n", recipe_p->makes_unit );
    }
    else
    {
        //  NO:     End-of-Line
        fprintf( out_file_fp, "\"\n" );
    }

    //-----------------------------------------------------------------------
    //  TIME COOK:
    //-----------------------------------------------------------------------

    if ( recipe_p->time_cook != NULL )
    {
    fprintf( out_file_fp, "T(Cook Time):" );
        fprintf( out_file_fp, "\"%s\"\n", recipe_p->time_cook );
    }

    //-----------------------------------------------------------------------
    //  Ratings:
    //-----------------------------------------------------------------------

    if (    ( recipe_p->skill  != NULL )
         || ( recipe_p->rating != NULL ) )
    {
        fprintf( out_file_fp, "RATINGS           : " );
        if ( recipe_p->skill != NULL )
        {
            fprintf( out_file_fp, "30%s\n", recipe_p->skill );
        }
        if ( recipe_p->rating != NULL )
        {
            fprintf( out_file_fp, "%s\n", recipe_p->rating );
        }
    }

    //-----------------------------------------------------------------------
    //  Recipe End:
    //-----------------------------------------------------------------------
    fprintf( out_file_fp, "                    - - - - - - - - - - - - - - - - - -\n" );

    //-----------------------------------------------------------------------
    //  NOTES:
    if ( list_query_count( recipe_p->notes ) > 0 )
    {
        fprintf( out_file_fp, "NOTES:\n" );

        for( data_p = list_get_first( recipe_p->notes );
             data_p != NULL;
             data_p = list_get_next( recipe_p->notes, data_p ) )
        {
            fprintf( out_file_fp, "%s\n", data_p );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Close the output file.
    file_close( out_file_fp );

    //  Free all storage consumed by the recipe.
    recipe_kill( recipe_p );

    //  DONE!
}

/****************************************************************************/
