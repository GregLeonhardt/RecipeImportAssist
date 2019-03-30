/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private functions that makeup the internal
 *  library components of the 'decode_mx2' library.
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
#include <decode_mx2_api.h>     //  API for all decode_mx2_*        PUBLIC
#include "decode_mx2_lib.h"     //  API for all DECODE_MX2__*       PRIVATE
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
#define MX2_BUFFER_L            ( 1024 * 100 * 10 )     //  10 MB.
//----------------------------------------------------------------------------
#define MX2_DATE_L              ( 32 )
#define MX2_NAME_L              ( 64 )
#define MX2_AUTHOR_L            ( 64 )
//----------------------------------------------------------------------------
#define MX2_TAG_MX2             (  1 )
#define MX2_TAG_EMX2            (  2 )
#define MX2_TAG_SUMM            (  3 )
#define MX2_TAG_ESUM            (  4 )
#define MX2_TAG_NAM             (  5 )
#define MX2_TAG_ENAM            (  6 )
#define MX2_TAG_RCPE            (  7 )
#define MX2_TAG_ERCPE           (  8 )
#define MX2_TAG_SRV             (  9 )
#define MX2_TAG_ESRV            ( 10 )
#define MX2_TAG_PREPT           ( 11 )
#define MX2_TAG_EPREPT          ( 12 )
#define MX2_TAG_PRPT            ( 13 )
#define MX2_TAG_EPRPT           ( 14 )
#define MX2_TAG_CATS            ( 15 )
#define MX2_TAG_ECATS           ( 16 )
#define MX2_TAG_CATT            ( 17 )
#define MX2_TAG_ECATT           ( 18 )
#define MX2_TAG_INGRE           ( 19 )
#define MX2_TAG_INGR            ( 20 )
#define MX2_TAG_EINGR           ( 21 )
#define MX2_TAG_IPRP            ( 22 )
#define MX2_TAG_EIPRP           ( 23 )
#define MX2_TAG_INTI            ( 24 )
#define MX2_TAG_EINTI           ( 25 )
#define MX2_TAG_DIRS            ( 26 )
#define MX2_TAG_EDIRS           ( 27 )
#define MX2_TAG_DIRT            ( 28 )
#define MX2_TAG_EDIRT           ( 29 )
#define MX2_TAG_DESC            ( 30 )
#define MX2_TAG_EDESC           ( 31 )
#define MX2_TAG_NATN            ( 32 )
#define MX2_TAG_ENATN           ( 33 )
#define MX2_TAG_SRCE            ( 34 )
#define MX2_TAG_ESRCE           ( 35 )
#define MX2_TAG_ALTSE           ( 37 )
#define MX2_TAG_ALTS            ( 38 )
#define MX2_TAG_EALTS           ( 39 )
#define MX2_TAG_CPYR            ( 40 )
#define MX2_TAG_ECPYR           ( 41 )
#define MX2_TAG_YIELD           ( 42 )
#define MX2_TAG_EYIELS          ( 43 )
#define MX2_TAG_TTIM            ( 44 )
#define MX2_TAG_ETTIM           ( 45 )
#define MX2_TAG_ALTT            ( 46 )
#define MX2_TAG_EALTT           ( 47 )
#define MX2_TAG_RATS            ( 48 )
#define MX2_TAG_ERATS           ( 49 )
#define MX2_TAG_RATT            ( 50 )
#define MX2_TAG_ERATT           ( 51 )
#define MX2_TAG_RATE            ( 52 )
#define MX2_TAG_ERATE           ( 53 )
#define MX2_TAG_WINEE           ( 54 )
#define MX2_TAG_WINE            ( 55 )
#define MX2_TAG_EWINE           ( 56 )
#define MX2_TAG_SRVI            ( 57 )
#define MX2_TAG_ESRVI           ( 58 )
#define MX2_TAG_NOTE            ( 59 )
#define MX2_TAG_ENOTE           ( 60 )
#define MX2_TAG_NUTR            ( 61 )
#define MX2_TAG_ENUTR           ( 62 )
#define MX2_TAG_RTXT            ( 63 )
#define MX2_TAG_ERTXT           ( 64 )

#define MX2_TAG_CDATA           ( 90 )
#define MX2_TAG_ECDATA          ( 91 )

