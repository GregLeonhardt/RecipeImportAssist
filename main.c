/*******************************  COPYRIGHT  ********************************/
/*
 *  Copyright (c) 2017 Gregory N. Leonhardt All rights reserved.
 *
 ****************************************************************************/

/******************************** JAVADOC ***********************************/
/**
 *  This is the main entry point for the application.
 *
 *  @note
 *
 ****************************************************************************/

/****************************************************************************
 *  Compiler directives
 ****************************************************************************/

#define ALLOC_MAIN            ( "ALLOCATE STORAGE FOR MAIN" )

/****************************************************************************
 * System Function API
 ****************************************************************************/

                                //*******************************************
#include <stdint.h>             //  Alternative storage types
#include <stdbool.h>            //  TRUE, FALSE, etc.
#include <stdio.h>              //  Standard I/O definitions
                                //*******************************************
#include <string.h>             //  Functions for managing strings
#include <stdlib.h>             //  ANSI standard library.
                                //*******************************************

/****************************************************************************
 * Application APIs
 ****************************************************************************/

                                //*******************************************
#include <main_api.h>           //  Global stuff for this application
#include <libtools_api.h>       //  My Tools Library
                                //*******************************************
#include <decode_api.h>         //  API for all decode_*            PUBLIC
#include <decodel1_api.h>       //  API for all decodel1_*          PUBLIC
#include <decode_html_api.h>    //  API for all decode_html_*       PUBLIC
#include <email_api.h>          //  API for all email_*             PUBLIC
#include <encode_api.h>         //  API for all encode_*            PUBLIC
#include <xlate_api.h>          //  API for all xlate_*             PUBLIC
                                //*******************************************

/****************************************************************************
 * API Enumerations
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

//----------------------------------------------------------------------------
#define NO_IF_OR_ID             ( 1 )
#define BOTH_IF_AND_ID          ( 2 )
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Structures
 ****************************************************************************/

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Storage Allocation
 ****************************************************************************/

//----------------------------------------------------------------------------
/**
 * @param in_file_name_p    Pointer to the input file name                  */
char                        *   in_file_name_p;
//----------------------------------------------------------------------------
/**
 * @param in_dir_name_p     Pointer to the input directory name             */
char                        *   in_dir_name_p;
//----------------------------------------------------------------------------

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  Scan the command line and extract parameters for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return main_rc             TRUE when all parameters are successfully
 *                              processed, else FALSE is returned
 *
 *  @note
 *
 ****************************************************************************/

static
void
help(
    int                             error_id
    )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Function
     ************************************************************************/

    switch( error_id )
    {
        case    NO_IF_OR_ID:
        {
            log_write( MID_INFO, "main: help",
                          "Missing -if or -id     "
                          "Did not find a file name or directory name that "
                          "is to be scanned.\n" );
        }   break;
        case    BOTH_IF_AND_ID:
        {
            log_write( MID_INFO, "main: help",
                          "Both -if and -id found "
                          "Only one of the two may be used.\n" );
        }   break;
    }

    //  Command line options
    log_write( MID_INFO, "main: help",
                  "-if {file_name}          Input file name\n" );
    log_write( MID_INFO, "main: help",
                  "-id {directory_name}     Input directory name\n" );
    log_write( MID_FATAL, "main: help",
                  "-od {directory_name}     Output directory name\n" );

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/
/**
 *  Scan the command line and extract parameters for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return void                Nothing is returned from this function
 *
 *  @note
 *
 ****************************************************************************/

static
void
command_line(
    int                             argc,
    char                        *   argv[]
      )
{

    /************************************************************************
     *  Function Initialization
     ************************************************************************/


    /************************************************************************
     *  Scan for parameters
     ************************************************************************/

    //  Scan for        Input File name
    in_file_name_p = get_cmd_line_parm( argc, argv, "if" );

    //  Scan for        Input Directory name
    in_dir_name_p = get_cmd_line_parm( argc, argv, "id" );

    //  DEBUG DEFAULTS
    if(    ( in_file_name_p       == NULL )
        && ( in_dir_name_p        == NULL ) )
    {
//      in_file_name_p       = "../samples/1996.mxp";
//      in_file_name_p       = "../samples/Alacarte.mxp";
//      in_file_name_p       = "../samples/test.txt";
//      in_file_name_p       = "../samples/MM3500_1.TXT";
//      in_dir_name_p        = "../samples/";
        in_dir_name_p        = "/home/greg/TEST";
    }

    /************************************************************************
     *  Verify everything is good
     ************************************************************************/

    //  Is there an Input File name or an Input Directory name ?
    if(    ( in_file_name_p == NULL )
        && ( in_dir_name_p  == NULL ) )
    {
        //  NO:     Write some help information
        help( NO_IF_OR_ID );
    }

    //  Is there both an Input File name and an Input Directory name ?
    if(    ( in_file_name_p != NULL )
        && ( in_dir_name_p  != NULL ) )
    {
        //  YES:    Write some help information
        help( BOTH_IF_AND_ID );
    }

    /************************************************************************
     *  Save the parameters we will be using
     ************************************************************************/

    //  Is there an Input File name ?
    if( in_file_name_p != NULL )
    {
        //  YES:    Log it.
        log_write( MID_DEBUG_0, "main",
                      "Input File = '%s'\n", in_file_name_p );

        //  Give the file name to the encoder
        encode_set_if( in_file_name_p );
    }

    //  Is there an Input File name ?
    if( in_dir_name_p != NULL )
    {
        //  YES:    Log it.
        log_write( MID_DEBUG_0, "main",
                      "Input Directory = '%s'\n", in_dir_name_p );

        //  Give the file name to the encoder
        encode_set_if( in_dir_name_p );
    }

    /************************************************************************
     *  Function Exit
     ************************************************************************/

    //  DONE!
}

