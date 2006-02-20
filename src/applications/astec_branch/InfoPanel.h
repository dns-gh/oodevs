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

class AgentStatePanel;
class AgentResourcesPanel;
class AgentCargoPanel;
class AgentKnowledgePanel;
class AgentMaintenancePanel;
class AgentMedicalPanel;
class AgentSupplyPanel;
class ReportPanel;
class ObjectPanel;
class ObjectReportPanel;
class ObjectKnowledgePanel;
class PopulationPanel;
class PopulationKnowledgePanel;

class Controller;
class ActionController;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class InfoPanel : public QWidgetStack // $$$$ AGE 2006-02-17: renommer en InfoPanels
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoPanel( QWidget* pParent, Controller& controller, ActionController& actionController );
    virtual ~InfoPanel();
    //@}

    //! @name Operations
    //@{
    void Add   ( QWidget* widget, const QString& name );
    void Remove( QWidget* widget );
    //@}
    
    
private:
    //! @name Copy / Assignment
    //@{
    InfoPanel( const InfoPanel& );
    InfoPanel& operator=( const InfoPanel& );
    //@}

    //! @name Helpers
    //@{
    QSize sizeHint() const;
    //@}

private:
    QTabWidget*               pTabWidget_;
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

//    QTabWidget* pTabWidget_;
};

#endif // __InfoPanel_h_