#define MX2_TAG_END             ( 99 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Structures local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param  mx2_tag_t           MX2 Tag Entities                            */
struct  mx2_tag_t
{
    char                            mx2_tag[ 10 ];
    int                             code;
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Storage Allocation local to this file
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param  mx2_table           Table with all the MX2 tags                 */
struct  mx2_tag_t                   mx2_table[ ] =
{
//  MX2_TAG         CODE                    Description
    {   "<mx2",         MX2_TAG_MX2     },  //  Start of MX2
    {   "</mx2",        MX2_TAG_EMX2    },  //  End   of MX2
    {   "<summ>",       MX2_TAG_SUMM    },  //  Start of Recipe List
    {   "</summ>",      MX2_TAG_ESUM    },  //  End   of    "     "
    {   "<nam>",        MX2_TAG_NAM     },  //  Start of Recipe List Name
    {   "</nam>",       MX2_TAG_ENAM    },  //  End   of    "     "    "
    {   "<rcpe ",       MX2_TAG_RCPE    },  //  Start of Recipe Entry
    {   "</rcpe>",      MX2_TAG_ERCPE   },  //  End   of    "     "
    {   "<serv ",       MX2_TAG_SRV     },  //  Start of Servings
    {   "</serv>",      MX2_TAG_ESRV    },  //  End   of     "
    {   "<prept ",      MX2_TAG_PREPT   },  //  Start of Preparation Time
    {   "</prept>",     MX2_TAG_EPREPT  },  //  End   of      "        "
    {   "<prpt ",       MX2_TAG_PRPT    },  //  Start of Preparation Time
    {   "</prpt>",      MX2_TAG_EPRPT   },  //  End   of      "        "
    {   "<cats>",       MX2_TAG_CATS    },  //  Start of Category list
    {   "</cats>",      MX2_TAG_ECATS   },  //  End   of    "       "
    {   "<catt>",       MX2_TAG_CATT    },  //  Start of Category Entry
    {   "</catt>",      MX2_TAG_ECATT   },  //  End   of    "       "
    {   "<ingr>",       MX2_TAG_INGRE   },  //  Start of Recipe Ingredient
    {   "<ingr ",       MX2_TAG_INGR    },  //  Start of    "       "
    {   "</ingr>",      MX2_TAG_EINGR   },  //  End   of    "       "
    {   "<iprp>",       MX2_TAG_IPRP    },  //  Start of Ingredient Preparation
    {   "</iprp>",      MX2_TAG_EIPRP   },  //  End   of     "          "
    {   "<inti>",       MX2_TAG_INTI    },  //  Start of Ingredient Preparation
    {   "</inti>",      MX2_TAG_EINTI   },  //  End   of     "          "
    {   "<dirs>",       MX2_TAG_DIRS    },  //  Start of Directions
    {   "</dirs>",      MX2_TAG_EDIRS   },  //  End   of     "
    {   "<dirt>",       MX2_TAG_DIRT    },  //  Start of Directions Text
    {   "</dirt>",      MX2_TAG_EDIRT   },  //  End   of     "        "
    {   "<desc>",       MX2_TAG_DESC    },  //  Start of Description
    {   "</desc>",      MX2_TAG_EDESC   },  //  End   of     "
    {   "<natn>",       MX2_TAG_NATN    },  //  Start of Cuisine
    {   "</natn>",      MX2_TAG_ENATN   },  //  End   of     "
    {   "<srce>",       MX2_TAG_SRCE    },  //  Start of Source
    {   "</srce>",      MX2_TAG_ESRCE   },  //  End   of    "
    {   "<alts>",       MX2_TAG_ALTSE   },  //  Start of Alternate Source
    {   "<alts ",       MX2_TAG_ALTS    },  //  Start of     "       "
    {   "</alts>",      MX2_TAG_EALTS   },  //  End   of     "       "
    {   "<cpyr>",       MX2_TAG_CPYR    },  //  Start of Copyright
    {   "</cpyr>",      MX2_TAG_ECPYR   },  //  End   of     "
    {   "<yield ",      MX2_TAG_YIELD   },  //  Start of Yield
    {   "</yield>",     MX2_TAG_EYIELS  },  //  End   of   "
    {   "<ttim ",       MX2_TAG_TTIM    },  //  Start of Time
    {   "</ttim>",      MX2_TAG_ETTIM   },  //  End   of   "
    {   "<altt ",       MX2_TAG_ALTT    },  //  Start of Alternate Time
    {   "</altt>",      MX2_TAG_EALTT   },  //  End   of     "       "
    {   "<rats>",       MX2_TAG_RATS    },  //  Start of Ratings Section
    {   "</rats>",      MX2_TAG_ERATS   },  //  End   of     "       "
    {   "<ratt ",       MX2_TAG_RATT    },  //  Start of Ratings
    {   "</ratt>",      MX2_TAG_ERATT   },  //  End   of     "
    {   "<rate ",       MX2_TAG_RATE    },  //  Start of Ratings
    {   "</rate>",      MX2_TAG_ERATE   },  //  End   of     "
    {   "<wine>",       MX2_TAG_WINEE   },  //  Start of Wine List
    {   "<wine ",       MX2_TAG_WINE    },  //  Start of   "    "
    {   "</wine>",      MX2_TAG_EWINE   },  //  End   of   "    "
    {   "<srvi>",       MX2_TAG_SRVI    },  //  Start of Serving Ideas
    {   "</srvi>",      MX2_TAG_ESRVI   },  //  End   of    "       "
    {   "<note>",       MX2_TAG_NOTE    },  //  Start of Notes
    {   "</note>",      MX2_TAG_ENOTE   },  //  End   of   "
    {   "<nutr>",       MX2_TAG_NUTR    },  //  Start of Nutrition
    {   "</nutr>",      MX2_TAG_ENUTR   },  //  End   of     "
    {   "<rtxt>",       MX2_TAG_RTXT    },  //  Start of Embedded MXP
    {   "</rtxt>",      MX2_TAG_ERTXT   },  //  End   of     "

    {   "<![cdata[",    MX2_TAG_CDATA   },  //  Start of Embedded MXP
    {   "<]]>",         MX2_TAG_ECDATA  },  //  End   of    "     "

