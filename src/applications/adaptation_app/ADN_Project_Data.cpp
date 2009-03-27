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
#include "ADN_SaveFile_Exception.h"
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

namespace 
{
    void ReadFile( xml::xistream& input, const std::string& file, ADN_Type_String& outfile )
    {
        input >> xml::start( file )
                >> xml::attribute( "file", outfile )
              >> xml::end();
    }

    void WriteFile( xml::xostream& output, const std::string& file, ADN_Type_String& outfile )
    {
        output << xml::start( file )
                 << xml::attribute( "file", outfile )
               << xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: DataInfos::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::ReadArchive( xml::xistream& input )
{
    input >> xml::start( "physical" );
    ReadFile( input, "decisional", szDecisional_ );
    ReadFile( input, "volumes", szSizes_ );
    ReadFile( input, "protections", szArmors_ );
    ReadFile( input, "dotation-natures", szDotationNatures_ );
    ReadFile( input, "objects", szObjects_ );
    ReadFile( input, "dotations", szEquipements_ );
    ReadFile( input, "launchers", szLaunchers_ );
    ReadFile( input, "weapon-systems", szWeapons_ );
    ReadFile( input, "sensors", szSensors_ );
    ReadFile( input, "components", szComponents_ );
    ReadFile( input, "units", szUnits_);
    ReadFile( input, "automats", szAutomata_);
    ReadFile( input, "nbc", szNBC_);
    ReadFile( input, "fire", szFire_ );
    ReadFile( input, "medical-treatment", szMedicalTreatment_ );
    ReadFile( input, "health", szHealth_);
    ReadFile( input, "classeids", szIDs_);
    ReadFile( input, "human-factors", szHumanFactors_);
    ReadFile( input, "breakdowns", szBreakdowns_);
    ReadFile( input, "knowledge-groups", szKnowledgeGroups_);
    ReadFile( input, "maintenance", szMaintenance_);
    ReadFile( input, "supply", szSupply_);
    ReadFile( input, "communications", szCom_);
    ReadFile( input, "populations", szPopulation_);
    ReadFile( input, "reports", szReports_);
    ReadFile( input, "pathfinder", szPathfinder_);
    ReadFile( input, "models", szModels_);
    ReadFile( input, "missions", szMissions_);
    input >> xml::end();
}


// -----------------------------------------------------------------------------
// Name: DataInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "physical" );
    WriteFile( output, "decisional", szDecisional_ );
    WriteFile( output, "volumes", szSizes_ );
    WriteFile( output, "protections", szArmors_ );
    WriteFile( output, "dotation-natures", szDotationNatures_ );
    WriteFile( output, "objects", szObjects_ );
    WriteFile( output, "dotations", szEquipements_ );
    WriteFile( output, "launchers", szLaunchers_ );
    WriteFile( output, "weapon-systems", szWeapons_ );
    WriteFile( output, "sensors", szSensors_ );
    WriteFile( output, "components", szComponents_ );
    WriteFile( output, "units", szUnits_ );
    WriteFile( output, "automats", szAutomata_ );
    WriteFile( output, "nbc", szNBC_ );
    WriteFile( output, "fire", szFire_ );
    WriteFile( output, "medical-treatment", szMedicalTreatment_ );
    WriteFile( output, "health", szHealth_ );
    WriteFile( output, "classeids", szIDs_ );
    WriteFile( output, "human-factors", szHumanFactors_ );
    WriteFile( output, "breakdowns", szBreakdowns_ );
    WriteFile( output, "knowledge-groups", szKnowledgeGroups_ );
    WriteFile( output, "maintenance", szMaintenance_ );
    WriteFile( output, "supply", szSupply_ );
    WriteFile( output, "communications", szCom_ );
    WriteFile( output, "pathfinder", szPathfinder_ );
    WriteFile( output, "populations", szPopulation_ );
    WriteFile( output, "reports", szReports_ );
    WriteFile( output, "models", szModels_ );
    WriteFile( output, "missions", szMissions_ );
    output << xml::end();
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
    if( _strcmpi( dir.c_str(), full.substr( 0, dir.size() ).c_str() ) )
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
        szTempDir_ = ADN_Tools::Replace( pTempDir, '\\', '/' ) + "sword ot data.tmp/";
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
    xml::xistringstream defaultFile( physicalXml );
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
    xml::xifstream input( szFile_.GetFileNameFull() );
    dataInfos_.ReadArchive( input );
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Save
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Save()
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    std::string szFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory()  + szFile_.GetFileName().GetData();
    ADN_Tools::CreatePathToFile( szFile );
    xml::xofstream output( szFile );
    dataInfos_.WriteArchive( output );

    std::string szIdsFile = ADN_Project_Data::GetWorkDirInfos().GetSaveDirectory() + dataInfos_.szIDs_.GetData();
    ADN_Tools::CreatePathToFile( szIdsFile );
    std::ofstream idsOutput( szIdsFile.c_str() );
    idsOutput << idClassesXml;
    idsOutput.close();
}
