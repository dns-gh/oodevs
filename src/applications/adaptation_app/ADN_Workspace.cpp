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
#include "ADN_Drawings_Data.h"
#include "ADN_Drawings_GUI.h"
#include "ADN_enums.h"
#include "ADN_Resources_Data.h"
#include "ADN_Resources_GUI.h"
#include "ADN_FireClass_Data.h"
#include "ADN_FireClass_GUI.h"
#include "ADN_GuiBuilder.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_HumanFactors_GUI.h"
#include "ADN_Inhabitants_Data.h"
#include "ADN_Inhabitants_GUI.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_KnowledgeGroups_GUI.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Launchers_GUI.h"
#include "ADN_Logistic_Data.h"
#include "ADN_Logistic_GUI.h"
#include "ADN_MainWindow.h"
#include "ADN_Missions_Data.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Models_Data.h"
#include "ADN_Models_GUI.h"
#include "ADN_NBC_Datas.h"
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
#include "ADN_UnitSymbols_Data.h"
#include "ADN_UnitSymbols_GUI.h"
#include "ADN_Urban_Data.h"
#include "ADN_Urban_GUI.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Weapons_GUI.h"
#include "ADN_WorkspaceElement.h"
#include "ADN_Disasters_Data.h"
#include "ADN_Disasters_GUI.h"
#include "ENT/ENT_Tr.h"
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <errno.h>
#include <io.h>
#include <windows.h>
#pragma warning( push, 0 )
#include <Qtcore/qtimer.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qlayout.h>
#pragma warning( pop )

namespace bfs = boost::filesystem;

ADN_Workspace* ADN_Workspace::pWorkspace_=0;

