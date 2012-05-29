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
#include "ADN_Composantes_Data.h"
#include "ADN_Composantes_GUI.h"
#include "ADN_DialogLog.h"
#include "ADN_Drawings_Data.h"
#include "ADN_Drawings_GUI.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_FireClass_Data.h"
#include "ADN_FireClass_GUI.h"
#include "ADN_GuiBuilder.h"
#include "ADN_GuiTools.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_HumanFactors_GUI.h"
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
#include "ADN_People_Data.h"
#include "ADN_People_GUI.h"
#include "ADN_Population_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_ProgressIndicator_ABC.h"
#include "ADN_Project_Data.h"
#include "ADN_Reports_Data.h"
#include "ADN_Reports_GUI.h"
#include "ADN_ResourceNetworks_Data.h"
#include "ADN_ResourceNetworks_GUI.h"
#include "ADN_SaveFile_Exception.h"
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
#include "qtundo.h"
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
    , pUndoStack_        ( 0 )
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
    projectData_ = new ADN_Project_Data();
    // Creation order
    elements_[eDrawings]          = new ADN_WorkspaceElement< ADN_Drawings_Data, ADN_Drawings_GUI>( tr( "Drawings" ) );
    elements_[eSymbols]           = new ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI>( tr( "Symbols" ) );
    elements_[eUnitSymbols]       = new ADN_WorkspaceElement< ADN_UnitSymbols_Data, ADN_UnitSymbols_GUI>( tr( "UnitSymbols" ) );
    elements_[eCategories]        = new ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >( tr( "Categories" ) );
    elements_[eUrban]             = new ADN_WorkspaceElement< ADN_Urban_Data, ADN_Urban_GUI >( tr( "Urban" ) );
    elements_[eNBC]               = new ADN_WorkspaceElement< ADN_NBC_Datas, ADN_NBC_GUI >( tr( "NBC" ) );
    elements_[eLaunchers]         = new ADN_WorkspaceElement< ADN_Launchers_Data, ADN_Launchers_GUI >( tr( "Launchers" ) );
    elements_[eEquipement]        = new ADN_WorkspaceElement< ADN_Equipement_Data, ADN_Equipement_GUI >( tr( "Resources" ) );
    elements_[eActiveProtections] = new ADN_WorkspaceElement< ADN_ActiveProtections_Data, ADN_ActiveProtections_GUI >( tr( "Active Protections" ) );
    elements_[eObjects]           = new ADN_WorkspaceElement< ADN_Objects_Data, ADN_Objects_GUI>( tr( "Objects" ) );
    elements_[eWeapons]           = new ADN_WorkspaceElement< ADN_Weapons_Data, ADN_Weapons_GUI >( tr( "Weapon systems" ) );
    elements_[eSensors]           = new ADN_WorkspaceElement< ADN_Sensors_Data, ADN_Sensors_GUI >( tr( "Sensors" ) );
    elements_[eComposantes]       = new ADN_WorkspaceElement< ADN_Composantes_Data, ADN_Composantes_GUI >( tr( "Equipments" ) );
    elements_[eResourceNetworks]  = new ADN_WorkspaceElement< ADN_ResourceNetworks_Data, ADN_ResourceNetworks_GUI >( tr( "Resource networks" ) );
    elements_[eAiEngine]          = new ADN_WorkspaceElement< ADN_AiEngine_Data, ADN_AiEngine_GUI >( tr( "Op. indicators" ) );
    elements_[eModels]            = new ADN_WorkspaceElement< ADN_Models_Data, ADN_Models_GUI >( tr( "Doctrine models" ) );
    elements_[eUnits]             = new ADN_WorkspaceElement< ADN_Units_Data, ADN_Units_GUI >( tr( "Units" ) );
    elements_[eAutomata]          = new ADN_WorkspaceElement< ADN_Automata_Data, ADN_Automata_GUI >( tr( "Automata" ) );
    elements_[eBreakdowns]        = new ADN_WorkspaceElement< ADN_Breakdowns_Data, ADN_Breakdowns_GUI >( tr( "Breakdowns" ) );
    elements_[eCommunications]    = new ADN_WorkspaceElement< ADN_Communications_Data, ADN_Communications_GUI>( tr( "Jamming" ) );
    elements_[eHumanFactors]      = new ADN_WorkspaceElement< ADN_HumanFactors_Data, ADN_HumanFactors_GUI>( tr( "Human factors" ) );
    elements_[eMissions]          = new ADN_WorkspaceElement< ADN_Missions_Data, ADN_Missions_GUI>( tr( "Missions" ) );
    elements_[eKnowledgeGroups]   = new ADN_WorkspaceElement< ADN_KnowledgeGroups_Data, ADN_KnowledgeGroups_GUI>( tr( "Knowledge groups" ) );
    elements_[ePopulation]        = new ADN_WorkspaceElement< ADN_Population_Data, ADN_Population_GUI >( tr( "Crowds" ) );
    elements_[ePeople]            = new ADN_WorkspaceElement< ADN_People_Data, ADN_People_GUI >( tr( "Populations" ) );
    elements_[eReports]           = new ADN_WorkspaceElement< ADN_Reports_Data, ADN_Reports_GUI >( tr( "Reports" ) );
    elements_[eFireClasses]       = new ADN_WorkspaceElement< ADN_FireClass_Data, ADN_FireClass_GUI >( tr( "Fires" ) );
    elements_[eLogistic]          = new ADN_WorkspaceElement< ADN_Logistic_Data, ADN_Logistic_GUI >( tr( "Log" ) );
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
    delete pUndoStack_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Build
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void ADN_Workspace::Build( ADN_MainWindow& mainwindow )
{
    pUndoStack_ = new QtUndoStack( & mainwindow );
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
    AddPage( mainwindow, eEquipement );
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
    AddPage( mainwindow, eComposantes );
    AddPage( mainwindow, eUnits );
    AddPage( mainwindow, eAutomata );
    AddPage( mainwindow, ePopulation );
    AddPage( mainwindow, ePeople );
    AddPage( mainwindow, eLogistic );
    AddPage( mainwindow, eCommunications );
    AddPage( mainwindow, eKnowledgeGroups );
    AddPage( mainwindow, eHumanFactors );
    AddPage( mainwindow, eAiEngine );
    //AddPage( mainwindow, eReports ); // $$$$ JSR 2012-01-04: TODO : reports � supprimer compl�tement?

    pProgressIndicator_->Reset( tr( "GUI loaded" ) );
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
        pProgressIndicator_->Reset( tr( "Project reseted" ) );

    GetUndoStack().clear();
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
// Name: ADN_Workspace::IsValidDatabase
// Created: PHC 2011-01-19
// -----------------------------------------------------------------------------
bool ADN_Workspace::IsValidDatabase()
{
    BOOST_FOREACH( ADN_WorkspaceElement_ABC* element, elements_ )
        if( !element->GetDataABC().IsValidDatabase() )
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
// Name: ADN_Workspace::IsValidDatabase
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
                bfs::path dest( to / it->leaf() );
                if( !bfs::exists( dest ) )
                    bfs::create_directories( dest );
                CopyUnsavedFiles( *it, dest );
            }
            else
            {
                if( !bfs::exists( to / it->leaf() ) )
                    bfs::copy_file( *it, to / it->leaf() );
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
    if( !IsValidDatabase() )
        return false;

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
    dlgLog.setMsg( tr( "Error(s) have been encountered during saving of project " ).ascii() + filename );
    dlgLog.setMsgFormat( tr( "<p>- Unable to save %s : file is write protected</p>" ).ascii());

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
    catch( ADN_Exception_ABC& )
    {
        dirInfos.SetWorkingDirectory( szOldWorkDir ); // $$$$ NLD 2007-01-15: needed ???
        ResetProgressIndicator();
        throw;
    }

    /////////////////////////////////////
    // Copy Tmp Files To Real Files
    for( T_StringList::iterator it = files.begin(); it != files.end(); ++it )
        if( !ADN_Tools::CopyFileToFile( dirInfos.GetTempDirectory().GetData() + *it, dirInfos.GetWorkingDirectory().GetData() + *it ) )
        {
            dirInfos.SetWorkingDirectory( szOldWorkDir );
            ResetProgressIndicator();
            throw ADN_SaveFile_Exception( *it );
        }
    // Copy remaining files if any
    if( szOldWorkDir != dirInfos.GetWorkingDirectory().GetData() )
        CopyUnsavedFiles( bfs::path( szOldWorkDir ), bfs::path( dirInfos.GetWorkingDirectory().GetData() ) );
    pProgressIndicator_->Increment( "" );

    GetUndoStack().clear();
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
// Name: ADN_Workspace::GetUndoStack
// Created: AGN 2004-05-12
// -----------------------------------------------------------------------------
QtUndoStack& ADN_Workspace::GetUndoStack()
{
    assert( pUndoStack_ != 0 );
    return *pUndoStack_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::AddCommand
// Created: AGN 2004-05-13
// -----------------------------------------------------------------------------
void ADN_Workspace::AddCommand( QtCommand* pNewCommand )
{
    if( pNewCommand == 0 )
        return;
    pUndoStack_->push( pNewCommand );
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

    if( !elements_[ goToInfo.targetTab_ ]->GetGuiABC().SelectItem( goToInfo.targetName_ ) )
        elements_[ goToInfo.targetTab_ ]->GetGuiABC().FindSubTabAndSelectItem( goToInfo.targetName_, goToInfo.sourceColumn_ );
}
