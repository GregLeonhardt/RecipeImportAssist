/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019,2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'encode_ria' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ENCODE_RIA        ( "ALLOCATE STORAGE FOR ENCODE_RIA" )

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
#include <encode_ria_api.h>     //  API for all encode_ria_*        PUBLIC
#include "encode_ria_lib.h"     //  API for all ENCODE_RIA__*       PRIVATE
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
 *  Encode the recipe in 'RIA' format
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
encode_ria(
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
     *  Write the recipe to a file.
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  Recipe Start marker
    fprintf( out_file_fp, "#! START	RIA Recipe Format\n" );
    //-----------------------------------------------------------------------
    //  Empty Line
    fprintf( out_file_fp, " \n" );
    //-----------------------------------------------------------------------
    //  RECIPE ID:
    //      "A unique recipe identifier"
    fprintf( out_file_fp, "RECIPE ID:\n\t\"%s\"\n",
             recipe_p->recipe_id );
    //-----------------------------------------------------------------------
    //  NAME:
    //      "Recipe Name"
    fprintf( out_file_fp, "NAME:\n\t\"%s\"\n",
             recipe_p->name );
    //-----------------------------------------------------------------------
    //  AUTHOR:
    //      "The creator of the recipe"
    fprintf( out_file_fp, "AUTHOR:\n\t\"%s\"\n",
             recipe_p->author );
    //-----------------------------------------------------------------------
    //  COPYRIGHT:
    //      "nnnn (c) Copyright or (CC) Creative commons information"
    fprintf( out_file_fp, "COPYRIGHT:\n\t\"%s\"\n",
             recipe_p->copyright );
    //-----------------------------------------------------------------------
    //  DESCRIPTION:
    //      "Some text that describes the recipe."
    if ( recipe_p->description != NULL )
    {
        fprintf( out_file_fp, "DESCRIPTION:\r\n\t\"%s\"\n",
                 recipe_p->description );
    }
    //-----------------------------------------------------------------------
    //  APPLIANCE:
    //      "Oven" "Crockpot" ...
    if ( list_query_count( recipe_p->appliance ) != 0 )
    {
        fprintf( out_file_fp, "APPLIANCE:\n\t" );
        for( data_p = list_get_first( recipe_p->appliance );
             data_p != NULL;
             data_p = list_get_next( recipe_p->appliance, data_p ) )
        {
            fprintf( out_file_fp, "\"%s\" ", data_p );
        }
        //  Terminate the list with EOL.
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  CUISINE:
    //      "French" Italian" ...
    if ( list_query_count( recipe_p->cuisine ) != 0 )
    {
        fprintf( out_file_fp, "CUISINE:\n\t" );
        for( data_p = list_get_first( recipe_p->cuisine );
             data_p != NULL;
             data_p = list_get_next( recipe_p->cuisine, data_p ) )
        {
            fprintf( out_file_fp, "\"%s\" ", data_p );
        }
        //  Terminate the list with EOL.
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  OCCASION:
    //      "Easter"  "Halloween" ...
    if ( list_query_count( recipe_p->occasion ) != 0 )
    {
        fprintf( out_file_fp, "OCCASION:\n\t" );
        for( data_p = list_get_first( recipe_p->occasion );
             data_p != NULL;
             data_p = list_get_next( recipe_p->occasion, data_p ) )
        {
            fprintf( out_file_fp, "\"%s\" ", data_p );
        }
        //  Terminate the list with EOL.
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  COURSE:
    //      "Breakfast" "Appetizer" ...
    if ( list_query_count( recipe_p->course ) != 0 )
    {
        fprintf( out_file_fp, "COURSE:\n\t" );
        for( data_p = list_get_first( recipe_p->course );
             data_p != NULL;
             data_p = list_get_next( recipe_p->course, data_p ) )
        {
            fprintf( out_file_fp, "\"%s\" ", data_p );
        }
        //  Terminate the list with EOL.
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  DIET:
    //      "Low-Fat" "Diabetic" ...
    if ( list_query_count( recipe_p->diet ) != 0 )
    {
        fprintf( out_file_fp, "DIET:\n\t" );
        for( data_p = list_get_first( recipe_p->diet );
             data_p != NULL;
             data_p = list_get_next( recipe_p->diet, data_p ) )
        {
            fprintf( out_file_fp, "\"%s\" ", data_p );
        }
        //  Terminate the list with EOL.
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  GROUP:
    //      "Something" "Else" ...
    if ( list_query_count( recipe_p->chapter ) != 0 )
    {
        fprintf( out_file_fp, "GROUP:\n\t" );
        for( data_p = list_get_first( recipe_p->chapter );
             data_p != NULL;
             data_p = list_get_next( recipe_p->chapter, data_p ) )
        {
            fprintf( out_file_fp, "\"%s\" ", data_p );
        }
        //  Terminate the list with EOL.
        fprintf( out_file_fp, "\n" );
    }
    //-----------------------------------------------------------------------
    //  SERVES:
    //      "nn"
    if ( recipe_p->serves != NULL )
    {
        fprintf( out_file_fp, "SERVES:\r\n\t\"%s\"\n",
                 recipe_p->serves );
    }
    //-----------------------------------------------------------------------
    //  MAKES:
    //      "nn" "somethings"
    if ( recipe_p->makes  != NULL )
    {
        fprintf( out_file_fp, "MAKES:\r\n\t\"%s\"",
                 recipe_p->makes );
        if ( recipe_p->makes_unit != NULL )
        {
            fprintf( out_file_fp, " \"%s\"\n",
                     recipe_p->makes_unit );
        }
        else
        {
            fprintf( out_file_fp, "\n" );
        }
    }
    //-----------------------------------------------------------------------
    //  PREP TIME:
    //      "0:00"
    if ( recipe_p->time_prep != NULL )
    {
        fprintf( out_file_fp, "PREP TIME:\r\n\t\"%s\"\n",
                 recipe_p->time_prep );
    }
    //-----------------------------------------------------------------------
    //  PRE-COOK TIME:
    //      "0:00"
    if ( recipe_p->time_rest != NULL )
    {
        fprintf( out_file_fp, "PRE-COOK TIME:\r\n\t\"%s\"\n",
                 recipe_p->time_rest );
    }
    //-----------------------------------------------------------------------
    //  COOK TIME:
    //      "0:00"
    if ( recipe_p->time_cook != NULL )
    {
        fprintf( out_file_fp, "COOK TIME:\r\n\t\"%s\"\n",
                 recipe_p->time_cook );
    }
    //-----------------------------------------------------------------------
    //  REST TIME:
    //      "0:00"
    if ( recipe_p->time_rest != NULL )
    {
        fprintf( out_file_fp, "PRE-COOK TIME:\r\n\t\"%s\"\n",
                 recipe_p->time_rest );
    }
    //-----------------------------------------------------------------------
    //  SKILL LEVEL:
    //      "0-5"
    if ( recipe_p->skill != NULL )
    {
        fprintf( out_file_fp, "SKILL LEVEL:\r\n\t\"%s\"\n",
                 recipe_p->skill );
    }
    //-----------------------------------------------------------------------
    //  RATING:
    //      "0-5"
    if ( recipe_p->rating != NULL )
    {
        fprintf( out_file_fp, "RATING:\r\n\t\"%s\"\n",
                 recipe_p->rating );
    }
    //-----------------------------------------------------------------------
    //  PUBLISHED FORMAT:
    //      "MMF"
    if (    ( recipe_p->dir_name         != NULL )
         || ( recipe_p->file_name        != NULL )
         || ( recipe_p->file_size        != NULL )
         || ( recipe_p->file_date_time   != NULL )

         || ( recipe_p->group_from       != NULL )
         || ( recipe_p->posted_by        != NULL )
         || ( recipe_p->posted_subject   != NULL )

         || ( recipe_p->formatted_by     != NULL )
         || ( recipe_p->edited_by        != NULL ) )
    {
        if ( recipe_p->source_format == NULL )
        {
            fprintf( out_file_fp, "PUBLISHED FORMAT:\r\n\t\"TXT\"\n" );
        }
        else
        {
            fprintf( out_file_fp, "PUBLISHED FORMAT:\r\n\t\"%s\"\n",
                     recipe_p->source_format );
        }
        //-------------------------------------------------------------------
        //  IMPORTED FROM:
        //      "FileDirectory: "
        //      "FileName: "
        //      "FileSize: "
        //      "FileDateTime: "
        if (    ( recipe_p->dir_name         != NULL )
             || ( recipe_p->file_name        != NULL )
             || ( recipe_p->file_size        != NULL )
             || ( recipe_p->file_date_time   != NULL ) )
        {
            fprintf( out_file_fp, "ImportedFrom:\n" );
            if ( recipe_p->dir_name != NULL )
            {
                fprintf( out_file_fp, "      \"FileDirectory: %s\"\n",
                         recipe_p->dir_name );
            }
            if ( recipe_p->file_name != NULL )
            {
                fprintf( out_file_fp, "      \"FileName:      %s\"\n",
                         recipe_p->file_name );
            }
            if ( recipe_p->file_size != NULL )
            {
                fprintf( out_file_fp, "      \"FileSize:      %s\"\n",
                         recipe_p->file_size );
            }
            if ( recipe_p->file_date_time != NULL )
            {
                fprintf( out_file_fp, "      \"FileDateTime:  %s\"\n",
                         recipe_p->file_date_time );
            }
        }
        //-------------------------------------------------------------------
        //  E-MAIL:
        //      "Group: "
        //      "Subject: "
        //      "PostedBy: "
        //      "DateTime: "
        if (    ( recipe_p->group_from       != NULL )
             || ( recipe_p->group_subject    != NULL )
             || ( recipe_p->posted_by        != NULL )
             || ( recipe_p->posted_subject   != NULL ) )
        {
            fprintf( out_file_fp, "E-MAIL:\n" );
            //---------------------------------------------------------------
            //  <GROUPNAME>
            if ( recipe_p->group_from != NULL )
            {
                fprintf( out_file_fp, "    \"Group:\t%s\"\n",
                         recipe_p->group_from );
            }
            //---------------------------------------------------------------
            //  <GROUPSUBJECT>
            if ( recipe_p->group_subject != NULL )
            {
                fprintf( out_file_fp, "      \"Subject:\t%s\"\n",
                         recipe_p->group_subject );
            }
            //---------------------------------------------------------------
            //  <POSTEDBY>
            if ( recipe_p->posted_by != NULL )
            {
                fprintf( out_file_fp, "      \"PostedBy:\t%s\"\n",
                         recipe_p->posted_by );
            }
            //---------------------------------------------------------------
            //  <POSTEDDATE>
            if ( recipe_p->posted_date != NULL )
            {
                fprintf( out_file_fp, "      \"DateTime:\t%s\"\n",
                         recipe_p->posted_date );
            }
        }
    }
    //-----------------------------------------------------------------------
    //  AUIP:
    //      "I: Standard AUIP"
    //      "S: Substitutions"
    //      "T: Additional text."
    //      "R: {recipe-id}"
    if ( list_query_count( recipe_p->ingredient ) > 0 )
    {
        fprintf( out_file_fp, "AUIP:\n" );
        for( auip_p = list_get_first( recipe_p->ingredient );
             auip_p != NULL;
             auip_p = list_get_next( recipe_p->ingredient, auip_p ) )
        {
            if ( auip_p->type_p == NULL )
            {
                fprintf( out_file_fp, "    \"I:\t" );
            }
            else
            {
                fprintf( out_file_fp, "    \"%s\t",
                        auip_p->type_p );
            }
            //  Is there an amount ?
            if ( auip_p->amount_p != NULL )
            {
                fprintf( out_file_fp, "%s",
                         auip_p->amount_p );
            }
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p != NULL )
            {
                fprintf( out_file_fp, " %s",
                         auip_p->unit_p );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p != NULL )
            {
                fprintf( out_file_fp, " %s",
                         auip_p->ingredient_p );
            }
            //  Is there a preparation method ?
            if ( auip_p->preparation_p != NULL )
            {
                fprintf( out_file_fp, " %s",
                         auip_p->preparation_p );
            }
            fprintf( out_file_fp, "\"\n" );
        }
    }
    //-----------------------------------------------------------------------
    //  <DIRECTIONS>
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        fprintf( out_file_fp, "DIRECTIONS:\n" );
        for( data_p = list_get_first( recipe_p->directions );
             data_p != NULL;
             data_p = list_get_next( recipe_p->directions, data_p ) )
        {
            fprintf( out_file_fp, "%s\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  <NOTES>
    if ( list_query_count( recipe_p->notes ) > 0 )
    {
        fprintf( out_file_fp, "NOTES:\n" );
        for( data_p = list_get_first( recipe_p->notes );
             data_p != NULL;
             data_p = list_get_next( recipe_p->notes, data_p ) )
        {
            fprintf( out_file_fp, "%s\n",
                     data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  </RIA>
    fprintf( out_file_fp, "#! END	RIA Recipe Format\n\n" );
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

/****************************************************************************/
