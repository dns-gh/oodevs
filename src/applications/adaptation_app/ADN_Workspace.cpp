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
#include "ADN_Health_Data.h"
#include "ADN_Health_GUI.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_HumanFactors_GUI.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_KnowledgeGroups_GUI.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Launchers_GUI.h"
#include "ADN_MainWindow.h"
#include "ADN_Maintenance_Data.h"
#include "ADN_Maintenance_GUI.h"
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
#include "ADN_Supply_Data.h"
#include "ADN_Supply_GUI.h"
#include "ADN_Funeral_Data.h"
#include "ADN_Funeral_GUI.h"
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
#include <errno.h>
#include <io.h>
#include <windows.h>
#pragma warning( push, 0 )
#include <Qtcore/qtimer.h>
#include <QtGui/qmessagebox.h>
#include <QtGui/qlayout.h>
#pragma warning( pop )

ADN_Workspace* ADN_Workspace::pWorkspace_=0;

#define WAIT_RESET 2000

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::GetWorkspace
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
ADN_Workspace& ADN_Workspace::GetWorkspace()
{
    if (!pWorkspace_)
        new ADN_Workspace();
    return *pWorkspace_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::CleanWorkspace
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
void ADN_Workspace::CleanWorkspace()
{
    if (pWorkspace_)
        delete pWorkspace_;
    pWorkspace_=0;
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
    pWorkspace_ = this;

    projectData_ = new ADN_Project_Data();
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
    elements_[eMaintenance]       = new ADN_WorkspaceElement< ADN_Maintenance_Data, ADN_Maintenance_GUI>( tr( "Maintenance" ) );
    elements_[eMissions]          = new ADN_WorkspaceElement< ADN_Missions_Data, ADN_Missions_GUI>( tr( "Missions" ) );
    elements_[eKnowledgeGroups]   = new ADN_WorkspaceElement< ADN_KnowledgeGroups_Data, ADN_KnowledgeGroups_GUI>( tr( "Knowledge groups" ) );
    elements_[eHealth]            = new ADN_WorkspaceElement< ADN_Health_Data, ADN_Health_GUI>( tr( "Health" ) );
    elements_[eSupply]            = new ADN_WorkspaceElement< ADN_Supply_Data, ADN_Supply_GUI>( tr( "Supply" ) );
    elements_[eFuneral]           = new ADN_WorkspaceElement< ADN_Funeral_Data, ADN_Funeral_GUI>( tr( "Funeral" ) );
    elements_[ePopulation]        = new ADN_WorkspaceElement< ADN_Population_Data, ADN_Population_GUI >( tr( "Crowds" ) );
    elements_[ePeople]            = new ADN_WorkspaceElement< ADN_People_Data, ADN_People_GUI >( tr( "Populations" ) );
    elements_[eReports]           = new ADN_WorkspaceElement< ADN_Reports_Data, ADN_Reports_GUI >( tr( "Reports" ) );
    elements_[eFireClasses]       = new ADN_WorkspaceElement< ADN_FireClass_Data, ADN_FireClass_GUI >( tr( "Fires" ) );
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
void ADN_Workspace::Build( ADN_MainWindow& mainWindow )
{
    pUndoStack_ = new QtUndoStack( & mainWindow );
    pProgressIndicator_->Reset( tr( "Loading GUI..." ) );
    pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );

    for( int n = 0; n < eNbrWorkspaceElements; ++n )
    {
        elements_[n]->GetGuiABC().Build();
        elements_[n]->GetGuiABC().RegisterTable( mainWindow );
        pProgressIndicator_->Increment( elements_[n]->GetName().toStdString().c_str() );
    }

    // Force an order for the tabs. Combine some other tabs. Exclude others... All in all, has to be done by hand.
    mainWindow.AddPage( elements_[eCategories]->GetName(), * elements_[eCategories]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eUrban]->GetName(), * elements_[eUrban]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eLaunchers]->GetName(), * elements_[eLaunchers]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eEquipement]->GetName(), * elements_[eEquipement]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eWeapons]->GetName(), * elements_[eWeapons]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eActiveProtections]->GetName(), * elements_[eActiveProtections]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eSensors]->GetName(), * elements_[eSensors]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eBreakdowns]->GetName(), * elements_[eBreakdowns]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eComposantes]->GetName(), * elements_[eComposantes]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eResourceNetworks]->GetName(), * elements_[eResourceNetworks]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eUnits]->GetName(), * elements_[eUnits]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eAutomata]->GetName(), * elements_[eAutomata]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[ePopulation]->GetName(), * elements_[ePopulation]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[ePeople]->GetName(), * elements_[ePeople]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eModels]->GetName(), * elements_[eModels]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eObjects]->GetName(), * elements_[eObjects]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eNBC]->GetName(), * elements_[eNBC]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eFireClasses]->GetName(), * elements_[eFireClasses]->GetGuiABC().GetMainWidget() );

    QWidget* pLogPage = new QWidget();
    Q3VBoxLayout* pLayout = new Q3VBoxLayout( pLogPage );

    elements_[eMaintenance]->GetGuiABC().GetMainWidget()->reparent( pLogPage, QPoint( 0, 0 ) );
    elements_[eSupply]->GetGuiABC().GetMainWidget()->reparent( pLogPage, QPoint( 0, 0 ) );
    elements_[eHealth]->GetGuiABC().GetMainWidget()->reparent( pLogPage, QPoint( 0, 0 ) );
    elements_[eFuneral]->GetGuiABC().GetMainWidget()->reparent( pLogPage, QPoint( 0, 0 ) );
    pLayout->addWidget( elements_[eMaintenance]->GetGuiABC().GetMainWidget() );
    pLayout->addWidget( elements_[eSupply]->GetGuiABC().GetMainWidget() );
    pLayout->addWidget( elements_[eHealth]->GetGuiABC().GetMainWidget() );
    pLayout->addWidget( elements_[eFuneral]->GetGuiABC().GetMainWidget() );
    ADN_GuiBuilder builder;
    builder.AddStretcher( pLayout, Qt::Vertical );
    mainWindow.AddPage( tr( "Log" ), *pLogPage );

    mainWindow.AddPage( elements_[eHumanFactors]->GetName(), * elements_[eHumanFactors]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eCommunications]->GetName(), * elements_[eCommunications]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eKnowledgeGroups]->GetName(), * elements_[eKnowledgeGroups]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eMissions]->GetName(), * elements_[eMissions]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eReports]->GetName(), * elements_[eReports]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eAiEngine]->GetName(), * elements_[eAiEngine]->GetGuiABC().GetMainWidget() );

    pProgressIndicator_->Reset( tr( "GUI loaded" ) );
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

    for( int n = 0; n < eNbrWorkspaceElements; ++n )
    {
        pProgressIndicator_->Increment( tr( "Loading: %1..." ).arg( elements_[n]->GetName() ).toStdString().c_str() );
        qApp->processEvents();
        elements_[n]->GetDataABC().Load( fileLoader );
    }
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
// Name: ADN_Workspace::IsValidDatabase
// Created: LDC 2011-09-21
// -----------------------------------------------------------------------------
void ADN_Workspace::ShowSymbols( bool symbols )
{
    symbols_ = symbols;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::IsValidDatabase
// Created: PHC 2011-09-21
// -----------------------------------------------------------------------------
bool ADN_Workspace::ShowSymbols() const
{
    return symbols_;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::SaveAs
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
bool ADN_Workspace::SaveAs( const std::string& filename )
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
        pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );
        projectData_->SetFile( filename );
        projectData_->Save();
        for( int n = 0; n < eNbrWorkspaceElements; ++n )
        {
            elements_[n]->GetDataABC().Save();
            pProgressIndicator_->Increment( elements_[n]->GetName().toStdString().c_str() );
        }
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

    GetUndoStack().clear();
    // Save is ended
    ResetProgressIndicator();
    return true;
}

//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Save
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
bool ADN_Workspace::Save()
{
    return SaveAs( GetProject().GetFileInfos().GetFileNameFull() );
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
