// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_InfoPanel.h $
// $Author: Age $
// $Modtime: 21/04/05 16:03 $
// $Revision: 8 $
// $Workfile: MOS_InfoPanel.h $
//
// *****************************************************************************

#ifndef __MOS_InfoPanel_h_
#define __MOS_InfoPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_AgentStatePanel;
class MOS_AgentResourcesPanel;
class MOS_AgentCargoPanel;
class MOS_AgentKnowledgePanel;
class MOS_Agent;
class MOS_SelectedElement;
class MOS_AgentMaintenancePanel;
class MOS_AgentMedicalPanel;
class MOS_AgentSupplyPanel;
class MOS_ReportPanel;
class MOS_ObjectPanel;
class MOS_ObjectReportPanel;
class MOS_ObjectKnowledgePanel;
class MOS_PopulationPanel;
class MOS_PopulationKnowledgePanel;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class MOS_InfoPanel : public QWidgetStack
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_InfoPanel );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     explicit MOS_InfoPanel( QWidget* pParent );
    ~MOS_InfoPanel();
    //@}

    //! @name Accessors
    //@{
    MOS_AgentKnowledgePanel&		GetAgentKnowledgePanel();
    MOS_ObjectKnowledgePanel&		GetObjectKnowledgePanel();
    //@}

    QSize sizeHint() const;

public slots:
    void SetSelectedElement( MOS_SelectedElement& selectedElement );
    void ClearAll();

private:
    void ShowAgentPanel              ( bool bShow );
    void ShowAgentKnowledgePanel     ( bool bShow );
    void ShowObjectKnowledgePanel    ( bool bShow );
    void ShowObjectPanel             ( bool bShow );
    void ShowLogisticPanels          ( bool bShow );
	void ShowPopulationPanel		 ( bool bShow );
    void ShowPopulationKnowledgePanel( bool bShow );

private:
    MOS_AgentStatePanel*          pStatePanel_;
    MOS_ReportPanel*              pReportPanel_;
    MOS_AgentResourcesPanel*      pResourcesPanel_;
    MOS_AgentKnowledgePanel*      pAgentKnowledgePanel_;
    MOS_AgentMaintenancePanel*    pAgentMaintenancePanel_;
    MOS_AgentMedicalPanel*        pAgentMedicalPanel_;
    MOS_AgentSupplyPanel*         pAgentSupplyPanel_;

    MOS_ObjectPanel*              pObjectPanel_;
    MOS_ObjectReportPanel*        pObjectReportPanel_;
    MOS_ObjectKnowledgePanel*     pObjectKnowledgePanel_;

	MOS_PopulationPanel*          pPopulationPanel_;
    MOS_ReportPanel*              pPopulationReportPanel_;
    MOS_PopulationKnowledgePanel* pPopulationKnowledgePanel_;

    bool bAgentVisible_;
    bool bAgentKnowledgeVisible_;
    bool bObjectVisible_;
    bool bObjectKnowledgeVisible_;
    bool bLogisiticVisible_;
	bool bPopulationVisible_;
    bool bPopulationKnowledgeVisible_;

    QTabWidget* pTabWidget_;
};

#   include "MOS_InfoPanel.inl"

#endif // __MOS_InfoPanel_h_
