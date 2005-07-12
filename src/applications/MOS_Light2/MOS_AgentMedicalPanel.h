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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentMedicalPanel.h $
// $Author: Nld $
// $Modtime: 27/04/05 18:27 $
// $Revision: 3 $
// $Workfile: MOS_AgentMedicalPanel.h $
//
// *****************************************************************************

#ifndef __MOS_AgentMedicalPanel_h_
#define __MOS_AgentMedicalPanel_h_

#include "MOS_AgentLogisticPanel_ABC.h"

class MOS_Agent;

template< typename Consign, typename Item > class MOS_LogConsignListView;
class MOS_LogMedicalConsign;
class MOS_LogMedicalConsign_ListView_Item;

// =============================================================================
/** @class  MOS_AgentMedicalPanel
    @brief  Agent medical panel
*/
// Created: AGE 2005-04-01
// =============================================================================
class MOS_AgentMedicalPanel : public MOS_AgentLogisticPanel_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MOS_AgentMedicalPanel( QWidget* pParent );
    virtual ~MOS_AgentMedicalPanel();
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnUpdate();
    virtual void OnAgentUpdated( MOS_Agent& agent );
    //@}

private:
    //! @name Types
    //@{
    typedef MOS_LogConsignListView< MOS_LogMedicalConsign, MOS_LogMedicalConsign_ListView_Item > T_List;
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

#endif // __MOS_AgentMedicalPanel_h_
