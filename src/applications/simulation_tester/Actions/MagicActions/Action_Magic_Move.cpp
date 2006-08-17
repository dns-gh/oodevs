// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_tester_pch.h"
#include "Action_Magic_Move.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"
#include "Actions/Scheduler.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Action_Magic_Move constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Action_Magic_Move::Action_Magic_Move( Pawn& pawn )
    : Action< Pawn > ( "Magic Move", pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_Magic_Move destructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Action_Magic_Move::~Action_Magic_Move()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Action_Magic_Move::Serialize
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
void Action_Magic_Move::Serialize()
{
    ASN1T_CoordUTM* pCoordUTM = new ASN1T_CoordUTM();
    ASN_Tools::CopyPoint( pTarget_->GetTestParam_Point(), *pCoordUTM );

    // build din/asn msg
    asnMsg_.GetAsnMsg().oid              = pTarget_->GetId();
    asnMsg_.GetAsnMsg().action.t         = T_MsgUnitMagicAction_action_move_to;
    asnMsg_.GetAsnMsg().action.u.move_to = pCoordUTM;
}

// -----------------------------------------------------------------------------
// Name: Action_Magic_Move::Send
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Action_Magic_Move::Send( const Scheduler& scheduler )
{
    asnMsg_.Send( 56 );
}

// -----------------------------------------------------------------------------
// Name: Action_Magic_Move::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Action_Magic_Move::Clean()
{
    delete asnMsg_.GetAsnMsg().action.u.move_to;
}
