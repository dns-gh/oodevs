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
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"

using namespace TEST;

IDManager Mission_Pawn_ABC::idManager_( 145 );

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Pawn_ABC::Mission_Pawn_ABC( Pawn& pawn, uint nExecutionTick /* = 0 */ )
    : Action< Pawn > ( pawn, nExecutionTick )
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
    ASN_Tools::CopyId       ( pTarget_->GetId()                , asnMsg_.GetAsnMsg().oid_unite_executante );
    ASN_Tools::CopyId       ( idManager_.GetFreeIdentifier()   , asnMsg_.GetAsnMsg().order_id             );
    ASN_Tools::CopyId       ( pTarget_->GetTP_LeftLimit()      , asnMsg_.GetAsnMsg().oid_limite_gauche    );
    ASN_Tools::CopyId       ( pTarget_->GetTP_RightLimit()     , asnMsg_.GetAsnMsg().oid_limite_droite    );
    ASN_Tools::CopyIdList   ( pTarget_->GetTP_Limas()          , asnMsg_.GetAsnMsg().oid_limas            );
    ASN_Tools::CopyDirection( pTarget_->GetTP_DangerDirection(), asnMsg_.GetAsnMsg().direction_dangereuse );
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC::Send
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC::Send()
{
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
