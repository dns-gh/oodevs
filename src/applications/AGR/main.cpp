// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "AGR_pch.h"
#include "AGR_Workspace.h"
#include "AGR_EnumGenerator.h"
#include "MT_Tools/MT_Version.h"
#include <windows.h>
#undef max
#include <iostream>

namespace bfs = boost::filesystem;

int main( int /*argc*/, char** /*argv*/ )
{
    SetConsoleTitle( "ASN Based Class Generator - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );
    AGR_Workspace workspace;
    try
    {
        std::cout << "Current directory : " << bfs::current_path() << std::endl;
        std::cout << "Destroying old directories" << std::endl;
        const std::string strOutputPath = "./Generated/";
        try
        {
            bfs::remove_all( "agr_tmp" );
            bfs::remove_all( strOutputPath );
        }
        catch( std::exception& e )
        {
            std::cout << e.what() << std::endl;
        }
        std::cout << "Generating XSD files from ASN files" << std::endl;
        bfs::create_directories( "agr_tmp" );
        std::string strCommandLine( "asn2xsd.exe ../../src/libraries/game_asn/*.asn -o ./agr_tmp" );
        int nErrorCode = system( strCommandLine.c_str() );
        if( nErrorCode != EXIT_SUCCESS )
        {
            std::cout << "asn2xsd.exe exited with code " << nErrorCode << "." << std::endl;
            return nErrorCode;
        }
        std::cout << "Parsing XSD files" << std::endl;
        for( bfs::directory_iterator it( "./agr_tmp" ); it != bfs::directory_iterator(); ++it )
            if( bfs::extension( *it ) == ".xsd" )
                workspace.Read( it->string() );
        bfs::create_directories( strOutputPath );
        AGR_EnumGenerator generator;
        generator.Generate( workspace, strOutputPath );
    }
    catch( std::exception& e )
    {
        MessageBox( 0, e.what(), "ASN Class Generator - Error while parsing files", MB_ICONERROR | MB_OK );
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
