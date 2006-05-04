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
#include "ADN_pch.h"
#include "ADN_Workspace.h"
#include "moc_ADN_Workspace.cpp"

#include "ADN_WorkspaceElement.h"
#include "ADN_NBC_GUI.h"
#include "ADN_NBC_Datas.h"
#include "ADN_Launchers_GUI.h"
#include "ADN_Launchers_Data.h"
#include "ADN_Project_GUI.h"
#include "ADN_Project_Data.h"
#include "ADN_Categories_GUI.h"
#include "ADN_Categories_Data.h"
#include "ADN_Equipement_GUI.h"
#include "ADN_Equipement_Data.h"
#include "ADN_Objects_GUI.h"
#include "ADN_Objects_Data.h"
#include "ADN_Weapons_GUI.h"
#include "ADN_Weapons_Data.h"
#include "ADN_Sensors_GUI.h"
#include "ADN_Sensors_Data.h"
#include "ADN_Composantes_GUI.h"
#include "ADN_Composantes_Data.h"
#include "ADN_Units_GUI.h"
#include "ADN_Units_Data.h"
#include "ADN_Models_GUI.h"
#include "ADN_Models_Data.h"
#include "ADN_Automata_GUI.h"
#include "ADN_Automata_Data.h"
#include "ADN_Population_GUI.h"
#include "ADN_Population_Data.h"
#include "ADN_AiEngine_GUI.h"
#include "ADN_AiEngine_Data.h"
#include "ADN_Breakdowns_GUI.h"
#include "ADN_Breakdowns_Data.h"
#include "ADN_Communications_GUI.h"
#include "ADN_Communications_Data.h"
#include "ADN_HumanFactors_GUI.h"
#include "ADN_HumanFactors_Data.h"
#include "ADN_Maintenance_GUI.h"
#include "ADN_Maintenance_Data.h"
#include "ADN_Missions_GUI.h"
#include "ADN_Missions_Data.h"
#include "ADN_KnowledgeGroups_GUI.h"
#include "ADN_KnowledgeGroups_Data.h"
#include "ADN_HLA_GUI.h"
#include "ADN_HLA_Data.h"
#include "ADN_Health_GUI.h"
#include "ADN_Health_Data.h"
#include "ADN_Supply_GUI.h"
#include "ADN_Supply_Data.h"
#include "ADN_HtmlBuilder.h"
#include "ADN_GuiBuilder.h"
#include "ADN_ProgressIndicator_ABC.h"
#include "ADN_DialogLog.h"
#include "ADN_SaveFile_Exception.h"
#include "ADN_MainWindow.h"

#include "MT_Tools/MT_ScipioException.h"

#include "qtundo.h"

#include <io.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <qlayout.h>

