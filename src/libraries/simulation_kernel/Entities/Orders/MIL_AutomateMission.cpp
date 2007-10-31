// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateMission.h"

#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_ModelAutomate.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

int MIL_AutomateMission::nDIADirectionDanger_ = 0;
int MIL_AutomateMission::nDIAFormation_       = 0;

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::InitializeDIA
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
// static
void MIL_AutomateMission::InitializeDIA()
{
    const DIA_TypeDef& diaType = DEC_Tools::GetDIAType( "T_Mission_Automate" );
    nDIADirectionDanger_ = DEC_Tools::InitializeDIAField( "directionEnnemi_", diaType );
    nDIAFormation_       = DEC_Tools::InitializeDIAField( "formation_"      , diaType );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( const MIL_AutomateMissionType& type, MIL_Automate& automate, const ASN1T_MsgAutomatOrder& asn )
    : MIL_Mission_ABC          ( type, automate.GetKnowledge(), asn.parametres, asn.order_context, automate.GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition() )
    , type_                    ( type )
    , automate_                ( automate )
    , bDIAMrtBehaviorActivated_( false )
    , bDIACdtBehaviorActivated_( false )
{
    GetVariable( nDIADirectionDanger_ ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    GetVariable( nDIAFormation_       ).SetValue( (int)asn.formation );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( const MIL_AutomateMissionType& type, MIL_Automate& automate )
    : MIL_Mission_ABC          ( type, automate.GetKnowledge() )
    , type_                    ( type )
    , automate_                ( automate )
    , bDIAMrtBehaviorActivated_( false )
    , bDIACdtBehaviorActivated_( false )
{
    GetVariable( nDIADirectionDanger_ ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    GetVariable( nDIAFormation_       ).SetValue( (int)EnumAutomatOrderFormation::deux_echelons );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( const MIL_AutomateMissionType& type, MIL_Automate& automate, const MIL_AutomateMission& parent )
    : MIL_Mission_ABC          ( type, automate.GetKnowledge(), parent )
    , type_                    ( type )
    , automate_                ( automate )
    , bDIAMrtBehaviorActivated_( false )
    , bDIACdtBehaviorActivated_( false )
{
    GetVariable( nDIADirectionDanger_ ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    GetVariable( nDIAFormation_       ).SetValue( (int)EnumAutomatOrderFormation::deux_echelons );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission constructor
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_AutomateMission::MIL_AutomateMission( MIL_Automate& automate, const MIL_AutomateMission& rhs )
    : MIL_Mission_ABC          ( automate.GetKnowledge(), rhs )
    , type_                    ( rhs.type_ )
    , automate_                ( automate)
    , bDIAMrtBehaviorActivated_( false )
    , bDIACdtBehaviorActivated_( false )
{
    GetVariable( nDIADirectionDanger_ ).SetValue( new MT_Vector2D( GetDirDanger() ), &DEC_Tools::GetTypeDirection() );
    GetVariable( nDIAFormation_       ).SetValue( const_cast< MIL_AutomateMission& >( rhs ).GetVariable( nDIAFormation_ ).ToId() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission destructor
// Created: NLD 2006-11-25
// -----------------------------------------------------------------------------
MIL_AutomateMission::~MIL_AutomateMission()
{
    Stop();
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::CreateCopy
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
MIL_AutomateMission& MIL_AutomateMission::CreateCopy( MIL_Automate& target ) const
{
    return *new MIL_AutomateMission( target, *this );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::IsFragOrderAvailable
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
bool MIL_AutomateMission::IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const
{
    return automate_.GetType().GetModel().IsFragOrderAvailableForMission( type_, fragOrderType );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::Start
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateMission::Start()
{
    assert( !bDIAMrtBehaviorActivated_ );

    automate_.GetDecision().StartMissionMrtBehavior( *this );
    bDIAMrtBehaviorActivated_ = true;
    Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::Stop
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateMission::Stop()
{
    if( bDIAMrtBehaviorActivated_ )
        automate_.GetDecision().StopMissionMrtBehavior( *this );
    if( bDIACdtBehaviorActivated_ )
        automate_.GetDecision().StopMissionConduiteBehavior( *this );

    SendNoMission( automate_ );
    
    bDIACdtBehaviorActivated_ = false;
    bDIAMrtBehaviorActivated_ = false;    
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::GoToCdt
// Created: NLD 2006-11-23
// -----------------------------------------------------------------------------
void MIL_AutomateMission::GoToCdt()
{
    assert(  bDIAMrtBehaviorActivated_ );
    assert( !bDIACdtBehaviorActivated_ );

    automate_.GetDecision().StopMissionMrtBehavior( *this );
    bDIAMrtBehaviorActivated_ = false;

    automate_.GetDecision().StartMissionConduiteBehavior( *this );
    bDIACdtBehaviorActivated_ = true;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::SendNoMission
// Created: NLD 2007-04-25
// -----------------------------------------------------------------------------
// static
void MIL_AutomateMission::SendNoMission( const MIL_Automate& automate )
{
    NET_ASN_MsgAutomatOrder asn;

    asn().oid                                = automate.GetID();
    asn().mission                            = 0;
    asn().formation                          = EnumAutomatOrderFormation::un_echelon;
    asn().parametres.n                       = 0;
    asn().order_context.direction_dangereuse = 0;
    asn().order_context.limas.n              = 0;
    asn().order_context.intelligences.n      = 0;
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateMission::Send
// Created: NLD 2006-11-21
// -----------------------------------------------------------------------------
void MIL_AutomateMission::Send() const
{
    NET_ASN_MsgAutomatOrder asn;

    asn().oid       = automate_.GetID();
    asn().mission   = type_    .GetID();

    NET_ASN_Tools::CopyEnumeration( const_cast< MIL_AutomateMission* >( this )->GetVariable( nDIAFormation_ ), asn().formation );

    MIL_Mission_ABC::Serialize( asn().order_context );
    MIL_Mission_ABC::Serialize( asn().parametres    );

    asn.Send();

    MIL_Mission_ABC::CleanAfterSerialization( asn().parametres    );
    MIL_Mission_ABC::CleanAfterSerialization( asn().order_context );
}

