//*****************************************************************************
//
// $Created: JDY 03-06-23 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Project_Data.cpp $
// $Author: Ape $
// $Modtime: 22/04/05 16:47 $
// $Revision: 11 $
// $Workfile: ADN_Project_Data.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Project_Data.h"

#include "ADN_Tools.h"
#include "ADN_Workspace.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_Resources.h"
#include "ADN_Xml_Exception.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_XmlInput_Helper.h"
#include "ADN_DataException.h"

#include <windows.h>

ADN_Project_Data::WorkDirInfos  ADN_Project_Data::workDir_;
std::string                     ADN_Project_Data::FileInfos::szUntitled_ = "Untitled";

//-----------------------------------------------------------------------------
// Name: DataInfos::DataInfos
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Project_Data::DataInfos::DataInfos()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DataInfos::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::ReadArchive( ADN_XmlInput_Helper& input )
{
    input.Section("physical");
    input.ReadField( "Decisionnel", szDecisional_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Volumes", szSizes_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Protections", szArmors_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "DotationNatures", szDotationNatures_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Objets", szObjects_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Dotations", szEquipements_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Lanceurs", szLaunchers_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Armements", szWeapons_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Capteurs", szSensors_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Composantes", szComponents_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Pions", szUnits_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Automates", szAutomata_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "NBC", szNBC_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Sante", szHealth_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "ClasseIDs", szIDs_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "FacteursHumains", szHumanFactors_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Pannes", szBreakdowns_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "GroupesConnaissance", szKnowledgeGroups_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Maintenance", szMaintenance_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Ravitaillement", szSupply_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Communications", szCom_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Populations", szPopulation_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "ComptesRendus", szReports_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "PathFinder", szPathfinder_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Modeles", szModels_, ADN_XmlInput_Helper::eThrow );
    input.ReadField( "Missions", szMissions_, ADN_XmlInput_Helper::eThrow );
    input.EndSection();   // physical
}


// -----------------------------------------------------------------------------
// Name: DataInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::WriteArchive( MT_OutputArchive_ABC& output )
{
    output.Section("physical");
    output.WriteField( "Decisionnel", szDecisional_.GetData() );
    output.WriteField( "Volumes", szSizes_.GetData() );
    output.WriteField( "Protections", szArmors_.GetData() );
    output.WriteField( "DotationNatures", szDotationNatures_.GetData() );
    output.WriteField( "Objets", szObjects_.GetData() );
    output.WriteField( "Dotations", szEquipements_.GetData() );
    output.WriteField( "Lanceurs", szLaunchers_.GetData() );
    output.WriteField( "Armements", szWeapons_.GetData() );
    output.WriteField( "Capteurs", szSensors_.GetData() );
    output.WriteField( "Composantes", szComponents_.GetData() );
    output.WriteField( "Pions", szUnits_.GetData() );
    output.WriteField( "Automates", szAutomata_.GetData() );
    output.WriteField( "NBC", szNBC_.GetData() );
    output.WriteField( "Sante", szHealth_.GetData() );
    output.WriteField( "ClasseIDs", szIDs_.GetData() );
    output.WriteField( "FacteursHumains", szHumanFactors_.GetData() );
    output.WriteField( "Pannes", szBreakdowns_.GetData() );
    output.WriteField( "GroupesConnaissance", szKnowledgeGroups_.GetData() );
    output.WriteField( "Maintenance", szMaintenance_.GetData() );
    output.WriteField( "Ravitaillement", szSupply_.GetData() );
    output.WriteField( "Communications", szCom_.GetData() );
    output.WriteField( "PathFinder", szPathfinder_.GetData() );
    output.WriteField( "Populations", szPopulation_.GetData() );
    output.WriteField( "ComptesRendus", szReports_.GetData() );
    output.WriteField( "Modeles", szModels_.GetData() );
    output.WriteField( "Missions", szMissions_.GetData() );
    output.EndSection();   // physical
}

//-----------------------------------------------------------------------------
// Name: FileInfos::FileInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::FileInfos::FileInfos()
    : szFileName_()
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: FileInfos::FileInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::FileInfos::FileInfos( const std::string& filename )
    : szFileName_( ADN_Project_Data::GetWorkDirInfos().GetRelativePath( filename ) )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: FileInfos::operator =
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::FileInfos& ADN_Project_Data::FileInfos::operator=( const std::string& filename )
{
    szFileName_ = ADN_Project_Data::GetWorkDirInfos().GetRelativePath( filename );
    return *this;
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::WorkDirInfos
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::WorkDirInfos::WorkDirInfos()
    : szWorkingDir_ ()
    , szTempDir_    ()
    , bTmpActivated_( false )
{
    // NOTHING
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::GetFullPath
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
std::string ADN_Project_Data::WorkDirInfos::GetFullPath( const std::string& part, E_WorkDir e )
{
    return ( ( e == eWorking ) ? szWorkingDir_.GetData() : szTempDir_.GetData() ) + part;
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::GetPartPath
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
std::string ADN_Project_Data::WorkDirInfos::GetRelativePath( const std::string& full, E_WorkDir e )
{
    std::string dir = ( e == eWorking ) ? szWorkingDir_.GetData() : szTempDir_.GetData();
    if( strcmpi( dir.c_str(), full.substr( 0, dir.size() ).c_str() ) )
        return std::string();
    else
        return full.substr( dir.size(), full.size() - dir.size() );
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::SetWorkingDirectory
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
void ADN_Project_Data::WorkDirInfos::SetWorkingDirectory( const std::string& filename )
{
    char szDrive[_MAX_PATH];
    char szDir[_MAX_PATH];
    char szFile[_MAX_PATH];
    char szExt[_MAX_PATH];
    char szPath[_MAX_PATH];
    _splitpath( filename.c_str(), szDrive, szDir, szFile, szExt );
    _makepath( szPath, szDrive, szDir, 0, 0 );
    szWorkingDir_ = std::string( szPath );
}


//-----------------------------------------------------------------------------
// Name: WorkDirInfos::UseTempDirectory
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
void ADN_Project_Data::WorkDirInfos::UseTempDirectory( bool bActivateTemp )
{
    bTmpActivated_ = bActivateTemp;
    if( bActivateTemp )
    {
        char *pTempDir = ( char* )malloc( sizeof( char )*_MAX_PATH );
        int   len;
        if( ( len = GetTempPath( _MAX_PATH, pTempDir ) ) > _MAX_PATH )
        {
            pTempDir = ( char* )realloc( pTempDir, sizeof( char ) * len + 1 );
            assert( ( int )GetTempPath( len + 1, pTempDir ) <= len + 1 );
        }
        szTempDir_ = ADN_Tools::Replace( pTempDir, '\\', '/' ) + "scipio data.tmp/";
        free( pTempDir );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_Data constructor
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
ADN_Project_Data::ADN_Project_Data()
    : ADN_Data_ABC   ()
    , dataInfos_     ()
    , szFile_        ()
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data destructor
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
ADN_Project_Data::~ADN_Project_Data()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::SetFile
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
void ADN_Project_Data::SetFile( const std::string& strFile )
{
    workDir_.SetWorkingDirectory( strFile );
    szFile_ = strFile;
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::FilesNeeded
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
void ADN_Project_Data::FilesNeeded( T_StringList& vFiles ) const
{
    ADN_Project_Data* that = const_cast< ADN_Project_Data* >( this );
    assert( ! that->szFile_.GetFileName().GetData().empty() );
    vFiles.push_back( that->dataInfos_.szIDs_.GetData() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Reset
// Created: JDY 03-06-23
//-----------------------------------------------------------------------------
void ADN_Project_Data::Reset()
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    // load default parameters (included has resource)
    ADN_XmlInput_Helper defaultFile;
    defaultFile.SetData( physicalXml );
    ReadArchive( defaultFile );
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Load
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Load()
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    // Read main file
    ADN_XmlInput_Helper input;

    if( !input.Open(szFile_.GetFileNameFull(), ADN_XmlInput_Helper::eNothing ) )
        throw ADN_OpenFile_Exception( szFile_.GetFileNameFull() );

    try
    {
        dataInfos_.ReadArchive( input );
    }
    catch( ADN_Xml_Exception& xmlException )
    {
        throw ADN_Xml_Exception( szFile_.GetFileNameFull(), xmlException.GetContext(), xmlException.GetErrorMessage() );
    }
    catch( MT_ArchiveLogger_Exception& xmlException )
    {
        throw ADN_DataException( "", xmlException.what() );
    }
}


//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Save
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Save()
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    // Save the scipio file.
    MT_XXmlOutputArchive output;
    dataInfos_.WriteArchive( output );

    std::string szFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory()  + szFile_.GetFileName().GetData();
    ADN_Tools::CreatePathToFile( szFile );
    if( ! output.WriteToFile( szFile ) )
        throw ADN_SaveFile_Exception( szFile );

    // Save the Id file (from static resource)
    MT_TextOutputArchive idsOutput;
    idsOutput.GetOutputStream() << idClassesXml;

    std::string szIdsFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + dataInfos_.szIDs_.GetData();
    ADN_Tools::CreatePathToFile( szIdsFile );
    if( ! idsOutput.WriteToFile( szIdsFile ) )
        throw ADN_SaveFile_Exception( szIdsFile );
}
