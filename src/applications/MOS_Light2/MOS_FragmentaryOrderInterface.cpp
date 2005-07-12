// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_FragmentaryOrderInterface.cpp $
// $Author: Age $
// $Modtime: 31/03/05 16:48 $
// $Revision: 7 $
// $Workfile: MOS_FragmentaryOrderInterface.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MOS_FragmentaryOrderInterface.h"
#include "moc_MOS_FragmentaryOrderInterface.cpp"

#include "MOS_ASN_Messages.h"
#include "MOS_ASN_Types.h"
#include "MOS_MissionPanel.h"
#include "MOS_Agent.h"
#include "MOS_Tools.h"

#ifndef MOS_USE_INLINE
#   include "MOS_FragmentaryOrderInterface.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface constructor
/** @param  agent 
    @param  nOrderType 
    @param  parentPanel 
*/
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
MOS_FragmentaryOrderInterface::MOS_FragmentaryOrderInterface( MOS_Agent& agent, uint nMissionId, MOS_MissionPanel& parentPanel )
    : MOS_MissionInterface_ABC( agent, parentPanel )
    , nMissionId_             ( nMissionId )
{
    pASNMsgOrder_ = new MOS_ASN_MsgOrderConduite();
    pASNMsgOrder_->GetAsnMsg().unit_id = agent.GetAgentID();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = nMissionId_;

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromFragOrder( E_FragOrder( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    this->CreateInterface();

    QWidget* pSpacer = new QWidget( this );
    this->setStretchFactor( pSpacer, 100 );
}


// -----------------------------------------------------------------------------
// Name: MOS_FragmentaryOrderInterface::OnOk
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void MOS_FragmentaryOrderInterface::OnOk()
{
    if( ! this->CheckValidity() )
        return;

    std::stringstream strMsg;

    for( IT_ParamVector it = paramVector_.begin() ; it != paramVector_.end() ; ++it )
    {
        if( it != paramVector_.begin() )
            strMsg << "\n";
        (*it)->WriteMsg( strMsg );
    }

    std::stringstream strMsgTitle;
    strMsgTitle << "Ordre de conduite " << ENT_Tr::ConvertFromFragOrder( E_FragOrder( nMissionId_ ) ) << " pour agent " << agent_.GetName();
    MT_LOG_INFO( strMsgTitle.str().c_str(), eSent, strMsg.str().c_str() );

    pASNMsgOrder_->GetAsnMsg().order_id = 43;
    pASNMsgOrder_->Send( 36999 );
    
    parentPanel_.hide();
}


#include "MOS_FragmentaryOrderInterface_Gen.cpp"