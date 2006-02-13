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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentMedicalPanel.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: AgentMedicalPanel.h $
//
// *****************************************************************************

#ifndef __AgentMedicalPanel_h_
#define __AgentMedicalPanel_h_

#include "AgentLogisticPanel_ABC.h"

class Agent;

template< typename Consign, typename Item > class LogConsignListView;
class LogMedicalConsign;
class LogMedicalConsign_ListView_Item;

// =============================================================================
/** @class  AgentMedicalPanel
    @brief  Agent medical panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class AgentMedicalPanel : public AgentLogisticPanel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AgentMedicalPanel( QWidget* pParent );
    virtual ~AgentMedicalPanel();
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
    typedef LogConsignListView< LogMedicalConsign, LogMedicalConsign_ListView_Item > T_List;
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


    QListView* pDispoReleveAmbulances_;
    QListView* pDispoRamassageAmbulances_;
    QListView* pDispoDoctors_;
    //@}
};

#endif // __AgentMedicalPanel_h_
