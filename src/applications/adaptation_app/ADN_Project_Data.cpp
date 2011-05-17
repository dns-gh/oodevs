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
#include "ADN_Objects_Data.h"
#include "ADN_OpenFile_Exception.h"
#include "ADN_Resources.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_DataException.h"
#include "tools/Loader_ABC.h"
#include <boost/bind.hpp>
#include <tools/XmlCrc32Signature.h>
#include <boost/filesystem/operations.hpp>
#include <boost/bind.hpp>
#include <windows.h>

namespace bfs = boost::filesystem;

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
              >> xml::end;
    }

    void WriteFile( xml::xostream& output, const std::string& file, ADN_Type_String& outfile )
    {
        output << xml::start( file )
                 << xml::attribute( "file", outfile )
               << xml::end;
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
    ReadFile( input, "active-protections", szActiveProtections_ );
    ReadFile( input, "human-protections", szHumanProtections_ );
    ReadFile( input, "resource-natures", szDotationNatures_ );
    ReadFile( input, "objects", szObjects_ );
    ReadFile( input, "resources", szEquipements_ );
    ReadFile( input, "launchers", szLaunchers_ );
    ReadFile( input, "weapon-systems", szWeapons_ );
    ReadFile( input, "sensors", szSensors_ );
    ReadFile( input, "components", szComponents_ );
    ReadFile( input, "units", szUnits_);
    ReadFile( input, "automats", szAutomata_);
    ReadFile( input, "nbc", szNBC_);
    ReadFile( input, "fires", szFireClasses_ );
    ReadFile( input, "medical-treatment", szMedicalTreatment_ );
    ReadFile( input, "health", szHealth_);
    ReadFile( input, "human-factors", szHumanFactors_);
    ReadFile( input, "breakdowns", szBreakdowns_);
    ReadFile( input, "knowledge-groups", szKnowledgeGroups_);
    ReadFile( input, "maintenance", szMaintenance_);
    ReadFile( input, "supply", szSupply_);
    ReadFile( input, "communications", szCom_);
    ReadFile( input, "populations", szPopulation_);
    ReadFile( input, "inhabitants", szPeople_);
    ReadFile( input, "reports", szReports_);
    ReadFile( input, "pathfinder", szPathfinder_);
    ReadFile( input, "object-names", szObjectNames_ );
    ReadFile( input, "models", szModels_);
    ReadFile( input, "missions", szMissions_);
    ReadFile( input, "urban", szUrban_ );
    ReadFile( input, "resource-networks", szResourceNetworks_ );
    input >> xml::end;
    szSymbols_ = "DrawingTemplates.xml"; // $$$$ SBO 2011-04-18: hard coded, should use physical
}


// -----------------------------------------------------------------------------
// Name: DataInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::WriteArchive( xml::xostream& output )
{
    output << xml::start( "physical" );
    ADN_Tools::AddSchema( output, "Physical" );
    WriteFile( output, "decisional", szDecisional_ );
    WriteFile( output, "volumes", szSizes_ );
    WriteFile( output, "protections", szArmors_ );
    WriteFile( output, "active-protections", szActiveProtections_ );
    WriteFile( output, "human-protections", szHumanProtections_ );
    WriteFile( output, "resource-natures", szDotationNatures_ );
    WriteFile( output, "objects", szObjects_ );
    WriteFile( output, "resources", szEquipements_ );
    WriteFile( output, "launchers", szLaunchers_ );
    WriteFile( output, "weapon-systems", szWeapons_ );
    WriteFile( output, "sensors", szSensors_ );
    WriteFile( output, "components", szComponents_ );
    WriteFile( output, "units", szUnits_ );
    WriteFile( output, "automats", szAutomata_ );
    WriteFile( output, "nbc", szNBC_ );
    WriteFile( output, "fires", szFireClasses_ );
    WriteFile( output, "medical-treatment", szMedicalTreatment_ );
    WriteFile( output, "health", szHealth_ );
    WriteFile( output, "human-factors", szHumanFactors_ );
    WriteFile( output, "breakdowns", szBreakdowns_ );
    WriteFile( output, "knowledge-groups", szKnowledgeGroups_ );
    WriteFile( output, "maintenance", szMaintenance_ );
    WriteFile( output, "supply", szSupply_ );
    WriteFile( output, "communications", szCom_ );
    WriteFile( output, "pathfinder", szPathfinder_ );
    WriteFile( output, "populations", szPopulation_ );
    WriteFile( output, "inhabitants", szPeople_ );
    WriteFile( output, "reports", szReports_ );
    WriteFile( output, "models", szModels_ );
    WriteFile( output, "missions", szMissions_ );
    WriteFile( output, "object-names", szObjectNames_ );
    WriteFile( output, "urban", szUrban_ );
    WriteFile( output, "resource-networks", szResourceNetworks_ );
    output << xml::end;
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

#pragma warning( push )
#pragma warning( disable : 4996 )

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

#pragma warning( pop )

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
        std::string res( pTempDir );
        std::replace( res.begin(), res.end(), '\\', '/' );
        szTempDir_ = res + "sword ot data.tmp/";
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
void ADN_Project_Data::FilesNeeded( T_StringList& /*vFiles*/ ) const
{
    ADN_Project_Data* that = const_cast< ADN_Project_Data* >( this );
    assert( ! that->szFile_.GetFileName().GetData().empty() );
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
    dataInfos_.ReadArchive( defaultFile );
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Load
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Load( const tools::Loader_ABC& fileLoader )
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    fileLoader.LoadFile( szFile_.GetFileNameFull(), boost::bind( &DataInfos::ReadArchive, &dataInfos_, _1 ) );

    // Check XML Validity for files not loaded
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + dataInfos_.szPathfinder_.GetData() );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + dataInfos_.szObjectNames_.GetData() );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + dataInfos_.szHumanProtections_.GetData() );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + dataInfos_.szMedicalTreatment_.GetData() );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + "DrawingTemplates.xml" );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + "templates.xml" );
    //fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + "dis.xml" );
    //fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + "FOM.xml" );
    //fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() + "mapping.xml" );
}

