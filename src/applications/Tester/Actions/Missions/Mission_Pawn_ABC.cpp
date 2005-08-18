// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-08 $
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
#include "Mission_Pawn_ABC.h"
#include "Entities/Pawn.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

IDManager Mission_Pawn_ABC::idManager_( 145 );

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Pawn_ABC::Mission_Pawn_ABC( Pawn& pawn )
    : Action< Pawn > ( pawn )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC destructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Pawn_ABC::~Mission_Pawn_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC::Serialize
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC::Serialize()
{
    const T_IdVector& oid_limas = pTarget_->GetTestParam_Limas();

    ASN_Tools::CopyID       ( pTarget_->GetId()                  , asnMsg_.GetAsnMsg().oid_unite_executante );
    ASN_Tools::CopyID       ( idManager_.GetFreeIdentifier()     , asnMsg_.GetAsnMsg().order_id             );
    ASN_Tools::CopyID       ( pTarget_->GetTestParam_LeftLimit() , asnMsg_.GetAsnMsg().oid_limite_gauche    );
    ASN_Tools::CopyID       ( pTarget_->GetTestParam_RightLimit(), asnMsg_.GetAsnMsg().oid_limite_droite    );
    ASN_Tools::CopyIDList   ( oid_limas                          , asnMsg_.GetAsnMsg().oid_limas            );
    ASN_Tools::CopyDirection( pTarget_->GetTestParam_Direction() , asnMsg_.GetAsnMsg().direction_dangereuse );

    delete &oid_limas;
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC::Send
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC::Send()
{
    // disengage automat first
    MOS_ASN_MsgSetAutomateMode msg;
    msg.GetAsnMsg().mode    = EnumAutomateState::debraye;
    msg.GetAsnMsg().unit_id = pTarget_->GetAutomat().GetId();
    msg.Send( 56 );
    // then send msg
    asnMsg_.Send( 56 );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC::Clean
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC::Clean()
{
    // TODO
}
