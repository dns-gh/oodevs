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
// $Archive: /MVW_v10/Build/SDK/Light2/src/FragmentaryOrderInterface.cpp $
// $Author: Age $
// $Modtime: 31/03/05 16:48 $
// $Revision: 7 $
// $Workfile: FragmentaryOrderInterface.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "FragmentaryOrderInterface.h"
#include "moc_FragmentaryOrderInterface.cpp"

#include "ASN_Messages.h"
#include "ASN_Types.h"
#include "MissionPanel.h"
#include "Agent_ABC.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: FragmentaryOrderInterface constructor
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
FragmentaryOrderInterface::FragmentaryOrderInterface( Agent_ABC& agent, uint nMissionId, MissionPanel& parentPanel )
    : MissionInterface_ABC( agent, parentPanel )
    , nMissionId_             ( nMissionId )
{
    pASNMsgOrder_ = new ASN_MsgOrderConduite();
    pASNMsgOrder_->GetAsnMsg().unit_id = agent.GetID();
    pASNMsgOrder_->GetAsnMsg().order_conduite.t = nMissionId_;

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromFragOrder( ( E_FragOrder )nMissionId_ ).c_str(), this );
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
// Name: FragmentaryOrderInterface::OnOk
// Created: APE 2004-05-12
// -----------------------------------------------------------------------------
void FragmentaryOrderInterface::OnOk()
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
    strMsgTitle << "Ordre de conduite " << ENT_Tr::ConvertFromFragOrder( ( E_FragOrder )nMissionId_ ) << " pour agent " << agent_.GetName();
    MT_LOG_INFO( strMsgTitle.str().c_str(), eSent, strMsg.str().c_str() );

    pASNMsgOrder_->GetAsnMsg().order_id = 43;
    pASNMsgOrder_->Send( 36999 );
    
    parentPanel_.hide();
}


#include "FragmentaryOrderInterface_Gen.cpp"