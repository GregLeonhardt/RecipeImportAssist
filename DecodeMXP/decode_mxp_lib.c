/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_mxp' library.
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
#include <ctype.h>              //  Determine the type contained
#include <string.h>             //  Functions for managing strings
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <recipe_api.h>         //  API for all recipe_*            PUBLIC
#include <xlate_api.h>          //  API for all xlate_*             PUBLIC
                                //*******************************************
#include <decode_mxp_api.h>     //  API for all decode_mxp_*        PUBLIC
#include "decode_mxp_lib.h"     //  API for all DECODE_MXP__*       PRIVATE
                                //*******************************************

/****************************************************************************
 * Enumerations local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    categories_state_e
{
    CSS_IDLE                    =   0,
    CSS_STARTED                 =   1,
    CSS_COMPLETE                =   2
};
//----------------------------------------------------------------------------
enum    auip_state_e
{
    AUIPS_IDLE                  =   0,
    AUIPS_TEXT                  =   1,
    AUIPS_DASH                  =   2,
    AUIPS_AMIP                  =   3,
    AUIPS_COMPLETE              =   4
};
//----------------------------------------------------------------------------
enum    direction_state_e
{
    DSS_IDLE                    =   0,
    DSS_STARTED                 =   1,
    DSS_COMPLETE                =   2
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Definitions local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
#define MXP_S_PART_1            "*"
#define MXP_S_PART_1_L          strlen( MXP_S_PART_1 )
//----------------------------------------------------------------------------
#define MXP_S_PART_2            "Exported"
#define MXP_S_PART_2_L          strlen( MXP_S_PART_2 )
//----------------------------------------------------------------------------
#define MXP_S_PART_3            "from"
#define MXP_S_PART_3_L          strlen( MXP_S_PART_3 )
//----------------------------------------------------------------------------
#define MXP_START_1             "*  Exported from  MasterCook Mac  *"
#define MXP_START_1_L           strlen( MXP_START_1 )
//----------------------------------------------------------------------------
#define MXP_START_2             "*  Exported from  MasterCook  *"
#define MXP_START_2_L           strlen( MXP_START_2 )
//----------------------------------------------------------------------------
#define MXP_START_3             "*  Exported from  MasterCook II  *"
#define MXP_START_3_L           strlen( MXP_START_3 )
//----------------------------------------------------------------------------
#define MXP_START_4             "*Exported from MasterCook"
#define MXP_START_4_L           strlen( MXP_START_4 )
//----------------------------------------------------------------------------
#define MXP_START_5             "* Exported from MasterCook"
#define MXP_START_5_L           strlen( MXP_START_5 )
//----------------------------------------------------------------------------
#define MXP_START_6             "{ Exported from MasterCook Mac }"
#define MXP_START_6_L           strlen( MXP_START_6 )
//----------------------------------------------------------------------------
#define MXP_START_7             "*  Exported from CookWorks"
#define MXP_START_7_L           strlen( MXP_START_7 )
//----------------------------------------------------------------------------
#define MXP_START_8             "*  Exported from  MasterCook  Buster  *"
#define MXP_START_8_L           strlen( MXP_START_8 )
//----------------------------------------------------------------------------
#define MXP_START_9             "* Exported for MasterCook 4 by Living Cookbook *"
#define MXP_START_9_L           strlen( MXP_START_9 )
//----------------------------------------------------------------------------
#define MXP_START_10            "*  Exported from  Key Home Gourmet  *"
#define MXP_START_10_L          strlen( MXP_START_10 )
//----------------------------------------------------------------------------
#define MXP_END_1               "- - - - - - - - - - - - - - - - - -"
#define MXP_END_1_L             strlen( MXP_END_1 )
//----------------------------------------------------------------------------
#define MXP_END_2               "-+-"
#define MXP_END_2_L             strlen( MXP_END_2 )
//----------------------------------------------------------------------------
#define MXP_BY                  "Recipe By     :"
#define MXP_BY_L                strlen( MXP_BY )
//----------------------------------------------------------------------------
#define MXP_SRV_SIZE            "Serving Size  :"
#define MXP_SRV_SIZE_L          strlen( MXP_SRV_SIZE )
//----------------------------------------------------------------------------
#define MXP_PREP_TIME           "Preparation Time :"
#define MXP_PREP_TIME_L         strlen( MXP_PREP_TIME )
//----------------------------------------------------------------------------
#define MXP_CATEGORIES          "Categories    :"
#define MXP_CATEGORIES_L        strlen( MXP_CATEGORIES )
//----------------------------------------------------------------------------
#define MXP_CAT_END_1           "Amount  Measure"
#define MXP_CAT_END_1_L         strlen( MXP_CAT_END_1 )
//----------------------------------------------------------------------------
#define MXP_CAT_END_2           "- --------  ------------"
#define MXP_CAT_END_2_L         strlen( MXP_CAT_END_2 )
//----------------------------------------------------------------------------
#define MXP_CAT_END_3           "--------  ------------"
#define MXP_CAT_END_3_L         strlen( MXP_CAT_END_3 )
//----------------------------------------------------------------------------
#define MXP_APPLIANCE           "Appliance:"
#define MXP_APPLIANCE_L         strlen( MXP_APPLIANCE )
//----------------------------------------------------------------------------
#define MXP_DIET                "Diet:"
#define MXP_DIET_L              strlen( MXP_DIET )
//----------------------------------------------------------------------------
#define MXP_COURSE              "Course:"
#define MXP_COURSE_L            strlen( MXP_COURSE )
//----------------------------------------------------------------------------
#define MXP_CUISINE             "Cuisine:"
#define MXP_CUISINE_L           strlen( MXP_CUISINE )
//----------------------------------------------------------------------------
#define MXP_OCCASION            "Occasion:"
#define MXP_OCCASION_L          strlen( MXP_OCCASION )
//----------------------------------------------------------------------------
#define MXP_CATEGORY            "Category:"
#define MXP_CATEGORY_L          strlen( MXP_CATEGORY )
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
static
enum    categories_state_e      categories_scan_state;
//----------------------------------------------------------------------------
static
enum    auip_state_e            auip_scan_state;
//----------------------------------------------------------------------------
static
enum    direction_state_e       direction_scan_state;
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Test the string for a recipe START marker.
 *      FORMATS:
 *         1         *  Exported from  MasterCook Mac  *
 *         2         *  Exported from  MasterCook  *
 *         3         *  Exported from  MasterCook II  *
 *
 *         4         *Exported from MasterCook
 *         5         * Exported from MasterCook
 *         6         { Exported from MasterCook
 *         7         *  Exported from CookWorks
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__start(
    char                        *   data_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain a 'start' marker.
    mxp_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    //  Initialize process state registers
    categories_scan_state = CSS_IDLE;
    auip_scan_state = AUIPS_IDLE;
    direction_scan_state = DSS_IDLE;

    /************************************************************************
     *  Generic Detection
     ************************************************************************/

    //  Is this the start of a MasterCook MXP recipe ?
    if ( strncmp( tmp_data_p, MXP_S_PART_1,  MXP_S_PART_1_L  ) == 0 )
    {
            if (    ( strstr( tmp_data_p, MXP_S_PART_2 ) != NULL )     //  Exported
                 && ( strstr( tmp_data_p, MXP_S_PART_3 ) != NULL ) )   //  from
            {
                //  YES:    Change the return code
                mxp_rc = true;
            }
    }

    /************************************************************************
     *  Specific Detection
     ************************************************************************/

    //  Is this the start of a MasterCook MXP recipe ?
    if(    ( strncmp( tmp_data_p, MXP_START_1,  MXP_START_1_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_2,  MXP_START_2_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_3,  MXP_START_3_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_4,  MXP_START_4_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_5,  MXP_START_5_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_6,  MXP_START_6_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_7,  MXP_START_7_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_8,  MXP_START_8_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_9,  MXP_START_9_L  ) == 0 )
        || ( strncmp( tmp_data_p, MXP_START_10, MXP_START_10_L ) == 0 ) )
    {
        //  YES:    Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  Any NON blank line will be used as the recipe title.
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__title(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain a 'title' information.
    mxp_rc = false;

    /************************************************************************
     *  Recipe Name or Title
     ************************************************************************/

    //  Is this a blank line ?
    if( text_is_blank_line( data_p ) == false )
    {
        //  NO:     Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( data_p );

        //  Cleanup the recipe name before saving it.
        recipe_name_cleanup( tmp_data_p, strlen( tmp_data_p ) );

        //  Save the recipe title (name)
        recipe_p->name = text_copy_to_new( tmp_data_p );

        //  Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  Copy 'recipe by' information to the recipe structure.
 *      FORMATS:
 *         1         Recipe By     :
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__recipe_by(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'recipe by' information.
    mxp_rc = false;

    /************************************************************************
     *  Recipe By     :
     ************************************************************************/

    //  Search the data buffer
    tmp_data_p = strstr( data_p, MXP_BY );

    //  Does this line contain
    if( tmp_data_p != NULL )
    {
        //  YES:    Jump past the search string
        tmp_data_p += MXP_BY_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe title (name)
        recipe_p->author = text_copy_to_new( tmp_data_p );

        //  Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  Copy 'serving size' information to the recipe structure.
 *      FORMATS:
 *         1         Serving Size  :
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__srv_size(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'serving size' information.
    mxp_rc = false;

    /************************************************************************
     *  Serving Size  :
     ************************************************************************/

    //  Search the data buffer
    tmp_data_p = strstr( data_p, MXP_SRV_SIZE );

    //  Does this line contain
    if( tmp_data_p != NULL )
    {
        //  YES:    Jump past the search string
        tmp_data_p += MXP_SRV_SIZE_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe title (name)
        recipe_p->serves = text_copy_to_new( tmp_data_p );

        //  Is there anything past the serving size on this line ?
        tmp_data_p = strchr( recipe_p->serves, ' ' );
        if( tmp_data_p != NULL )
        {
            //  YES:    Terminate the line after the serving size
            tmp_data_p[ 0 ] = '\0';
        }

        //  Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  Copy 'preparation time' information to the recipe structure.
 *      FORMATS:
 *         1         Preparation Time :
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__prep_time(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'preperation time' information.
    mxp_rc = false;

    /************************************************************************
     *  Preparation Time :
     ************************************************************************/

    //  Search the data buffer
    tmp_data_p = strstr( data_p, MXP_PREP_TIME );

    //  Does this line contain
    if( tmp_data_p != NULL )
    {
        //  YES:    Jump past the search string
        tmp_data_p += MXP_PREP_TIME_L;

        //  Skip past any leading whitespace.
        tmp_data_p = text_skip_past_whitespace( tmp_data_p );

        //  Save the recipe title (name)
        recipe_p->time_prep = text_copy_to_new( tmp_data_p );

        //  Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  Copy 'categories' information to the recipe structure.
 *      FORMATS:
 *         1          Categories    :
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__categories(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;
    /**
     *  @param  raw_category    A temporary holding buffer for a category   */
    char                            raw_category[ SIZE_CATEGORY + 1 ];
    /**
     *  @param  xlated_chapter  A pointer to the translated chapter         */
    char                        *   xlated_chapter_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'categories' information.
    mxp_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //
    if( categories_scan_state == CSS_IDLE )
    {
        //  Search the data buffer
        tmp_data_p = strstr( tmp_data_p, MXP_CATEGORIES );

        //  Does this line contain ?
        if( tmp_data_p != NULL )
        {
            //  YES:    This is the first categories line being scanned.
            categories_scan_state = CSS_STARTED;

            // Move the pointer past the marker.
            tmp_data_p += MXP_CATEGORIES_L;

            //  Locate the first character in the buffer
            tmp_data_p = text_skip_past_whitespace( tmp_data_p );
        }
    }
    if( categories_scan_state == CSS_STARTED )
    {
        //  Is this the end of the categories list ?
        if(    ( strncmp( tmp_data_p, MXP_CAT_END_1, MXP_CAT_END_1_L ) == 0 )
            || ( strncmp( tmp_data_p, MXP_CAT_END_2, MXP_CAT_END_2_L ) == 0 )
            || ( strncmp( tmp_data_p, MXP_CAT_END_3, MXP_CAT_END_3_L ) == 0 )
            || ( text_is_blank_line( tmp_data_p )                == true ) )
        {
            //  YES:    Then this is the end of the categories section
            categories_scan_state = CSS_COMPLETE;
        }
        //  Process all categories on the line
        while( 1 )
        {
            //  Skip past any leading whitespace.
            tmp_data_p = text_skip_past_whitespace( tmp_data_p );

            //  Is there anything else in this line ?
            if ( tmp_data_p[ 0 ] == '\0' )
            {
                //  NO:     Then this is the end of the categories section
                categories_scan_state = CSS_COMPLETE;

                //  There isn't anything else to get here
                break;
            }
            else
            {
                //  Wipe the temporary buffer clean
                memset( raw_category, '\0', sizeof( raw_category ) );

                //  Copy the new category to the temp buffer
                strncpy( raw_category, tmp_data_p, SIZE_CATEGORY );

                //  Remove any trailing whitespace
                text_strip_whitespace( raw_category );

                // Translate the category
                xlated_chapter_p = xlate_chapter( raw_category );

                //  Do we have a category to save ?
                if ( xlated_chapter_p != NULL )
                {
                    //  Appliance ?
                    if( strncmp( MXP_APPLIANCE, xlated_chapter_p, MXP_APPLIANCE_L ) == 0 )
                    {
                        recipe_append( recipe_p->appliance,
                                       ( xlated_chapter_p + MXP_APPLIANCE_L ) );
                    }
                    //  Diet ?
                    else
                    if( strncmp( MXP_DIET, xlated_chapter_p, MXP_DIET_L ) == 0 )
                    {
                        recipe_append( recipe_p->diet,
                                       ( xlated_chapter_p + MXP_DIET_L ) );
                    }
                    //  Course ?
                    else
                    if( strncmp( MXP_COURSE, xlated_chapter_p, MXP_COURSE_L ) == 0 )
                    {
                        recipe_append( recipe_p->course,
                                       ( xlated_chapter_p + MXP_COURSE_L ) );
                    }
                    //  Cuisine ?
                    else
                    if( strncmp( MXP_CUISINE, xlated_chapter_p, MXP_CUISINE_L ) == 0 )
                    {
                        recipe_append( recipe_p->cuisine,
                                       ( xlated_chapter_p + MXP_CUISINE_L ) );
                    }
                    //  Occasion ?
                    else
                    if( strncmp( MXP_OCCASION, xlated_chapter_p, MXP_OCCASION_L ) == 0 )
                    {
                        recipe_append( recipe_p->occasion,
                                       ( xlated_chapter_p + MXP_OCCASION_L ) );
                    }
                    //  Category ?
                    else
                    if( strncmp( MXP_CATEGORY, xlated_chapter_p, MXP_CATEGORY_L ) == 0 )
                    {
                        recipe_append( recipe_p->chapter,
                                       ( xlated_chapter_p + MXP_CATEGORY_L ) );
                    }
                }
            }

            //  Is there room on this line for another category ?
            if( strlen( tmp_data_p ) <= SIZE_CATEGORY )
            {
                //  NO:     Time to go
                break;
            }
            else
            {
                //  YES:    Move the data pointer past the completed category
                tmp_data_p += SIZE_CATEGORY;
            }

        }
    }
    if( categories_scan_state == CSS_COMPLETE )
    {
        //  Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  The AUIP (Ingredients     List) immediate follow the tag lines listed
 *  below.  After the line of dashes, everything is input as AUIP until
 *  the first blank line is detected.
 *
 *      FORMATS:
 *         1
 *              Amount  Measure       Ingredient -- Preparation Method
 *              --------  ------------  --------------------------------
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__auip(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'auip' information.
    mxp_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    switch( auip_scan_state )
    {
        case    AUIPS_IDLE:
        {
            //  Is this a blank line ?
            if( text_is_blank_line( tmp_data_p ) != true )
            {
                //  YES:    Change the state
                auip_scan_state = AUIPS_TEXT;
            }
        }   //  The break is intentionally missing.
        case    AUIPS_TEXT:
        {
            //  Is this the AUIP column description ?
            if( strncmp( tmp_data_p, MXP_CAT_END_1, MXP_CAT_END_1_L ) == 0 )
            {
                //  YES:    Change the state
                auip_scan_state = AUIPS_DASH;
            }
        }   break;
        case    AUIPS_DASH:
        {
            //  Is this the AUIP separator line ?
            if(    ( strncmp( tmp_data_p, MXP_CAT_END_2, MXP_CAT_END_2_L ) == 0 )
                || ( strncmp( tmp_data_p, MXP_CAT_END_3, MXP_CAT_END_3_L ) == 0 ) )
            {
                //  YES:    Change the state
                auip_scan_state = AUIPS_AMIP;
            }
        }   break;
        case    AUIPS_AMIP:
        {
            //  Is this a blank line ?
            if( text_is_blank_line( tmp_data_p ) == true )
            {
                //  YES:    DONE: Change the return code.
                mxp_rc = true;

                //  Change the state
                auip_scan_state = AUIPS_COMPLETE;
            }
            else
            {
                // Format the AUIP line
                recipe_fmt_auip( recipe_p, tmp_data_p, RECIPE_FORMAT_MXP );
            }
        }   break;
        case    AUIPS_COMPLETE:
        {
            //  Is this a blank line ?
            if( text_is_blank_line( tmp_data_p ) == true )
            {
                //  YES:    DONE: Change the return code.
                mxp_rc = true;

                //  Change the state
                auip_scan_state = AUIPS_COMPLETE;
            }
        }   break;
    }
    //  Is this the start of a MasterCook MXP recipe ?
    if(    ( strncmp( tmp_data_p, MXP_END_1, MXP_END_1_L ) == 0 )
        || ( strncmp( tmp_data_p, MXP_END_2, MXP_END_2_L ) == 0 ) )
    {
        //  YES:    Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  Following the blank line that follows the ingredients, everything is
 *  part of the directions until the MXP termination line is detected.
 *
 *      FORMATS:
 *         1
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__directions(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param mxp_rc           Return Code                                 */
    int                             mxp_rc;
    /**
     *  @param tmp_data_p       Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'directions' information.
    mxp_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the end of the recipe ?
    if( DECODE_MXP__end( tmp_data_p ) == true )
    {
        //  YES:    Change the return code
        mxp_rc = true;

        //  Change the state
        direction_scan_state = DSS_COMPLETE;
    }
    else
    {
        //  First blank line before directions start ?
        if(    ( text_is_blank_line( tmp_data_p ) == true )
            && ( direction_scan_state            == DSS_IDLE ) )
        {
            //  YES:    Just move on to the next line
        }
        else
        {
            //  NO:     Change the state
            direction_scan_state = DSS_STARTED;

            //  Add the line to the recipe directions
            recipe_add_instructions( recipe_p, tmp_data_p );
        }
    }
    //  Is this the start of a MasterCook MXP recipe ?

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  Copy 'notes' information to the recipe structure.
 *      FORMATS:
 *         1         NOTES:
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Pointer to a recipe structure.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *      The problem here is that there is no real definitive end to a
 *      MasterCook MXP formatted recipe.
 *
 ****************************************************************************/

int
DECODE_MXP__notes(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain any 'notes' information.
    mxp_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the start of a MasterCook MXP recipe ?
    if(    ( strncmp( tmp_data_p, MXP_END_1, MXP_END_1_L ) == 0 ) )
    {
        //  YES:    Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe recipe end marker.
 *      FORMATS:
 *         1         ------------------------------
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return mxp_rc              TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MXP__end(
    char                        *   data_p
    )
{
    /**
     * @param mxp_rc            Return Code                                 */
    int                             mxp_rc;
    /**
     * @param tmp_data_p        Pointer to a temp data buffer               */
    char                        *   tmp_data_p;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  By default this is does NOT contain a 'recipe end' marker.
    mxp_rc = false;

    //  Locate the first character in the buffer
    tmp_data_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Is this the end of a MasterCook MXP recipe ?
    if(    ( strncmp( tmp_data_p, MXP_END_1, MXP_END_1_L ) == 0 )
        || ( email_is_group_break( tmp_data_p )         == true )
        || ( email_is_start( tmp_data_p )               == true ) )
    {
        //  YES:    Change the return code
        mxp_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( mxp_rc );
}

/****************************************************************************/