    {   "END",          MX2_TAG_END     }   //  Table end.
};
//----------------------------------------------------------------------------

/****************************************************************************
 * LIB Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Locate the attribute string (if any) and copy it to a buffer.
 *
 *  @param  xml_p               Pointer to the start of an xml tag.
 *  @param  attribute_p         Pointer to a buffer to store the text string.
 *  @param  attribute_l         size of the text string.
 *
 *  @return end_p               Pointer to the next XML tag.
 *
 *  @note
 *
 ****************************************************************************/

char    *
DECODE_MX2__attribute(
    char                        *   xml_p,
    char                        *   attribute_p,
    int                             attribute_l
    )
{
    /**
     *  @param  tmp_p           Pointer inside the tag                      */
    char                        *   tmp_p;
    /**
     *  @param  ndx             Data pointer index                          */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Assume that we don't have any attributes for this tag.
    tmp_p = NULL;

    //  Clear out the attribute buffer
    memset( attribute_p, '\0', attribute_l );

    /************************************************************************
     *  Locate the start of the data
     ************************************************************************/

    //  Is this the special <![CDATA[
    if ( strncmp( xml_p, "<![", 3 ) == 0 )
    {
        //  YES:    Skip past it
        tmp_p = &xml_p[ 1 ];
        do
        {
            //  Search for the end of the comment string
            tmp_p = strchr(  &tmp_p[ 1 ], ']' );
        }   while (    ( strncmp( tmp_p, "]]>",  3 ) != 0 )
                    && ( strncmp( tmp_p, "]] >", 4 ) != 0 ) );
        //  NOTE:   The above test for ']] >' is for when the ']]' are the
        //          last two characters of the buffer.  When the next read
        //          occurs a pad space is inserted into the line.  I
        //          suppose I should also add a test for '] ]>' but that is 
        //          for another day.
        
        //  Now point to the next tag
        tmp_p = strchr(  tmp_p, '<' );
    }

    //  Are we pointed at a tag ?
    else
    if ( xml_p[ 0 ] == '<' )
    {
        //  YES:    Is this the end of the tag element name ?
        for( tmp_p = xml_p;
             (    ( tmp_p[ 0 ] != ' ' )
               && ( tmp_p[ 0 ] != '>' ) );
             tmp_p += 1 )
        {
            //  Nothing to do here, we are just moving the pointer
        }

        //  Is there an attribute string here ?
        if ( tmp_p[ 0 ] == ' ' )
        {
            //  YES:    Advance the data pointer to the start of the data string
            tmp_p += 1;

            //  Copy the data to the buffer
            for( ndx = 0;
                 (    ( tmp_p[ ndx ] != '>' )
                   && ( tmp_p[ ndx ] != '/' ) );
                 ndx ++ )
            {
                //  Copy the data one byte at a time
                attribute_p[ ndx ] = tmp_p[ ndx ];

                //  Are we going inside quoted text ?
                if ( tmp_p[ ndx ] == '"' )
                {
                    //  Copy the everything until the next quotation
                    for( ndx ++ ; tmp_p[ ndx ] != '"' ; ndx ++ )
                    {
                        //  Is there room for it ?
                        if ( ndx == attribute_l )
                        {
                            //  NO:     Error message and terminate
                            log_write( MID_WARNING, "DECODE_MX2__attribute",
                                    "The defined buffer size is too small for "
                                    "the text data.\n" );
                            log_write( MID_FATAL, " ", "%s\n", tmp_p );
                        }

                        //  Copy the data one byte at a time
                        attribute_p[ ndx ] = tmp_p[ ndx ];
                    }
                    //  Also copy in the last quotation mark.
                    attribute_p[ ndx ] = tmp_p[ ndx ];
                }
            }
            //  Update the end of tag pointer
            tmp_p = &(tmp_p[ ndx + 1 ] );
        }
        else
        {
            //  Advance the pointer pase the closing brace
            tmp_p += 1;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Convert any encoded characters to ASCII
    html2txt_str_2_char( attribute_p );

    //  DONE!
    return( tmp_p );
}

/****************************************************************************/
/**
 *  Locate the attribute string of a tag (if any) and copy it to a buffer.
 *
 *  @param  src_data_p          Pointer to the source data buffer
 *  @param  text_p              Pointer to a buffer to store the text string.
 *  @param  text_l              Size of the text string.
 *
 *  @return void                No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__text(
    char                        *   src_data_p,
    char                        *   text_p,
    int                             text_l
    )
{
    /**
     *  @param  start_p         Start of text pointer                       */
    char                        *   start_p;
    /**
     *  @param  ndx             Depth index into the src and dst buffers    */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clear out the text buffer
    memset( text_p, '\0', text_l );

    /************************************************************************
     *  Build the tag close string
     ************************************************************************/

    //  Find the end of the start tag
    start_p = strchr( src_data_p, '>' );

    //  Was it found ?
    if ( start_p != NULL )
    {
        //  YES:    skip any leading white space.
        start_p = text_skip_past_whitespace( ++start_p );

        //  Is this another tag ?
        if ( start_p[ 0 ] != '<' )
        {
            //  NO:     Good, now copy the text data
            for ( ndx = 0;
                  start_p[ ndx ] != '<';
                  ndx += 1 )
            {
                //  Is the text too big for the provided buffer ?
                if ( ndx == ( text_l - 1 ) )
                {
                    //  YES:    Log and terminate
                    log_write( MID_FATAL, "DECODE_MX2__text",
                            "The defined buffer size is too small for the text data.\n" );
                }

                //  Copy another byte of data
                text_p [ ndx ] = start_p[ ndx ];
            }
            //  Strip off any trailing white space.
            text_strip_whitespace( text_p );
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Convert any encoded characters to ASCII
    html2txt_str_2_char( text_p );

    //  DONE!
}

/****************************************************************************/
/**
 *  Extract a quoted string
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  srch_p              Pointer to the search string
 *
 *  @return string_p            Pointer to the data string
 *
 *  @note
 *
 ****************************************************************************/

char    *
DECODE_MX2__srch(
    char                        *   data_p,
    char                        *   srch_p
    )
{
    /**
     *  @param  tmp_p           Temporary data buffer pointer               */
    char                        *   tmp_p;
    /**
     *  @param  tmp_data        Temporary data buffer                       */
    char                            tmp_data[ 2048 ];
    /**
     *  @param  ndx             Data pointer index                          */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Find a match for the search string
     ************************************************************************/

    //  Locate the search string
    tmp_p = strstr( data_p, srch_p );

    //  Did we locate it ?
    if ( tmp_p != NULL )
    {
        //  YES:    Adjust the pointer to the start of data
        tmp_p = ( strchr( tmp_p, '"' ) + 1 );

        //  Clean out the temporary data buffer
        memset( tmp_data, '\0', sizeof( tmp_data ) );

        //  Copy the data to the temporary data buffer
        for( ndx = 0; tmp_p[ ndx ] != '"'; ndx += 1 )
        {
            tmp_data[ ndx ] = tmp_p[ ndx ];

            //  Have we exceeded the buffer size ?
            if ( strlen( tmp_data ) >= sizeof( tmp_data ) )
            {
                //  YES:    OOPS!
                log_write( MID_FATAL, "DECODE_MX2__srch",
                           "The string ts too large!\n" );
            }
        }

        //  Convert any encoded characters to ASCII
        html2txt_str_2_char( tmp_data );

        //  Move the string to the recipe
        tmp_p = text_copy_to_new( tmp_data );

        log_write( MID_DEBUG_1, "decode_mx2_lib.c", "Line: %d\n", __LINE__ );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return( tmp_p );
}

/****************************************************************************/
/**
 *  Start of Source
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__srce(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  tmp_p           Temporary data buffer pointer               */
    char                        *   tmp_p;
    /**
     *  @param  tmp_data        Temporary data buffer                       */
    char                            tmp_data[ 1024 ];
    /**
     *  @param  ndx             Data pointer index                          */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clean the temporary buffer
    memset( tmp_data, '\0',sizeof( tmp_data ) );

    tmp_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/


    //  Copy the source data to the temporary data buffer
    for( ndx = 0; tmp_p[ ndx ] != '<'; ndx += 1 )
    {
        tmp_data[ ndx ] = tmp_p[ ndx ];

        //  Have we exceeded the buffer size ?
        if ( strlen( tmp_data ) >= sizeof( tmp_data ) )
        {
            //  YES:    OOPS!
            log_write( MID_FATAL, "DECODE_MX2__srce",
                       "The source string ts too large!\n" );
        }
    }

    //  Move the description string to the recipe
    recipe_p->group_from = text_copy_to_new( tmp_data );
 
    log_write( MID_DEBUG_1, "decode_mx2_lib.c", "Line: %d\n", __LINE__ );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Alternate Source
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__alts(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  tmp_p           Temporary data buffer pointer               */
    char                        *   tmp_p;
    /**
     *  @param  tmp_data        Temporary data buffer                       */
    char                            tmp_data[ 1024 ];
    /**
     *  @param  ndx             Data pointer index                          */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clean the temporary buffer
    memset( tmp_data, '\0',sizeof( tmp_data ) );

    tmp_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/

    //  Copy the alternate source data to the temporary data buffer
    for( ndx = 0;
         (    ( tmp_p[ ndx - 1 ] != '/' )
           && ( tmp_p[ ndx     ] != '>' ) );
         ndx += 1 )
    {
        tmp_data[ ndx ] = tmp_p[ ndx ];

        //  Have we exceeded the buffer size ?
        if ( strlen( tmp_data ) >= sizeof( tmp_data ) )
        {
            //  YES:    OOPS!
            log_write( MID_FATAL, "DECODE_MX2__alts",
                       "The alternate source string ts too large!\n" );
        }
    }

    //  Add it to the directions
    recipe_add_instructions( recipe_p, &(tmp_data[ 0 ]) );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Copyright
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__cpyr(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  tmp_p           Temporary data buffer pointer               */
    char                        *   tmp_p;
    /**
     *  @param  tmp_data        Temporary data buffer                       */
    char                            tmp_data[ 1024 ];
    /**
     *  @param  ndx             Data pointer index                          */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clean the temporary buffer
    memset( tmp_data, '\0',sizeof( tmp_data ) );

    tmp_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/


    //  Copy the Copyright data to the temporary data buffer
    for( ndx = 0; tmp_p[ ndx ] != '<'; ndx += 1 )
    {
        tmp_data[ ndx ] = tmp_p[ ndx ];

        //  Have we exceeded the buffer size ?
        if ( strlen( tmp_data ) >= sizeof( tmp_data ) )
        {
            //  YES:    OOPS!
            log_write( MID_FATAL, "DECODE_MX2__cpyr",
                       "The Copyright string ts too large!\n" );
        }
    }

    //  Move the description string to the recipe
    recipe_p->copyright = text_copy_to_new( tmp_data );

    log_write( MID_DEBUG_1, "decode_mx2_lib.c", "Line: %d\n", __LINE__ );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Alternate Time
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__altt(
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


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Ratings Section
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__rats(
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


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Ratings
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__rate(
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


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Wine List
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__wine(
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


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Serving Ideas
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__srvi(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  tmp_p           Temporary data buffer pointer               */
    char                        *   tmp_p;
    /**
     *  @param  tmp_data        Temporary data buffer                       */
    char                            tmp_data[ 2048 ];
    /**
     *  @param  ndx             Data pointer index                          */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clean the temporary buffer
    memset( tmp_data, '\0',sizeof( tmp_data ) );

    tmp_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/


    //  Copy the Serving Ideas data to the temporary data buffer
    for( ndx = 0; tmp_p[ ndx ] != '<'; ndx += 1 )
    {
        tmp_data[ ndx ] = tmp_p[ ndx ];

        //  Have we exceeded the buffer size ?
        if ( strlen( tmp_data ) >= sizeof( tmp_data ) )
        {
            //  YES:    OOPS!
            log_write( MID_FATAL, "DECODE_MX2__srvi",
                       "The Serving Ideas string ts too large!\n" );
        }
    }

    //  Move the description string to the recipe
    recipe_p->serve_with = text_copy_to_new( tmp_data );

    log_write( MID_DEBUG_1, "decode_mx2_lib.c", "Line: %d\n", __LINE__ );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Notes
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__note(
    char                        *   data_p,
    struct   recipe_t           *   recipe_p
    )
{
    /**
     *  @param  tmp_p           Temporary data buffer pointer               */
    char                        *   tmp_p;
    /**
     *  @param  tmp_data        Temporary data buffer                       */
    char                            tmp_data[ 4096 ];
    /**
     *  @param  ndx             Data pointer index                          */
    int                             ndx;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clean the temporary buffer
    memset( tmp_data, '\0',sizeof( tmp_data ) );

    tmp_p = text_skip_past_whitespace( data_p );

    /************************************************************************
     *  Function
     ************************************************************************/


    //  Copy the Notes data to the temporary data buffer
    for( ndx = 0; tmp_p[ ndx ] != '<'; ndx += 1 )
    {
        tmp_data[ ndx ] = tmp_p[ ndx ];

        //  Have we exceeded the buffer size ?
        if ( strlen( tmp_data ) >= sizeof( tmp_data ) )
        {
            //  YES:    OOPS!
            log_write( MID_FATAL, "DECODE_MX2__note",
                       "The Notes string ts too large!\n" );
        }
    }

    //  Add it to the directions
    recipe_fmt_notes( recipe_p, &(tmp_data[ 0 ]) );


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Start of Nutrition
 *
 *  @param  data_p              Pointer to text string to be tested.
 *  @param  recipe_p            Primary structure for a recipe
 *
 *  @return void                No information is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__nutr(
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


    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Test the string for a recipe START marker.
 *      FORMATS:
 *          1   |<mx2|
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MX2__start(
    char                        *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mx2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the start a MX2 recipe
    mx2_rc = false;

    /************************************************************************
     *  |<RcpE name=|
     ************************************************************************/

    if ( strstr( data_p, "<mx2" ) != NULL )
    {
        mx2_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( mx2_rc );
}

/****************************************************************************/
/**
 *  Test the string for a recipe end marker.
 *      FORMATS:
 *          1   |</mx2>|
 *
 *  @param  data_p              Pointer to text string to be tested.
 *
 *  @return                     TRUE when the text string matches a known
 *                              end of recipe marker, else FALSE
 *
 *  @note
 *
 ****************************************************************************/

int
DECODE_MX2__end(
    char                        *   data_p
    )
{
    /**
     * @param mmf_rc            Return Code                                 */
    int                             mx2_rc;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  The assumption is that this is NOT the end a MX2 recipe
    mx2_rc = false;

    /************************************************************************
     *  |</RcpE>|
     ************************************************************************/

    if ( strstr( data_p, "</mx2>" ) != NULL )
    {
        mx2_rc = true;
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
    return ( mx2_rc );
}

/****************************************************************************/
/**
 *  This is where all the heavy lifting is done for a MasterCook MX2 recipe.
 *
 *  @param  decode_p            Pointer to text string to be decoded.
 *  @param  source_info_p       Pointer to source data information
 *
 *  @return void                No information is returned from this function.
 *
 *  @note
 *
 ****************************************************************************/

void
DECODE_MX2__decode(
    char                        *   decode_p,
    struct  source_info_t       *   source_info_p
    )
{
    /**
     *  @param  mx2_table_ndx   Index into the MX2 conversion table         */
    int                             mx2_table_ndx;
    /**
     *  @param  tmp_a_p         Pointer to a temporary data buffer          */
    char                        *   tmp_a_p;
    /**
     *  @param  mx2_offset_p    Pointer to the first space in MX2 buffer    */
    char                        *   mx2_offset_p;
    /**
     *  @param  compare         A place to put a lowercase version          */
    char                            compare[ 10 ];
    /**
     *  @param recipe_p         Primary structure for a recipe              */
    struct   recipe_t           *   recipe_p;
    /**
     *  @param  auip_p         Pointer to allocated AUIP structure          */
    struct  auip_t              *   auip_p;
    /**
     *  @param  attribute_p    Pointer to the tags attribute string         */
    char                        *   attribute_p;
    /**
     *  @param  text_p         Pointer to the tags text string              */
    char                        *   text_p;
    /**
     *  @param  decode_l        Size of the decode buffer                   */
    int                             decode_l;
    /**
     *  @param  mxp_inside_mx2  Flag indicating that we found a MXP recipe  */
    int                             mxp_inside_mx2;

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Get the size of the decode buffer.
    decode_l = strlen( decode_p );

    //  Allocate the attribute buffer
    attribute_p = mem_malloc( decode_l + 1 );

    log_write( MID_DEBUG_1, "decode_mx2_lib.c", "Line: %d\n", __LINE__ );

    //  Allocate the text buffer
    text_p = mem_malloc( decode_l + 1 );

    log_write( MID_DEBUG_1, "decode_mx2_lib.c", "Line: %d\n", __LINE__ );

    //  Null out the recipe pointer
    recipe_p = NULL;

    //  Set the found MXP recipe flag FALSE
    mxp_inside_mx2 = false;

    /************************************************************************
     *  Look for character strings 'Entity', 'MX2 Tags'
     ************************************************************************/

    //  Initialize the pointers
    mx2_offset_p = decode_p;

    while( 1 )
    {
        //  Just in case there is some white space before the tag
        mx2_offset_p = text_skip_past_whitespace( mx2_offset_p );

        //  Locate the start of a Character Name string
        mx2_offset_p = strchr(  mx2_offset_p, '<' );

        //  Did we find it ?
        if ( mx2_offset_p != NULL )
        {

            /****************************************************************
             *  Scan for MX2 tags
             ****************************************************************/

            //  YES:    Fill the compare buffer from the decode buffer
            memset( compare, '\0', sizeof( compare ) );
            memcpy( compare, mx2_offset_p, ( sizeof( compare ) - 1 ) );
            text_to_lowercase( compare );

            //  Loop through all entries in the MX2 conversion table
            for( mx2_table_ndx = 0;
                 mx2_table[ mx2_table_ndx ].code != MX2_TAG_END;
                 mx2_table_ndx += 1 )
            {

                //  Is the string a match to a MX2 Tag ?
                if ( strncmp( compare,
                              mx2_table[ mx2_table_ndx ].mx2_tag,
                              strlen( mx2_table[ mx2_table_ndx ].mx2_tag ) ) == 0 )
                {
                    //  Get the attributes for this tag
                    tmp_a_p = DECODE_MX2__attribute( mx2_offset_p,
                            attribute_p, decode_l );

                    //  Did we find a MXP recipe inside a MX2 recipe ?
                    if ( mxp_inside_mx2 == true )
                    {
                        //  YES:    Is this the end of the MXP recipe ?
                        if ( mx2_table[ mx2_table_ndx ].code != MX2_TAG_ERTXT )
                        {
                            //  NO:     Update the decode pointer.
                            mx2_offset_p = tmp_a_p;

                            //  Skip processing until we find it.
                            break;
                        }
                    }

                    switch( mx2_table[ mx2_table_ndx ].code )
                    {
                        //----------------------------------------------------
                        case    MX2_TAG_RCPE:   //  Start of Recipe Entry
                        {
                            //  Allocate a new recipe data structure
                            recipe_p = recipe_new( RECIPE_FORMAT_MX2 );

                            //  Copy source information to the recipe structure
                            decode_copy_info_to_recipe( recipe_p, source_info_p );

                            //  DATE="..."
                            if ( recipe_p->posted_date != NULL )
                            {
                                //  YES:    There is already a posted date
                                mem_free( recipe_p->posted_date );
                            }
                            recipe_p->posted_date
                                    = DECODE_MX2__srch( attribute_p, "date=\"" );

                        //----------------------------------------------------
                            //  NAME="..."
                            recipe_p->name
                                    = DECODE_MX2__srch( attribute_p, "name=\"" );

                            //  AUTHOR="..."
                            recipe_p->author
                                    = DECODE_MX2__srch( attribute_p, "author=\"" );

                            //  Change to Title Case
                            text_title_case( recipe_p->name, recipe_p->name );

                            //  Log the new title
                            log_write( MID_INFO, "decode_mx2",
                               "'%s'\n", recipe_p->name );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_SRV:    //  Start of Servings
                        {
                            //  QTY="..."
                            recipe_p->serves
                                    = DECODE_MX2__srch( attribute_p, "qty=\"" );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_PREPT:  //  Start of Total Time
                        {
                            //  ELAPSED="..."
                            recipe_p->time_total
                                    = DECODE_MX2__srch( attribute_p, "elapsed=\"" );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_PRPT:   //  Start of Preparation Time
                        {
                            //  ELAPSED="..."
                            recipe_p->time_prep
                                    = DECODE_MX2__srch( attribute_p, "elapsed=\"" );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_CATT:   //  Start of Category Entry
                        case    MX2_TAG_NATN:   //  Start of Cuisine
                        {
                            /**
                             *  @param xlated_data_p    Translated chapter  */
                            char                         *  xlated_data_p;

                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            //  Translate the chapter
                            xlated_data_p = xlate_chapter( text_p );

                            //  Do we have a chapter to save ?
                            if ( xlated_data_p != NULL )
                            {
                                //  YES:    Save it
                                decode_save_chapter( xlated_data_p, recipe_p );
                            }

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_INGR:   //  Start of Recipe Ingredient
                        {
                            //  Allocate a new ingredient structure
                            auip_p = mem_malloc( sizeof( struct auip_t ) );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                            //  NAME="..."
                            auip_p->ingredient_p
                                    = DECODE_MX2__srch( attribute_p, "name=\"" );

                            //  UNIT="..."
                            auip_p->unit_p
                                    = DECODE_MX2__srch( attribute_p, "unit=\"" );

                            //  QTY="..."
                            auip_p->amount_p
                                    = DECODE_MX2__srch( attribute_p, "qty=\"" );

                            //  CODE="..."
                            auip_p->type_p
                                    = DECODE_MX2__srch( attribute_p, "code=\"" );

                            //  Append the new ingredient to the list
                            list_put_last( recipe_p->ingredient, auip_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_INGRE:  //  Start of "EMPTY" Recipe Ingredient
                        {
                            //  Allocate a new ingredient structure
                            auip_p = mem_malloc( sizeof( struct auip_t ) );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                            //  NAME="..."
                            auip_p->ingredient_p = text_copy_to_new( " " );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                            //  UNIT="..."
                            auip_p->unit_p = text_copy_to_new( " " );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                            //  QTY="..."
                            auip_p->amount_p = text_copy_to_new( " " );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                            //  CODE="..."
                            auip_p->type_p = text_copy_to_new( "T" );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                            //  PREPARATION=
                            auip_p->preparation_p = text_copy_to_new( " " );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                            //  Append the new ingredient to the list
                            list_put_last( recipe_p->ingredient, auip_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_IPRP:   //  Start of Ingredient Preparation
                        {
                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            //  Get the previous ingredient from the list
                            auip_p = list_get_last( recipe_p->ingredient );

                            //  Were we able to get the AUIP pointer ?
                            if ( auip_p != NULL )
                            {
                                //  YES:    Remove it from the list
                                list_delete( recipe_p->ingredient, auip_p );
                            }
                            else
                            {
                                //  NO:     Allocate a new ingredient structure
                                auip_p = mem_malloc( sizeof( struct auip_t ) );

                                log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                        "Line: %d\n", __LINE__ );
                            }

                            //  Move the preparations string to the recipe
                            auip_p->preparation_p = text_copy_to_new( text_p );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                            //  Append the updated ingredient to the list
                            list_put_last( recipe_p->ingredient, auip_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_DIRT:   //  Start of Directions Text
                        {
                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            //  Add it to the Directions
                            recipe_add_instructions( recipe_p, text_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_DESC:   //  Start of Description
                        {
                            //  <Desc>

                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            //  Add it to the Directions
                            recipe_p->description = text_copy_to_new( text_p );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_SRCE:   //  Start of Source
                        {
                            //  <Srce>

                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            //  Are we overwriting something ?
                            if (    ( strlen( text_p ) > 0 )
                                 && ( recipe_p->group_from != NULL ) )
                            {
                                //  YES:    Free storage for the old data
                                mem_free( recipe_p->group_from );
                            }

                            //  Move the description string to the recipe
                            recipe_p->group_from = text_copy_to_new( text_p );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_ALTS:   //  Start of Alternate Source
                        {
                            /**
                             *  @param  label_p                             */
                            char                        *   label_p;
                            /**
                             *  @param  source_p                            */
                            char                        *   source_p;

                            //  Locate the information
                            label_p = DECODE_MX2__srch( attribute_p, "label=\"" );
                            source_p = DECODE_MX2__srch( attribute_p, "source=\"" );

                            //  Did we get the information ?
                            if (    ( label_p  != NULL )
                                 && ( source_p != NULL ) )
                            {
                                //  YES:    Add it to the directions
                                recipe_add_instructions( recipe_p, " " );
                                recipe_add_instructions( recipe_p, "SOURCE: " );
                                recipe_add_instructions( recipe_p, " " );
                                recipe_add_instructions( recipe_p, "<LABEL> " );
                                recipe_add_instructions( recipe_p, label_p );
                                recipe_add_instructions( recipe_p, "<SOURCE> " );
                                recipe_add_instructions( recipe_p, source_p );
                            }
                            
                            //  Release their buffers.
                            if ( label_p  != NULL )
                                mem_free( label_p );
                            if ( source_p  != NULL )
                                mem_free( source_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_CPYR:   //  Start of Copyright
                        {
                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            //  Move the description string to the recipe
                            recipe_p->copyright = text_copy_to_new( text_p );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_YIELD:  //  Start of Yield
                        {
                            //  UNIT=
                            recipe_p->makes_unit
                                    = DECODE_MX2__srch( attribute_p, "unit=\"" );

                            //  QTY=
                            recipe_p->makes
                                    = DECODE_MX2__srch( attribute_p, "qty=\"" );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_TTIM:   //  Start of Total Time
                        {
                            //  <TTIM> ELAPSED=
                            recipe_p->time_total
                                    = DECODE_MX2__srch( attribute_p, "elapsed=\"" );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_ALTT:   //  Start of Alternate Time
                        {
                            /**
                             *  @param  label_p                             */
                            char                        *   label_p;
                            /**
                             *  @param  elapsed_p                           */
                            char                        *   elapsed_p;

                            //  Locate the information
                            label_p = DECODE_MX2__srch( attribute_p, "label=\"" );
                            elapsed_p = DECODE_MX2__srch( attribute_p, "elapsed=\"" );

                            //  Did we get the information ?
                            if (    ( label_p   != NULL )
                                 && ( elapsed_p != NULL ) )
                            {
                                //  <ALTT LABEL=".." SOURCE=".."
                                recipe_add_instructions( recipe_p, " "  );
                                recipe_add_instructions( recipe_p, "ALTERNATE TIME: " );
                                recipe_add_instructions( recipe_p, " " );
                                recipe_add_instructions( recipe_p, "<LABEL> " );
                                recipe_add_instructions( recipe_p, label_p );
                                recipe_add_instructions( recipe_p, "<ELAPSED> " );
                                recipe_add_instructions( recipe_p, elapsed_p );
                            }

                            //  Release their buffers.
                            if ( label_p  != NULL )
                                mem_free( label_p );
                            if ( elapsed_p  != NULL )
                                mem_free( elapsed_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_RATE:   //  Start of Ratings
                        {
                            /**
                             *  @param  name_p                              */
                            char                        *   name_p;
                            /**
                             *  @param  value_p                             */
                            char                        *   value_p;

                            //  Locate the information
                            name_p = DECODE_MX2__srch( attribute_p, "name=\"" );
                            value_p = DECODE_MX2__srch( attribute_p, "value=\"" );

                            //  Did we get the information ?
                            if (    ( name_p != NULL )
                                 && ( value_p != NULL ) )
                            {
                                //  <RATE> NAME=".." VALUE=".."
                                recipe_add_instructions( recipe_p, " " );
                                recipe_add_instructions( recipe_p, "RATING: " );
                                recipe_add_instructions( recipe_p, " " );
                                recipe_add_instructions( recipe_p, "<NAME> " );
                                recipe_add_instructions( recipe_p, name_p );
                                recipe_add_instructions( recipe_p, "<VALUE> " );
                                recipe_add_instructions( recipe_p, value_p );
                            }

                            //  Release their buffers.
                            if ( name_p  != NULL )
                                mem_free( name_p );
                            if ( value_p  != NULL )
                                mem_free( value_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_RATT:   //  Start of Ratings
                        {
                            /**
                             *  @param  name_p                              */
                            char                        *   name_p;
                            /**
                             *  @param  value_p                             */
                            char                        *   value_p;

                            //  Locate the information
                            name_p = DECODE_MX2__srch( attribute_p, "name=\"" );
                            value_p = DECODE_MX2__srch( attribute_p, "value=\"" );

                            //  Did we get the information ?
                            if (    ( name_p  != NULL )
                                 && ( value_p != NULL ) )
                            {
                                //  <RATT> NAME=".." VALUE=".."
                                recipe_add_instructions( recipe_p, " " );
                                recipe_add_instructions( recipe_p, "RATING: " );
                                recipe_add_instructions( recipe_p, " " );
                                recipe_add_instructions( recipe_p, "<NAME> " );
                                recipe_add_instructions( recipe_p, name_p );
                                recipe_add_instructions( recipe_p, "<VALUE> " );
                                recipe_add_instructions( recipe_p, value_p );
                            }

                            //  Release their buffers.
                            if ( name_p  != NULL )
                                mem_free( name_p );
                            if ( value_p  != NULL )
                                mem_free( value_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_WINE:   //  Start of Wine List
                        {
                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            recipe_p->wine = text_copy_to_new( text_p );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_SRVI:   //  Start of Serving Ideas
                        {
                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            recipe_p->serve_with = text_copy_to_new( text_p );

                            log_write( MID_DEBUG_1, "decode_mx2_lib.c", 
                                    "Line: %d\n", __LINE__ );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_NOTE:   //  Start of Notes
                        {
                            //  Copy the tag text filed (if there is one)
                            DECODE_MX2__text( mx2_offset_p, text_p, decode_l );

                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, "NOTES: " );
                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, text_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_NUTR:   //  Start of Nutrition
                        {
                            //  <NUTR> ... </NUTR>
//                          I am choosing to exclude the nutritional
//                          information for now.

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_ERCPE:  //  Start of Recipe Entry
                        {
                            //  End-of-Recipe
                            decode_finalize( recipe_p );

                            //  Done with the old one
                            recipe_p = NULL;

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_RTXT:   //  Start of Embedded MXP
                        {
                            //  Set the flag
                            mxp_inside_mx2 = true;
                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_ERTXT:  //  End   of Embedded MXP
                        {
                            //  Clear the found MXP recipe flag
                            mxp_inside_mx2 = false;
                        }   break;
                        //----------------------------------------------------
                        //      I don't know what the following are or what
                        //      to do with them.
//  TODO    [MX2]   What is this ?
                        case    MX2_TAG_WINEE:  //  Start of Wine list
//  TODO    [MX2]   What is this ?
                        case    MX2_TAG_INTI:   //  Start of ???
//  TODO    [MX2]   What is this ?
                        case    MX2_TAG_ALTSE:  //  Start of Alternate Source
                        //----------------------------------------------------
                        //      The following are section starters that
                        //      do NOT contain any (important) data.
                        case    MX2_TAG_SUMM:   //  Start of Recipe List
                        case    MX2_TAG_NAM:    //  Start of Recipe List Name
                        case    MX2_TAG_DIRS:   //  Start of Directions Section
                        case    MX2_TAG_CATS:   //  Start of Category list
                        case    MX2_TAG_RATS:   //  Start of Ratings Section
                        //----------------------------------------------------
                        //      The following are ending statements.
                        case    MX2_TAG_MX2:    //  Start of MX2
                        case    MX2_TAG_EMX2:   //  End   of MX2
                        case    MX2_TAG_ESUM:   //  End   of Recipe List
                        case    MX2_TAG_ENAM:   //  End   of Recipe List Name
                        case    MX2_TAG_ESRV:   //  End   of Servings
                        case    MX2_TAG_EPREPT: //  End   of Preparation Time
                        case    MX2_TAG_ECATS:  //  End   of Category list
                        case    MX2_TAG_EPRPT:  //  End   of Preparation Time
                        case    MX2_TAG_ECATT:  //  End   of Category Entry
                        case    MX2_TAG_EINGR:  //  End   of Recipe Ingredient
                        case    MX2_TAG_EIPRP:  //  End   of Ingredient Preparation
                        case    MX2_TAG_EDIRS:  //  End   of Directions Section
                        case    MX2_TAG_EDIRT:  //  End   of Directions Text
                        case    MX2_TAG_EDESC:  //  End   of Description
                        case    MX2_TAG_ENATN:  //  End   of Cuisine
                        case    MX2_TAG_ESRCE:  //  End   of Source
                        case    MX2_TAG_EALTS:  //  End   of Alternate Source
                        case    MX2_TAG_ECPYR:  //  End   of Copyright
                        case    MX2_TAG_EYIELS: //  End   of Yield
                        case    MX2_TAG_ETTIM:  //  End   of Time
                        case    MX2_TAG_EALTT:  //  End   of Alternate Time
                        case    MX2_TAG_ERATS:  //  End   of Ratings Section
                        case    MX2_TAG_ERATT:  //  End   of Ratings
                        case    MX2_TAG_ERATE:  //  End   of Ratings
                        case    MX2_TAG_EWINE:  //  End   of Wine List
                        case    MX2_TAG_ESRVI:  //  End   of Serving Ideas
                        case    MX2_TAG_ENOTE:  //  End   of Notes
                        case    MX2_TAG_ENUTR:  //  End   of Nutrition
                        case    MX2_TAG_EINTI:  //  End   of ???

                        case    MX2_TAG_CDATA:  //  Start of Embedded MXP
                        case    MX2_TAG_ECDATA: //  End   of    "     "

                        case    MX2_TAG_END:    //  Table end.
                        {
                            //  Nothing to do here, Move on down the line.
                        }   break;
                        //----------------------------------------------------
                        default:
                        {
                            log_write( MID_FATAL, "DECODE_MX2__decode",
                                    "An unidentified MX2 tag '%s' was found\n",
                                    mx2_table[ mx2_table_ndx ].mx2_tag );
                        }
                        //----------------------------------------------------
                    }

                    //  Update the decode pointer.
                    mx2_offset_p = tmp_a_p;

                    //  We are done here.
                    break;
                }
            }
        }

        //  Still decoding ?
        if ( mx2_offset_p == NULL )
        {
            //  NO:
            break;
        }

        //  Did we locate the search tag in the tag table ?
        if ( mx2_table[ mx2_table_ndx ].code == MX2_TAG_END )
        {
            //  NO:     Are we actively processing a recipe ?
            if ( recipe_p != NULL )
            {
                //  YES:    Write a fatal error message
                log_write( MID_WARNING, "DECODE_MX2__decode",
                        "An unidentified MX2 tag '%s' was found in the recipe stream.\n",
                        compare );
                log_write( MID_WARNING, "DECODE_MX2__decode",
                        "'%.80s'\n", mx2_offset_p );
            }

            //  Move on past the tag
            mx2_offset_p += 1;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Release storage allocated in this function.
    mem_free( attribute_p );
    mem_free( text_p );

    //  DONE!
}

/****************************************************************************/