#define WAIT_RESET 2000

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::GetWorkspace
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Workspace& ADN_Workspace::GetWorkspace()
{
    if( !pWorkspace_ )
    {
        pWorkspace_ = new ADN_Workspace();
        pWorkspace_->Initialize();
    }
    return *pWorkspace_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::CleanWorkspace
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void ADN_Workspace::CleanWorkspace()
{
    delete pWorkspace_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace constructor
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
ADN_Workspace::ADN_Workspace()
    : pProgressIndicator_( 0 )
    , nOpenMode_         ( eOpenMode_Normal )
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
    // Creation order
    elements_[eDrawings]          = new ADN_WorkspaceElement< ADN_Drawings_Data, ADN_Drawings_GUI>                   ( eDrawings );
    elements_[eSymbols]           = new ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI>                     ( eSymbols );
    elements_[eUnitSymbols]       = new ADN_WorkspaceElement< ADN_UnitSymbols_Data, ADN_UnitSymbols_GUI>             ( eUnitSymbols );
    elements_[eCategories]        = new ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >              ( eCategories );
    elements_[eUrban]             = new ADN_WorkspaceElement< ADN_Urban_Data, ADN_Urban_GUI >                        ( eUrban );
    elements_[eNBC]               = new ADN_WorkspaceElement< ADN_NBC_Datas, ADN_NBC_GUI >                           ( eNBC );
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

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Build
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void ADN_Workspace::Build( ADN_MainWindow& mainwindow )
{
    assert( pProgressIndicator_ != 0 );
    pProgressIndicator_->SetVisible( true );
    pProgressIndicator_->Reset( tr( "Loading GUI..." ) );
    pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );

    for( int n = 0; n < eNbrWorkspaceElements; ++n )
    {
        elements_[n]->GetGuiABC().Build();
        elements_[n]->GetGuiABC().RegisterTable( mainwindow );
        pProgressIndicator_->Increment( elements_[n]->GetName().toStdString().c_str() );
    }

    // Tab order
    AddPage( mainwindow, eCategories );
    AddPage( mainwindow, eUrban );
    AddPage( mainwindow, eResources );
    AddPage( mainwindow, eResourceNetworks );
    AddPage( mainwindow, eLaunchers );
    AddPage( mainwindow, eWeapons );
    AddPage( mainwindow, eSensors );
    AddPage( mainwindow, eActiveProtections );
    AddPage( mainwindow, eBreakdowns );
    AddPage( mainwindow, eFireClasses );
    AddPage( mainwindow, eNBC );
    AddPage( mainwindow, eObjects );
    AddPage( mainwindow, eMissions );
    AddPage( mainwindow, eModels );
    AddPage( mainwindow, eEquipments );
    AddPage( mainwindow, eUnits );
    AddPage( mainwindow, eAutomata );
    AddPage( mainwindow, eCrowds );
    AddPage( mainwindow, eInhabitants );
    AddPage( mainwindow, eLogistic );
    AddPage( mainwindow, eCommunications );
    AddPage( mainwindow, eKnowledgeGroups );
    AddPage( mainwindow, eHumanFactors );
    AddPage( mainwindow, eAiEngine );
    AddPage( mainwindow, eDisasters );

    //AddPage( mainwindow, eReports ); // $$$$ JSR 2012-01-04: TODO : reports � supprimer compl�tement?

    pProgressIndicator_->Reset( tr( "GUI loaded" ) );
    pProgressIndicator_->SetVisible( false );
    connect( this, SIGNAL( ChangeTab( E_WorkspaceElements ) ), &mainwindow, SIGNAL( ChangeTab( E_WorkspaceElements ) ) );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::AddPage
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
void ADN_Workspace::AddPage( ADN_MainWindow& mainWindow, E_WorkspaceElements element )
{
    mainWindow.AddPage( element, *elements_[ element ]->GetGuiABC().GetMainWidget(), elements_[ element ]->GetName() );
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Reset
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void ADN_Workspace::Reset(const std::string& filename, bool bVisible )
{
    if( bVisible )
    {
        pProgressIndicator_->SetVisible( true );
        pProgressIndicator_->Reset( tr( "Reseting project..." ) );
        pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );
    }

    projectData_->SetFile( filename );
    for( int n = eNbrWorkspaceElements - 1; n >= 0; --n )
    {
        if( bVisible )
            pProgressIndicator_->Increment( tr( "Unloading: %1..." ).arg( elements_[n]->GetName() ).toStdString().c_str() );
        qApp->processEvents();
        elements_[n]->GetDataABC().Reset();
    }
    projectData_->Reset();

    if( bVisible )
    {
        pProgressIndicator_->Reset( tr( "Project reseted" ) );
        pProgressIndicator_->SetVisible( false );
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Load
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void ADN_Workspace::Load( const std::string& filename, const tools::Loader_ABC& fileLoader )
{
    Reset( filename, false );

    // load configuration file
    // Must load it first
    pProgressIndicator_->SetVisible( true );
    pProgressIndicator_->Reset( tr( "Loading project..." ) );
    pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );

    projectData_->SetFile( filename );
    projectData_->Load( fileLoader );
    pProgressIndicator_->Increment();

    // Treatment order, ie E_WorkspaceElements order.
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
    {
        pProgressIndicator_->Increment( tr( "Loading: %1..." ).arg( elements_[n]->GetName() ).toStdString().c_str() );
        qApp->processEvents();
        elements_[n]->GetDataABC().Load( fileLoader );
    }
    // Allow circular dependences between pages
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[n]->GetDataABC().Initialize();
    ResetProgressIndicator();
    pProgressIndicator_->SetVisible( false );

    GetModels().GetGui().Enable( nOpenMode_ == eOpenMode_Admin );
    GetMissions().GetGui().Enable( nOpenMode_ == eOpenMode_Admin );
    GetObjects().GetGui().Enable( nOpenMode_ == eOpenMode_Admin );
}

inline bool isWritable(const std::string& filename)
{
    int fa=_access(filename.c_str(),2);
    if( fa == -1 && errno == EACCES)
        return false;
    return true;

}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::SetOptions
// Created: LDC 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Workspace::SetOptions( bool symbols, bool noreadonly )
{
    symbols_ = symbols;
    if( noreadonly )
        GetProject().GetDataInfos().SetNoReadOnly();
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::ShowSymbols
// Created: PHC 2011-09-21
// -----------------------------------------------------------------------------
bool ADN_Workspace::ShowSymbols() const
{
    return symbols_;
}

namespace
{
    void CopyUnsavedFiles( const bfs::path& from, const bfs::path& to )
    {
        bfs::directory_iterator end;
        for( bfs::directory_iterator it( from ); it != end; ++it )
        {
            if( bfs::is_directory( *it ) )
            {
                bfs::path dest( to / it->path().filename() );
                if( !bfs::exists( dest ) )
                    bfs::create_directories( dest );
                CopyUnsavedFiles( *it, dest );
            }
            else
            {
                if( !bfs::exists( to / it->path().filename() ) )
                    bfs::copy_file( *it, to / it->path().filename() );
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::SaveAs
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
bool ADN_Workspace::SaveAs( const std::string& filename, const tools::Loader_ABC& fileLoader )
{
    ADN_Project_Data::WorkDirInfos& dirInfos = ADN_Project_Data::GetWorkDirInfos();

    // Set a temporary working directory
    std::string szOldWorkDir = dirInfos.GetWorkingDirectory().GetData();
    dirInfos.SetWorkingDirectory( filename );

    // Retrieve list of needed files
    T_StringList files;
    files.push_back( dirInfos.GetRelativePath( filename ) );
    projectData_->FilesNeeded( files );
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[n]->GetDataABC().FilesNeeded( files );

    /////////////////////////////////////
    // Check Writability of files

    // dialog log
    ADN_DialogLog dlgLog( 0 );
    dlgLog.setCaption( tr( "Sword Adaptation Tool - Saving Errors" ) );
    dlgLog.setMsg( tr( "Error(s) have been encountered during saving of project " ).toStdString() + filename );
    dlgLog.setMsgFormat( tr( "<p>- Unable to save %s : file is write protected</p>" ).toStdString());

    T_StringList uncopiedFiles;
    if( szOldWorkDir != dirInfos.GetWorkingDirectory().GetData() )
    {
        const ADN_Project_Data::DataInfos& infos = projectData_->GetDataInfos();
        uncopiedFiles.push_back( infos.szPathfinder_.GetData() );
        uncopiedFiles.push_back( infos.szObjectNames_.GetData() );
        uncopiedFiles.push_back( infos.szHumanProtections_.GetData() );
        uncopiedFiles.push_back( infos.szMedicalTreatment_.GetData() );
        uncopiedFiles.push_back( infos.szExtensions_.GetData() );
        uncopiedFiles.push_back( infos.szDrawingTemplates_.GetData() );
        uncopiedFiles.push_back( infos.szScores_.GetData() );
        uncopiedFiles.push_back( infos.szSymbols_.GetData() );
        uncopiedFiles.push_back( infos.szFilters_.GetData() );
        files.insert( files.end(), uncopiedFiles.begin(), uncopiedFiles.end() );
    }

    for( T_StringList::iterator it = files.begin(); it != files.end(); ++it )
    {
        const std::string file = dirInfos.GetWorkingDirectory().GetData() + *it;
        if( !isWritable( file ) )
            dlgLog.addMsg( file );
    }
    // set old working directory
    dirInfos.SetWorkingDirectory( szOldWorkDir );
    if( !dlgLog.empty() )
    {
        dlgLog.exec();
        ResetProgressIndicator();
        return false;
    }

    /////////////////////////////////////
    // Save Data files in temporary folder
    try
    {
        // saving in temporary files activated
        dirInfos.UseTempDirectory( true );
        pProgressIndicator_->SetVisible( true );
        pProgressIndicator_->Reset( tr( "Saving project..." ) );
        pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements + 1 );
        projectData_->SetFile( filename );
        projectData_->Save( fileLoader );
        for( int n = 0; n < eNbrWorkspaceElements; ++n )
        {
            elements_[n]->GetDataABC().Save();
            pProgressIndicator_->Increment( elements_[n]->GetName().toStdString().c_str() );
        }

        for( T_StringList::iterator it = uncopiedFiles.begin(); it != uncopiedFiles.end(); ++it )
            ADN_Tools::CopyFileToFile( szOldWorkDir + *it, dirInfos.GetTempDirectory().GetData() + *it );

        dirInfos.UseTempDirectory( false );
    }
    catch( const std::exception& )
    {
        dirInfos.SetWorkingDirectory( szOldWorkDir ); // $$$$ NLD 2007-01-15: needed ???
        ResetProgressIndicator();
        throw;
    }

    /////////////////////////////////////
    // Copy Tmp Files To Real Files
    for( T_StringList::iterator it = files.begin(); it != files.end(); ++it )
        if( !it->empty() && bfs::exists( dirInfos.GetTempDirectory().GetData() + *it ) )
            if( !ADN_Tools::CopyFileToFile( dirInfos.GetTempDirectory().GetData() + *it, dirInfos.GetWorkingDirectory().GetData() + *it ) )
            {
                dirInfos.SetWorkingDirectory( szOldWorkDir );
                ResetProgressIndicator();
                throw MASA_EXCEPTION( tr( "Could not save file '%1'.\nMake sure that the file is not write-protected." ).arg( it->c_str() ).toStdString() );
            }
    // Copy remaining files if any
    if( szOldWorkDir != dirInfos.GetWorkingDirectory().GetData() )
        CopyUnsavedFiles( bfs::path( szOldWorkDir ), bfs::path( dirInfos.GetWorkingDirectory().GetData() ) );
    pProgressIndicator_->Increment( "" );
    pProgressIndicator_->SetVisible( false );

    // Save is ended
    ResetProgressIndicator();
    projectData_->GetDataInfos().DisableReadOnly();
    return true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Save
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
bool ADN_Workspace::Save( const tools::Loader_ABC& fileLoader )
{
    return SaveAs( GetProject().GetFileInfos().GetFileNameFull(), fileLoader );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::ExportHtml
// Created: APE 2005-04-19
// -----------------------------------------------------------------------------
void ADN_Workspace::ExportHtml( const std::string& strPath )
{
    ADN_HtmlBuilder mainIndexBuilder;
    mainIndexBuilder.BeginHtml( tr( "ADN - Data export" ) );
    mainIndexBuilder.BeginList();

    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[n]->GetGuiABC().ExportHtml( mainIndexBuilder, strPath.c_str() );

    mainIndexBuilder.EndList();
    mainIndexBuilder.EndHtml();
    std::string strFileName = strPath + "/index.htm";
    mainIndexBuilder.WriteToFile( strFileName.c_str() );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::ResetProgressIndicator
// Created: SBO 2006-01-13
// -----------------------------------------------------------------------------
void ADN_Workspace::ResetProgressIndicator()
{
    pProgressIndicator_->Reset();
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
    emit ChangeTab( goToInfo.targetTab_ );
    assert( elements_[ goToInfo.targetTab_ ] != 0 );
    if( goToInfo.subTargetTab_ != -1 )
        elements_[ goToInfo.targetTab_ ]->GetGuiABC().ChangeCurrentSubTab( goToInfo.subTargetTab_ );
    elements_[ goToInfo.targetTab_ ]->GetGuiABC().ClearFilters();

    if( !elements_[ goToInfo.targetTab_ ]->GetGuiABC().SelectItem( goToInfo.targetName_ ) )
        elements_[ goToInfo.targetTab_ ]->GetGuiABC().FindSubTabAndSelectItem( goToInfo.targetName_ );
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
    if( helpers::ArmorInfos* infos = dynamic_cast< helpers::ArmorInfos* >( data ) )
        return FillUsingElements( eEquipments, *infos, GetEquipments().GetData(), &ADN_Equipments_Data::GetEquipmentsThatUse, result );
    if( ADN_Categories_Data::SizeInfos* infos = dynamic_cast< ADN_Categories_Data::SizeInfos* >( data ) )
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
    if( helpers::ResourceNatureInfos* infos = dynamic_cast< helpers::ResourceNatureInfos* >( data ) )
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
    if( ADN_Weapons_Data::WeaponInfos* infos = dynamic_cast< ADN_Weapons_Data::WeaponInfos* >( data ) )
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
    if( helpers::LogisticSupplyClass* infos = dynamic_cast< helpers::LogisticSupplyClass* >( data ) )
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
    if( helpers::ArmorInfos* infos = dynamic_cast< helpers::ArmorInfos* >( data ) )
    {
        result[ eUrban ];
        result[ eCrowds ];
        result[ eResources ] = GetResources().GetData().GetResourcesWithDirectFire();
        return result;
    }
    // All crowds and sensors use volume
    if( ADN_Categories_Data::SizeInfos* infos = dynamic_cast< ADN_Categories_Data::SizeInfos* >( data ) )
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
