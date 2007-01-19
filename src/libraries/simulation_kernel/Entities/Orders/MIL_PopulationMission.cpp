// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_PopulationMission.h"

#include "MIL_PopulationMissionType.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Decision/DEC_ModelPopulation.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::InitializeDIA
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
// static
void MIL_PopulationMission::InitializeDIA()
{
    /*const DIA_TypeDef& diaType = */DEC_Tools::GetDIAType( "T_Mission_Population" );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PopulationMission::MIL_PopulationMission( const MIL_PopulationMissionType& type, MIL_Population& population, const ASN1T_MsgPopulationOrder& asn )
    : MIL_Mission_ABC       ( type, population.GetKnowledge(), asn.parametres )
    , type_                 ( type )
    , population_           ( population )
    , bDIABehaviorActivated_( false )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_PopulationMission::~MIL_PopulationMission()
{
    Stop();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::IsFragOrderAvailable
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_PopulationMission::IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const
{
    return population_.GetType().GetModel().IsFragOrderAvailableForMission( type_, fragOrderType );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Start
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Start()
{
    assert( !bDIABehaviorActivated_ );

    population_.GetDecision().StartMissionBehavior( *this );
    bDIABehaviorActivated_ = true;
    Send();
    SendMsgOrderManagement( EnumOrderState::started );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Stop()
{
    if( bDIABehaviorActivated_ )
    {
        population_.GetDecision().StopMissionBehavior( *this );
        bDIABehaviorActivated_ = false;
        SendMsgOrderManagement( EnumOrderState::stopped );
    }    
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::Send() const
{
    NET_ASN_MsgPopulationOrder asn;

    asn().oid_unite_executante = population_.GetID();
    asn().mission              = type_.GetID();

    MIL_Mission_ABC::Serialize( asn().parametres    );

    asn.Send();

    MIL_Mission_ABC::CleanAfterSerialization( asn().parametres    );
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationMission::SendMsgOrderManagement
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PopulationMission::SendMsgOrderManagement( ASN1T_EnumOrderState nState ) const
{
    NET_ASN_MsgPopulationOrderManagement asnMsg;
    asnMsg().oid_unite_executante = population_.GetID();
    asnMsg().etat                 = nState;
    asnMsg.Send();
}
