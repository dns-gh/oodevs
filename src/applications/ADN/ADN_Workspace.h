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

class ADN_MainWindow;
class ADN_ProgressIndicator_ABC;

class QtUndoStack;
class QtCommand;

enum E_OpenMode;

class ADN_NBC_GUI;
class ADN_NBC_Datas;
class ADN_Launchers_GUI;
class ADN_Launchers_Data;
class ADN_Project_GUI;
class ADN_Project_Data;
class ADN_Categories_GUI;
class ADN_Categories_Data;
class ADN_Equipement_GUI;
class ADN_Equipement_Data;
class ADN_Objects_GUI;
class ADN_Objects_Data;
class ADN_Weapons_GUI;
class ADN_Weapons_Data;
class ADN_Sensors_GUI;
class ADN_Sensors_Data;
class ADN_Composantes_GUI;
class ADN_Composantes_Data;
class ADN_Units_GUI;
class ADN_Units_Data;
class ADN_Models_GUI;
class ADN_Models_Data;
class ADN_Automata_GUI;
class ADN_Automata_Data;
class ADN_AiEngine_GUI;
class ADN_AiEngine_Data;
class ADN_Breakdowns_Data;
class ADN_Breakdowns_GUI;
class ADN_Population_Data;
class ADN_Population_GUI;




// =============================================================================
/** @class  ADN_Workspace
    @brief  ADN_Workspace
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_Workspace : public QObject
{
    Q_OBJECT
    MT_COPYNOTALLOWED( ADN_Workspace )

public:
    static ADN_Workspace&  GetWorkspace();
    static void            CleanWorkspace();

public:
    void Build( ADN_MainWindow& mainWindow );
    void Reset( const std::string& filename, bool bVisible = true );
    void Load( const std::string& filename );
    bool Save();
    bool SaveAs( const std::string& filename );
    
    void ExportHtml( const std::string& strPath );

    ADN_WorkspaceElement< ADN_NBC_Datas, ADN_GUI_ABC >& GetNbc();
    ADN_WorkspaceElement< ADN_Launchers_Data, ADN_Launchers_GUI >& GetLaunchers();
    ADN_WorkspaceElement< ADN_Project_Data, ADN_Project_GUI>& GetProject();
    ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >& GetCategories();
    ADN_WorkspaceElement< ADN_Equipement_Data, ADN_Equipement_GUI >& GetEquipements();
    ADN_WorkspaceElement< ADN_Objects_Data, ADN_Objects_GUI>& GetObjects();
    ADN_WorkspaceElement< ADN_Weapons_Data, ADN_Weapons_GUI>& GetWeapons();
    ADN_WorkspaceElement< ADN_Sensors_Data, ADN_Sensors_GUI>& GetSensors();
    ADN_WorkspaceElement< ADN_Composantes_Data, ADN_Composantes_GUI >& GetComposantes();
    ADN_WorkspaceElement< ADN_Units_Data, ADN_Units_GUI >& GetUnits();
    ADN_WorkspaceElement< ADN_Models_Data, ADN_GUI_ABC >& GetModels();
    ADN_WorkspaceElement< ADN_Automata_Data, ADN_Automata_GUI >& GetAutomata();
    ADN_WorkspaceElement< ADN_AiEngine_Data, ADN_AiEngine_GUI>& GetAiEngine();
    ADN_WorkspaceElement< ADN_Breakdowns_Data, ADN_Breakdowns_GUI>& GetBreakdowns();
    ADN_WorkspaceElement< ADN_Population_Data, ADN_Population_GUI >& GetPopulation();

    void SetProgressIndicator( ADN_ProgressIndicator_ABC* pProgressIndicator );
    void ResetProgressIndicator();

    QtUndoStack&                GetUndoStack();

    void AddCommand( QtCommand* pNewCommand );
    
    E_OpenMode GetOpenMode() const;
    void SetOpenMode( E_OpenMode nNewMode );

private:
    explicit ADN_Workspace();
    virtual ~ADN_Workspace();

private: 
    enum E_WorkspaceElements
    {
        eProject,
        eCategories,
        eNBC,    
        eLaunchers,
        eEquipement,
        eObjects,
        eWeapons,
        eSensors,
        eBreakdowns,
        eComposantes,
        eAiEngine,
        eModels,
        eUnits,
        eAutomata,
        eCommunications,
        eHumanFactors,
        eLog,
        eMissions,
        eKnowledgeGroups,
        eHLA,
        eSupply,
        eHealth,
        ePopulation,
        eNbrWorkspaceElements
    };

private:
    ADN_WorkspaceElement_ABC* elements_[eNbrWorkspaceElements];

    ADN_ProgressIndicator_ABC* pProgressIndicator_;

    QtUndoStack*            pUndoStack_;

    E_OpenMode              nOpenMode_;

    static ADN_Workspace*   pWorkspace_;
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
ADN_WorkspaceElement< ADN_Project_Data, ADN_Project_GUI>& ADN_Workspace::GetProject()
{
    return (ADN_WorkspaceElement< ADN_Project_Data, ADN_Project_GUI>&)(*elements_[eProject]);
}


// -----------------------------------------------------------------------------
// Name: ADN_Workspace::GetCategories
// Created: APE 2004-12-07
// -----------------------------------------------------------------------------
inline
ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >& ADN_Workspace::GetCategories()
{
    return (ADN_WorkspaceElement< ADN_Categories_Data, ADN_Categories_GUI >&)(*elements_[eCategories]);
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
ADN_WorkspaceElement< ADN_Models_Data, ADN_GUI_ABC >& ADN_Workspace::GetModels()
{
    return (ADN_WorkspaceElement< ADN_Models_Data, ADN_GUI_ABC >&)(*elements_[eModels]);
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