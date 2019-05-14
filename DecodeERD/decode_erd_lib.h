/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2019 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef DECODE_ERD_LIB_H
#define DECODE_ERD_LIB_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains private definitions (etc.) that apply to internal 
 *  library components of the 'decode_erd' library.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_DECODE_ERD
   #define DECODE_ERD_EXT
#else
   #define DECODE_ERD_EXT         extern
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
#define ERD_PREFIX              "--------"
#define ERD_PREFIX_L            strlen( ERD_PREFIX )
//----------------------------------------------------------------------------
#define ERD_START               "Easy Recipe Deluxe Export"
#define ERD_START_L             strlen( ERD_START )
//----------------------------------------------------------------------------
#define ERD_SUFFIX              "- - - - "
#define ERD_SUFFIX_L            strlen( ERD_SUFFIX )
//----------------------------------------------------------------------------
#define ERD_END                 "http://www.wensoftware.com"
#define ERD_END_L               strlen( ERD_END )
//----------------------------------------------------------------------------
#define ERD_TITLE               "Title:"
#define ERD_TITLE_L             strlen( ERD_TITLE )
//----------------------------------------------------------------------------
#define ERD_RECIPE_BY           "Recipe By:"
#define ERD_RECIPE_BY_L         strlen( ERD_RECIPE_BY )
//----------------------------------------------------------------------------
#define ERD_CATEGORY            "Category:"
#define ERD_CATEGORY_L          strlen( ERD_CATEGORY )
//----------------------------------------------------------------------------
#define ERD_MAIN_INGRED         "Main Ingredient:"
#define ERD_MAIN_INGRED_L       strlen( ERD_MAIN_INGRED )
//----------------------------------------------------------------------------
#define ERD_QUISINE             "Cuisine Style:"
#define ERD_QUISINE_L           strlen( ERD_QUISINE )
//----------------------------------------------------------------------------
#define ERD_YIELD               "Yield:"
#define ERD_YIELD_L             strlen( ERD_YIELD )
//----------------------------------------------------------------------------
#define ERD_PREP_TIME           "Preparation Time:"
#define ERD_PREP_TIME_L         strlen( ERD_PREP_TIME )
//----------------------------------------------------------------------------
#define ERD_COOK_TIME           "Cooking Time:"
#define ERD_COOK_TIME_L         strlen( ERD_COOK_TIME )
//----------------------------------------------------------------------------
#define ERD_AMOUNT              "[Amount]"
#define ERD_AMOUNT_L            strlen( ERD_AMOUNT )
//----------------------------------------------------------------------------
#define ERD_MEASURE             "[Measure]"
#define ERD_MEASURE_L           strlen( ERD_MEASURE )
//----------------------------------------------------------------------------
#define ERD_INGRED              "[Ingredient (or Header)]"
#define ERD_INGRED_L            strlen( ERD_INGRED )
//----------------------------------------------------------------------------
#define ERD_PREP                "[Preparation]"
#define ERD_PREP_L              strlen( ERD_PREP )
//----------------------------------------------------------------------------
#define ERD_PREPARATION         "[Preparation]"
#define ERD_PREPARATION_L       strlen( ERD_PREPARATION )
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------

/****************************************************************************
 * Library Private Prototypes
 ****************************************************************************/

//----------------------------------------------------------------------------
int
DECODE_ERD__start(
    char                        *   char_string_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__end(
    char                        *   char_string_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__title(
    struct  recipe_t            *   recipe_p,
    char                        *   title_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__recipe_by(
    struct  recipe_t            *   recipe_p,
    char                        *   recipe_by_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__category(
    struct  recipe_t            *   recipe_p,
    char                        *   category_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__main_ingred(
    struct  recipe_t            *   recipe_p,
    char                        *   category_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__quisine(
    struct  recipe_t            *   recipe_p,
    char                        *   category_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__yield(
    struct  recipe_t            *   recipe_p,
    char                        *   category_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__prep_time(
    struct  recipe_t            *   recipe_p,
    char                        *   category_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__cook_time(
    struct  recipe_t            *   recipe_p,
    char                        *   category_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__auip(
    struct  recipe_t            *   recipe_p,
    char                        *   category_p
    );
//----------------------------------------------------------------------------
int
DECODE_ERD__directions(
    struct  recipe_t            *   recipe_p,
    char                        *   category_p
    );
//----------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    DECODE_ERD_LIB_H
