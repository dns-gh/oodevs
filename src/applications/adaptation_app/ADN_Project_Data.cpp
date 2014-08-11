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
#include "ADN_Missions_Data.h"
#include "ADN_WorkspaceElement.h"
#include "XmlResources.cpp"
#include "tools/Loader_ABC.h"
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <process.h>
#include <windows.h>

ADN_Project_Data::WorkDirInfos  ADN_Project_Data::workDir_;
tools::Path                     ADN_Project_Data::FileInfos::szUntitled_ = "Untitled";

//-----------------------------------------------------------------------------
// Name: DataInfos::DataInfos
// Created: JDY 03-06-26
//-----------------------------------------------------------------------------
ADN_Project_Data::DataInfos::DataInfos()
    : readOnly_       ( false )
    , readOnlyEnabled_( true )
{
    // NOTHING
}

namespace
{
    void ReadFile( xml::xistream& input, const std::string& file, tools::Path& outfile )
    {
        input >> xml::start( file )
                >> xml::attribute( "file", outfile )
              >> xml::end;
    }

    void ReadPath( xml::xistream& input, const std::string& file, tools::Path& outfile )
    {
        input >> xml::start( file )
            >> xml::attribute( "path", outfile )
            >> xml::end;
    }

    void ReadOptionalPath( xml::xistream& input, const std::string& file, tools::Path& outfile )
    {
        input >> xml::optional >> xml::start( file )
            >> xml::attribute( "path", outfile )
            >> xml::end;
    }

    void ReadOptionalFile( xml::xistream& input, const std::string& file, tools::Path& outfile )
    {
        input >> xml::optional >> xml::start( file )
                >> xml::attribute( "file", outfile )
              >> xml::end;
    }

    void WriteFile( xml::xostream& output, const std::string& file, const tools::Path& outfile )
    {
        if( outfile.IsEmpty() )
            return;
        output << xml::start( file )
                 << xml::attribute( "file", outfile )
               << xml::end;
    }

