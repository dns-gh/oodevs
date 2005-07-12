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
class MOS_DynaObjectPanel;
class MOS_DynaObjectKnowledgePanel;
class MOS_Agent;
class MOS_SelectedElement;
class MOS_AgentMaintenancePanel;
class MOS_AgentMedicalPanel;
class MOS_AgentSupplyPanel;
class MOS_ReportPanel;

// =============================================================================
/** @class  MOS_InfoPanel
    @brief  MOS_InfoPanel
    @par    Using example
    @code
    MOS_InfoPanel;
    @endcode
*/
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
     MOS_InfoPanel( QWidget* pParent );
    ~MOS_InfoPanel();
    //@}

    //! @name Accessors
    //@{
    MOS_AgentKnowledgePanel&      GetAgentKnowledgePanel();
    MOS_DynaObjectKnowledgePanel& GetDynaObjectKnowledgePanel();
    //@}

    QSize sizeHint() const;

public slots:
    void SetSelectedElement( MOS_SelectedElement& selectedElement );
    void ClearAll();

private:
    void ShowAgentPanel             ( bool bShow );
    void ShowAgentKnowledgePanel    ( bool bShow );
    void ShowObjectKnowledgePanel   ( bool bShow );
    void ShowObjectPanel            ( bool bShow );
    void ShowLogisticPanels         ( bool bShow );

private:
    MOS_AgentStatePanel*          pStatePanel_;
    MOS_ReportPanel*              pReportPanel_;
    MOS_AgentResourcesPanel*      pResourcesPanel_;
    MOS_AgentCargoPanel*          pCargoPanel_;
    MOS_AgentKnowledgePanel*      pAgentKnowledgePanel_;
    MOS_DynaObjectPanel*          pDynaObjectPanel_;
    MOS_DynaObjectKnowledgePanel* pDynaObjectKnowledgePanel_;
    MOS_AgentMaintenancePanel*    pAgentMaintenancePanel_;
    MOS_AgentMedicalPanel*        pAgentMedicalPanel_;
    MOS_AgentSupplyPanel*         pAgentSupplyPanel_;
    
    bool bAgentVisible_;
    bool bAgentKnowledgeVisible_;
    bool bDynaObjectVisible_;
    bool bDynaObjectKnowledgeVisible_;
    bool bLogisiticVisible_;

    QTabWidget* pTabWidget_;
};



#ifdef MOS_USE_INLINE
#   include "MOS_InfoPanel.inl"
#endif

#endif // __MOS_InfoPanel_h_
