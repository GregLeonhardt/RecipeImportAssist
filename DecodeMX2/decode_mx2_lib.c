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
    {   "<rtxt>",       MX2_TAG_RTXT    },  //  Start of Embeded MXP
    {   "</rtxt>",      MX2_TAG_ERTXT   },  //  End   of     "

    {   "<![cdata[",    MX2_TAG_CDATA   },  //  Start of Embeded MXP
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

    //  Are we pointed at a tag ?
    if( xml_p[ 0 ] == '<' )
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
        if( tmp_p[ 0 ] == ' ' )
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
                if( tmp_p[ ndx ] == '"' )
                {
                    //  Copy the everything until the next quotation
                    for( ndx ++ ; tmp_p[ ndx ] != '"' ; ndx ++ )
                    {
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
 *  @param  xml_p               Pointer to the start of an xml tag.
 *  @param  text_p              Pointer to a buffer to store the text string.
 *  @param  text_l              Size of the text string.
 *  @param  tag_p               Pointer to the tag name.
 *
 *  @return end_p               Pointer to the next XML tag.
 *
 *  @note
 *
 ****************************************************************************/

char    *
DECODE_MX2__text(
    char                        *   xml_p,
    char                        *   text_p,
    int                             text_l,
    char                        *   tag_p
    )
{
    /**
     *  @param  start_p         Start of text pointer                       */
    char                        *   start_p;
    /**
     *  @param  end_p           End of tag pointer                          */
    char                        *   end_p;
    /**
     *  @param  tag_search      Tag Close String                            */
    char                            tag_search[ 32 ];
    /**
     *  @param  compare         A place to put a lowercase version          */
    char                            compare[ 10 ];

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Clear out the text buffer
    memset( text_p, '\0', text_l );

    /************************************************************************
     *  Build the tag close string
     ************************************************************************/

    //  Clean the buffer
    memset( tag_search, '\0', sizeof( tag_search ) );

    //  Set the prefix
    strncat( tag_search, "</", sizeof( tag_search ) );

    //  Copy the tag name to the tag close buffer
    for( start_p = ( tag_p + 1 );
            (    ( start_p[ 0 ] != '\0' )
              && ( start_p[ 0 ] != ' '  )
              && ( start_p[ 0 ] != '>'  ) );
         start_p += 1 )
    {
        //  Copy the characters in one-by-one
        strncat( tag_search, start_p, 1 );
    }

    //  Close the end tag
    strcat( tag_search, ">" );

    /************************************************************************
     *  Copy the text data to the provided buffer
     ************************************************************************/

    //  Set the start of the search
    end_p = xml_p;

    do
    {
        //  Look for an XML tag
        end_p = strchr( ++end_p, '<' );

            //  Is this the start of the next tag ?
        if( end_p != NULL )
        {
            //  YES:    Fill the compare buffer from the decode buffer
            memset( compare, '\0', sizeof( compare ) );
            memcpy( compare, end_p, ( sizeof( compare ) - 1 ) );
            text_to_lowercase( compare );

            //  Is this the tag we are looking for ?
            if( strncmp( compare, tag_search, strlen( tag_search ) ) == 0 )
            {
                //  YES:    Will the text data fit in the buffer ?
                if( ( end_p - xml_p ) > ( text_l - 1 ) )
                {
                    //  NO:     Log and terminate
                    log_write( MID_FATAL, "DECODE_MX2__text",
                            "The defined buffer size is too small for the text data.\n" );
                }
                else
                {
                    //  YES:    Copy the data to the buffer
                    memcpy( text_p, xml_p, ( end_p - xml_p ) );

                    //  Locate the end of the text area
                    end_p += strlen( tag_search );

                    //  The end of the search
                    break;
                }
            }
        }
    }   while( end_p != NULL );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  Convert any encoded characters to ASCII
    html2txt_str_2_char( text_p );

    //  DONE!
    return( end_p );
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
    if( tmp_p != NULL )
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
            if( strlen( tmp_data ) >= sizeof( tmp_data ) )
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
        if( strlen( tmp_data ) >= sizeof( tmp_data ) )
        {
            //  YES:    OOPS!
            log_write( MID_FATAL, "DECODE_MX2__srce",
                       "The source string ts too large!\n" );
        }
    }

    //  Move the description string to the recipe
    recipe_p->group_from = text_copy_to_new( tmp_data );

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
        if( strlen( tmp_data ) >= sizeof( tmp_data ) )
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
        if( strlen( tmp_data ) >= sizeof( tmp_data ) )
        {
            //  YES:    OOPS!
            log_write( MID_FATAL, "DECODE_MX2__cpyr",
                       "The Copyright string ts too large!\n" );
        }
    }

    //  Move the description string to the recipe
    recipe_p->copyright = text_copy_to_new( tmp_data );

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
        if( strlen( tmp_data ) >= sizeof( tmp_data ) )
        {
            //  YES:    OOPS!
            log_write( MID_FATAL, "DECODE_MX2__srvi",
                       "The Serving Ideas string ts too large!\n" );
        }
    }

    //  Move the description string to the recipe
    recipe_p->serve_with = text_copy_to_new( tmp_data );

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
        if( strlen( tmp_data ) >= sizeof( tmp_data ) )
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

    /************************************************************************
     *  Function Initialization
     ************************************************************************/

    //  Get the size of the decode buffer.
    decode_l = strlen( decode_p );

    //  Allocate the attribute buffer
    attribute_p = mem_malloc( decode_l + 1 );

    //  Allocate the text buffer
    text_p = mem_malloc( decode_l + 1 );

    /************************************************************************
     *  Look for character strings 'Entity', 'MX2 Tags'
     ************************************************************************/

    //  Initialize the pointers
    mx2_offset_p = decode_p;

    while( 1 )
    {
        //  Locate the start of a Character Name string
        mx2_offset_p = strchr(  mx2_offset_p, '<' );

        //  Did we find it ?
        if( mx2_offset_p != NULL )
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
                if( strncmp( compare,
                             mx2_table[ mx2_table_ndx ].mx2_tag,
                             strlen( mx2_table[ mx2_table_ndx ].mx2_tag ) ) == 0 )
                {
                    //  Get the attributes for this tag
                    tmp_a_p = DECODE_MX2__attribute( mx2_offset_p,
                            attribute_p, decode_l );

                    switch( mx2_table[ mx2_table_ndx ].code )
                    {
                        //----------------------------------------------------
                        case    MX2_TAG_MX2:    //  Start of MX2
                        {
                            //  Allocate a new recipe data structure
                            recipe_p = recipe_new( RECIPE_FORMAT_MX2 );

                            //  Copy source information to the recipe structure
                            decode_copy_info_to_recipe( recipe_p, source_info_p );

                            //  DATE="..."
                            if( recipe_p->posted_date != NULL )
                            {
                                //  YES:    There is already a posted date
                                mem_free( recipe_p->posted_date );
                            }
                            recipe_p->posted_date
                                    = DECODE_MX2__srch( attribute_p, "date=\"" );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_RCPE:   //  Start of Recipe Entry
                        {
                            //  NAME="..."
                            recipe_p->name
                                    = DECODE_MX2__srch( attribute_p, "name=\"" );

                            //  AUTHOR="..."
                            recipe_p->author
                                    = DECODE_MX2__srch( attribute_p, "author=\"" );

                            //  Log the new recipe
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
                        case    MX2_TAG_PREPT:  //  Start of Preparation Time
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

                            //  NAME="..."
                            auip_p->ingredient_p = text_copy_to_new( " " );

                            //  UNIT="..."
                            auip_p->unit_p = text_copy_to_new( " " );

                            //  QTY="..."
                            auip_p->amount_p = text_copy_to_new( " " );

                            //  CODE="..."
                            auip_p->type_p = text_copy_to_new( "T" );

                            //  PREPARATION=
                            auip_p->preparation_p = text_copy_to_new( " " );

                            //  Append the new ingredient to the list
                            list_put_last( recipe_p->ingredient, auip_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_IPRP:   //  Start of Ingredient Preparation
                        {
                            //  <IPrp>

                            //  Get the previous ingredient from the list
                            auip_p = list_get_last( recipe_p->ingredient );

                            //  Were we able to get the AUIP pointer ?
                            if( auip_p != NULL )
                            {
                                //  YES:    Remove it from the list
                                list_delete( recipe_p->ingredient, auip_p );
                            }
                            else
                            {
                                //  NO:     Allocate a new ingredient structure
                                auip_p = mem_malloc( sizeof( struct auip_t ) );
                            }

                            //  Move the preparations string to the recipe
                            auip_p->preparation_p = text_copy_to_new( text_p );

                            //  Append the updated ingredient to the list
                            list_put_last( recipe_p->ingredient, auip_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_DIRT:   //  Start of Directions Text
                        {
                            //  <DirT img=...>

                            //  Add it to the Directions
                            recipe_add_instructions( recipe_p, text_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_DESC:   //  Start of Description
                        {
                            //  <Desc>

                            //  Add it to the Directions
                            recipe_p->description = text_copy_to_new( text_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_SRCE:   //  Start of Source
                        {
                            //  <Srce>

                            //  Move the description string to the recipe
                            recipe_p->group_from = text_copy_to_new( text_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_ALTS:   //  Start of Alternate Source
                        {
                            //  <AltS

                            //  Add it to the directions
                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, "SOURCE: " );
                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, "<LABEL> " );
                            recipe_add_instructions( recipe_p, text_copy_to_new(
                                    DECODE_MX2__srch( attribute_p, "label=\"" ) ) );
                            recipe_add_instructions( recipe_p, "<SOURCE> " );
                            recipe_add_instructions( recipe_p, text_copy_to_new(
                                    DECODE_MX2__srch( attribute_p, "source=\"" ) ) );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_CPYR:   //  Start of Copyright
                        {
                            //

                            //  Move the description string to the recipe
                            recipe_p->copyright = text_copy_to_new( text_p );

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
                            //  <ALTT LABEL=".." SOURCE=".."
                            recipe_add_instructions( recipe_p, " "  );
                            recipe_add_instructions( recipe_p, "ALTERNATE TIME: " );
                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, "<LABEL> " );
                            recipe_add_instructions( recipe_p, text_copy_to_new(
                                    DECODE_MX2__srch( attribute_p, "label=\"" ) ) );
                            recipe_add_instructions( recipe_p, "<ELAPSED> " );
                            recipe_add_instructions( recipe_p, text_copy_to_new(
                                    DECODE_MX2__srch( attribute_p, "elapsed=\"" ) ) );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_RATE:   //  Start of Ratings
                        {
                            //  <RATE> NAME=".." VALUE=".."
                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, "RATING: " );
                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, "<NAME> " );
                            recipe_add_instructions( recipe_p, text_copy_to_new(
                                    DECODE_MX2__srch( attribute_p, "name=\"" ) ) );
                            recipe_add_instructions( recipe_p, "<VALUE> " );
                            recipe_add_instructions( recipe_p, text_copy_to_new(
                                    DECODE_MX2__srch( attribute_p, "value=\"" ) ) );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_RATT:   //  Start of Ratings
                        {
                            //  <RATT> NAME=".." VALUE=".."
                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, "RATING: " );
                            recipe_add_instructions( recipe_p, " " );
                            recipe_add_instructions( recipe_p, "<NAME> " );
                            recipe_add_instructions( recipe_p, text_copy_to_new(
                                    DECODE_MX2__srch( attribute_p, "name=\"" ) ) );
                            recipe_add_instructions( recipe_p, "<VALUE> " );
                            recipe_add_instructions( recipe_p, text_copy_to_new(
                                    DECODE_MX2__srch( attribute_p, "value=\"" ) ) );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_WINE:   //  Start of Wine List
                        {
                            //  <WINE> ... </WINE>
                            recipe_p->wine = text_copy_to_new( text_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_SRVI:   //  Start of Serving Ideas
                        {
                            //  <SRVI> ... </SRVI>
                            recipe_p->serve_with = text_copy_to_new( text_p );

                        }   break;
                        //----------------------------------------------------
                        case    MX2_TAG_NOTE:   //  Start of Notes
                        {
                            //  <NOTE> ... </NOTE>
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
                        case    MX2_TAG_EMX2:   //  End   of MX2
                        {
                            //  End-of-Recipe
                            decode_finalize( recipe_p );

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
                        case    MX2_TAG_ESUM:   //  End   of Recipe List
                        case    MX2_TAG_ENAM:   //  End   of Recipe List Name
                        case    MX2_TAG_ERCPE:  //  End   of Recipe Entry
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

                        case    MX2_TAG_CDATA:  //  Start of Embeded MXP
                        case    MX2_TAG_ECDATA: //  End   of    "     "
                        case    MX2_TAG_RTXT:   //  Start of Embeded MXP
                        case    MX2_TAG_ERTXT:  //  End   of    "     "

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

        //  Did we locate the search tag in the tag table ?
        if( mx2_table[ mx2_table_ndx ].code == MX2_TAG_END )
        {
            //  NO:     Write a fatal error message
            log_write( MID_WARNING, "DECODE_MX2__decode",
                    "An unidentified MX2 tag '%s' was found in the recipe stream.\n",
                    compare );
            log_write( MID_WARNING, "DECODE_MX2__decode",
                    "'%.80s'\n", mx2_offset_p );

            //  Move on past the tag
            mx2_offset_p += 1;
        }

        //  Still decoding ?
        if( mx2_offset_p == NULL )
        {
            //  NO:
            break;
        }
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
