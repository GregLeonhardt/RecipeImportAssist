/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019,2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'encode_txt' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_ENCODE_TXT        ( "ALLOCATE STORAGE FOR ENCODE_TXT" )

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
#include <encode_txt_api.h>     //  API for all encode_txt_*        PUBLIC
#include "encode_txt_lib.h"     //  API for all ENCODE_TXT__*       PRIVATE
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
encode_txt(
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
     *  Write TXT-XML to the file
     ************************************************************************/

    //-----------------------------------------------------------------------
    //  TXT Recipe start tag
    fprintf( out_file_fp, "@@@@@\n" );
    //-----------------------------------------------------------------------
    //  NAME
    fprintf( out_file_fp, "%s\n\n", recipe_p->name );
    //-----------------------------------------------------------------------
    //  Recipe-Id
    fprintf( out_file_fp, "%s\n\n", recipe_p->recipe_id );
    //-----------------------------------------------------------------------
    //  <AUIP>
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
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p != NULL )
            {
                fprintf( out_file_fp, "%-12s",     auip_p->unit_p );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p != NULL )
            {
                fprintf( out_file_fp, "%s",     auip_p->ingredient_p );
            }
            //  Is there a preparation method ?
            if ( auip_p->preparation_p != NULL )
            {
                fprintf( out_file_fp, " -- %s",     auip_p->preparation_p );
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
    }
    //  Add a blank line after the AUIP
    fprintf( out_file_fp, "\n" );
    //-----------------------------------------------------------------------
    //  DIRECTIONS
    if ( list_query_count( recipe_p->directions ) > 0 )
    {
        for( data_p = list_get_first( recipe_p->directions );
             data_p != NULL;
             data_p = list_get_next( recipe_p->directions, data_p ) )
        {
            fprintf( out_file_fp, "%s\n", data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  Extra Stuff:
    fprintf( out_file_fp, "\n============== EXTRA STUFF: ==============\n\n" );
    //-----------------------------------------------------------------------
    //  DESCRIPTION:
    if ( recipe_p->description != NULL )
    {
        fprintf( out_file_fp, "Description:\t%s\n", recipe_p->description );
    }
    //-----------------------------------------------------------------------
    //  NOTES:
    if ( list_query_count( recipe_p->notes ) > 0 )
    {
        //  Notes tag:
        fprintf( out_file_fp, "NOTES:\n" );
        for( data_p = list_get_first( recipe_p->notes );
             data_p != NULL;
             data_p = list_get_next( recipe_p->notes, data_p ) )
        {
            fprintf( out_file_fp, "%s\n", data_p );
        }
    }
    //-----------------------------------------------------------------------
    //  CATEGORIES
    if (    ( list_query_count( recipe_p->cuisine  ) != 0 )
         || ( list_query_count( recipe_p->occasion ) != 0 )
         || ( list_query_count( recipe_p->course   ) != 0 )
         || ( list_query_count( recipe_p->diet     ) != 0 ) )
    {
        int                             first_category;
        
        //  Categories tag
        fprintf( out_file_fp, "Categories:\n" );

        //  ####################
        //  type=CUISINE
        if ( list_query_count( recipe_p->cuisine ) != 0 )
        {
            //  Set the flag
            first_category = true;
            
            for( data_p = list_get_first( recipe_p->cuisine );
                 data_p != NULL;
                 data_p = list_get_next( recipe_p->cuisine, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "\tCUISINE:\t\t%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
        //  ####################
        //  type=OCCASION
        if ( list_query_count( recipe_p->occasion ) != 0 )
        {
            //  Set the flag
            first_category = true;
            
            for( data_p = list_get_first( recipe_p->occasion );
                 data_p != NULL;
                 data_p = list_get_next( recipe_p->occasion, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "\tOCCASION:\t%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
        //  ####################
        //  type=COURSE
        if ( list_query_count( recipe_p->course ) != 0 )
        {
            //  Set the flag
            first_category = true;
            
            for( data_p = list_get_first( recipe_p->course );
                 data_p != NULL;
                 data_p = list_get_next( recipe_p->course, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "\tCOURSE:\t\t%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
        //  ####################
        //  type=DIET
        if ( list_query_count( recipe_p->diet ) != 0 )
        {
            //  Set the flag
            first_category = true;
            
            for( data_p = list_get_first( recipe_p->diet );
                 data_p != NULL;
                 data_p = list_get_next( recipe_p->diet, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "\tDIET:\t\t%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
        //  ####################
        //  type=APPLIANCE
        if ( list_query_count( recipe_p->appliance ) != 0 )
        {
            //  Set the flag
            first_category = true;
            
            for( data_p = list_get_first( recipe_p->appliance );
                 data_p != NULL;
                 data_p = list_get_next( recipe_p->appliance, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "\tAPPLIANCE:\t%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
        //  ####################
        //  type=CHAPTER
        if ( list_query_count( recipe_p->chapter ) != 0 )
        {
            //  Set the flag
            first_category = true;
            
            for( data_p = list_get_first( recipe_p->chapter );
                 data_p != NULL;
                 data_p = list_get_next( recipe_p->chapter, data_p ) )
            {
                if ( first_category == true )
                {
                    fprintf( out_file_fp, "\tCHAPTER:\t%s", data_p );
                    first_category = false;
                }
                else
                {
                    fprintf( out_file_fp, ", %s", data_p );
                }
            }
            //  End-of-Line
            fprintf( out_file_fp, "\n" );
        }
    }
    //-----------------------------------------------------------------------
    //  YIELD
    if (    ( recipe_p->serves     != NULL )
         || ( recipe_p->makes      != NULL ) )
    {
        if ( recipe_p->serves != NULL )
        {
            fprintf( out_file_fp, "Yield:\t\t%s\n", recipe_p->serves );
        }
        if ( recipe_p->makes  != NULL )
        {
            fprintf( out_file_fp, "Makes:\t\t%s", recipe_p->makes );
            
            //  Is there a unit of measurement ?
            if ( recipe_p->makes_unit != NULL )
            {
                //  YES:    Write it
                fprintf( out_file_fp, " %s\n", recipe_p->makes_unit );
            }
            else
            {
                //  NO:     End-of-Line
                fprintf( out_file_fp, "\n" );
            }
        }
    }
    //-----------------------------------------------------------------------
    //  AUTHOR:
    fprintf( out_file_fp, "Author:\t\t%s\n", recipe_p->author );
    //-----------------------------------------------------------------------
    //  COPYRIGHT:
    fprintf( out_file_fp, "Copyright:\t%s\n", recipe_p->copyright );
    //-----------------------------------------------------------------------
    //  TIME PREP:
    if ( recipe_p->time_prep != NULL )
    {
        fprintf( out_file_fp, "Time Prep:\t%s\n", recipe_p->time_prep );
    }
    //-----------------------------------------------------------------------
    //  TIME WAIT:
    //  @TODO:  This field isn't decoded yet!
    //          I'm using the resting time as a place holder.
    if ( recipe_p->time_resting != NULL )
    {
        fprintf( out_file_fp, "Time Wait:\t%s\n", recipe_p->time_resting );
    }
    //-----------------------------------------------------------------------
    //  TIME COOK:
    if ( recipe_p->time_cook != NULL )
    {
        fprintf( out_file_fp, "Time Cook:\t%s\n", recipe_p->time_cook );
    }
    //-----------------------------------------------------------------------
    //  TIME REST:
    if ( recipe_p->time_resting != NULL )
    {
        fprintf( out_file_fp, "Time Rest:\t%s\n", recipe_p->time_resting );
    }
    //-----------------------------------------------------------------------
    //  SKILL LEVEL:
    if ( recipe_p->skill != NULL )
    {
        fprintf( out_file_fp, "Skill Level:\t%s\n", recipe_p->skill );
    }
    //-----------------------------------------------------------------------
    //  RATING:
    if ( recipe_p->rating != NULL )
    {
        fprintf( out_file_fp, "Rating:\t\t%s\n", recipe_p->rating );
    }
    //-----------------------------------------------------------------------
    //  FORMAT:
    if ( recipe_p->source_format == NULL )
    {
        fprintf( out_file_fp, "Format:\t\tTXT\n" );
    }
    else
    {
        fprintf( out_file_fp, "Format:\t\t%s\n", recipe_p->source_format );
    }
    //-----------------------------------------------------------------------
    //  <IMPORTED_FROM>
    if (    ( recipe_p->dir_name         != NULL )
         || ( recipe_p->file_name        != NULL )
         || ( recipe_p->file_size        != NULL )
         || ( recipe_p->file_date_time   != NULL ) )
    {
        if ( recipe_p->dir_name != NULL )
        {
            fprintf( out_file_fp, "FileDirectory:\t%s\n", recipe_p->dir_name );
        }
        if ( recipe_p->file_name != NULL )
        {
            fprintf( out_file_fp, "FileName:\t%s\n", recipe_p->file_name );
        }
        if ( recipe_p->file_date_time != NULL )
        {
            fprintf( out_file_fp, "FileSize:\t%s\n", recipe_p->file_size );
        }
        {
            fprintf( out_file_fp, "FileDateTime:\t%s\n", recipe_p->file_date_time );
        }
    }
    //-----------------------------------------------------------------------
    //  <E-MAIL>
    if (    ( recipe_p->group_from       != NULL )
         || ( recipe_p->group_subject    != NULL )
         || ( recipe_p->posted_by        != NULL )
         || ( recipe_p->posted_subject   != NULL ) )
    {
        //  ####################
        //  GROUP-NAME:
        if ( recipe_p->group_from != NULL )
        {
            fprintf( out_file_fp, "G-Name:\t\t%s\n",
                     recipe_p->group_from );
        }
        //  ####################
        //  GROUP-SUBJECT:
        if ( recipe_p->group_subject != NULL )
        {
            fprintf( out_file_fp, "G-Subject:\t%s\n",
                     recipe_p->group_subject );
        }
        //  ####################
        //  GROUP-DATE:
        if ( recipe_p->group_date != NULL )
        {
            fprintf( out_file_fp, "G-Date:\t\t%s\n",
                     recipe_p->group_date );
        }
        //  ####################
        //  POSTED-BY:
        if ( recipe_p->posted_by != NULL )
        {
            fprintf( out_file_fp, "P-By:\t\t%s\n",
                     recipe_p->posted_by );
        }
        //  ####################
        //  POSTED-SUBJECT:
        if ( recipe_p->posted_subject != NULL )
        {
            fprintf( out_file_fp, "P-Subject:\t%s\n",
                     recipe_p->posted_subject );
        }
        //  ####################
        //  POSTED-DATE:
        if ( recipe_p->posted_date != NULL )
        {
            fprintf( out_file_fp, "P-Date:\t\t%s\n",
                     recipe_p->posted_date );
        }
    }
    //-----------------------------------------------------------------------
    //  <EDITING>
    if (    ( recipe_p->formatted_by     != NULL )
         || ( recipe_p->edited_by        != NULL ) )
    {
        if ( recipe_p->formatted_by != NULL )
        {
            fprintf( out_file_fp, "FormattedBy:\t%s\n", recipe_p->formatted_by );
        }
        if ( recipe_p->edited_by != NULL )
        {
            fprintf( out_file_fp, "EditedBy:\t%s\n", recipe_p->edited_by );
        }
    }
    //-----------------------------------------------------------------------
    //  TXT Recipe end tag
    fprintf( out_file_fp, "@@@@@\n\n\n" );

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