#include <windows.h>
#include <errno.h>

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

    elements_[eProject]        = new ADN_WorkspaceElement< ADN_Project_Data, ADN_Project_GUI >( tr( "Project" ) );
    elements_[eCategories]     = new ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >( tr( "Categories" ) );
    elements_[eNBC]            = new ADN_WorkspaceElement< ADN_NBC_Datas, ADN_NBC_GUI >( tr( "NBC" ) );
    elements_[eLaunchers]      = new ADN_WorkspaceElement< ADN_Launchers_Data, ADN_Launchers_GUI >( tr( "Launchers" ) );
    elements_[eEquipement]     = new ADN_WorkspaceElement< ADN_Equipement_Data, ADN_Equipement_GUI >( tr( "Equipements" ) );
    elements_[eObjects]        = new ADN_WorkspaceElement< ADN_Objects_Data, ADN_Objects_GUI>( tr( "Objects" ) );
    elements_[eWeapons]        = new ADN_WorkspaceElement< ADN_Weapons_Data, ADN_Weapons_GUI >( tr( "Weapons" ) );
    elements_[eSensors]        = new ADN_WorkspaceElement< ADN_Sensors_Data, ADN_Sensors_GUI >( tr( "Sensors" ) );
    elements_[eComposantes]    = new ADN_WorkspaceElement< ADN_Composantes_Data, ADN_Composantes_GUI >( tr( "Composantes" ) );
    elements_[eAiEngine]       = new ADN_WorkspaceElement< ADN_AiEngine_Data, ADN_AiEngine_GUI >( tr( "Ai" ) );   
    elements_[eModels]         = new ADN_WorkspaceElement< ADN_Models_Data, ADN_Models_GUI >( tr( "Models" ) );
    elements_[eUnits]          = new ADN_WorkspaceElement< ADN_Units_Data, ADN_Units_GUI >( tr( "Units" ) );
    elements_[eAutomata]       = new ADN_WorkspaceElement< ADN_Automata_Data, ADN_Automata_GUI >( tr( "Automata" ) );
    elements_[eBreakdowns]     = new ADN_WorkspaceElement< ADN_Breakdowns_Data, ADN_Breakdowns_GUI >( tr( "Breakdowns" ) );
    elements_[eCommunications] = new ADN_WorkspaceElement< ADN_Communications_Data, ADN_Communications_GUI>( tr( "Communications" ) );
    elements_[eHumanFactors]   = new ADN_WorkspaceElement< ADN_HumanFactors_Data, ADN_HumanFactors_GUI>( tr( "Human factors" ) );
    elements_[eMaintenance]    = new ADN_WorkspaceElement< ADN_Maintenance_Data, ADN_Maintenance_GUI>( tr( "Maintenance" ) );
    elements_[eMissions]       = new ADN_WorkspaceElement< ADN_Missions_Data, ADN_Missions_GUI>( tr( "Missions" ) );
    elements_[eKnowledgeGroups]= new ADN_WorkspaceElement< ADN_KnowledgeGroups_Data, ADN_KnowledgeGroups_GUI>( tr( "Knowledge groups" ) );
    elements_[eHLA]            = new ADN_WorkspaceElement< ADN_HLA_Data, ADN_HLA_GUI>( tr( "HLA" ) );
    elements_[eHealth]         = new ADN_WorkspaceElement< ADN_Health_Data, ADN_Health_GUI>( tr( "Health" ) );
    elements_[eSupply]         = new ADN_WorkspaceElement< ADN_Supply_Data, ADN_Supply_GUI>( tr( "Supply" ) );
    elements_[ePopulation]     = new ADN_WorkspaceElement< ADN_Population_Data, ADN_Population_GUI >( tr( "Populations" ) );
}