namespace
{
    void CopyAttributes( const std::string& attribute, xml::xistream& xis, xml::xostream& xos )
    {
        std::string value;
        xis >> value;
        xos << xml::attribute( attribute, value );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::FilterNode
// Created: JSR 2011-02-15
// -----------------------------------------------------------------------------
void ADN_Project_Data::FilterNode( const std::string& node, xml::xistream& xis, xml::xostream& xos )
{
    if( node == "unit-rules" || node == "rule" || node == "object-costs" )
    {
        xos << xml::start( node );
        xis >> xml::attributes( boost::bind( &CopyAttributes, _2, _3, boost::ref( xos ) ) )
            >> xml::list( boost::bind( &ADN_Project_Data::FilterNode, this, _2, _3, boost::ref( xos ) ) );
        xos << xml::end;
    }
    else if( node == "object-cost" )
    {
        std::string type = xis.attribute< std::string >( "type" );
        double value = xis.attribute< double >( "value" );
        if( ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( type ) )
            xos << xml::start( node )
                    << xml::attribute( "type", type )
                    << xml::attribute( "value", value )
                << xml::end;
        for( std::map< std::string, std::string >::const_iterator it = addedObjects_.begin(); it != addedObjects_.end(); ++it )
        {
            if( it->second == type )
                xos << xml::start( node )
                        << xml::attribute( "type", it->first )
                        << xml::attribute( "value", value )
                    << xml::end;
        }
    }
    else
        xos << xml::content( node, xis );
}

namespace
{
    void ExtractRootNode( const std::string& nodeName, xml::xistream& , std::string& rootNode )
    {
        rootNode = nodeName;
    }

    void ChangeSchemaAndSignature( const std::string& inputFile, const std::string& schemaName )
    {
        if( !bfs::exists( bfs::path( inputFile ) ) )
            return;

        std::string rootNode;
        xml::xifstream xis( inputFile );
        xis >> xml::list( boost::bind( &ExtractRootNode, _2, _3, boost::ref( rootNode ) ) );
        xis >> xml::start( rootNode );

        xml::xofstream xos( inputFile );
        xos << xml::start( rootNode );
        xos << xis;
        ADN_Tools::AddSchema( xos, schemaName );
        xos << xml::end();

        tools::WriteXmlCrc32Signature( inputFile );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Save
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Save()
{
    assert( ! szFile_.GetFileName().GetData().empty() );

    std::string szFile = workDir_.GetSaveDirectory() + szFile_.GetFileName().GetData();
    ADN_Tools::CreatePathToFile( szFile );
    {
        xml::xofstream output( szFile );
        dataInfos_.WriteArchive( output );
    }
    tools::WriteXmlCrc32Signature( szFile );
    // Update pathfind.xml
    if( !addedObjects_.empty() )
    {
        std::string path = workDir_.GetWorkingDirectory().GetData() + dataInfos_.szPathfinder_.GetData();
        xml::xifstream xis( path );
        xml::xofstream xos( path );
        xis >> xml::start( "pathfind" );
        xos << xml::start( "pathfind" );
        ADN_Tools::AddSchema( xos, "Pathfind" );
        xis >> xml::list( boost::bind( &ADN_Project_Data::FilterNode, this, _2, _3, boost::ref( xos ) ) );
    }
    addedObjects_.clear();

    // Save XML Signature for files not loaded, bypassing "temp" folder
    ChangeSchemaAndSignature( workDir_.GetWorkingDirectory().GetData() + dataInfos_.szObjectNames_.GetData(), "ObjectNames" );
    ChangeSchemaAndSignature( workDir_.GetWorkingDirectory().GetData() + dataInfos_.szHumanProtections_.GetData(), "HumanProtections" );
    ChangeSchemaAndSignature( workDir_.GetWorkingDirectory().GetData() + dataInfos_.szMedicalTreatment_.GetData(), "MedicalTreatment" );
    ChangeSchemaAndSignature( workDir_.GetWorkingDirectory().GetData() + dataInfos_.szPathfinder_.GetData(), "Pathfind" );
    // ?? Same thing to do ??1
    tools::WriteXmlCrc32Signature( workDir_.GetWorkingDirectory().GetData() + "dis.xml" );
    tools::WriteXmlCrc32Signature( workDir_.GetWorkingDirectory().GetData() + "DrawingTemplates.xml" );
    tools::WriteXmlCrc32Signature( workDir_.GetWorkingDirectory().GetData() + "FOM.xml" );
    tools::WriteXmlCrc32Signature( workDir_.GetWorkingDirectory().GetData() + "mapping.xml" );
    tools::WriteXmlCrc32Signature( workDir_.GetWorkingDirectory().GetData() + "templates.xml" );
}
