/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef DECODE_MXP_API_H
#define DECODE_MXP_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that apply to external 
 *  library components of the 'decode_mxp' library.
 *
 *  @note
 *                   *  Exported from  MasterCook Mac  *
 *                   *  Exported from  MasterCook  *
 *                   *  Exported from  MasterCook II  *
 * 
 *                   *Exported from MasterCook
 *                   * Exported from MasterCook
 *                   { Exported from MasterCook
 *                   *  Exported from CookWorks
 *
 *                       Recipe Title
 *
 *Recipe By     : Who created the recipe
 *Serving Size  : 12   Preparation Time :0:10
 *Categories    : Cakes                            Desserts
 *                Londontowne
 *
 *
 *  Amount  Measure       Ingredient -- Preparation Method
 *- --------  ------------  --------------------------------
 *                        ---Crust----
 *   1 1/2  c             low-fat graham cracker crumbs
 *                        ----Filling----
 *  16      ozs           fat-free cream cheese -- softened
 *     1/2  c             granulated sugar
 *
 *The directions go here. 
 *Please note that additional information may be in the directions.
 *
 *                   - - - - - - - - - - - - - - - - - -
 *
 *NOTES : This sauce is good with a thin pasta such as thin spaghetti; see
 *step 3 for instructions on cooking the pasta.
 *
 *Per serving: 218 Calories; 3g Fat (11% calories from fat); 8g Protein; 39g
 *Carbohydrate; 6mg Cholesterol; 425mg Sodium
 *
 *Serving Ideas : Serve with remaining blueberry pie filling.
 *
 *
 *Nutr. Assoc. : 0 25048 0 0 0 0 0 0 0
 *
 * @note
 *      Title:          'Recipe Title'
 *      Author:         Recipe By     : 
 *      Servings:       Serving Size  : 
 *      Prep Time:      Preparation Time :
 *      Categories:     Categories    : 
 * 
 *  @note:
 *      Because the categories field is generally "crap" I will probably
 *      just ignore them and create my own categories based on the recipe
 *      title and information contained in the directions.
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
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <decode_api.h>         //  API for all decode_*            PUBLIC
                                //*******************************************
#include <email_api.h>          //  API for all email_*             PUBLIC
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
decode_mxp(
    struct  list_base_t         *   mxp_list_p,
    struct  source_info_t       *   source_info_p
    );
//---------------------------------------------------------------------------
int
decode_mxp_start(
    char                        *   data_p
    );
//---------------------------------------------------------------------------
int
decode_mxp_end(
    char                        *   data_p
    );
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DECODE_MXP_API_H
