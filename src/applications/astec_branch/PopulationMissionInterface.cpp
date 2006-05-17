// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationMissionInterface.h"
#include "ASN_Messages.h"
#include "Population.h"
#include "Tools.h"

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface constructor
// Created: APE 2004-04-20
// -----------------------------------------------------------------------------
PopulationMissionInterface::PopulationMissionInterface( QWidget* parent, Population& population, unsigned long nMissionId, ActionController& controller, ParametersLayer& layer, const CoordinateConverter_ABC& converter  )
    : MissionInterface_ABC( parent, population, controller, layer, converter )
    , nMissionId_         ( nMissionId )
{
    pASNMsgOrder_ = new ASN_MsgPopulationOrder();
    pASNMsgOrder_->GetAsnMsg().order_id = (uint)(&population);
    pASNMsgOrder_->GetAsnMsg().oid_unite_executante = population.GetId();

    QLabel* pLabel = new QLabel( ENT_Tr::ConvertFromPopulationMission( E_PopulationMission( nMissionId_ ) ).c_str(), this );
    pLabel->setFrameStyle( QFrame::Box | QFrame::Sunken );
    pLabel->setAlignment( Qt::AlignCenter );
    QFont font = pLabel->font();
    font.setBold( true );
    pLabel->setFont( font );

    CreateInterface();
}

// -----------------------------------------------------------------------------
// Name: PopulationMissionInterface::CreateDefaultParameters
// Created: AGE 2006-04-10
// -----------------------------------------------------------------------------
void PopulationMissionInterface::CreateDefaultParameters()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UnitMissionInterface::OnOk
// Created: APE 2004-04-26
// -----------------------------------------------------------------------------
void PopulationMissionInterface::OnOk()
{
    if( ! CheckValidity() )
        return;

    Commit();
    pASNMsgOrder_->Send( 45 );
    parentWidget()->hide();
}

#include "PopulationMissionInterface_Gen.cpp"
