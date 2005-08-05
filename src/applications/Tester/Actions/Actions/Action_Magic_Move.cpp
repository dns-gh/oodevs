// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-04 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "Tester_pch.h"
#include "Action_Magic_Move.h"
#include "Entities/Pawn.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

// -----------------------------------------------------------------------------
// Name: Action_Magic_Move constructor
// Created: SBO 2005-08-04
// -----------------------------------------------------------------------------
Action_Magic_Move::Action_Magic_Move( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Action< Pawn > ( pawn, nExecutionTick )
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
bool Action_Magic_Move::Serialize() const
{
    ASN1T_CoordUTM  coordUTM;
    ASN_Tools::CopyPosition( pTarget_->GetTP_Position(), coordUTM );

    // build din/asn msg
    MOS_ASN_MsgUnitMagicAction          asnMsg;
    asnMsg.GetAsnMsg().oid              = pTarget_->GetId();
    asnMsg.GetAsnMsg().action.t         = T_MsgUnitMagicAction_action_move_to;
    asnMsg.GetAsnMsg().action.u.move_to = &coordUTM;

    // send to SIM
    asnMsg.Send( 56 );

    // clean
    std::cout << "executing magic move" << std::endl;
    return true;
}
