/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017,2018 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public function that makeup the external
 *  library components of the 'decode' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_DECODE          ( "ALLOCATE STORAGE FOR DECODE" )

#define _GNU_SOURCE

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
#include <decode_bof_api.h>     //  API for all decode_bof_*        PUBLIC
#include <decode_cp2_api.h>     //  API for all decode_cp2_*        PUBLIC
#include <decode_gf2_api.h>     //  API for all decode_gf2_*        PUBLIC
#include <decode_grf_api.h>     //  API for all decode_grf_*        PUBLIC
#include <decode_mmf_api.h>     //  API for all decode_mmf_*        PUBLIC
#include <decode_mxp_api.h>     //  API for all decode_mxp_*        PUBLIC
#include <decode_mx2_api.h>     //  API for all decode_mx2_*        PUBLIC
#include <decode_nyc_api.h>     //  API for all decode_nyc_*        PUBLIC
#include <decode_txt_api.h>     //  API for all decode_txt_*        PUBLIC
#include <decode_erd_api.h>     //  API for all encode_erd_*        PUBLIC
#include <encode_mxp_api.h>     //  API for all encode_mxp_*        PUBLIC
#include <encode_rxf_api.h>     //  API for all encode_rxf_*        PUBLIC
#include <encode_ria_api.h>     //  API for all encode_ria_*        PUBLIC
#include <encode_txt_api.h>     //  API for all encode_txt_*        PUBLIC
                                //*******************************************
#include <decode_api.h>         //  API for all decode_*            PUBLIC
#include "decode_lib.h"         //  API for all DECODE__*           PRIVATE
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
struct  decode_data_t
{
    /**
     *  @param  recipe_format       The recipe format being decoded         */
    enum    recipe_format_e         recipe_format;
    /**
     *  @param  level3_list_p       Pointer to a data list                  */
    struct  list_base_t         *   thread_list_p;
    /**
     *  @param  source_info_p       Pointer to source data information      */
    struct  source_info_t       *   source_info_p;
};
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
 *  Test the data string for the tag string provided.
 *
 *  @param  data_p              Pointer to the test data string.
 *  @param  tag_p               Pointer to the tag to be tested.
 *
 *  @return return_data_p       When the tag string is located, a pointer to
 *                              the first byte of the tag data;  else NULL
 *                              is returned.
 *
 *  @note
 *
 ****************************************************************************/

