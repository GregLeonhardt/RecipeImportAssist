/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef ENCODE_RIA_API_H
#define ENCODE_RIA_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external 
 *  library components of the 'encode_ria' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/


/****************************************************************************
 * System APIs
 ****************************************************************************/

                                //*******************************************
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <recipe_api.h>         //  API for all recipe_*            PUBLIC
                                //*******************************************

/****************************************************************************
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Public Storage Allocation
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************
 * Library Public Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
void
encode_ria(
    struct   recipe_t           *   recipe_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    ENCODE_RIA_API_H