//-----------------------------------------------------------------------------
// Name: ADN_Workspace destructor
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
ADN_Workspace::~ADN_Workspace()
{
    for( int n = 0; n < eNbrWorkspaceElements; ++n )
        delete elements_[n];

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
        pProgressIndicator_->Increment( elements_[n]->GetName() );
    }

    // Force an order for the tabs. Combine some other tabs. Exclude others... All in all, has to be done by hand.
    mainWindow.AddPage( elements_[eProject]->GetName(), * elements_[eProject]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eCategories]->GetName(), * elements_[eCategories]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eLaunchers]->GetName(), * elements_[eLaunchers]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eEquipement]->GetName(), * elements_[eEquipement]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eWeapons]->GetName(), * elements_[eWeapons]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eSensors]->GetName(), * elements_[eSensors]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eBreakdowns]->GetName(), * elements_[eBreakdowns]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eComposantes]->GetName(), * elements_[eComposantes]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eUnits]->GetName(), * elements_[eUnits]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eAutomata]->GetName(), * elements_[eAutomata]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[ePopulation]->GetName(), * elements_[ePopulation]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eModels]->GetName(), * elements_[eModels]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eObjects]->GetName(), * elements_[eObjects]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eNBC]->GetName(), * elements_[eNBC]->GetGuiABC().GetMainWidget() );

    QWidget* pLogPage = new QWidget();
    QVBoxLayout* pLayout = new QVBoxLayout( pLogPage );
    
    elements_[eMaintenance]->GetGuiABC().GetMainWidget()->reparent( pLogPage, QPoint( 0, 0 ) );
    elements_[eSupply]->GetGuiABC().GetMainWidget()->reparent( pLogPage, QPoint( 0, 0 ) );
    elements_[eHealth]->GetGuiABC().GetMainWidget()->reparent( pLogPage, QPoint( 0, 0 ) );
    pLayout->addWidget( elements_[eMaintenance]->GetGuiABC().GetMainWidget() );
    pLayout->addWidget( elements_[eSupply]->GetGuiABC().GetMainWidget() );
    pLayout->addWidget( elements_[eHealth]->GetGuiABC().GetMainWidget() );
    ADN_GuiBuilder builder;
    builder.AddStretcher( pLayout, Qt::Vertical );
    mainWindow.AddPage( tr( "Log" ), *pLogPage );

    mainWindow.AddPage( elements_[eHumanFactors]->GetName(), * elements_[eHumanFactors]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eCommunications]->GetName(), * elements_[eCommunications]->GetGuiABC().GetMainWidget() );
    mainWindow.AddPage( elements_[eKnowledgeGroups]->GetName(), * elements_[eKnowledgeGroups]->GetGuiABC().GetMainWidget() );
    //mainWindow.AddPage( elements_[eMissions]->GetName(), * elements_[eMissions]->GetGuiABC().GetMainWidget() );
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

    this->GetProject().GetData().SetFile( filename );

    for( int n = eNbrWorkspaceElements - 1; n >= 0; --n )
    {
        elements_[n]->GetDataABC().Reset();
        if( bVisible )
            pProgressIndicator_->Increment( elements_[n]->GetName() );
    }

    if( bVisible )
        pProgressIndicator_->Reset( tr( "Project reseted" ) );

    GetUndoStack().clear();
}


//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Load
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
void ADN_Workspace::Load(const std::string& filename)
{
    Reset( filename, false );

    // load configuration file
    // Must load it first
    pProgressIndicator_->Reset( tr( "Loading project..." ) );
    pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );

    this->GetProject().GetData().SetFile( filename );
    this->GetProject().GetData().Load();
    pProgressIndicator_->Increment();

    for( int n = 1; n < eNbrWorkspaceElements; ++n )
    {
        elements_[n]->GetDataABC().Load();
        pProgressIndicator_->Increment( elements_[n]->GetName() );
    }

    pProgressIndicator_->Reset();
}


inline bool isWritable(const std::string& filename)
{
    int fa=_access(filename.c_str(),2);
    if ( fa == -1 && errno == EACCES)
        return false;
    return true;
        
}