char    *
decode_is_tag(
    char                        *   data_p,
    char                        *   tag_p
    )
{
    /**
     *  @param  return_data_p       Pointer to the data string              */
    char                        *   return_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Look for the first occurence of the search tag data.
    return_data_p = strcasestr( data_p, tag_p );

    //  Was the tag string found ?
    if ( return_data_p != NULL )
    {
        //  YES:    Skip over the tag before looking for blank characters
        return_data_p += strlen( tag_p );

        //  Skip past blank characters to what should be a colon [:].
        for ( ;
              isblank( return_data_p[ 0 ] ) != 0;
              return_data_p += 1 );

        //  Is this a colon [:] character ?
        if ( return_data_p[ 0 ] == ':' )
        {
            //  YES:    Fix the pointer and we are done.
            return_data_p += 1;
        }
        else
        {
            //  NO:     This isn't the tag we are looking for
            return_data_p = NULL;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( return_data_p );
}


/****************************************************************************/
/**
 *  The decode_save_chapter will identify which category subsection is
 *  identified and store the data in that section.
 *
 *  @param  data_p              Pointer to the raw data category
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_save_chapter(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    //  Do we have a category to save ?
    if ( data_p != NULL )
    {
        //  Appliance ?
        if ( strncmp( APPLIANCE, data_p, APPLIANCE_L ) == 0 )
        {
            recipe_append( recipe_p->appliance,
                           ( data_p + APPLIANCE_L ) );
        }
        //  Diet ?
        else
        if ( strncmp( DIET, data_p, DIET_L ) == 0 )
        {
            recipe_append( recipe_p->diet,
                           ( data_p + DIET_L ) );
        }
        //  Course ?
        else
        if ( strncmp( COURSE, data_p, COURSE_L ) == 0 )
        {
            recipe_append( recipe_p->course,
                           ( data_p + COURSE_L ) );
        }
        //  Cuisine ?
        else
        if ( strncmp( CUISINE, data_p, CUISINE_L ) == 0 )
        {
            recipe_append( recipe_p->cuisine,
                           ( data_p + CUISINE_L ) );
        }
        //  Occasion ?
        else
        if ( strncmp( OCCASION, data_p, OCCASION_L ) == 0 )
        {
            recipe_append( recipe_p->occasion,
                           ( data_p + OCCASION_L ) );
        }
        //  Category ?
        else
        if ( strncmp( CATEGORY, data_p, CATEGORY_L ) == 0 )
        {
            recipe_append( recipe_p->chapter,
                           ( data_p + CATEGORY_L ) );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  The decode_copy_info_to_recipe will transfer the information contained in
 *  the source_infp structure into the recipe structure
 *
 *  @param  recipe_p            Pointer to a recipe structure.
 *  @param  source_info_p       Pointer to source data information
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_copy_info_to_recipe(
    struct  recipe_t            *   recipe_p,
    struct  source_info_t       *   source_info_p
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Group (From, Subject, Date)
     ************************************************************************/

    //------------------------------------------------------------------------
    //  Source:
    if ( strlen( source_info_p->g_from ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->group_from != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->group_from );
        }
        //  Save to the recipe
        recipe_p->group_from = text_copy_to_new( source_info_p->g_from );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------
    //  Subject:
    if ( strlen( source_info_p->g_subject ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->group_subject != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->group_subject );
        }
        //  Save to the recipe
        recipe_p->group_subject = text_copy_to_new( source_info_p->g_subject );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------
    //  Date:
    if ( strlen( source_info_p->g_datetime ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->group_date != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->group_date );
        }
        //  Save to the recipe
        recipe_p->group_date = text_copy_to_new( source_info_p->g_datetime );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------

    /************************************************************************
     *  Posted (By, Subject, Date)
     ************************************************************************/

    //------------------------------------------------------------------------
    //  Posted By:
    if ( strlen( source_info_p->e_from ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->posted_by != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->posted_by );
        }
        //  Save to the recipe
        recipe_p->posted_by = text_copy_to_new( source_info_p->e_from );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------
    //  Posted Subject:
    if ( strlen( source_info_p->e_subject ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->posted_subject != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->posted_subject );
        }
        //  Save to the recipe
        recipe_p->posted_subject = text_copy_to_new( source_info_p->e_subject );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------
    //  Posted Date:
    if ( strlen( source_info_p->e_datetime ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->posted_date != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->posted_date );
        }
        //  Save to the recipe
        recipe_p->posted_date = text_copy_to_new( source_info_p->e_datetime );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------

    /************************************************************************
     *  File Information
     ************************************************************************/

    //------------------------------------------------------------------------
    //  Directory Name:
    if ( strlen( source_info_p->f_dir_name ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->dir_name != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->dir_name );
        }
        //  Save to the recipe
        recipe_p->dir_name = text_copy_to_new( source_info_p->f_dir_name );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------
    //  File Name:
    if ( strlen( source_info_p->f_file_name ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->file_name != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->file_name );
        }
        //  Save to the recipe
        recipe_p->file_name = text_copy_to_new( source_info_p->f_file_name );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------
    //  File Creation Data/Time:
    if ( strlen( source_info_p->f_date_time ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->file_date_time != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->file_date_time );
        }
        //  Save to the recipe
        recipe_p->file_date_time = text_copy_to_new( source_info_p->f_date_time );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------
    //  File Size:
    if ( strlen( source_info_p->f_file_size ) > 0 )
    {
        //  YES:    Is there something there already ?
        if ( recipe_p->file_size != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->file_size );
        }
        //  Save to the recipe
        recipe_p->file_size = text_copy_to_new( source_info_p->f_file_size );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //------------------------------------------------------------------------
    //  WEB Address
    if ( strlen( source_info_p->f_dir_name ) > 0 )
    {
        /**
         *  @param  tmp_web_address     Temp buffer to build the web addr   */
        char                            *   tmp_web_address;
        /**
         *  @param                      Pointers into web address           */
        char                            *   addr_beg_p;
        char                            *   addr_end_p;
        char                            *   dot_1_p;
        char                            *   dot_2_p;

        //  YES:    Is there something there already ?
        if ( recipe_p->web_address != NULL )
        {
            //  YES:    Get rid of the old value.
            mem_free( recipe_p->web_address );
        }
        //  Allocate storage for the work buffer
        tmp_web_address = mem_malloc( strlen( source_info_p->f_dir_name ) + 1 );

        //  Copy the directory name to the work buffer
        strncpy( tmp_web_address, source_info_p->f_dir_name,
                 strlen( source_info_p->f_dir_name ) );

        //  Initialize everything
        addr_beg_p = NULL;
        addr_end_p = NULL;
        dot_1_p    = NULL;
        dot_2_p    = NULL;

        //  Look for a web address
        dot_1_p = strchr( tmp_web_address, '.' );
        if ( dot_1_p != NULL )
            dot_2_p = strchr( dot_1_p + 1, '.' );
        if ( dot_2_p != NULL )
            addr_end_p = strchrnul( dot_2_p, '/' );
        if ( addr_end_p != NULL ) {
            addr_end_p[ 0 ] = '\0';
            addr_beg_p = strrchr( tmp_web_address, '/' ); }
        if ( addr_beg_p != NULL )
            addr_beg_p += 1;
        else
            addr_beg_p = &tmp_web_address[ 0 ];

        //  Does a web address exist ?
        if (    ( dot_1_p    != NULL )
             && ( dot_2_p    != NULL )
             && ( addr_end_p != NULL ) )
        {
            //  YES:    Save it
            recipe_p->web_address = text_copy_to_new( addr_beg_p );
            log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
        }

        //  Release the working buffer
        mem_free( tmp_web_address );
    }
    //------------------------------------------------------------------------

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Analyze the data and determine if it contains something that can be
 *  interpreted as the end of a recipe other than a normal termination line.
 *
 *  @param  data_p              Pointer to the raw data category
 *
 *  @return decode_rc           TRUE when the data contains a recipe break,
 *                              else FALSE is returned.
 *
 *  @note
 *
 ****************************************************************************/

int
decode_end_of_recipe(
    enum    recipe_format_e         recipe_format,
    char                        *   data_p
    )
{
    /**
     *  @param  decode_rc       Return code from this function              */
    int                             decode_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that all tests will fail
    decode_rc = false;

    /************************************************************************
     *  Function
     ************************************************************************/

    //  NOTE:
    //  When a MXP formatted recipe is embedded inside a MX2, this was producing
    //  a false positive for a recipe break.  If this creates a problem then
    //  I will have to pass the current recipe type for additional testing.

    //  NOTE:
    //  MX2 is a special case because it may contain an embedded MXP recipe
    //  inside it.

    //  Is this something that can end a recipe ?
    if (    ( email_is_start(       data_p ) == true )
         || ( email_is_group_break( data_p ) == true )
         || ( decode_bof_start(     data_p ) == true )
         || ( decode_cp2_start(     data_p ) == true )
         || ( decode_gf2_start(     data_p ) == true )
         || ( decode_grf_start(     data_p ) == true )
         || ( decode_mmf_start(     data_p ) == true )
         || ( decode_mx2_start(     data_p ) == true )
         || (    ( recipe_format != RECIPE_FORMAT_MX2 )
              && ( decode_mxp_start(     data_p ) == true ) )
         || ( decode_nyc_start(     data_p ) == true ) )
    {
        //  YES:    Change the return code
        decode_rc = true;
    }


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( decode_rc );
}

/****************************************************************************/
/**
 *  Analyze the data and determine if it contains something that can be
 *  interpreted as the end of a recipe other than a normal termination line.
 *
 *  @param  recipe_format       The recipe format being decoded
 *  @param  level3_list_p       Pointer to a list where the file is stored
 *  @param  source_info_p       Pointer to source data information
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_xxx(
    enum    recipe_format_e         recipe_format,
    struct  list_base_t         *   level3_list_p,
    struct  source_info_t       *   source_info_p
    )
{
#if 1   //  @ToDo   [THREAD] Pick one of the following methods

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    /************************************************************************
     *  Recipe decode
     ************************************************************************/

    //  Go process the recipe
    switch( recipe_format )
    {
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_BOF:
        {
            decode_bof( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_CP2:
        {
            decode_cp2( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_GRF:
        {
            decode_grf( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_GF2:
        {
            decode_gf2( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MMF:
        {
            decode_mmf( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MXP:
        {
            decode_mxp( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_MX2:
        {
            decode_mx2( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_NYC:
        {
            decode_nyc( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        case    RECIPE_FORMAT_ERD:
        {
            decode_erd( level3_list_p, source_info_p );
        }   break;
        //--------------------------------------------------------------------
        default:
        {
            //  If we get here the recipe format isn't known.
            decode_txt( level3_list_p, source_info_p );
        }
        //--------------------------------------------------------------------
    }
#else
    /**
     *  @param  decode_data_p   Structure to pass to the thread             */
    struct  decode_data_t       *   decode_data_p;
    /**
     * @param list_lock_key     File list key                               */
    int                             list_lock_key;
    /**
     *  @param list_data_p      Pointer to the read data                    */
    char                        *   list_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Allocate storage for the parameter structure
    decode_data_p = mem_malloc( sizeof( struct decode_data_t ) );

    log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );

    //  Create the file-list
    decode_data_p->thread_list_p = list_new( );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Save the recipe format type
    decode_data_p->recipe_format = recipe_format;

    //  Save the source information structure
    decode_data_p->source_info_p = mem_malloc( sizeof( struct source_info_t ) );

    log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );

    memcpy( decode_data_p->source_info_p,
            source_info_p, sizeof( struct source_info_t ) );

    //  @ToDo   I am taking a big shortcut here by moving the message data
    //          from one queue to another.  This is because the creator of
    //          the list owns the list and this can't be true for a
    //          threaded operation.  Ideally the main code will be changed
    //          to the creator of the list passing it on, then creating a
    //          new list when needed.  For now this is the smallest foot
    //          print I can have and still be able to switch back to a no
    //          thread execution.

    //  Lock the list for fast(er) access
    list_lock_key = list_user_lock( level3_list_p );

    //  Go through everything on the list
    for( list_data_p = list_fget_first( level3_list_p, list_lock_key );
         list_data_p != NULL;
         list_data_p = list_fget_next( level3_list_p, list_data_p, list_lock_key ) )
    {
        //  Just move the data to the new list
        list_put_last( decode_data_p->thread_list_p, list_data_p );

        //  Remove it from the level 3 list
        list_fdelete( level3_list_p, list_data_p, list_lock_key );
    }

    //  Unlock the list
    list_user_unlock( level3_list_p, list_lock_key );

    //  Put the decode_data on the decode queue.
    queue_put_payload( decode_queue_id, decode_data_p );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Main entry point for all decode formats.
 *
 *  @param  not_used            Required to be here but not used.
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_thread(
    void                        *   not_used
    )
{
    /**
     *  @param  decode_data_p   Structure to pass to the thread             */
    struct  decode_data_t       *   decode_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    decode_queue_id = queue_new( "DECODE" );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Run for the life of the application
    while( 1 )
    {
        //  Get another recipe from the queue
        decode_data_p = queue_get_payload( decode_queue_id );

        //  Go process the recipe
        switch( decode_data_p->recipe_format )
        {
            //----------------------------------------------------------------
            case    RECIPE_FORMAT_BOF:
            {
                decode_bof( decode_data_p->thread_list_p,
                            decode_data_p->source_info_p );
            }   break;
            //----------------------------------------------------------------
            case    RECIPE_FORMAT_CP2:
            {
                decode_cp2( decode_data_p->thread_list_p,
                            decode_data_p->source_info_p );
            }   break;
            //----------------------------------------------------------------
            case    RECIPE_FORMAT_MMF:
            {
                decode_mmf( decode_data_p->thread_list_p,
                            decode_data_p->source_info_p );
            }   break;
            //----------------------------------------------------------------
            case    RECIPE_FORMAT_MXP:
            {
                decode_mxp( decode_data_p->thread_list_p,
                            decode_data_p->source_info_p );
            }   break;
            //----------------------------------------------------------------
            default:
            {
                //  If we get here the recipe format isn't known.
                decode_txt( decode_data_p->thread_list_p,
                            decode_data_p->source_info_p );
            }
            //----------------------------------------------------------------
        }

        /********************************************************************
        *  Release resources passed through the queue
        ********************************************************************/

        //  Is the data list empty like it is supposed to be ?
        if ( list_query_count( decode_data_p->thread_list_p ) != 0 )
        {
            //  YES:    Something bad happened.
            log_write( MID_FATAL, "decode_thread",
                       "There is still something on the list.\n" );
        }

        //  Release the data list
        list_kill( decode_data_p->thread_list_p );

        //  Release the information structure
        mem_free( decode_data_p->source_info_p );

        //  Release storage for the parameter structure
        mem_free( decode_data_p );
    }
#endif

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  The recipe is source is completely decoded.  Look it over to see if
 *  there are things in it that can be used to categorize it.
 *
 *  @param recipe_t             Primary structure for a recipe
 *
 *  @return void                No return code from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
decode_finalize(
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  AUIP_count      Number of ingredients in this recipe        */
    int                             AUIP_count;
    /**
     *  @param  AUIP_part       Temporary data buffer for an ingred part    */
    unsigned char                   AUIP_part[ SHA1_DIGEST_SIZE + 2 ];
    /**
     *  @param  recipe_id       Temporary data buffer for the recipe id     */
    unsigned char                   recipe_id[ SHA1_DIGEST_SIZE + 2 ];
    /**
     *  @param  id_string       Recipe-ID string to identify a recipe       */
    char                            id_string[ ( SHA1_DIGEST_SIZE * 2 ) + 4 ];
    /**
     *  @param  auip_p          Pointer to AUIP structure                   */
    struct  auip_t              *   auip_p;
    /**
     *  @param  context         Control structure used by SHA1 functions    */
    SHA1_CTX                        context;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Initialize the final Recipe-ID storage buffer.
    memset( recipe_id, 0x00, sizeof( recipe_id ) );

    /************************************************************************
     *  Compute the recipe checksum (Recipe-ID)
     ************************************************************************/

    //  Query the number of ingredients for this recipe
    AUIP_count = list_query_count( recipe_p->ingredient );

    //  Are there any ingredients in this recipe ?
    if ( AUIP_count > 0 )
    {
        //  YES:    Loop through all of the ingredients in this recipe
        for( auip_p = list_get_first( recipe_p->ingredient );
             auip_p != NULL;
             auip_p = list_get_next( recipe_p->ingredient, auip_p ) )
        {
            //  Is there an amount ?
            if ( auip_p->amount_p != NULL )
            {
                //  YES:    Initialize SHA1
                sha1_init( &context );

                //  Build SHA1 version of the amount field
                sha1_update( &context, auip_p->amount_p, strlen( auip_p->amount_p ) );

                //  Finalize the SHA1 operation
                sha1_final( &context, (char*)AUIP_part );

                //  Add the two together
                sha1_sum( (char*)recipe_id, (char*)recipe_id, (char*)AUIP_part );
            }
            //  Is there a unit of measurement ?
            if ( auip_p->unit_p != NULL )
            {
                //  YES:    Initialize SHA1
                sha1_init( &context );

                //  Build SHA1 version of the unit field
                sha1_update( &context, auip_p->unit_p, strlen( auip_p->unit_p ) );

                //  Finalize the SHA1 operation
                sha1_final( &context, (char*)AUIP_part );

                //  Add the two together
                sha1_sum( (char*)recipe_id, (char*)recipe_id, (char*)AUIP_part );
            }
            //  Is there an ingredient ?
            if ( auip_p->ingredient_p != NULL )
            {
                //  YES:    Initialize SHA1
                sha1_init( &context );

                //  Build SHA1 version of the unit field
                sha1_update( &context, auip_p->ingredient_p, strlen( auip_p->ingredient_p ) );

                //  Finalize the SHA1 operation
                sha1_final( &context, (char*)AUIP_part );

                //  Add the two together
                sha1_sum( (char*)recipe_id, (char*)recipe_id, (char*)AUIP_part );
            }
        }

        //  Format the Recipe-ID as a hex string
        snprintf( id_string, sizeof( id_string ),
                  "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
                  "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
                  recipe_id[  0 ], recipe_id[  1 ], recipe_id[  2 ], recipe_id[  3 ], recipe_id[  4 ],
                  recipe_id[  5 ], recipe_id[  6 ], recipe_id[  7 ], recipe_id[  8 ], recipe_id[  9 ],
                  recipe_id[ 10 ], recipe_id[ 11 ], recipe_id[ 12 ], recipe_id[ 13 ], recipe_id[ 14 ],
                  recipe_id[ 15 ], recipe_id[ 16 ], recipe_id[ 17 ], recipe_id[ 18 ], recipe_id[ 19 ],
                  AUIP_count );

        //  Add it to the recipe
        recipe_p->recipe_id = text_copy_to_new( id_string );
    }
    else
    {
        //  NO:     A recipe without ingredients isn't a recipe.
        recipe_p->recipe_id = text_copy_to_new( "000000000000000000000000000000000000000000" );
    }

    /************************************************************************
     *  Recipe Title analysis
     *
     *  @NOTE:
     *      There are MANY-MANY-MANY incorrect categorizations here.
     *      -- IS IT WORTH IT --
     *
     ************************************************************************/

    //  Recipe Title analysis
    DECODE__title_information( recipe_p );

    /************************************************************************
     *  Recipe Directions analysis
     *
     ************************************************************************/

    if ( recipe_p->instructions != NULL )
    {
        //  Directions  analysis
        DECODE__directions_cleanup( recipe_p );

        //  Directions format
        recipe_fmt_directions( recipe_p );

        //  Find "FROM:" in the directions
        DECODE__directions_from( recipe_p );

        //  Find "SOURCE:" in the directions
        DECODE__directions_source( recipe_p );

        //  Find "COPYRIGHT:" in the directions
        DECODE__directions_copyright( recipe_p );

        //  Find "DESCRIPTION:" in the directions
        DECODE__directions_description( recipe_p );

        //  Find "MAKES:" in the directions
        DECODE__directions_makes( recipe_p );

        //  Find "TIME-wxyz:" in the directions
        DECODE__directions_time( recipe_p );

        //  Find "S(Imported From):" in the directions
        DECODE__directions_import_from( recipe_p );

        //  @ToDo   Ratings:

        //  Find "NOTES:" in the directions
        DECODE__directions_notes( recipe_p );
    }

    /************************************************************************
     *  Set the recipe defaults when information is not present
     ************************************************************************/

    //-----------------------------------------------------------------------
    if ( recipe_p->name == NULL )
    {
        //  Create a default string
        recipe_p->name = text_copy_to_new( "No Recipe Name Found" );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    else
    if ( text_is_blank_line( recipe_p->name ) == true )
    {
        //  Release the old buffer
        mem_free( recipe_p->name );

        //  Create a default string
        recipe_p->name = text_copy_to_new( "No Recipe Name Found" );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->author == NULL )
    {
        //  Create a default string
        recipe_p->author = text_copy_to_new( "UNKNOWN" );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    else
    if ( text_is_blank_line( recipe_p->author ) == true )
    {
        //  Release the old buffer
        mem_free( recipe_p->author );

        //  Create a default string
        recipe_p->author = text_copy_to_new( "UNKNOWN" );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------
    if ( recipe_p->copyright == NULL )
    {
        //  Create a default string
        recipe_p->copyright = text_copy_to_new( "Copyright (c) and related "
                "rights reserved via Creative Commons [CC BY-SA 4.0]  "
                "https://creativecommons.org/licenses/by-sa/4.0/" );

        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    else
    if ( text_is_blank_line( recipe_p->copyright ) == true )
    {
        //  Release the old buffer
        mem_free( recipe_p->copyright );

        //  Create a default string
        recipe_p->copyright = text_copy_to_new( "Copyright (c) and related "
                "rights reserved via Creative Commons [CC BY-SA 4.0]  "
                "https://creativecommons.org/licenses/by-sa/4.0/" );
        log_write( MID_DEBUG_1, "decode_api.c", "Line: %d\n", __LINE__ );
    }
    //-----------------------------------------------------------------------

    /************************************************************************
     *  Function Exit
     ************************************************************************/

#if ( THREADING == false )
    encode_mxp( recipe_p );
//  encode_rxf( recipe_p );
//  encode_ria( recipe_p );
//  encode_txt( recipe_p );
#else
    queue_put_payload( encode_queue_id, recipe_p );
#endif

    //  DONE!
}

/****************************************************************************/
