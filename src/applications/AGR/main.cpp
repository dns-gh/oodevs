#include "AGR_pch.h"
#include "AGR_Workspace.h"
#include "AGR_MilMissionGenerator.h"
#include "AGR_MilFragOrderGenerator.h"
#include "AGR_MilProjectGenerator.h"
#include "AGR_Mos2Generator.h"
#include "AGR_HalGenerator.h"
#include "AGR_EnumGenerator.h"
#include "AGR_Adn2Generator.h"
#include "AGR_FichesMissionsGenerator.h"
#include "AGR_TesterMissionGenerator.h"
#include "AGR_TesterProjectGenerator.h"
#include "AGR_MilRCGenerator.h"

#include "MT_Tools/MT_Version.h"
#include <windows.h>

bool CheckModifications( const std::string& strDir1, const std::string& strExt1
                       , const std::string& strDir2, const std::string& strExt2 );

int main( int /*argc*/, char** /*argv*/ )
{
    SetConsoleTitle( "ASN Based Class Generator - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );

    AGR_Workspace workspace;

    try
    {
//        if( !CheckModifications( "./agr_tmp", "*.xsd", "./asn/src/asn", "*.asn" ) )
//        {
//            std::cout << "AGR : Nothing to do." << std::endl;
//            return EXIT_SUCCESS;
//        }

        std::cout << "Current directory : " << MT_GetCurrentDir() << std::endl;

        std::cout << "Destroying old directories" << std::endl;
        const std::string strOutputPath = "./Generated/";
        try
        {
            boost::filesystem::remove_all( "agr_tmp" );
            boost::filesystem::remove_all( strOutputPath );        
        }
        catch( std::exception& e )
        {
            std::cout << e.what() << std::endl;
        }

        std::cout << "Generating XSD files from ASN files" << std::endl;
        MT_MakeDir( "agr_tmp" );
        std::string strCommandLine( "asn2xsd.exe ../src/libraries/asn/asn/*.asn -o ./agr_tmp" );
        int nErrorCode = system( strCommandLine.c_str() );
        if( nErrorCode != EXIT_SUCCESS )
        {
            std::cout << "asn2xsd.exe exited with code " << nErrorCode << "." << std::endl;
            return nErrorCode;
        } 

        std::cout << "Parsing XSD files" << std::endl;

        MT_DirectoryBrowser browser( "./agr_tmp", "*.xsd", MT::eFilesOnly );
        const MT::T_DirEntryList& files = browser.GetEntryList();
        for( MT::T_DirEntryList::const_iterator itFiles = files.begin(); itFiles != files.end(); ++itFiles )
            workspace.Read( itFiles->fileName_ );

        std::vector< AGR_Generator_ABC* > generators;
        generators.push_back( new AGR_MilMissionGenerator() );
        generators.push_back( new AGR_MilFragOrderGenerator() );
        generators.push_back( new AGR_MilProjectGenerator() );
        generators.push_back( new AGR_MilRCGenerator() );
        generators.push_back( new AGR_Mos2Generator() );
        generators.push_back( new AGR_HalGenerator() );
        generators.push_back( new AGR_EnumGenerator() );
        generators.push_back( new AGR_Adn2Generator() );
        generators.push_back( new AGR_FichesMissionsGenerator() );
        generators.push_back( new AGR_TesterMissionGenerator() );
        generators.push_back( new AGR_TesterProjectGenerator() );
        

        MT_MakeDir( strOutputPath );        
        for( std::vector< AGR_Generator_ABC* >::iterator it = generators.begin(); it != generators.end(); ++it )
        {
            AGR_Generator_ABC& generator = **it;
            try
            {
                generator.Generate( workspace, strOutputPath );
                delete &generator;
            }
            catch( std::exception& e )
            {
                std::cout << e.what() << std::endl;
                delete &generator;
            }
        }
    }
    catch( std::exception& e )
    {
        MessageBox( 0, e.what(), "ASN Class Generator - Error while parsing files", MB_ICONERROR | MB_OK );
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// -----------------------------------------------------------------------------
// Name: MostRecentModification
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
uint64 MostRecentModification( const std::string& strDir, const std::string& strExt )
{
    MT_DirectoryBrowser browser( strDir, strExt, MT::eFilesOnly );
    const MT::T_DirEntryList& files = browser.GetEntryList();
    uint64 nMostRecentModificationTime = 0;
    for( MT::T_DirEntryList::const_iterator itFiles = files.begin(); itFiles != files.end(); ++itFiles )
        if( nMostRecentModificationTime < itFiles->nLastWriteTime_ )
            nMostRecentModificationTime = itFiles->nLastWriteTime_;
    return nMostRecentModificationTime;
}

// -----------------------------------------------------------------------------
// Name: CheckModifications
// Created: AGE 2004-09-21
// -----------------------------------------------------------------------------
bool CheckModifications( const std::string& strDir1, const std::string& strExt1
                       , const std::string& strDir2, const std::string& strExt2 )
{
    return MostRecentModification( strDir1, strExt1 ) < MostRecentModification( strDir2, strExt2 );
}
