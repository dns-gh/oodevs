// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_PopulationMissionInterface.h"
#include "moc_MOS_PopulationMissionInterface.cpp"

#include "MOS_MissionPanel.h"
#include "MOS_ASN_Messages.h"
#include "MOS_ASN_Types.h"
#include "MOS_ParamPoint.h"
#include "MOS_ParamAgent.h"
#include "MOS_ParamAgentList.h"
#include "MOS_ParamPath.h"
#include "MOS_ParamLimits.h"
#include "MOS_ParamLimaList.h"
#include "MOS_ParamRadioBtnGroup.h"
#include "MOS_ParamGDH.h"
#include "MOS_ParamComboBox.h"
#include "MOS_ParamBool.h"
#include "MOS_ParamDirection.h"
#include "MOS_Population.h"
#include "MOS_Tools.h"

// -----------------------------------------------------------------------------
// Name: MOS_UnitMissionInterface constructor
/** @param  mission 
    @param  pParent 
*/
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
MOS_PopulationMissionInterface::MOS_PopulationMissionInterface( MOS_Population& population, uint nMissionId, MOS_MissionPanel& parentPanel )
    : MOS_MissionInterface_ABC( population, parentPanel )
    , nMissionId_             ( nMissionId )
{
    pASNMsgOrder_ = new MOS_ASN_MsgPopulationOrder();
    pASNMsgOrder_->GetAsnMsg().order_id = (uint)(&agent_);
    pASNMsgOrder_->GetAsnMsg().oid_unite_executante = agent_.GetID();

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromPopulationMission( E_PopulationMission( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    this->CreateInterface();
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitMissionInterface::CreateDefaultParameters
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void MOS_PopulationMissionInterface::CreateDefaultParameters()
{
    //$$$$ tr

    ASN1T_MsgPopulationOrder& order = pASNMsgOrder_->GetAsnMsg();

    
}


// -----------------------------------------------------------------------------
// Name: MOS_UnitMissionInterface::OnOk
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void MOS_PopulationMissionInterface::OnOk()
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
    strMsgTitle << "Mission pion " << ENT_Tr::ConvertFromPopulationMission( E_PopulationMission( nMissionId_ ) ) << " pour population " << agent_.GetName();
    MT_LOG_INFO( strMsgTitle.str().c_str(), eSent, strMsg.str().c_str() );

    pASNMsgOrder_->Send( 45 );

    parentPanel_.hide();
}



#include "MOS_PopulationMissionInterface_Gen.cpp"
