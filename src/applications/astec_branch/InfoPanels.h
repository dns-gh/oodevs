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
// $Archive: /MVW_v10/Build/SDK/Light2/src/InfoPanels.h $
// $Author: Age $
// $Modtime: 21/04/05 16:03 $
// $Revision: 8 $
// $Workfile: InfoPanels.h $
//
// *****************************************************************************

#ifndef __InfoPanels_h_
#define __InfoPanels_h_

class AgentStatePanel;
class AgentResourcesPanel;
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
class Controllers;

// =============================================================================
// Created: APE 2004-03-10
// =============================================================================
class InfoPanels : public QWidgetStack
{

public:
    //! @name Constructors/Destructor
    //@{
             InfoPanels( QWidget* pParent, Controllers& controllers );
    virtual ~InfoPanels();
    //@}

    //! @name Operations
    //@{
    void Add   ( QWidget* widget, const QString& name );
    void Remove( QWidget* widget );
    //@}
    
    
private:
    //! @name Copy / Assignment
    //@{
    InfoPanels( const InfoPanels& );
    InfoPanels& operator=( const InfoPanels& );
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
    PopulationKnowledgePanel* pPopulationKnowledgePanel_;
};

#endif // __InfoPanels_h_
