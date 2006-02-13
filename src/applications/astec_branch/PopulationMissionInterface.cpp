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

#include "astec_pch.h"
#include "PopulationMissionInterface.h"
#include "moc_PopulationMissionInterface.cpp"

#include "MissionPanel.h"
#include "ASN_Messages.h"
#include "ASN_Types.h"
#include "ParamPoint.h"
#include "ParamAgent.h"
#include "ParamAgentList.h"
#include "ParamPath.h"
#include "ParamLimits.h"
#include "ParamLimaList.h"
#include "ParamRadioBtnGroup.h"
#include "ParamGDH.h"
#include "ParamComboBox.h"
#include "ParamBool.h"
#include "ParamDirection.h"
#include "Population.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
/** @param  mission 
    @param  pParent 
*/
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
PopulationMissionInterface::PopulationMissionInterface( Population& population, uint nMissionId, MissionPanel& parentPanel )
    : MissionInterface_ABC( population, parentPanel )
    , nMissionId_             ( nMissionId )
{
    pASNMsgOrder_ = new ASN_MsgPopulationOrder();
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
// Name: UnitMissionInterface::CreateDefaultParameters
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
void PopulationMissionInterface::CreateDefaultParameters()
{
    //$$$$ tr

    ASN1T_MsgPopulationOrder& order = pASNMsgOrder_->GetAsnMsg();

    
}


// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::OnOk
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void PopulationMissionInterface::OnOk()
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



#include "PopulationMissionInterface_Gen.cpp"
