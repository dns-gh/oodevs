//*****************************************************************************
//
// $Created: JDY 03-06-30 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Workspace.cpp $
// $Author: Ape $
// $Modtime: 25/04/05 12:02 $
// $Revision: 17 $
// $Workfile: ADN_Workspace.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Workspace.h"
#include "moc_ADN_Workspace.cpp"

#include "ADN_ActiveProtections_Data.h"
#include "ADN_ActiveProtections_GUI.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_AiEngine_GUI.h"
#include "ADN_Automata_Data.h"
#include "ADN_Automata_GUI.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Breakdowns_GUI.h"
#include "ADN_Categories_Data.h"
#include "ADN_Categories_GUI.h"
#include "ADN_Communications_Data.h"
#include "ADN_Communications_GUI.h"
#include "ADN_Equipments_Data.h"
#include "ADN_Equipments_GUI.h"
#include "ADN_Crowds_Data.h"
#include "ADN_Crowds_GUI.h"
#include "ADN_DialogLog.h"
#include "ADN_Disasters_Data.h"
#include "ADN_Disasters_GUI.h"
#include "ADN_Drawings_Data.h"
#include "ADN_Drawings_GUI.h"
#include "ADN_enums.h"
#include "ADN_Resources_Data.h"
#include "ADN_Resources_GUI.h"
#include "ADN_FileLoaderObserver.h"
#include "ADN_FireClass_Data.h"
#include "ADN_FireClass_GUI.h"
#include "ADN_GeneralConfig.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_HumanFactors_GUI.h"
#include "ADN_Inhabitants_Data.h"
#include "ADN_Inhabitants_GUI.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_KnowledgeGroups_GUI.h"
#include "ADN_Languages_Data.h"
#include "ADN_Languages_GUI.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Launchers_GUI.h"
#include "ADN_Logistic_Data.h"
#include "ADN_Logistic_GUI.h"
#include "ADN_MainWindow.h"
#include "ADN_Missions_Data.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Models_Data.h"
#include "ADN_Models_GUI.h"
#include "ADN_NavigationInfos.h"
#include "ADN_NBC_Data.h"
#include "ADN_NBC_GUI.h"
#include "ADN_Objects_Data.h"
#include "ADN_Objects_GUI.h"
#include "ADN_ProgressIndicator_ABC.h"
#include "ADN_Project_Data.h"
#include "ADN_Reports_Data.h"
#include "ADN_Reports_GUI.h"
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_ResourceNetworks_GUI.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Sensors_GUI.h"
#include "ADN_Symbols_Data.h"
#include "ADN_Symbols_GUI.h"
#include "ADN_Units_Data.h"
#include "ADN_Units_GUI.h"
#include "ADN_Urban_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Weapons_GUI.h"
#include "ENT/ENT_Tr.h"
#include <boost/foreach.hpp>
#include "tools/DefaultLoader.h"
#include "tools/GeneralConfig.h"
#include "tools/ZipExtractor.h"

