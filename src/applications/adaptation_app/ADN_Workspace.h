//*****************************************************************************
//
// $Created: JDY 03-06-30 $
// $Archive: /MVW_v10/Build/SDK/ADN2/src/ADN_Workspace.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:40 $
// $Revision: 11 $
// $Workfile: ADN_Workspace.h $
//
//*****************************************************************************

#ifndef __ADN_Workspace_h_
#define __ADN_Workspace_h_

#include "ADN_Enums.h"
#include "ADN_NavigationInfos.h"
#include "ADN_WorkspaceElement.h"

class ADN_ActiveProtections_Data;
class ADN_ActiveProtections_GUI;
class ADN_AiEngine_Data;
class ADN_AiEngine_GUI;
class ADN_Automata_Data;
class ADN_Automata_GUI;
class ADN_Breakdowns_Data;
class ADN_Breakdowns_GUI;
class ADN_Categories_Data;
class ADN_Categories_GUI;
class ADN_Crowds_Data;
class ADN_Crowds_GUI;
class ADN_Drawings_Data;
class ADN_Drawings_GUI;
class ADN_Equipments_Data;
class ADN_Equipments_GUI;
class ADN_FireClass_Data;
class ADN_FireClass_GUI;
class ADN_GeneralConfig;
class ADN_HumanFactors_Data;
class ADN_HumanFactors_GUI;
class ADN_Inhabitants_Data;
class ADN_Inhabitants_GUI;
class ADN_KnowledgeGroups_Data;
class ADN_KnowledgeGroups_GUI;
class ADN_Languages_Data;
class ADN_Languages_GUI;
class ADN_Launchers_Data;
class ADN_Launchers_GUI;
class ADN_Logistic_Data;
class ADN_Logistic_GUI;
class ADN_MainWindow;
class ADN_Missions_Data;
class ADN_Missions_GUI;
class ADN_Models_Data;
class ADN_Models_GUI;
class ADN_NBC_Datas;
class ADN_NBC_GUI;
class ADN_Objects_Data;
class ADN_Objects_GUI;
class ADN_ProgressIndicator_ABC;
class ADN_Project_Data;
class ADN_Reports_Data;
class ADN_Reports_GUI;
class ADN_ResourceNetworks_Data;
class ADN_ResourceNetworks_GUI;
class ADN_Resources_Data;
class ADN_Resources_GUI;
class ADN_Sensors_Data;
class ADN_Sensors_GUI;
class ADN_Symbols_Data;
class ADN_Symbols_GUI;
class ADN_Units_Data;
class ADN_Units_GUI;
class ADN_UnitSymbols_Data;
class ADN_UnitSymbols_GUI;
class ADN_Urban_Data;
class ADN_Urban_GUI;
class ADN_Weapons_Data;
class ADN_Weapons_GUI;
class ADN_Disasters_Data;
class ADN_Disasters_GUI;
enum E_OpenMode;

namespace tools
{
    class Loader_ABC;
}

