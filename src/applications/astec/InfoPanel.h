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
// $Archive: /MVW_v10/Build/SDK/Light2/src/InfoPanel.h $
// $Author: Age $
// $Modtime: 21/04/05 16:03 $
// $Revision: 8 $
// $Workfile: InfoPanel.h $
//
// *****************************************************************************

#ifndef __InfoPanel_h_
#define __InfoPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

class AgentStatePanel;
class AgentResourcesPanel;
class AgentCargoPanel;
class AgentKnowledgePanel;
class Agent;
class SelectedElement;
class AgentMaintenancePanel;
class AgentMedicalPanel;
class AgentSupplyPanel;
class ReportPanel;
class ObjectPanel;
class ObjectReportPanel;
class ObjectKnowledgePanel;
class PopulationPanel;
class PopulationKnowledgePanel;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class InfoPanel : public QWidgetStack
{
    Q_OBJECT
    MT_COPYNOTALLOWED( InfoPanel );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     explicit InfoPanel( QWidget* pParent );
    ~InfoPanel();
    //@}

    //! @name Accessors
    //@{
    AgentKnowledgePanel&		GetAgentKnowledgePanel();
    ObjectKnowledgePanel&		GetObjectKnowledgePanel();
    //@}

    QSize sizeHint() const;

public slots:
    void SetSelectedElement( SelectedElement& selectedElement );
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
    AgentStatePanel*          pStatePanel_;
    ReportPanel*              pReportPanel_;
    AgentResourcesPanel*      pResourcesPanel_;
    AgentKnowledgePanel*      pAgentKnowledgePanel_;
    AgentMaintenancePanel*    pAgentMaintenancePanel_;
    AgentMedicalPanel*        pAgentMedicalPanel_;
    AgentSupplyPanel*         pAgentSupplyPanel_;

    ObjectPanel*              pObjectPanel_;
    ObjectReportPanel*        pObjectReportPanel_;
    ObjectKnowledgePanel*     pObjectKnowledgePanel_;

	PopulationPanel*          pPopulationPanel_;
    ReportPanel*              pPopulationReportPanel_;
    PopulationKnowledgePanel* pPopulationKnowledgePanel_;

    bool bAgentVisible_;
    bool bAgentKnowledgeVisible_;
    bool bObjectVisible_;
    bool bObjectKnowledgeVisible_;
    bool bLogisiticVisible_;
	bool bPopulationVisible_;
    bool bPopulationKnowledgeVisible_;

    QTabWidget* pTabWidget_;
};

#   include "InfoPanel.inl"

#endif // __InfoPanel_h_
