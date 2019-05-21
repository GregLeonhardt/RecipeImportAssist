/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

#ifndef MAIN_API_H
#define MAIN_API_H

/******************************** JAVADOC ***********************************/
/**
 *  This file contains public definitions (etc.) that are accessible to the
 *  entire project.  I.E. "global".
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#ifdef ALLOC_MAIN
   #define MAIN_EXT
#else
   #define MAIN_EXT             extern
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
 * Library Public Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 *  @param  THREADING           TRUE will enable threading
 *                              FALSE run everything in a single thread     */
#define THREADING               ( false )
//----------------------------------------------------------------------------
/**
 *  @param  MEM_DUMP            TRUE will enable memory stack size
 *                              FALSE will disable memory stack size        */
#define MEM_DUMP                ( false )
//----------------------------------------------------------------------------
#define SUBDIRECTORY_L          ( 4 )
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

//----------------------------------------------------------------------------
/**
 * @param out_dir_name_p        Pointer to the output directory name        */
char                        *   out_dir_name_p;
//---------------------------------------------------------------------------
/**
 *  @param  count_bof           Number of BOF recipes decoded               */
int                             count_bof;
/**
 *  @param  count_cp2           Number of CP2 recipes decoded               */
int                             count_cp2;
/**
 *  @param  count_erd           Number of ERD recipes decoded               */
int                             count_erd;
/**
 *  @param  count_grf           Number of GRF recipes decoded               */
int                             count_gf2;
/**
 *  @param  count_grf           Number of GRF recipes decoded               */
int                             count_grf;
/**
 *  @param  count_mmf           Number of MMF recipes decoded               */
int                             count_mmf;
/**
 *  @param  count_mx2           Number of MX2 recipes decoded               */
int                             count_mx2;
/**
 *  @param  count_mxp           Number of MXP recipes decoded               */
int                             count_mxp;
/**
 *  @param  count_nyc           Number of NYC recipes decoded               */
int                             count_nyc;
/**
 *  @param  count_rxf           Number of RXF recipes decoded               */
int                             count_rxf;
/**
 *  @param  count_txt           Number of TXT recipes decoded               */
int                             count_txt;
//---------------------------------------------------------------------------
/**
 *  @param  encode_queue_id     ID number for the ENCODE queue              */
int                             encode_queue_id;
/**
 *  @param  decode_queue_id     ID number for the DECODE queue              */
int                             decode_queue_id;
//---------------------------------------------------------------------------
/**
 *  @param  store_value_p       ID number for the DECODE queue              */
char                        *   recipe_id_p;
//---------------------------------------------------------------------------

/****************************************************************************
 * Library Public Prototypes
 ****************************************************************************/

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

/****************************************************************************/

#endif                      //    MAIN_API_H