/****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************/
/**
 *  This is the main entry point for the application.
 *
 *  @param  argc                Number of command line parameters.
 *  @param  argv                Indexed list of command line parameters
 *
 *  @return                     Zero for success. Any other value is an error.
 *
 *  @note
 *
 ****************************************************************************/

int
main(
    int                             argc,
    char                        *   argv[]
      )
{
    /**
     * @param main_rc           Return code from called functions.          */
    enum    queue_rc_e              main_rc;
    /**
     * @param input_file_fp     Input File pointer                          */
    FILE                        *   in_file_fp;
    /**
     * @param read_data_p       Pointer to the read data                    */
    char                        *   read_data_p;
    /**
     * @param file_ll_p         Pointer to a list where the file is stored  */
    struct  list_base_t         *   level1_list_p;
    /**
     *  @param  file_list       Pointer to a list of files                  */
    struct  list_base_t         *   file_list_p;
    /**
     *  @param  file_info_p     Pointer to a file information structure     */
    struct  file_info_t         *   file_info_p;
    /**
     *  @param  input_file_name Buffer to hold the directory/file name      */
    char                            input_file_name[ FILE_NAME_L ];
    /**
     * @param source_info_p     Pointer to source data information          */
    struct  source_info_t       *   source_info_p;
    /**
     *  @param  queue_rc        Return code from queue management           */
    enum    queue_rc_e              queue_rc;
    /**
     *  @param  html_found      Flag showing that an HTML tag was found      */
    int                             html_found;
    /**
     *  @param  extention       Buffer to hold file extention               */
    char                            extention[ 10 ];

    /************************************************************************
     *  Application Initialization
     ************************************************************************/

    //  Variable initialization
    html_found = false;

    //  Initialize the CommonCode memory process
    token_init( );
    mem_init( );
    store_init( );

    /************************************************************************
     *  Initialize the Recipe-ID:
     ************************************************************************/

    //  Until a recipe_id is available
    do
    {
        /**
         *  @param  recipe_id       Temporary data buffer for the recipe id */
        unsigned char                   recipe_id[ SHA1_DIGEST_SIZE + 1 ];

        //  Get the current recipe-id.
        recipe_id_p = store_get( "RECIPE_ID" );

        //  Success ?
        if( recipe_id_p == NULL )
        {
            //  NO:     Create it.
            snprintf( (char*)recipe_id, sizeof( recipe_id ), "%016d", 0 );

            //  Put the new recipe-id
            store_put( "RECIPE_ID", (char*)recipe_id );
        }

    }   while( recipe_id_p == NULL );

    /************************************************************************
     *  Initialize the log process
     ************************************************************************/

    //  Initialize the log handler
    log_init( "RecipeImportAssist.log" );

    //  Mark the start of the run in the log file
    log_write( MID_INFO, "main",
                  "|==============================================|\n" );
    log_write( MID_INFO, "main",
                  "Starting: Recipe Import Assist Version %s %s.\n",
                  __DATE__, __TIME__ );

    //  Log the event
    log_write( MID_INFO, "main",
                  "Log initialization complete.\n" );

    /************************************************************************
     *  Initialize the queue process
     ************************************************************************/

    //  Initialize the queue handler
    main_rc = queue_init( );

    //  Was the queue initialization successful ?
    if( main_rc != QUEUE_RC_SUCCESS )
    {
        //  NO:     Something bad happened.
        log_write( MID_FATAL, "main",
                      "Queue initialization failed.\n" );
    }
    else
    {
        //  YES:    Log the event
        log_write( MID_INFO, "main",
                      "Queue initialization complete.\n" );
    }

    /************************************************************************
     *  Initialize the translation process
     ************************************************************************/

    //  Initialize the queue handler
    main_rc = xlate_init( );

    //  Was the queue initialization successful ?
    if( main_rc != true )
    {
        //  NO:     Something bad happened.
        log_write( MID_FATAL, "main",
                      "Xlate initialization failed.\n" );
    }
    else
    {
        //  YES:    Log the event
        log_write( MID_INFO, "main",
                      "Xlate initialization complete.\n" );
    }

    /************************************************************************
     *  Thread Initialization
     ************************************************************************/

    //  Initialize queue management
    queue_rc = queue_init( );

    //  Verify initialization was successful
    if ( queue_rc != QUEUE_RC_SUCCESS )
    {
        //  ToDo
    }

    //  Encode thread
    thread_new( encode_thread, NULL );

#if 1   //  @ToDo   [THREAD] Pick one of the following methods
#else
    //  Decode thread
    thread_new( decode_thread, NULL );
#endif

    /************************************************************************
     *  Command line processing
     ************************************************************************/

    //  Process the command line parameters
    command_line( argc, argv );

    //  Create the file-list
    file_list_p = list_new( );

    //  Are we processing a directory ?
    if( in_dir_name_p != NULL )
    {
        //  Unzip all "*.zip" files
        file_unzip( in_dir_name_p );

        //  YES:    Build the file list
        file_ls( file_list_p, in_dir_name_p, NULL );
    }
    else
    {
        //  Get the file information for the input file
        file_info_p = file_stat( in_file_name_p, in_file_name_p );

        //  Add the new file to the list
        list_put_last( file_list_p, file_info_p );
    }

    /************************************************************************
     *  Application Code
     ************************************************************************/

    //  Create a new list
    level1_list_p = list_new( );

    log_write( MID_DEBUG_0, "main",
                  "Allocate a new list structure 'level1_list_p' [%p].\n",
                  level1_list_p );

    //  Scan the list
    for( file_info_p = list_get_first( file_list_p );
         file_info_p != NULL;
         file_info_p = list_get_next( file_list_p, file_info_p ) )
    {
        //  Will the fully qualified file name will fit in the buffer ?
        if(     (   ( strlen( file_info_p->dir_name  ) )
                  + ( strlen( file_info_p->file_name ) ) )
             >= ( sizeof( input_file_name ) ) )
        {
            //  NO:     This is bad..
            log_write( MID_WARNING, "main",
                          "The file name is too big for the buffer provided. \n" );
            log_write( MID_FATAL, "main",
                          "'%s/%s'\n",
                          file_info_p->dir_name, file_info_p->file_name );
        }
        else
        {
            //  YES:    build the full file name.
            snprintf( input_file_name, sizeof( input_file_name ) - 1,
                       "%s/%s",
                       file_info_p->dir_name, file_info_p->file_name );
        }

        //  Is this a restricted file type ?
        read_data_p = strrchr( input_file_name, '.' );

        //  Is there one ?
        if( read_data_p != NULL )
        {
            //  YES:    Copy the file extention to a new buffer
            strncpy( extention, ++read_data_p, sizeof( extention ) );

            //  Make the file extention all lowercase
            text_to_lowercase( extention );

            if(    ( strncmp( read_data_p, ".discarded_data",      15 ) == 0 )
                || ( strncmp( read_data_p, ".unformatted_recipes", 20 ) == 0 )
                || ( strncmp( read_data_p, ".bat",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".bmp",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".ccf",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".cdx",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".clb",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".cgi",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".cli",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".cwt",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".dbf",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".dbt",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".diz",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".dll",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".dtd",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".dvr",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".ex_",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".exe",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".gcf",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".ged",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".gif",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".idx",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".ima",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".imx",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".ini",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".jpg",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".mc2",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".mcd",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".mcf",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".mcx",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".mnu",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".mrf",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".ndx",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".otm",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".out",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".pdf",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".pgo",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".png",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".raw",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".rdf",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".rea",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".rli",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".rp_",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".rxf",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".sql",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".tag",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".tx_",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".vb_",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".wht",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".xml",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".xsi",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".yml",                  4 ) == 0 )
                || ( strncmp( read_data_p, ".END",                  4 ) == 0 ) )
            {
                //  YES:    Skip it.
                continue;
            }
        }
        //  Open the input file
        in_file_fp = file_open_read( input_file_name );

        //  Create a new source information structure
        source_info_p = mem_malloc( sizeof( struct source_info_t ) );

        //  Copy file information into source information structure
        strncpy( source_info_p->f_dir_name, file_info_p->dir_name,
                 sizeof( source_info_p->f_dir_name ) );
        strncpy( source_info_p->f_file_name, file_info_p->file_name,
                 sizeof( source_info_p->f_file_name ) );
        strncpy( source_info_p->f_date_time, file_info_p->date_time,
                 sizeof( source_info_p->f_date_time ) );
        strncpy( source_info_p->f_file_size, file_info_p->file_size,
                 sizeof( source_info_p->f_file_size ) );

        /********************************************************************
         *  Process the file
         ********************************************************************/
        do
        {
            //  Read another line from the file
            read_data_p = file_read_text( in_file_fp, 0 );

            //  Was the read successful ?
            if(    ( read_data_p != END_OF_FILE )
                && ( read_data_p != NULL        ) )
            {
                //  YES:    Are we filtering out e-Mail junk (images etc.)
                if( email_filter( read_data_p ) == true )
                {
                    //  YES:    Discard the data
                    mem_free( read_data_p );
                }
                else
                {
                    //  Is there an HTML tag here ?
                    if( strstr( read_data_p, "<html>" ) != NULL )
                    {
                        //  YES:    Set the HTML found flag
                        html_found = true;
                    }

                    //  Is this the start of a new e-Mail ?
                    if( email_is_start( read_data_p ) == true )
                    {
                        //  YES:    Is there something already in the list ?
                        if( list_query_count( level1_list_p ) > 0 )
                        {
                            //  YES:    Have we seen an HTML tag ?
                            if( html_found == true )
                            {
                                //  YES:    Convert HTML to TEXT
                                decode_html( level1_list_p, source_info_p );

                                //  Reset the flag to FALSE
                                html_found = false;
                            }

                            //  YES:    Go process the list.
                            decodel1_parse( level1_list_p, source_info_p );

                            if( list_query_count( level1_list_p ) != 0 )
                            {
                                log_write( MID_FATAL, "main",
                                           "There is still something on the list\n" );
                            }
                            //  Reset getting ready for the next e-Mail
                            memset( source_info_p->g_from, '\0',
                                    sizeof( source_info_p->g_from ) );
                            memset( source_info_p->g_subject, '\0',
                                    sizeof( source_info_p->g_subject ) );
                            memset( source_info_p->g_datetime, '\0',
                                    sizeof( source_info_p->g_datetime ) );

                            memset( source_info_p->e_from, '\0',
                                    sizeof( source_info_p->e_from ) );
                            memset( source_info_p->e_subject, '\0',
                                    sizeof( source_info_p->e_subject ) );
                            memset( source_info_p->e_datetime, '\0',
                                    sizeof( source_info_p->e_datetime ) );
                        }
                    }
                    //  Put the new line on the list
                    list_put_last( level1_list_p, read_data_p );
                }
            }

        }   while( read_data_p != END_OF_FILE );

        /********************************************************************
         *  Clean out the list
         ********************************************************************/

        //  Is there something still in the list ?
        if( list_query_count( level1_list_p ) > 0 )
        {
            //  YES:    Have we seen an HTML tag ?
            if( html_found == true )
            {
                //  YES:    Convert HTML to TEXT
                decode_html( level1_list_p, source_info_p );

                //  Reset the flag to FALSE
                html_found = false;
            }
            //  YES:    Go process the list.
            decodel1_parse( level1_list_p, source_info_p );

            if( list_query_count( level1_list_p ) != 0 )
            {
                log_write( MID_FATAL, "main",
                           "There is still something on the list\n" );
            }
        }

        //  Close the input file
        file_close( in_file_fp );
    }

    /************************************************************************
     *  All done, delete the list
     ************************************************************************/

    //  Was there an error terminating the list ?
    if( list_kill( level1_list_p ) != true )
    {
        //  YES:    Let everyone know about the error
        log_write( MID_FATAL, "main",
                      "list_kill( level1_list_p ) failed\n" );
    }
    else
    {
        //  NO:     Log the successful termination of the list.
        log_write( MID_DEBUG_0, "main",
                      "list_kill( [%p] level1_list_p ) complete.\n",
                      level1_list_p );
    }

    /************************************************************************
     *  Application Exit
     ************************************************************************/

    //  Log the count
    log_write( MID_INFO, "main",
                  "RECIPE_ID = %s\n", recipe_id_p );

    //  Save the recipe-id for the next pass
    store_put( "RECIPE_ID", recipe_id_p );

    //  Mark the end of the run in the log file
    log_write( MID_INFO, "main",
                  "End\n" );
    log_write( MID_INFO, "main",
                  "|==============================================|\n\n" );

    return( 0 );
}

/****************************************************************************/