ADN_Workspace* ADN_Workspace::pWorkspace_ = 0;

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::CreateWorkspace
// Created: ABR 2013-09-11
// -----------------------------------------------------------------------------
void ADN_Workspace::CreateWorkspace( const ADN_GeneralConfig& config )
{
    if( pWorkspace_ )
        throw MASA_EXCEPTION( "Workspace already created" );
    pWorkspace_ = new ADN_Workspace( config );
    pWorkspace_->Initialize();
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::GetWorkspace
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Workspace& ADN_Workspace::GetWorkspace()
{
    if( !pWorkspace_ )
        throw MASA_EXCEPTION( "Workspace not created" );
    return *pWorkspace_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::CleanWorkspace
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void ADN_Workspace::CleanWorkspace()
{
    if( !pWorkspace_ )
        throw MASA_EXCEPTION( "Workspace not created" );
    delete pWorkspace_;
    pWorkspace_ = 0;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace constructor
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
ADN_Workspace::ADN_Workspace( const ADN_GeneralConfig& config )
    : config_( config )
    , fileLoaderObserver_( new ADN_FileLoaderObserver() )
    , fileLoader_( new tools::DefaultLoader( *fileLoaderObserver_ ) )
    , pProgressIndicator_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::Initialize
// Created: JSR 2012-04-26
// -----------------------------------------------------------------------------
void ADN_Workspace::Initialize()
{
    InitializeEnumType();
    projectData_ = new ADN_Project_Data();
    if( config_.IsNoReadOnly() )
        projectData_->GetDataInfos().SetNoReadOnly();

    // Creation order
    elements_[eDrawings]          = new ADN_WorkspaceElement< ADN_Drawings_Data, ADN_Drawings_GUI>                   ( eDrawings );
    elements_[eSymbols]           = new ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI>                     ( eSymbols );
    elements_[eCategories]        = new ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >              ( eCategories );
    elements_[eUrban]             = new ADN_WorkspaceElement< ADN_Urban_Data, ADN_Urban_GUI >                        ( eUrban );
    elements_[eNBC]               = new ADN_WorkspaceElement< ADN_NBC_Data, ADN_NBC_GUI >                           ( eNBC );
    elements_[eLaunchers]         = new ADN_WorkspaceElement< ADN_Launchers_Data, ADN_Launchers_GUI >                ( eLaunchers );
    elements_[eResources]         = new ADN_WorkspaceElement< ADN_Resources_Data, ADN_Resources_GUI >                ( eResources );
    elements_[eActiveProtections] = new ADN_WorkspaceElement< ADN_ActiveProtections_Data, ADN_ActiveProtections_GUI >( eActiveProtections );
    elements_[eObjects]           = new ADN_WorkspaceElement< ADN_Objects_Data, ADN_Objects_GUI>                     ( eObjects );
    elements_[eWeapons]           = new ADN_WorkspaceElement< ADN_Weapons_Data, ADN_Weapons_GUI >                    ( eWeapons );
    elements_[eSensors]           = new ADN_WorkspaceElement< ADN_Sensors_Data, ADN_Sensors_GUI >                    ( eSensors );
    elements_[eEquipments]        = new ADN_WorkspaceElement< ADN_Equipments_Data, ADN_Equipments_GUI >              ( eEquipments );
    elements_[eResourceNetworks]  = new ADN_WorkspaceElement< ADN_ResourceNetworks_Data, ADN_ResourceNetworks_GUI >  ( eResourceNetworks );
    elements_[eAiEngine]          = new ADN_WorkspaceElement< ADN_AiEngine_Data, ADN_AiEngine_GUI >                  ( eAiEngine );
    elements_[eModels]            = new ADN_WorkspaceElement< ADN_Models_Data, ADN_Models_GUI >                      ( eModels );
    elements_[eUnits]             = new ADN_WorkspaceElement< ADN_Units_Data, ADN_Units_GUI >                        ( eUnits );
    elements_[eAutomata]          = new ADN_WorkspaceElement< ADN_Automata_Data, ADN_Automata_GUI >                  ( eAutomata );
    elements_[eBreakdowns]        = new ADN_WorkspaceElement< ADN_Breakdowns_Data, ADN_Breakdowns_GUI >              ( eBreakdowns );
    elements_[eCommunications]    = new ADN_WorkspaceElement< ADN_Communications_Data, ADN_Communications_GUI>       ( eCommunications );
    elements_[eHumanFactors]      = new ADN_WorkspaceElement< ADN_HumanFactors_Data, ADN_HumanFactors_GUI>           ( eHumanFactors );
    elements_[eMissions]          = new ADN_WorkspaceElement< ADN_Missions_Data, ADN_Missions_GUI>                   ( eMissions );
    elements_[eKnowledgeGroups]   = new ADN_WorkspaceElement< ADN_KnowledgeGroups_Data, ADN_KnowledgeGroups_GUI>     ( eKnowledgeGroups );
    elements_[eCrowds]            = new ADN_WorkspaceElement< ADN_Crowds_Data, ADN_Crowds_GUI >                      ( eCrowds );
    elements_[eInhabitants]       = new ADN_WorkspaceElement< ADN_Inhabitants_Data, ADN_Inhabitants_GUI >            ( eInhabitants );
    elements_[eReports]           = new ADN_WorkspaceElement< ADN_Reports_Data, ADN_Reports_GUI >                    ( eReports );
    elements_[eFireClasses]       = new ADN_WorkspaceElement< ADN_FireClass_Data, ADN_FireClass_GUI >                ( eFireClasses );
    elements_[eLogistic]          = new ADN_WorkspaceElement< ADN_Logistic_Data, ADN_Logistic_GUI >                  ( eLogistic );
    elements_[eDisasters]         = new ADN_WorkspaceElement< ADN_Disasters_Data, ADN_Disasters_GUI >                ( eDisasters );
    elements_[eLanguages]         = new ADN_WorkspaceElement< ADN_Languages_Data, ADN_Languages_GUI >                ( eLanguages );
}

#define INITIALIZE_ADN_ENUMTYPE( TypeName ) \
    ADN_Type_Enum< E_##TypeName, eNbr##TypeName >::SetConverter( &ADN_Tr::ConvertFrom##TypeName );

#define INITIALIZE_ENT_ENUMTYPE( TypeName ) \
    ADN_Type_Enum< E_##TypeName, eNbr##TypeName >::SetConverter( &ENT_Tr::ConvertFrom##TypeName );

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::InitializeEnumType
// Created: ABR 2012-10-29
// -----------------------------------------------------------------------------
void ADN_Workspace::InitializeEnumType()
{
    INITIALIZE_ADN_ENUMTYPE( AgentTypeAutomate );
    INITIALIZE_ADN_ENUMTYPE( AgentTypePion );
    INITIALIZE_ENT_ENUMTYPE( AgentNbcSuit );
    INITIALIZE_ADN_ENUMTYPE( BreakdownNTI );
    INITIALIZE_ADN_ENUMTYPE( BreakdownType );
    INITIALIZE_ADN_ENUMTYPE( ConstructorType );
    INITIALIZE_ADN_ENUMTYPE( ConsumptionType );
    INITIALIZE_ADN_ENUMTYPE( CrossingHeight );
    INITIALIZE_ENT_ENUMTYPE( CrossingType );
    INITIALIZE_ADN_ENUMTYPE( Days );
    INITIALIZE_ADN_ENUMTYPE( EquipmentState_ADN );
    INITIALIZE_ENT_ENUMTYPE( Location );
    INITIALIZE_ADN_ENUMTYPE( MissionParameterType );
    INITIALIZE_ENT_ENUMTYPE( AmmunitionType );
    INITIALIZE_ADN_ENUMTYPE( NatureAtlasType );
    INITIALIZE_ENT_ENUMTYPE( NatureLevel );
    INITIALIZE_ENT_ENUMTYPE( PopulationAttitude );
    INITIALIZE_ADN_ENUMTYPE( PropagationModel );
    INITIALIZE_ADN_ENUMTYPE( ProtectionType );
    INITIALIZE_ADN_ENUMTYPE( RadarType );
    INITIALIZE_ADN_ENUMTYPE( SpeedImpact );
    INITIALIZE_ADN_ENUMTYPE( SupplyConvoyType );
    INITIALIZE_ENT_ENUMTYPE( UnitExperience );
    INITIALIZE_ENT_ENUMTYPE( UnitPosture );
    INITIALIZE_ENT_ENUMTYPE( UnitStress );
    INITIALIZE_ENT_ENUMTYPE( UnitTiredness );
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace destructor
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
ADN_Workspace::~ADN_Workspace()
{
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        delete elements_[n];
    delete projectData_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::IsNewBaseReadOnly
// Created: ABR 2013-09-12
// -----------------------------------------------------------------------------
bool ADN_Workspace::IsNewBaseReadOnly( const tools::Path& filename ) const
{
    if( !filename.Exists() )
        return false;
    bool readOnly = false;
    tools::Xifstream xis( filename );
    xis >> xml::optional >> xml::start( "physical" )
        >> xml::optional >> xml::attribute( "read-only", readOnly );
    return readOnly;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::IsDevMode
// Created: LGY 2013-05-16
// -----------------------------------------------------------------------------
bool ADN_Workspace::IsDevMode() const
{
    return config_.IsDevMode();
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Build
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void ADN_Workspace::Build( QMainWindow& mainWindow )
{
    mainWindow_ = &mainWindow;
    assert( pProgressIndicator_ );
    pProgressIndicator_->SetVisible( true );
    pProgressIndicator_->Reset( tr( "Loading GUI..." ) );
    pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );

    for( int n = 0; n < eNbrWorkspaceElements; ++n )
    {
        elements_[n]->GetGuiABC().Build();
        elements_[n]->GetGuiABC().RegisterTable( static_cast< ADN_MainWindow& >( *mainWindow_ ) );
        pProgressIndicator_->Increment( elements_[n]->GetName().toStdString().c_str() );
    }

    // Tab order
    AddPage( eCategories );
    AddPage( eUrban );
    AddPage( eResources );
    AddPage( eResourceNetworks );
    AddPage( eLaunchers );
    AddPage( eWeapons );
    AddPage( eSensors );
    AddPage( eActiveProtections );
    AddPage( eBreakdowns );
    AddPage( eFireClasses );
    AddPage( eNBC );
    AddPage( eObjects );
    AddPage( eMissions );
    AddPage( eModels );
    AddPage( eEquipments );
    AddPage( eUnits );
    AddPage( eAutomata );
    AddPage( eCrowds );
    AddPage( eInhabitants );
    AddPage( eLogistic );
    AddPage( eCommunications );
    AddPage( eKnowledgeGroups );
    AddPage( eHumanFactors );
    AddPage( eAiEngine );
    if( config_.IsDevMode() )
        AddPage( eDisasters );

    //AddPage( mainwindow, eReports ); // $$$$ JSR 2012-01-04: TODO : reports � supprimer compl�tement?

    pProgressIndicator_->Reset( tr( "GUI loaded" ) );
    pProgressIndicator_->SetVisible( false );
    connect( this, SIGNAL( ChangeTab( E_WorkspaceElements ) ), mainWindow_, SIGNAL( ChangeTab( E_WorkspaceElements ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::AddPage
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Workspace::AddPage( E_WorkspaceElements element )
{
    static_cast< ADN_MainWindow* >( mainWindow_ )->AddPage( element, *elements_[ element ]->GetGuiABC().GetMainWidget(), elements_[ element ]->GetName() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Reset
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void ADN_Workspace::Reset(const tools::Path& filename )
{
    if( pProgressIndicator_ )
    {
        pProgressIndicator_->SetVisible( true );
        pProgressIndicator_->Reset( tr( "Reseting project..." ) );
        pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );
    }

    projectData_->SetFile( filename );
    for( int n = eNbrWorkspaceElements - 1; n >= 0; --n )
    {
        if( pProgressIndicator_ )
            pProgressIndicator_->Increment( tr( "Unloading: %1..." ).arg( elements_[n]->GetName() ).toStdString().c_str() );
        qApp->processEvents();
        elements_[n]->GetGuiABC().DisconnectListView();
        elements_[n]->GetDataABC().ResetData();
    }
    projectData_->Reset();

    if( pProgressIndicator_ )
    {
        pProgressIndicator_->Reset( tr( "Project reseted" ) );
        pProgressIndicator_->SetVisible( false );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Load
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void ADN_Workspace::Load( const tools::Path& filename )
{
    if( pProgressIndicator_ )
    {
        pProgressIndicator_->SetVisible( true );
        pProgressIndicator_->Reset( tr( "Loading project..." ) );
        pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );
    }

    projectData_->SetFile( filename );
    projectData_->Load( *fileLoader_ );
    if( pProgressIndicator_ )
        pProgressIndicator_->Increment();

    // Treatment order, ie E_WorkspaceElements order.
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
    {
        if( pProgressIndicator_ )
            pProgressIndicator_->Increment( tr( "Loading: %1..." ).arg( elements_[n]->GetName() ).toStdString().c_str() );
        qApp->processEvents();
        elements_[n]->GetDataABC().Load( *fileLoader_ );
    }
    // Allow circular dependences between pages
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[n]->GetDataABC().Initialize();
    pProgressIndicator_->Reset();
    if( pProgressIndicator_ )
    pProgressIndicator_->SetVisible( false );
}

namespace
{
    class TempDirectory : private boost::noncopyable
    {
    public:
        TempDirectory( ADN_Project_Data::WorkDirInfos& dirInfos ) 
            : dirInfos_( dirInfos )
        { 
            dirInfos_.UseTempDirectory( true );
            directory_ = dirInfos_.GetTempDirectory();
        }
        ~TempDirectory() 
        {
            dirInfos_.UseTempDirectory( false );
        }

        const tools::Path& GetDirectory()
        {
            return directory_;
        }
    private:
        ADN_Project_Data::WorkDirInfos& dirInfos_;
        tools::Path directory_;
    };

    inline bool IsWritable( const tools::Path& filename )
    {
        int fa = _waccess( filename.ToUnicode().c_str(), 2 );
        if( fa == -1 && errno == EACCES )
            return false;
        return true;
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::SaveAs
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
bool ADN_Workspace::SaveAs( const tools::Path& filename )
{
    ADN_Project_Data::WorkDirInfos& dirInfos = ADN_Project_Data::GetWorkDirInfos();

    // Set a temporary working directory
    tools::Path szOldWorkDir = dirInfos.GetWorkingDirectory().GetData();
    dirInfos.SetWorkingDirectory( filename );

    // Retrieve list of needed files
    tools::Path::T_Paths files;
    files.push_back( dirInfos.GetRelativePath( filename ) );
    projectData_->FilesNeeded( files );
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[n]->GetDataABC().FilesNeeded( files );

    /////////////////////////////////////
    // Check Writability of files

    // dialog log
    ADN_DialogLog dlgLog( 0 );
    dlgLog.setCaption( tr( "Sword Adaptation Tool - Saving Errors" ) );
    dlgLog.setMsg( tr( "Error(s) have been encountered during saving of project " ).toStdString() + filename.ToUTF8() );
    dlgLog.setMsgFormat( tr( "<p>- Unable to save %s : file is write protected</p>" ).toStdString());

    // Unchanged files
    tools::Path::T_Paths unchangedFiles;
    const ADN_Project_Data::DataInfos& infos = projectData_->GetDataInfos();
    unchangedFiles.push_back( infos.szPathfinder_ );
    unchangedFiles.push_back( infos.szObjectNames_ );
    unchangedFiles.push_back( infos.szExtensions_ );
    unchangedFiles.push_back( infos.szDrawingTemplates_ );
    unchangedFiles.push_back( infos.szScores_ );
    unchangedFiles.push_back( infos.szSymbols_ );
    unchangedFiles.push_back( infos.szFilters_ );
    unchangedFiles.push_back( infos.szMissionSheetXslPath_ );
    unchangedFiles.push_back( infos.szStages_ );
    files.insert( files.end(), unchangedFiles.begin(), unchangedFiles.end() );

    for( auto it = files.begin(); it != files.end(); ++it )
    {
        const tools::Path file = dirInfos.GetWorkingDirectory().GetData() / *it;
        if( !IsWritable( file ) )
            dlgLog.addMsg( file.ToUTF8() );
    }
    // set old working directory
    dirInfos.SetWorkingDirectory( szOldWorkDir );
    if( !dlgLog.empty() )
    {
        dlgLog.exec();
        if( pProgressIndicator_ )
            pProgressIndicator_->Reset();
        return false;
    }

    /////////////////////////////////////
    // Save Data files in temporary folder
    TempDirectory tempDirectory( dirInfos );
    try
    {
        // saving in temporary files activated
        if( pProgressIndicator_ )
        {
            pProgressIndicator_->SetVisible( true );
            pProgressIndicator_->Reset( tr( "Saving project..." ) );
            pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements + 1 );
        }
        projectData_->SetFile( filename );
        projectData_->Save( *fileLoader_ );
        for( tools::Path::T_Paths::iterator it = unchangedFiles.begin(); it != unchangedFiles.end(); ++it )
        {
            if( !it->IsEmpty() )
                ( szOldWorkDir / *it ).Copy( tempDirectory.GetDirectory() / *it );
        }

        for( int n = 0; n < eNbrWorkspaceElements; ++n )
        {
            elements_[n]->GetDataABC().Save();
            if( pProgressIndicator_ )
                pProgressIndicator_->Increment( elements_[n]->GetName().toStdString().c_str() );
        }
    }
    catch( const std::exception& )
    {
        dirInfos.SetWorkingDirectory( szOldWorkDir ); // $$$$ NLD 2007-01-15: needed ???
        if( pProgressIndicator_ )
            pProgressIndicator_->Reset();
        throw;
    }

    /////////////////////////////////////
    // Copy Tmp Files To Real Files
    tempDirectory.GetDirectory().Copy( dirInfos.GetWorkingDirectory().GetData(), tools::Path::OverwriteIfExists );

    // Copy remaining files if any
    if( szOldWorkDir != dirInfos.GetWorkingDirectory().GetData() )
        szOldWorkDir.Copy( dirInfos.GetWorkingDirectory().GetData(), tools::Path::IgnoreIfExists );

    // Unzip symbols.pak if not already in the working directory
    if( !( dirInfos.GetWorkingDirectory().GetData() / projectData_->GetDataInfos().szSymbolsPath_ ).Exists() )
        tools::zipextractor::ExtractArchive( tools::GeneralConfig::BuildResourceChildFile( "symbols.pak" ),
                                             dirInfos.GetWorkingDirectory().GetData() / projectData_->GetDataInfos().szSymbolsPath_ );

    if( pProgressIndicator_ )
    {
        pProgressIndicator_->Increment( "" );
        pProgressIndicator_->SetVisible( false );
        pProgressIndicator_->Reset();
    }

    // Save is ended
    projectData_->GetDataInfos().DisableReadOnly();
    return true;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_Workspace::ExportHtml( const tools::Path& strPath )
{
    ADN_HtmlBuilder mainIndexBuilder;
    mainIndexBuilder.BeginHtml( tr( "ADN - Data export" ) );
    mainIndexBuilder.BeginList();

    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[n]->GetGuiABC().ExportHtml( mainIndexBuilder, strPath );

    mainIndexBuilder.EndList();
    mainIndexBuilder.EndHtml();
    tools::Path fileName = strPath / "index.htm";
    mainIndexBuilder.WriteToFile( fileName );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::OnUsersListRequested
// Created: ABR 2012-01-26
// -----------------------------------------------------------------------------
void ADN_Workspace::OnUsersListRequested( const ADN_NavigationInfos::UsedBy& usedByInfo )
{
    emit ChangeTab( usedByInfo.targetTab_ );
    elements_[ usedByInfo.targetTab_ ]->GetGuiABC().ApplyFilter( usedByInfo );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::OnGoToRequested
// Created: ABR 2012-03-05
// -----------------------------------------------------------------------------
void ADN_Workspace::OnGoToRequested( const ADN_NavigationInfos::GoTo& goToInfo )
{
    if( goToInfo.targetTab_ < 0 || goToInfo.targetTab_ >= eNbrWorkspaceElements )
        return;

    emit ChangeTab( goToInfo.targetTab_ );
    if( goToInfo.subTargetTab_ != -1 )
        elements_[ goToInfo.targetTab_ ]->GetGuiABC().ChangeCurrentSubTab( goToInfo.subTargetTab_ );
    elements_[ goToInfo.targetTab_ ]->GetGuiABC().ClearFilters();

    if( !elements_[ goToInfo.targetTab_ ]->GetGuiABC().SelectItem( goToInfo.targetName_ ) )
        elements_[ goToInfo.targetTab_ ]->GetGuiABC().FindSubTabAndSelectItem( goToInfo.targetName_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::OnChooseOptional
// Created: NPT 2013-04-15
// -----------------------------------------------------------------------------
void ADN_Workspace::OnChooseOptional( bool show )
{
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[ n ]->GetGuiABC().UpdateOptionalFieldsVisibility( show );
}

namespace
{
    template< typename T, typename DATA >
    ADN_Workspace::T_UsingElements& FillUsingElements( E_WorkspaceElements workspace, T& infos, DATA& data, QStringList ( DATA::* functor )( T& ), ADN_Workspace::T_UsingElements& result )
    {
        QStringList list = ( data.*functor )( infos );
        if( !list.isEmpty() )
            result[ workspace ] << list;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetElementThatWillBeDeleted
// Created: ABR 2012-08-01
// -----------------------------------------------------------------------------
ADN_Workspace::T_UsingElements ADN_Workspace::GetElementThatWillBeDeleted( ADN_Ref_ABC* data )
{
    T_UsingElements result;

    // Equipments to delete when armor or size deleted
    if( ADN_Armors_Data::ArmorInfos* infos = dynamic_cast< ADN_Armors_Data::ArmorInfos* >( data ) )
        return FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );
    if( ADN_Volumes_Data::VolumeInfos* infos = dynamic_cast< ADN_Volumes_Data::VolumeInfos* >( data ) )
        return FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );

    // Urban templates to delete when material, roofshape or accomodation deleted
    if( ADN_Urban_Data::UrbanMaterialInfos* infos = dynamic_cast< ADN_Urban_Data::UrbanMaterialInfos* >( data ) )
        return FillUsingElements( eUrban, *infos, GetUrban().GetData(), &ADN_Urban_Data::GetUrbanTemplateThatUse, result );
    if( ADN_Urban_Data::RoofShapeInfos* infos = dynamic_cast< ADN_Urban_Data::RoofShapeInfos* >( data ) )
        return FillUsingElements( eUrban, *infos, GetUrban().GetData(), &ADN_Urban_Data::GetUrbanTemplateThatUse, result );
    if( ADN_Urban_Data::AccommodationInfos* infos = dynamic_cast< ADN_Urban_Data::AccommodationInfos* >( data ) )
        return FillUsingElements( eUrban, *infos, GetUrban().GetData(), &ADN_Urban_Data::GetUrbanTemplateThatUse, result );

    // Resources to delete when object or nature deleted
    if( ADN_Objects_Data_ObjectInfos* infos = dynamic_cast< ADN_Objects_Data_ObjectInfos* >( data ) )
        return FillUsingElements( eResources, *infos, GetResources().GetData(), &ADN_Resources_Data::GetResourcesThatUse, result );
    if( ADN_Natures_Data::NatureInfos* infos = dynamic_cast< ADN_Natures_Data::NatureInfos* >( data ) )
        return FillUsingElements( eResources, *infos, GetResources().GetData(), &ADN_Resources_Data::GetResourcesThatUse, result );

    // Weapons to delete when launcher or ammo deleted
    if( ADN_Launchers_Data::LauncherInfos* infos = dynamic_cast< ADN_Launchers_Data::LauncherInfos* >( data ) )
        return FillUsingElements( eWeapons, *infos, GetWeapons().GetData(), &ADN_Weapons_Data::GetWeaponThatUse, result );
    if( ADN_Resources_Data::AmmoCategoryInfo* infos = dynamic_cast< ADN_Resources_Data::AmmoCategoryInfo* >( data ) )
        return FillUsingElements( eWeapons, *infos, GetWeapons().GetData(), &ADN_Weapons_Data::GetWeaponThatUse, result );

    // Automat, population or units to delete when model deleted
    if( ADN_Models_Data::ModelInfos* infos = dynamic_cast< ADN_Models_Data::ModelInfos* >( data ) )
    {
        FillUsingElements( eAutomata, *infos, GetAutomata().GetData(), &ADN_Automata_Data::GetAutomataThatUse, result );
        FillUsingElements( eCrowds, *infos, GetCrowds().GetData(), &ADN_Crowds_Data::GetCrowdsThatUse, result );
        FillUsingElements( eUnits, *infos, GetUnits().GetData(), &ADN_Units_Data::GetUnitsThatUse, result );
        return result;
    }

    // Automat to delete when his pc is deleted
    if( ADN_Units_Data::UnitInfos* infos = dynamic_cast< ADN_Units_Data::UnitInfos* >( data ) )
        return FillUsingElements( eAutomata, *infos, GetAutomata().GetData(), &ADN_Automata_Data::GetAutomataThatUseForPC, result );

    // Inhabitant to delete when crowd deleted
    if( ADN_Crowds_Data::CrowdsInfos* infos = dynamic_cast< ADN_Crowds_Data::CrowdsInfos* >( data ) )
        return FillUsingElements( eInhabitants, *infos, GetInhabitants().GetData(), &ADN_Inhabitants_Data::GetInhabitantsThatUse, result );

    // Actives protections to delete when ammo deleted
    if( ADN_Resources_Data::AmmoCategoryInfo* infos = dynamic_cast< ADN_Resources_Data::AmmoCategoryInfo* >( data ) )
        return FillUsingElements( eActiveProtections, *infos, GetActiveProtections().GetData(), &ADN_ActiveProtections_Data::GetActiveProtectionsThatUse, result );

    // RN to delete when ammunition deleted
    if( ADN_Resources_Data::CategoryInfo* infos = dynamic_cast< ADN_Resources_Data::CategoryInfo* >( data ) )
        return FillUsingElements( eResourceNetworks, *infos, GetResourceNetworks().GetData(), &ADN_ResourceNetworks_Data::GetResourceNetworksThatUse, result );

    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetElementThatUse
// Created: ABR 2012-07-23
// -----------------------------------------------------------------------------
ADN_Workspace::T_UsingElements ADN_Workspace::GetElementThatUse( ADN_Ref_ABC* data )
{
    T_UsingElements result;

    // Equipments that use radar, sensor, weapon, breakdown or active protection
    if( ADN_Radars_Data::RadarInfos* infos = dynamic_cast< ADN_Radars_Data::RadarInfos* >( data ) )
        // TODO: sensors that use radar, but no bind existence to cause no ptr in vector
        return FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );
    if( ADN_Sensors_Data::SensorInfos* infos = dynamic_cast< ADN_Sensors_Data::SensorInfos* >( data ) )
        // TODO: Sensors that use sensors, but no bind existence to cause no ptr in vector
        return FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );
    if( ADN_Weapons_Data_WeaponInfos* infos = dynamic_cast< ADN_Weapons_Data_WeaponInfos* >( data ) )
        return FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );
    if( ADN_Breakdowns_Data::BreakdownInfo* infos = dynamic_cast< ADN_Breakdowns_Data::BreakdownInfo* >( data ) )
        return FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );
    if( ADN_ActiveProtections_Data::ActiveProtectionsInfos* infos = dynamic_cast< ADN_ActiveProtections_Data::ActiveProtectionsInfos* >( data ) )
        return FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );

    // Equipments, sensors and objects that use object
    if( ADN_Objects_Data_ObjectInfos* infos = dynamic_cast< ADN_Objects_Data_ObjectInfos* >( data ) )
    {
        FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );
        FillUsingElements( eSensors, *infos, GetSensors().GetData(), &ADN_Sensors_Data::GetSensorsThatUse, result );
        FillUsingElements( eObjects, *infos, GetObjects().GetData(), &ADN_Objects_Data::GetObjectsThatUse, result );
        FillUsingElements( eMissions, *infos, GetMissions().GetData(), &ADN_Missions_Data::GetAllMissionsThatUse, result );
        return result;
    }

    // Models that use mission
    if( ADN_Missions_Mission* infos = dynamic_cast< ADN_Missions_Mission* >( data ) )
    {
        QStringList& list = result[ eModels ];
        for( int i = 0; i < eNbrEntityTypes; ++i )
            list << GetModels().GetData().GetModelsThatUse( static_cast< E_EntityType >( i ), *infos );
        return result;
    }
    // Models that use frag order
    if( ADN_Missions_FragOrder* infos = dynamic_cast< ADN_Missions_FragOrder* >( data ) )
    {
        QStringList& list = result[ eModels ];
        for( int i = 0; i < eNbrEntityTypes; ++i )
            list << GetModels().GetData().GetModelsThatUse( static_cast< E_EntityType >( i ), *infos );
        return result;
    }

    // Automat that use unit not as pc
    if( ADN_Units_Data::UnitInfos* infos = dynamic_cast< ADN_Units_Data::UnitInfos* >( data ) )
        return FillUsingElements( eAutomata, *infos, GetAutomata().GetData(), &ADN_Automata_Data::GetAutomataThatUseForElement, result );

    // Peoples that use resource network
    if( ADN_ResourceNetworks_Data::ResourceNetworkInfos* infos = dynamic_cast< ADN_ResourceNetworks_Data::ResourceNetworkInfos* >( data ) )
        return FillUsingElements( eInhabitants, *infos, GetInhabitants().GetData(), &ADN_Inhabitants_Data::GetInhabitantsThatUse, result );

    // Active protections, breakdowns, or fire that use category ammo
    if( ADN_Resources_Data::CategoryInfo* infos = dynamic_cast< ADN_Resources_Data::CategoryInfo* >( data ) )
    {
        FillUsingElements( eActiveProtections, *infos, GetActiveProtections().GetData(), &ADN_ActiveProtections_Data::GetActiveProtectionsThatUse, result );
        FillUsingElements( eBreakdowns, *infos, GetBreakdowns().GetData(), &ADN_Breakdowns_Data::GetBreakdownsThatUse, result );
        FillUsingElements( eFireClasses, *infos, GetFireClasses().GetData(), &ADN_FireClass_Data::GetFireThatUse, result );
        FillUsingElements( eObjects, *infos, GetObjects().GetData(), &ADN_Objects_Data::GetObjectsThatUse, result );
        FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );
    }

    // Units that use composante
    if( ADN_Equipments_Data::EquipmentInfos* infos = dynamic_cast< ADN_Equipments_Data::EquipmentInfos* >( data ) )
        return FillUsingElements( eUnits, *infos, GetUnits().GetData(), &ADN_Units_Data::GetUnitsThatUse, result );

    // Units and resources that use logistic supply stock
    if( ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* infos = dynamic_cast< ADN_LogisticSupplyClasses_Data::LogisticSupplyClass* >( data ) )
    {
        FillUsingElements( eUnits, *infos, GetUnits().GetData(), &ADN_Units_Data::GetUnitsThatUse, result );
        FillUsingElements( eResources, *infos, GetResources().GetData(), &ADN_Resources_Data::GetResourcesThatUse, result );
        return result;
    }

    // All objects, fires, and sensors use urban material. Urban template and resources that use urban material
    if( ADN_Urban_Data::UrbanMaterialInfos* infos = dynamic_cast< ADN_Urban_Data::UrbanMaterialInfos* >( data ) )
    {
        result[ eFireClasses ]; // Empty list means all resources
        result[ eSensors ];
        result[ eObjects ] = GetObjects().GetData().GetObjectsWithCapacity( ADN_Objects_Data::ADN_CapacityInfos_UrbanDestruction::TAG );
        result[ eResources ] = GetResources().GetData().GetResourcesWithDirectFire();
        FillUsingElements( eUrban, *infos, GetUrban().GetData(), &ADN_Urban_Data::GetUrbanTemplateThatUse, result );
        return result;
    }
    // All urban materials, ammunitions and crowds use armor. Resources that use armor
    if( ADN_Armors_Data::ArmorInfos* infos = dynamic_cast< ADN_Armors_Data::ArmorInfos* >( data ) )
    {
        result[ eUrban ];
        result[ eCrowds ];
        result[ eResources ] = GetResources().GetData().GetResourcesWithDirectFire();
        return result;
    }
    // All crowds and sensors use volume
    if( ADN_Volumes_Data::VolumeInfos* infos = dynamic_cast< ADN_Volumes_Data::VolumeInfos* >( data ) )
    {
        result[ eSensors ];
        result[ eCrowds ];
        result[ eWeapons ];
        return result;
    }
    // All objects use fire
    if( ADN_FireClass_Data::FireClassInfos* infos = dynamic_cast< ADN_FireClass_Data::FireClassInfos* >( data ) )
    {
        result[ eObjects ] = GetObjects().GetData().GetObjectsWithCapacity( ADN_Objects_Data::ADN_CapacityInfos_FirePropagationModifier::TAG );
        return result;
    }

    if( ADN_Disasters_Data::DisasterInfos* infos = dynamic_cast< ADN_Disasters_Data::DisasterInfos* >( data ) )
    {
        FillUsingElements( eObjects, *infos, GetObjects().GetData(), &ADN_Objects_Data::GetObjectsThatUse, result );
        FillUsingElements( eSensors, *infos, GetSensors().GetData(), &ADN_Sensors_Data::GetSensorsThatUse, result );
    }
    return result;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::OnLanguageChanged
// Created: ABR 2013-07-15
// -----------------------------------------------------------------------------
void ADN_Workspace::OnLanguageChanged( const std::string& language )
{
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[n]->GetDataABC().OnLanguageChanged( language );
}
