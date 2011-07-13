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

#include "ADN_WorkspaceElement.h"
#include <boost/noncopyable.hpp>

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
class ADN_Composantes_Data;
class ADN_Composantes_GUI;
class ADN_Drawings_Data;
class ADN_Drawings_GUI;
class ADN_Equipement_Data;
class ADN_Equipement_GUI;
class ADN_FireClass_Data;
class ADN_FireClass_GUI;
class ADN_Health_Data;
class ADN_Health_GUI;
class ADN_HumanFactors_Data;
class ADN_HumanFactors_GUI;
class ADN_KnowledgeGroups_Data;
class ADN_KnowledgeGroups_GUI;
class ADN_Launchers_Data;
class ADN_Launchers_GUI;
class ADN_MainWindow;
class ADN_Missions_Data;
class ADN_Missions_GUI;
class ADN_Models_Data;
class ADN_Models_GUI;
class ADN_NBC_Datas;
class ADN_NBC_GUI;
class ADN_Objects_Data;
class ADN_Objects_GUI;
class ADN_People_Data;
class ADN_People_GUI;
class ADN_Population_Data;
class ADN_Population_GUI;
class ADN_ProgressIndicator_ABC;
class ADN_Project_Data;
class ADN_Reports_Data;
class ADN_Reports_GUI;
class ADN_ResourceNetworks_Data;
class ADN_ResourceNetworks_GUI;
class ADN_Sensors_Data;
class ADN_Sensors_GUI;
class ADN_Supply_Data;
class ADN_Supply_GUI;
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
class QtCommand;
class QtUndoStack;
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
                    , private boost::noncopyable
{
    Q_OBJECT

public:
    static ADN_Workspace& GetWorkspace();
    static void CleanWorkspace();

public:
    void Build( ADN_MainWindow& mainWindow );
    void Reset( const std::string& filename, bool bVisible = true );
    void Load( const std::string& filename, const tools::Loader_ABC& fileLoader );
    bool Save();
    bool SaveAs( const std::string& filename );
    void ExportHtml( const std::string& strPath );
    bool IsValidDatabase();

    ADN_Project_Data& GetProject();
    ADN_WorkspaceElement< ADN_NBC_Datas, ADN_GUI_ABC >& GetNbc();
    ADN_WorkspaceElement< ADN_Launchers_Data, ADN_Launchers_GUI >& GetLaunchers();
    ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >& GetCategories();
    ADN_WorkspaceElement< ADN_Urban_Data, ADN_Urban_GUI >& GetUrban();
    ADN_WorkspaceElement< ADN_ActiveProtections_Data, ADN_ActiveProtections_GUI>& GetActiveProtections();
    ADN_WorkspaceElement< ADN_Equipement_Data, ADN_Equipement_GUI >& GetEquipements();
    ADN_WorkspaceElement< ADN_Objects_Data, ADN_Objects_GUI>& GetObjects();
    ADN_WorkspaceElement< ADN_Weapons_Data, ADN_Weapons_GUI>& GetWeapons();
    ADN_WorkspaceElement< ADN_Sensors_Data, ADN_Sensors_GUI>& GetSensors();
    ADN_WorkspaceElement< ADN_Composantes_Data, ADN_Composantes_GUI >& GetComposantes();
    ADN_WorkspaceElement< ADN_Units_Data, ADN_Units_GUI >& GetUnits();
    ADN_WorkspaceElement< ADN_Models_Data, ADN_Models_GUI >& GetModels();
    ADN_WorkspaceElement< ADN_Missions_Data, ADN_Missions_GUI >& GetMissions();
    ADN_WorkspaceElement< ADN_Automata_Data, ADN_Automata_GUI >& GetAutomata();
    ADN_WorkspaceElement< ADN_AiEngine_Data, ADN_AiEngine_GUI>& GetAiEngine();
    ADN_WorkspaceElement< ADN_Breakdowns_Data, ADN_Breakdowns_GUI>& GetBreakdowns();
    ADN_WorkspaceElement< ADN_Population_Data, ADN_Population_GUI >& GetPopulation();
    ADN_WorkspaceElement< ADN_People_Data, ADN_People_GUI >& GetPeople();
    ADN_WorkspaceElement< ADN_Reports_Data, ADN_Reports_GUI >& GetReports();
    ADN_WorkspaceElement< ADN_HumanFactors_Data, ADN_HumanFactors_GUI >& GetHumanFactors();
    ADN_WorkspaceElement< ADN_Health_Data, ADN_Health_GUI >& GetHealth();
    ADN_WorkspaceElement< ADN_Supply_Data, ADN_Supply_GUI>& GetSupply();
    ADN_WorkspaceElement< ADN_KnowledgeGroups_Data, ADN_KnowledgeGroups_GUI >& GetKnowledgeGroups();
    ADN_WorkspaceElement< ADN_ResourceNetworks_Data, ADN_ResourceNetworks_GUI >& GetResourceNetworks();
    ADN_WorkspaceElement< ADN_FireClass_Data, ADN_FireClass_GUI >& GetFireClasses();
    ADN_WorkspaceElement< ADN_Drawings_Data, ADN_Drawings_GUI >& GetDrawings() const;
    ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI >& GetSymbols() const;
    ADN_WorkspaceElement< ADN_UnitSymbols_Data, ADN_UnitSymbols_GUI >& GetUnitSymbols() const;

    void SetProgressIndicator( ADN_ProgressIndicator_ABC* pProgressIndicator );
    void ResetProgressIndicator();

    QtUndoStack& GetUndoStack();

    void AddCommand( QtCommand* pNewCommand );

    E_OpenMode GetOpenMode() const;
    void SetOpenMode( E_OpenMode nNewMode );

private:
    explicit ADN_Workspace();
    virtual ~ADN_Workspace();

private:
    enum E_WorkspaceElements
    {
        eCategories,
        eSymbols, // Must be before eUnits && before eUrban
        eUnitSymbols,
        eUrban,
        eNBC,
        eLaunchers,
        eEquipement,
        eFireClasses,
        eDrawings, // Must be before eObjects
        eObjects,
        eWeapons,
        eActiveProtections,
        eSensors,
        eBreakdowns,
        eComposantes,
        eResourceNetworks,
        eAiEngine,
        eMissions,
        eModels,
        eUnits,
        eAutomata,
        eCommunications,
        eHumanFactors,
        eMaintenance,
        eKnowledgeGroups,
        eSupply,
        eHealth,
        ePopulation,
        ePeople,
        eReports,
        eNbrWorkspaceElements
    };

private:
    ADN_Project_Data* projectData_;
    ADN_WorkspaceElement_ABC* elements_[eNbrWorkspaceElements];
    ADN_ProgressIndicator_ABC* pProgressIndicator_;
    QtUndoStack* pUndoStack_;
    E_OpenMode nOpenMode_;
    static ADN_Workspace* pWorkspace_;
};


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
        throw std::runtime_error( "Project data not initialized" );
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
// Name: ADN_Workspace::GetEquipements
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Equipement_Data, ADN_Equipement_GUI >& ADN_Workspace::GetEquipements()
{
    return (ADN_WorkspaceElement< ADN_Equipement_Data, ADN_Equipement_GUI >&)(*elements_[eEquipement]);
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
ADN_WorkspaceElement< ADN_Composantes_Data, ADN_Composantes_GUI >& ADN_Workspace::GetComposantes()
{
    return (ADN_WorkspaceElement< ADN_Composantes_Data, ADN_Composantes_GUI >&)(*elements_[eComposantes]);
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
// Name: ADN_Workspace::GetPopulation
// Created: SBO 2005-10-04
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Population_Data, ADN_Population_GUI >& ADN_Workspace::GetPopulation()
{
    return ( ADN_WorkspaceElement< ADN_Population_Data, ADN_Population_GUI >& )( *elements_[ ePopulation ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetPeople
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_People_Data, ADN_People_GUI >& ADN_Workspace::GetPeople()
{
    return ( ADN_WorkspaceElement< ADN_People_Data, ADN_People_GUI >& )( *elements_[ ePeople ] );
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
// Name: ADN_Workspace::GetHealth
// Created: JSR 2010-05-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Health_Data, ADN_Health_GUI >& ADN_Workspace::GetHealth()
{
    return ( ADN_WorkspaceElement< ADN_Health_Data, ADN_Health_GUI >& )( *elements_[ eHealth ] );
}

// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetSupply
// Created: JSR 2010-05-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Supply_Data, ADN_Supply_GUI>& ADN_Workspace::GetSupply()
{
    return ( ADN_WorkspaceElement< ADN_Supply_Data, ADN_Supply_GUI >& )( *elements_[ eSupply ] );
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
// Name: ADN_WorkspaceElement< ADN_Symbols_Data, ADN_Symbols_GUI >& ADN_Workspace::GetUnitSymbols
// Created: MMC 2011-07-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_UnitSymbols_Data, ADN_UnitSymbols_GUI >& ADN_Workspace::GetUnitSymbols() const
{
    return static_cast< ADN_WorkspaceElement< ADN_UnitSymbols_Data, ADN_UnitSymbols_GUI >& >( *elements_[ eUnitSymbols ] );
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

#endif // __ADN_Workspace_h_
