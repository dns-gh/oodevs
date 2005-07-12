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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentMaintenancePanel.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: MOS_AgentMaintenancePanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentMaintenancePanel_h_
#define __MOS_AgentMaintenancePanel_h_

#include "MOS_AgentLogisticPanel_ABC.h"

template< typename Consign, typename Item > class MOS_LogConsignListView;
class MOS_LogMaintenanceConsign;
class MOS_LogMaintenanceConsign_ListView_Item;

// =============================================================================
/** @class  MOS_AgentMaintenancePanel
    @brief  Agent maintenance panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class MOS_AgentMaintenancePanel : public MOS_AgentLogisticPanel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MOS_AgentMaintenancePanel( QWidget* pParent );
    virtual ~MOS_AgentMaintenancePanel();
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnUpdate();
    void OnAgentUpdated( MOS_Agent& agent );
    //@}

private:
    //! @name Types
    //@{
    typedef MOS_LogConsignListView< MOS_LogMaintenanceConsign, MOS_LogMaintenanceConsign_ListView_Item > T_List;
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

#endif // __MOS_AgentMaintenancePanel_h_