    void WritePath( xml::xostream& output, const std::string& file, const tools::Path& outfile )
    {
        if( outfile.IsEmpty() )
            return;
        output << xml::start( file )
            << xml::attribute( "path", outfile )
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::DataInfos::IsReadOnly
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
bool ADN_Project_Data::DataInfos::IsReadOnly()
{
    return readOnly_ && readOnlyEnabled_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::DataInfos::DisableReadOnly
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::DisableReadOnly()
{
    readOnly_ = false;
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::DataInfos::SetNoReadOnly
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::SetNoReadOnly()
{
    readOnlyEnabled_ = false;
}

// -----------------------------------------------------------------------------
// Name: DataInfos::ReadArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::ReadArchive( xml::xistream& input )
{
    readOnly_ = false;
    input >> xml::start( "physical" )
          >> xml::optional >> xml::attribute( "read-only", readOnly_ );
    ReadFile( input, "decisional", szDecisional_ );
    ReadFile( input, "volumes", szSizes_ );
    ReadFile( input, "protections", szArmors_ );
    ReadFile( input, "active-protections", szActiveProtections_ );
    ReadFile( input, "resource-natures", szDotationNatures_ );
    ReadFile( input, "logistic-supply-classes", szLogisticSupplyClasses_ );
    ReadFile( input, "disasters", szDisasters_ );
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
    ReadFile( input, "health", szHealth_);
    ReadFile( input, "human-factors", szHumanFactors_);
    ReadFile( input, "breakdowns", szBreakdowns_);
    ReadFile( input, "knowledge-groups", szKnowledgeGroups_);
    ReadFile( input, "maintenance", szMaintenance_);
    ReadFile( input, "supply", szSupply_);
    ReadFile( input, "funeral", szFuneral_ );
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
    ReadFile( input, "drawing-templates", szDrawingTemplates_ );
    ReadFile( input, "symbols", szSymbols_ );
    ReadFile( input, "scores", szScores_ );
    ReadFile( input, "urban-templates", szUrbanTemplates_ );
    ReadOptionalFile( input, "extensions", szExtensions_ );
    ReadOptionalFile( input, "filters", szFilters_ );

    ReadOptionalPath( input, "units-mission-sheets-directory", szUnitsMissionPath_ );
    ReadOptionalPath( input, "automata-mission-sheets-directory", szAutomataMissionPath_ );
    ReadOptionalPath( input, "crowds-mission-sheets-directory", szCrowdsMissionPath_ );
    ReadOptionalPath( input, "fragorders-mission-sheets-directory", szFragOrdersMissionPath_ );
    ReadPath( input, "symbols-directory", szSymbolsPath_ );
    ReadFile( input, "mission-sheets-xsl", szMissionSheetXslPath_ );
    ReadFile( input, "stages", szStages_ );
    ReadFile( input, "languages", szLanguages_ );
    ReadPath( input, "locales-directory", szLocalesDirectory_ );

    input >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DataInfos::WriteArchive
// Created: APE 2004-11-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::DataInfos::WriteArchive( xml::xostream& output ) const
{
    output << xml::start( "physical" );
    if( readOnly_ )
        output << xml::attribute( "read-only", readOnly_ );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( output, "Physical" );
    WriteFile( output, "decisional", szDecisional_ );
    WriteFile( output, "volumes", szSizes_ );
    WriteFile( output, "protections", szArmors_ );
    WriteFile( output, "active-protections", szActiveProtections_ );
    WriteFile( output, "resource-natures", szDotationNatures_ );
    WriteFile( output, "logistic-supply-classes", szLogisticSupplyClasses_ );
    WriteFile( output, "disasters", szDisasters_ );
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
    WriteFile( output, "health", szHealth_ );
    WriteFile( output, "human-factors", szHumanFactors_ );
    WriteFile( output, "breakdowns", szBreakdowns_ );
    WriteFile( output, "knowledge-groups", szKnowledgeGroups_ );
    WriteFile( output, "maintenance", szMaintenance_ );
    WriteFile( output, "supply", szSupply_ );
    WriteFile( output, "funeral", szFuneral_ );
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
    WriteFile( output, "extensions", szExtensions_ );
    WriteFile( output, "drawing-templates", szDrawingTemplates_ );
    WriteFile( output, "scores", szScores_ );
    WriteFile( output, "symbols", szSymbols_ );
    WriteFile( output, "filters", szFilters_);
    WriteFile( output, "urban-templates", szUrbanTemplates_ );
    WritePath( output, "units-mission-sheets-directory", szUnitsMissionPath_ );
    WritePath( output, "automata-mission-sheets-directory", szAutomataMissionPath_ );
    WritePath( output, "crowds-mission-sheets-directory", szCrowdsMissionPath_ );
    WritePath( output, "fragorders-mission-sheets-directory", szFragOrdersMissionPath_ );
    WritePath( output, "symbols-directory", szSymbolsPath_ );
    WriteFile( output, "mission-sheets-xsl", szMissionSheetXslPath_ );
    WriteFile( output, "stages", szStages_ );
    WriteFile( output, "languages", szLanguages_ );
    WritePath( output, "locales-directory", szLocalesDirectory_ );
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
ADN_Project_Data::FileInfos::FileInfos( const tools::Path& filename )
    : szFileName_( ADN_Project_Data::GetWorkDirInfos().GetRelativePath( filename ) )
{
    // NOTHING
}

//-----------------------------------------------------------------------------
// Name: FileInfos::operator =
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
ADN_Project_Data::FileInfos& ADN_Project_Data::FileInfos::operator=( const tools::Path& filename )
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
tools::Path ADN_Project_Data::WorkDirInfos::GetFullPath( const tools::Path& part, E_WorkDir e ) const
{
    return ( ( e == eWorking ) ? szWorkingDir_.GetData() : szTempDir_ ) / part;
}

//-----------------------------------------------------------------------------
// Name: WorkDirInfos::GetPartPath
// Created: JDY 03-06-24
//-----------------------------------------------------------------------------
tools::Path ADN_Project_Data::WorkDirInfos::GetRelativePath( const tools::Path& full, E_WorkDir e ) const
{
    const tools::Path dir = ( e == eWorking ) ? szWorkingDir_.GetData() : szTempDir_;
    return full.Relative( dir );
}

//-----------------------------------------------------------------------------
// Name: WorkDirInfos::SetWorkingDirectory
// Created: JDY 03-09-09
//-----------------------------------------------------------------------------
void ADN_Project_Data::WorkDirInfos::SetWorkingDirectory( const tools::Path& directory )
{
    if( directory.IsDirectory() )
    {
        directory.CreateDirectories();
        szWorkingDir_ = directory;
    }
    else
    {
        szWorkingDir_ = directory.Parent();
    }
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
        tools::Path osTempDir = tools::Path::TemporaryPath();
        szTempDir_ = osTempDir / "ADN_Temp_" + tools::Path::FromUTF8( boost::lexical_cast< std::string >( _getpid() ) );
        if( szTempDir_.Exists() )
        {
            szTempDir_.RemoveAll();
            szTempDir_.Remove();
        }
        szTempDir_.CreateDirectories();
    }
    else
        szTempDir_.RemoveAll();
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data constructor
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
ADN_Project_Data::ADN_Project_Data()
    : ADN_Data_ABC   ( eNbrWorkspaceElements )
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
void ADN_Project_Data::SetFile( const tools::Path& strFile )
{
    workDir_.SetWorkingDirectory( strFile );
    szFile_ = strFile;
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::FilesNeeded
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
void ADN_Project_Data::FilesNeeded( tools::Path::T_Paths& /*vFiles*/ ) const
{
#ifndef NDEBUG
    ADN_Project_Data* that = const_cast< ADN_Project_Data* >( this );
    assert( !that->szFile_.GetFileName().IsEmpty() );
#endif
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::New
// Created: ABR 2013-09-17
// -----------------------------------------------------------------------------
void ADN_Project_Data::New( const tools::Path& filename )
{
    SetFile( filename );
    xml::xistringstream defaultFile( physicalXml );
    dataInfos_.ReadArchive( defaultFile );
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::GetMissionDir
// Created: NPT 13-07-25
//-----------------------------------------------------------------------------
tools::Path ADN_Project_Data::GetMissionDir( E_MissionType missionType ) const
{
    switch( missionType )
    {
    case eMissionType_Pawn:
        return dataInfos_.szUnitsMissionPath_;
    case eMissionType_Automat:
        return dataInfos_.szAutomataMissionPath_;
    case eMissionType_Population:
        return dataInfos_.szCrowdsMissionPath_;
    case eMissionType_FragOrder:
        return dataInfos_.szFragOrdersMissionPath_;
    default:
        return "";
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Load
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Load( const tools::Loader_ABC& fileLoader )
{
    assert( !szFile_.GetFileName().IsEmpty() );

    fileLoader.LoadFile( szFile_.GetFileNameFull(), boost::bind( &DataInfos::ReadArchive, &dataInfos_, _1 ) );

    // Check XML Validity for files not loaded
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szPathfinder_ );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szObjectNames_ );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szStages_ );

    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() / "templates.xml" );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szDrawingTemplates_ );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szScores_ );
    fileLoader.CheckFile( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szSymbols_ );
    fileLoader.CheckOptionalFile( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szExtensions_ );
    fileLoader.CheckOptionalFile( workDir_.GetWorkingDirectory().GetData() / "dis.xml" );
    fileLoader.CheckOptionalFile( workDir_.GetWorkingDirectory().GetData() / "FOM.xml" );
    fileLoader.CheckOptionalFile( workDir_.GetWorkingDirectory().GetData() / "mapping.xml" );

    std::unique_ptr< xml::xistream > xslStream = fileLoader.LoadFile( workDir_.GetSaveDirectory() / dataInfos_.szMissionSheetXslPath_ );
    tools::Xofstream xosTemp( tools::Path::TemporaryPath() / ADN_Missions_Data::xslTemporaryFile_ );
    *xslStream >> xosTemp;
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
            if( ADN_Workspace::GetWorkspace().GetObjects().GetData().FindObject( it->first ) && it->second == type )
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

    void ChangeSchema( const tools::Path& inputFile, const tools::Path& schemaName )
    {
        if( !inputFile.Exists() || inputFile.IsDirectory() )
            return;

        std::string rootNode;
        tools::Xifstream xis( inputFile );
        xis >> xml::list( boost::bind( &ExtractRootNode, _2, _3, boost::ref( rootNode ) ) );
        xis >> xml::start( rootNode );
        tools::Xofstream xos( inputFile );
        xos << xml::start( rootNode );
        xos << xis;
        tools::SchemaWriter schemaWriter;
        schemaWriter.WritePhysicalSchema( xos, schemaName );
        xos << xml::end;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::WritePathfind
// Created: CMA 2012-04-25
//-----------------------------------------------------------------------------
void ADN_Project_Data::WritePathfind( xml::xistream& xis, const tools::Path& path )
{
    tools::Xofstream xos( path );
    xis >> xml::start( "pathfind" );
    xos << xml::start( "pathfind" );
    tools::SchemaWriter schemaWriter;
    schemaWriter.WritePhysicalSchema( xos, "Pathfind" );
    xis >> xml::list( boost::bind( &ADN_Project_Data::FilterNode, this, _2, _3, boost::ref( xos ) ) );
}

namespace
{
    void CreateNewPathfindFile( const tools::Path& path )
    {
        tools::Xofstream xos( path );
        xos << xml::start( "pathfind" );
        tools::SchemaWriter schemaWriter;
        schemaWriter.WritePhysicalSchema( xos, "Pathfind" );
        xos << xml::start( "configuration" )
                << xml::attribute( "distance-threshold", 15000 )
                << xml::attribute( "max-calculation-time", "15s" )
                << xml::attribute( "max-end-connections", 8 )
            << xml::end
            << xml::start( "unit-rules" )
            << xml::end
            << xml::start( "population-rules" )
            << xml::end;
    }

    void CreateObjectNames( const tools::Path& path )
    {
        if( !path.Exists() ) // I'm not even sure we should write something as the values written will most likely be wrong and may not map Type_Objets.bms
        {
            tools::Xofstream xos( path );
            xos << xml::start( "objects" );
            tools::SchemaWriter schemaWriter;
            schemaWriter.WritePhysicalSchema( xos, "ObjectNames" );
            const ADN_Objects_Data::T_ObjectsInfos_Vector& objects = ADN_Workspace::GetWorkspace().GetObjects().GetData().GetObjectInfos();
            unsigned int objectId = 0;
            for( auto it = objects.begin(); it != objects.end(); ++it, ++objectId )
            {
                if( ( *it )->strType_.GetData().empty() )
                    ( *it )->strType_ = ADN_Objects_Data_ObjectInfos::GenerateNextType();
                xos << xml::start( "object" )
                        << xml::attribute( "id", objectId )
                        << xml::attribute( "type", ( *it )->strType_.GetData() )
                    << xml::end;
            }
        }
    }
    void CreateEmptyFileIfNeeded( const tools::Path& path, const std::string& openning, const tools::Path& schema = "" )
    {
        if( !path.Exists() )
        {
            tools::Xofstream xos( path );
            xos << xml::start( openning );
            if( !schema.IsEmpty() )
            {
                tools::SchemaWriter schemaWriter;
                schemaWriter.WritePhysicalSchema( xos, schema );
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Project_Data::Save
// Created: JDY 03-06-20
//-----------------------------------------------------------------------------
void ADN_Project_Data::Save( const tools::Loader_ABC& fileLoader )
{
    assert( !szFile_.GetFileName().IsEmpty() );

    tools::Path szFile = workDir_.GetSaveDirectory() / szFile_.GetFileName();
    szFile.Parent().CreateDirectories();
    {
        tools::Xofstream output( szFile );
        dataInfos_.WriteArchive( output );
    }
    // Update pathfind.xml
    tools::Path pathfindFile = workDir_.GetWorkingDirectory().GetData() / dataInfos_.szPathfinder_;
    if( pathfindFile.Exists() )
        fileLoader.LoadFile( pathfindFile, boost::bind( &ADN_Project_Data::WritePathfind, this, _1, boost::cref( pathfindFile ) ) );
    else
        CreateNewPathfindFile( pathfindFile );

    CreateObjectNames( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szObjectNames_ );

    CreateEmptyFileIfNeeded( workDir_.GetWorkingDirectory().GetData() / "templates.xml", "templates" );
    CreateEmptyFileIfNeeded( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szDrawingTemplates_, "templates", "DrawingTemplates" );
    CreateEmptyFileIfNeeded( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szStages_, "stages", "Stages" );

    addedObjects_.clear();

    // Save XML Signature for files not loaded, bypassing "temp" folder
    ChangeSchema( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szObjectNames_, "ObjectNames" );
    ChangeSchema( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szExtensions_, "Extensions" );
    ChangeSchema( workDir_.GetWorkingDirectory().GetData() / dataInfos_.szStages_, "Stages" );
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::GetLocalDir
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
tools::Path ADN_Project_Data::GetLocalDir() const
{
    return workDir_.GetWorkingDirectory().GetData() / dataInfos_.szLocalesDirectory_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Project_Data::GetLocalMissionDir
// Created: ABR 2013-10-03
// -----------------------------------------------------------------------------
tools::Path ADN_Project_Data::GetLocalMissionDir( E_MissionType missionType ) const
{
    return workDir_.GetWorkingDirectory().GetData() / GetMissionDir( missionType ) / dataInfos_.szLocalesDirectory_;
}