// =============================================================================
/** @class  ADN_Workspace
    @brief  ADN_Workspace
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_Workspace : public QObject
{
    Q_OBJECT

public:
    static void CreateWorkspace( const ADN_GeneralConfig& config );
    static ADN_Workspace& GetWorkspace();
    static void CleanWorkspace();

public:
    typedef std::map< E_WorkspaceElements, QStringList >  T_UsingElements;

public:
    void Build( ADN_MainWindow& mainWindow, bool devMode );
    void Reset( const tools::Path& filename, bool bVisible = true );
    void Load( const tools::Path& filename, const tools::Loader_ABC& fileLoader );
    bool Save( const tools::Loader_ABC& fileLoader );
    bool SaveAs( const tools::Path& filename, const tools::Loader_ABC& fileLoader );
    void ExportHtml( const tools::Path& strPath );

    ADN_Project_Data& GetProject();
    ADN_WorkspaceElement_ABC& GetWorkspaceElement( E_WorkspaceElements workspaceElement );

    ADN_WorkspaceElement< ADN_NBC_Datas, ADN_GUI_ABC >& GetNbc();
    ADN_WorkspaceElement< ADN_Launchers_Data, ADN_Launchers_GUI >& GetLaunchers();
    ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >& GetCategories();
    ADN_WorkspaceElement< ADN_Urban_Data, ADN_Urban_GUI >& GetUrban();
    ADN_WorkspaceElement< ADN_ActiveProtections_Data, ADN_ActiveProtections_GUI>& GetActiveProtections();
    ADN_WorkspaceElement< ADN_Resources_Data, ADN_Resources_GUI >& GetResources();
    ADN_WorkspaceElement< ADN_Objects_Data, ADN_Objects_GUI>& GetObjects();
    ADN_WorkspaceElement< ADN_Weapons_Data, ADN_Weapons_GUI>& GetWeapons();
    ADN_WorkspaceElement< ADN_Sensors_Data, ADN_Sensors_GUI>& GetSensors();
    ADN_WorkspaceElement< ADN_Equipments_Data, ADN_Equipments_GUI >& GetEquipments();
    ADN_WorkspaceElement< ADN_Units_Data, ADN_Units_GUI >& GetUnits();
    ADN_WorkspaceElement< ADN_Models_Data, ADN_Models_GUI >& GetModels();
    ADN_WorkspaceElement< ADN_Missions_Data, ADN_Missions_GUI >& GetMissions();
    ADN_WorkspaceElement< ADN_Automata_Data, ADN_Automata_GUI >& GetAutomata();
    ADN_WorkspaceElement< ADN_AiEngine_Data, ADN_AiEngine_GUI>& GetAiEngine();
    ADN_WorkspaceElement< ADN_Breakdowns_Data, ADN_Breakdowns_GUI>& GetBreakdowns();
    ADN_WorkspaceElement< ADN_Crowds_Data, ADN_Crowds_GUI >& GetCrowds();
    ADN_WorkspaceElement< ADN_Inhabitants_Data, ADN_Inhabitants_GUI >& GetInhabitants();
    ADN_WorkspaceElement< ADN_Reports_Data, ADN_Reports_GUI >& GetReports();
    ADN_WorkspaceElement< ADN_HumanFactors_Data, ADN_HumanFactors_GUI >& GetHumanFactors();
    ADN_WorkspaceElement< ADN_KnowledgeGroups_Data, ADN_KnowledgeGroups_GUI >& GetKnowledgeGroups();
    ADN_WorkspaceElement< ADN_ResourceNetworks_Data, ADN_ResourceNetworks_GUI >& GetResourceNetworks();
    ADN_WorkspaceElement< ADN_FireClass_Data, ADN_FireClass_GUI >& GetFireClasses();
    ADN_WorkspaceElement< ADN_Drawings_Data, ADN_Drawings_GUI >& GetDrawings() const;
    ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI >& GetSymbols() const;
    ADN_WorkspaceElement< ADN_Logistic_Data, ADN_Logistic_GUI >& GetLogistic();
    ADN_WorkspaceElement< ADN_Disasters_Data, ADN_Disasters_GUI >& GetDisasters();
    ADN_WorkspaceElement< ADN_Languages_Data, ADN_Languages_GUI >& GetLanguages();

    T_UsingElements GetElementThatWillBeDeleted( ADN_Ref_ABC* data );
    T_UsingElements GetElementThatUse( ADN_Ref_ABC* data );

    void SetProgressIndicator( ADN_ProgressIndicator_ABC* pProgressIndicator );
    void ResetProgressIndicator();

    E_OpenMode GetOpenMode() const;
    void SetOpenMode( E_OpenMode nNewMode );
    bool IsDevMode() const;

signals:
    void ChangeTab( E_WorkspaceElements targetTab );

public slots:
    void OnUsersListRequested( const ADN_NavigationInfos::UsedBy& usedByInfo );
    void OnGoToRequested( const ADN_NavigationInfos::GoTo& goToInfo );
    void OnChooseOptional( bool choice );
    void OnLanguageChanged( const std::string& language );

private:
    void Initialize();
    void InitializeEnumType();
    void AddPage( ADN_MainWindow& mainWindow, E_WorkspaceElements element );

private:
    explicit ADN_Workspace( const ADN_GeneralConfig& config );
    virtual ~ADN_Workspace();

private:
    const ADN_GeneralConfig& config_;
    ADN_Project_Data* projectData_;
    ADN_WorkspaceElement_ABC* elements_[ eNbrWorkspaceElements ];
    ADN_ProgressIndicator_ABC* pProgressIndicator_;
    E_OpenMode nOpenMode_;
    bool symbols_;
    bool devMode_;
    static ADN_Workspace* pWorkspace_;
};

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetWorkspaceElement
// Created: ABR 2012-11-15
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement_ABC& ADN_Workspace::GetWorkspaceElement( E_WorkspaceElements workspaceElement )
{
    assert( elements_[ workspaceElement ] != 0 );
    return *elements_[ workspaceElement ];
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetNbc
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_NBC_Datas, ADN_GUI_ABC >& ADN_Workspace::GetNbc()
{
    return (ADN_WorkspaceElement< ADN_NBC_Datas, ADN_GUI_ABC >&)(*elements_[eNBC]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetLaunchers
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Launchers_Data, ADN_Launchers_GUI >& ADN_Workspace::GetLaunchers()
{
    return (ADN_WorkspaceElement< ADN_Launchers_Data, ADN_Launchers_GUI >&)(*elements_[eLaunchers]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetProject
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_Project_Data& ADN_Workspace::GetProject()
{
    if( !projectData_ )
        throw MASA_EXCEPTION( "Project data not initialized" );
    return *projectData_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetCategories
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >& ADN_Workspace::GetCategories()
{
    return ( ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >& )( *elements_[ eCategories ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetCategories
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Urban_Data, ADN_Urban_GUI >& ADN_Workspace::GetUrban()
{
    return (ADN_WorkspaceElement< ADN_Urban_Data, ADN_Urban_GUI >&)(*elements_[eUrban]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetResources
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Resources_Data, ADN_Resources_GUI >& ADN_Workspace::GetResources()
{
    return (ADN_WorkspaceElement< ADN_Resources_Data, ADN_Resources_GUI >&)(*elements_[eResources]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetObjects
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Objects_Data, ADN_Objects_GUI>& ADN_Workspace::GetObjects()
{
    return (ADN_WorkspaceElement< ADN_Objects_Data, ADN_Objects_GUI>&)(*elements_[eObjects]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetWeapons
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Weapons_Data, ADN_Weapons_GUI>& ADN_Workspace::GetWeapons()
{
    return (ADN_WorkspaceElement< ADN_Weapons_Data, ADN_Weapons_GUI>&)(*elements_[eWeapons]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetActiveProtections
// Created: FDS 2010-02-24
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_ActiveProtections_Data, ADN_ActiveProtections_GUI>& ADN_Workspace::GetActiveProtections()
{
    return (ADN_WorkspaceElement< ADN_ActiveProtections_Data, ADN_ActiveProtections_GUI>&)(*elements_[eActiveProtections]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetSensors
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Sensors_Data, ADN_Sensors_GUI>& ADN_Workspace::GetSensors()
{
    return (ADN_WorkspaceElement< ADN_Sensors_Data, ADN_Sensors_GUI>&)(*elements_[eSensors]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetComposantes
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Equipments_Data, ADN_Equipments_GUI >& ADN_Workspace::GetEquipments()
{
    return (ADN_WorkspaceElement< ADN_Equipments_Data, ADN_Equipments_GUI >&)(*elements_[eEquipments]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetUnits
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Units_Data, ADN_Units_GUI >& ADN_Workspace::GetUnits()
{
    return (ADN_WorkspaceElement< ADN_Units_Data, ADN_Units_GUI >&)(*elements_[eUnits]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetModels
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Models_Data, ADN_Models_GUI >& ADN_Workspace::GetModels()
{
    return (ADN_WorkspaceElement< ADN_Models_Data, ADN_Models_GUI >&)(*elements_[eModels]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetMissions
// Created: SBO 2006-12-04
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Missions_Data, ADN_Missions_GUI >& ADN_Workspace::GetMissions()
{
    return (ADN_WorkspaceElement< ADN_Missions_Data, ADN_Missions_GUI >&)(*elements_[eMissions]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetAutomata
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Automata_Data, ADN_Automata_GUI >& ADN_Workspace::GetAutomata()
{
    return (ADN_WorkspaceElement< ADN_Automata_Data, ADN_Automata_GUI >&)(*elements_[eAutomata]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetAiEngine
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_AiEngine_Data, ADN_AiEngine_GUI>& ADN_Workspace::GetAiEngine()
{
    return (ADN_WorkspaceElement< ADN_AiEngine_Data, ADN_AiEngine_GUI>&)(*elements_[eAiEngine]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetBreakdowns
// Created: APE 2005-03-17
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Breakdowns_Data, ADN_Breakdowns_GUI>& ADN_Workspace::GetBreakdowns()
{
    return (ADN_WorkspaceElement< ADN_Breakdowns_Data, ADN_Breakdowns_GUI>&)(*elements_[eBreakdowns]);
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetCrowds
// Created: SBO 2005-10-04
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Crowds_Data, ADN_Crowds_GUI >& ADN_Workspace::GetCrowds()
{
    return ( ADN_WorkspaceElement< ADN_Crowds_Data, ADN_Crowds_GUI >& )( *elements_[ eCrowds ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetInhabitants
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Inhabitants_Data, ADN_Inhabitants_GUI >& ADN_Workspace::GetInhabitants()
{
    return ( ADN_WorkspaceElement< ADN_Inhabitants_Data, ADN_Inhabitants_GUI >& )( *elements_[ eInhabitants ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetReports
// Created: SBO 2006-12-14
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Reports_Data, ADN_Reports_GUI >& ADN_Workspace::GetReports()
{
    return ( ADN_WorkspaceElement< ADN_Reports_Data, ADN_Reports_GUI >& )( *elements_[ eReports ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetHumanFactors
// Created: JSR 2010-04-28
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_HumanFactors_Data, ADN_HumanFactors_GUI >& ADN_Workspace::GetHumanFactors()
{
    return ( ADN_WorkspaceElement< ADN_HumanFactors_Data, ADN_HumanFactors_GUI >& )( *elements_[ eHumanFactors ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_WorkspaceElement< ADN_Logistic_Data, ADN_Logistic_GUI >& ADN_Workspace::GetLogistic
// Created: ABR 2012-01-18
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Logistic_Data, ADN_Logistic_GUI >& ADN_Workspace::GetLogistic()
{
    return ( ADN_WorkspaceElement< ADN_Logistic_Data, ADN_Logistic_GUI >& )( *elements_[ eLogistic ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetKnowledgeGroups
// Created: LGY 2010-08-18
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_KnowledgeGroups_Data, ADN_KnowledgeGroups_GUI >& ADN_Workspace::GetKnowledgeGroups()
{
    return ( ADN_WorkspaceElement< ADN_KnowledgeGroups_Data, ADN_KnowledgeGroups_GUI >& )( *elements_[ eKnowledgeGroups ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetResourceNetworks
// Created: JSR 2010-09-13
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_ResourceNetworks_Data, ADN_ResourceNetworks_GUI >& ADN_Workspace::GetResourceNetworks()
{
    return ( ADN_WorkspaceElement< ADN_ResourceNetworks_Data, ADN_ResourceNetworks_GUI >& )( *elements_[ eResourceNetworks ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_WorkspaceElement< ADN_FireClass_Data, ADN_FireClass_GUI >& ADN_Workspace::GetFireClasses
// Created: JSR 2010-12-02
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_FireClass_Data, ADN_FireClass_GUI >& ADN_Workspace::GetFireClasses()
{
    return static_cast< ADN_WorkspaceElement< ADN_FireClass_Data, ADN_FireClass_GUI >& >( *elements_[ eFireClasses ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_WorkspaceElement< ADN_Drawings_Data, ADN_Drawings_GUI >& ADN_Workspace::GetDrawings
// Created: ABR 2011-04-18
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Drawings_Data, ADN_Drawings_GUI >& ADN_Workspace::GetDrawings() const
{
    return static_cast< ADN_WorkspaceElement< ADN_Drawings_Data, ADN_Drawings_GUI >& >( *elements_[ eDrawings ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI >& ADN_Workspace::GetSymbols
// Created: ABR 2011-04-18
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI >& ADN_Workspace::GetSymbols() const
{
    return static_cast< ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI >& >( *elements_[ eSymbols ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_WorkspaceElement< ADN_Disasters_Data, ADN_Disasters_GUI >& ADN_Workspace::GetDisasters
// Created: LGY 2012-11-15
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Disasters_Data, ADN_Disasters_GUI >& ADN_Workspace::GetDisasters()
{
    return static_cast< ADN_WorkspaceElement< ADN_Disasters_Data, ADN_Disasters_GUI >& >( *elements_[ eDisasters ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_WorkspaceElement< ADN_Languages_Data, ADN_Languages_GUI >& ADN_Workspace::GetLanguages
// Created: ABR 2013-07-08
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Languages_Data, ADN_Languages_GUI >& ADN_Workspace::GetLanguages()
{
    return static_cast< ADN_WorkspaceElement< ADN_Languages_Data, ADN_Languages_GUI >& >( *elements_[ eLanguages ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::SetProgressIndicator
// Created: APE 2005-03-18
// -----------------------------------------------------------------------------
inline
void ADN_Workspace::SetProgressIndicator( ADN_ProgressIndicator_ABC* pProgressIndicator )
{
    pProgressIndicator_ = pProgressIndicator;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetOpenMode
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
inline
E_OpenMode ADN_Workspace::GetOpenMode() const
{
    return nOpenMode_;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::SetOpenMode
// Created: AGN 2004-05-25
// -----------------------------------------------------------------------------
inline
void ADN_Workspace::SetOpenMode( E_OpenMode nNewMode )
{
    nOpenMode_ = nNewMode;
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::IsDevMode
// Created: LGY 2013-05-16
// -----------------------------------------------------------------------------
inline
bool ADN_Workspace::IsDevMode() const
{
    return devMode_;
}

#endif // __ADN_Workspace_h_
