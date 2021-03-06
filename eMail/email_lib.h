/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef EMAIL_LIB_H
#define EMAIL_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal 
 *  library components of the 'email' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_EMAIL
   #define EMAIL_EXT
#else
   #define EMAIL_EXT            extern
#endif

/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Library Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define EMAIL_BOUNDARY_L            ( 100 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
enum    email_filter_e
{
    EMAIL_FILTER_NONE           =   0,
    EMAIL_FILTER_X_YMAILISG     =   1,
    EMAIL_FILTER_BASE64         =   2,
    EMAIL_FILTER_UTF_8          =   3,
    EMAIL_FILTER_REPLY          =   4
};
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param  email_filtering     When TRUE we are filtering the data stream  */
EMAIL_EXT
    enum    email_filter_e          email_filter_type;
//----------------------------------------------------------------------------
/**
 *  @param  email_blank_count   Number of blank lines needed before
 *                              normal processing resumes                   */
EMAIL_EXT
    int                             email_blank_count;
//----------------------------------------------------------------------------
/**
 *  @param  email_blank_count   Number of blank lines needed before
 *                              normal processing resumes                   */
EMAIL_EXT
    char                            email_boundary[ EMAIL_BOUNDARY_L ];
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    EMAIL_LIB_H
