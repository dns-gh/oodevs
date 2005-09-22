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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentSupplyPanel.h $
// $Author: Age $
// $Modtime: 6/04/05 10:41 $
// $Revision: 2 $
// $Workfile: MOS_AgentSupplyPanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentSupplyPanel_h_
#define __MOS_AgentSupplyPanel_h_

#include "MOS_AgentLogisticPanel_ABC.h"

template< typename Consign, typename Item > class MOS_LogConsignListView;
class MOS_LogSupplyConsign;
class MOS_LogSupplyConsign_ListView_Item;

// =============================================================================
/** @class  MOS_AgentSupplyPanel
    @brief  Agent supply panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class MOS_AgentSupplyPanel : public MOS_AgentLogisticPanel_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MOS_AgentSupplyPanel( QWidget* pParent );
    virtual ~MOS_AgentSupplyPanel();
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnUpdate();
    virtual void OnClearSelection();
    virtual void OnAgentUpdated( MOS_Agent& agent );
    //@}

private:
    //! @name Types
    //@{
    typedef MOS_LogConsignListView< MOS_LogSupplyConsign, MOS_LogSupplyConsign_ListView_Item > T_List;
    //@}

    //! @name Member data
    //@{
    T_List* pConsignListView_;
    T_List* pConsignHandledListView_;
    QListView* pState_;
    QListViewItem* pStateChainEnabled_;
    QListView* pStocks_;
    QListView* pQuotas_;
    QListView* pDispoTransporters_;
    QListView* pDispoCommanders_;
    //@}
};

#endif // __MOS_AgentSupplyPanel_h_
