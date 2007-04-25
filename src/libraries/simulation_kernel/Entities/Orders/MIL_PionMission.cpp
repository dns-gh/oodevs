// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_PionMission.h"

#include "MIL_PionMissionType.h"
#include "MIL_AutomateMission.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_ModelPion.h"
#include "Network/NET_ASN_Messages.h"

int MIL_PionMission::nDIADirectionDanger_ = 0;

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::InitializeDIA
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
// static
void MIL_PionMission::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Mission_Pion" );
    nDIADirectionDanger_ = DEC_Tools::InitializeDIAField( "dirDangereuse_", diaType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_PionMissionType& type, MIL_AgentPion& pion, const ASN1T_MsgPionOrder& asn )
    : MIL_Mission_ABC       ( type, pion.GetKnowledge(), asn.parametres, asn.order_context, pion.GetRole< PHY_RolePion_Location >().GetPosition() )
    , type_                 ( type )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    GetVariable( nDIADirectionDanger_ ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( MIL_AgentPion& pion, const MIL_PionMission& rhs )
    : MIL_Mission_ABC       ( pion.GetKnowledge(), rhs )
    , type_                 ( rhs.type_ )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    GetVariable( nDIADirectionDanger_ ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_PionMissionType& type, MIL_AgentPion& pion, const MIL_AutomateMission& parent )
    : MIL_Mission_ABC       ( type, pion.GetKnowledge(), parent )
    , type_                 ( type )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    GetVariable( nDIADirectionDanger_ ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );    
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission constructor
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
MIL_PionMission::MIL_PionMission( const MIL_PionMissionType& type, MIL_AgentPion& pion )
    : MIL_Mission_ABC       ( type, pion.GetKnowledge() )
    , type_                 ( type )
    , pion_                 ( pion )
    , bDIABehaviorActivated_( false )
{
    GetVariable( nDIADirectionDanger_ ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );    
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_PionMission::~MIL_PionMission()
{
    Stop();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::CreateCopy
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_PionMission& MIL_PionMission::CreateCopy( MIL_AgentPion& target ) const
{
    return *new MIL_PionMission( target, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::IsFragOrderAvailable
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_PionMission::IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const
{
    return pion_.GetType().GetModel().IsFragOrderAvailableForMission( type_, fragOrderType );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Start
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Start()
{
    assert( !bDIABehaviorActivated_ );

    pion_.GetDecision().StartMissionBehavior( *this );
    bDIABehaviorActivated_ = true;
    Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Stop()
{
    if( bDIABehaviorActivated_ )
    {
        pion_.GetDecision().StopMissionBehavior( *this );
        bDIABehaviorActivated_ = false;
    }
    SendNoMission( pion_ );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void MIL_PionMission::SendNoMission( const MIL_AgentPion& pion )
{
    NET_ASN_MsgPionOrder asn;

    asn().oid_unite_executante               = pion.GetID();
    asn().mission                            = 0;
    asn().parametres.n                       = 0;
    asn().order_context.direction_dangereuse = 0;
    asn().order_context.limas.n              = 0;
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PionMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_PionMission::Send() const
{
    NET_ASN_MsgPionOrder asn;

    asn().oid_unite_executante = pion_.GetID();
    asn().mission              = type_.GetID();

    MIL_Mission_ABC::Serialize( asn().order_context );
    MIL_Mission_ABC::Serialize( asn().parametres    );

    asn.Send();

    MIL_Mission_ABC::CleanAfterSerialization( asn().parametres    );
    MIL_Mission_ABC::CleanAfterSerialization( asn().order_context );
}
