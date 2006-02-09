// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-04-01 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentMaintenancePanel.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: AgentMaintenancePanel.h $
//
// *****************************************************************************

#ifndef __AgentMaintenancePanel_h_
#define __AgentMaintenancePanel_h_

#include "AgentLogisticPanel_ABC.h"

template< typename Consign, typename Item > class LogConsignListView;
class LogMaintenanceConsign;
class LogMaintenanceConsign_ListView_Item;

// =============================================================================
/** @class  AgentMaintenancePanel
    @brief  Agent maintenance panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMaintenancePanel : public AgentLogisticPanel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentMaintenancePanel( QWidget* pParent );
    virtual ~AgentMaintenancePanel();
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnUpdate();
    virtual void OnClearSelection();
    virtual void OnAgentUpdated( Agent& agent );
    //@}

private:
    //! @name Types
    //@{
    typedef LogConsignListView< LogMaintenanceConsign, LogMaintenanceConsign_ListView_Item > T_List;
    //@}

    //! @name Member data
    //@{
    T_List* pConsignListView_;
    T_List* pConsignHandledListView_;

    QListView* pState_;
    QListViewItem* pStateChainEnabled_;
    QListViewItem* pStateTempsBordee_;
    QListViewItem* pStatePriorites_;
    QListViewItem* pStateTacticalPriorites_;

    QListView* pDispoHaulers_;
    QListView* pDispoRepairers_;
    //@}
};

#endif // __AgentMaintenancePanel_h_
