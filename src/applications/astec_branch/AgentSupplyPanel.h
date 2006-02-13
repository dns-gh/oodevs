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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentSupplyPanel.h $
// $Author: Age $
// $Modtime: 6/04/05 10:41 $
// $Revision: 2 $
// $Workfile: AgentSupplyPanel.h $
//
// *****************************************************************************

#ifndef __AgentSupplyPanel_h_
#define __AgentSupplyPanel_h_

#include "AgentLogisticPanel_ABC.h"

template< typename Consign, typename Item > class LogConsignListView;
class LogSupplyConsign;
class LogSupplyConsign_ListView_Item;

// =============================================================================
/** @class  AgentSupplyPanel
    @brief  Agent supply panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentSupplyPanel : public AgentLogisticPanel_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentSupplyPanel( QWidget* pParent );
    virtual ~AgentSupplyPanel();
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
    typedef LogConsignListView< LogSupplyConsign, LogSupplyConsign_ListView_Item > T_List;
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

#endif // __AgentSupplyPanel_h_