//-----------------------------------------------------------------------------
// Name: ADN_Workspace::SaveAs
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
bool ADN_Workspace::SaveAs(const std::string& filename)
{
    // Set a temporary working directory
    std::string szOldWorkDir = ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData();
    ADN_Project_Data::GetWorkDirInfos().SetWorkingDirectory(filename);
    
    // Retrieve list of needed files
    T_StringList files;
    int n;
    files.push_back(  ADN_Project_Data::GetWorkDirInfos().GetPartPath(filename) );
    for( n = 0; n < eNbrWorkspaceElements; ++n )
        elements_[n]->GetDataABC().FilesNeeded( files );

    /////////////////////////////////////
    // Check Writability of files
    
    // dialog log
    ADN_DialogLog dlgLog( 0 );
    dlgLog.setCaption( tr( "Scipio Adaptation Tool - Saving Errors" ) );
    dlgLog.setMsg( tr( "Error(s) have been encountered during saving of project " ).ascii() + filename );
    dlgLog.setMsgFormat( tr( "<p>- Unable to save %s : file is write protected</p>" ).ascii());

    T_StringList::iterator itFileName;
    for( itFileName=files.begin();itFileName!=files.end();++itFileName)
    {
        std::string szFile=ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + *itFileName;
        if (! isWritable( szFile ) )
            dlgLog.addMsg(szFile);
    }
    
    // set old working directory
    ADN_Project_Data::GetWorkDirInfos().SetWorkingDirectory(szOldWorkDir);
        
    
    if ( ! dlgLog.empty() )
    {    
        // some files are nort writable so dlgbox
        // and return immediatelly
        dlgLog.exec();
        pProgressIndicator_->Reset();
        return false;
    }

    /////////////////////////////////////
    // Save Data files in temporary folder
    try
    {
        // saving in temporary files activated
        ADN_Project_Data::GetWorkDirInfos().UseTempDirectory(true);

        pProgressIndicator_->Reset( tr( "Saving project..." ) );
        pProgressIndicator_->SetNbrOfSteps( eNbrWorkspaceElements );

        this->GetProject().GetData().SetFile( filename );
        
        for( n = 0; n < eNbrWorkspaceElements; ++n )
        {
            elements_[n]->GetDataABC().Save();
            pProgressIndicator_->Increment( elements_[n]->GetName() );
        }

        // saving in temporary files disactivated
        ADN_Project_Data::GetWorkDirInfos().UseTempDirectory(false);

    }
    catch( ADN_Exception_ABC& exception )
    {
        QApplication::restoreOverrideCursor();	// restore original cursor
        QMessageBox::critical( 0, exception.GetExceptionTitle().c_str(), exception.GetExceptionMessage().c_str() );
        ADN_Project_Data::GetWorkDirInfos().SetWorkingDirectory(szOldWorkDir);

        pProgressIndicator_->Reset();
        return false;
    }
    catch ( MT_ScipioException& exception)
    {
        // an error occured . show message box
        // and set working directory as old one
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
               << "File : "        << exception.GetFile()        << std::endl
               << "Line : "        << exception.GetLine()        << std::endl
               << "Message : "     << exception.GetMsg()         << std::endl
               << "Description : " << exception.GetDescription() << std::endl;          
        QMessageBox::critical( 0, "Scipio - Adaptation Tool - Saving error", strMsg.str().c_str() );
        ADN_Project_Data::GetWorkDirInfos().SetWorkingDirectory(szOldWorkDir);

        pProgressIndicator_->Reset();
        return false;
    }
    
    /////////////////////////////////////
    // Copy Tmp Files To Real Files
    for( itFileName = files.begin(); itFileName != files.end(); ++itFileName )
    {
        if ( ! ADN_Tools::CopyFileToFile( ( ADN_Project_Data::GetWorkDirInfos().GetTempDirectory().GetData() + *itFileName ).c_str() ,
                                     ( ADN_Project_Data::GetWorkDirInfos().GetWorkingDirectory().GetData() + *itFileName ).c_str() ) 
           )
        {
            ADN_Project_Data::GetWorkDirInfos().SetWorkingDirectory(szOldWorkDir);
            pProgressIndicator_->Reset();
            throw ADN_SaveFile_Exception( *itFileName );
        }
    }
        

    //////////////////////////////////////////////
    // Dump the call stack to the changelog file

    MT_FileLogger logFile( "change_log.txt" );

    MT_LogManager::Instance().RegisterLogger( logFile );    

    MT_LOG_INFO_MSG( "Modifications faites le " << MT_GetCurrentDate() )
    char szUserName[256];
    DWORD nSize = 256;

    BOOL bRes = GetUserName( szUserName, &nSize );
    
    if( bRes )
        MT_LOG_INFO_MSG( "par " << std::string( szUserName, nSize ) )

    QStringList undoList = GetUndoStack().undoList();
    for( QStringList::iterator itCommand = undoList.begin(); itCommand != undoList.end(); ++itCommand )
    {
        MT_LOG_INFO_MSG( (*itCommand).ascii() )
    }

    MT_LogManager::Instance().UnregisterLogger( logFile );

    GetUndoStack().clear();

    // Save is ended
    pProgressIndicator_->Reset();
    return true;
}


//-----------------------------------------------------------------------------
// Name: ADN_Workspace::Save
// Created: JDY 03-07-04
//-----------------------------------------------------------------------------
bool ADN_Workspace::Save()
{
    return SaveAs(this->GetProject().GetData().GetFileInfos().GetFileNameFull() );
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
