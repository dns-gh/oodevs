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

#include "Tester_pch.h"
#include "Mission_Pawn_ABC.h"
#include "Entities/Pawn.h"
#include "Entities/Automat.h"
#include "Messages/ASN_Messages.h"
#include "Tools/ASN_Tools.h"
#include "Actions/Scheduler.h"

using namespace TEST;

IDManager Mission_Pawn_ABC::idManager_( 145 );

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC constructor
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
Mission_Pawn_ABC::Mission_Pawn_ABC( const std::string& name, Pawn& pawn )
    : Action< Pawn > ( name, pawn )
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
    uint nLimit = pTarget_->GetTestParam_LeftLimit();
    if( nLimit > 0 )
    {
        asnMsg_.GetAsnMsg().m.oid_limite_gauchePresent = 1;
        ASN_Tools::CopyID   ( pTarget_->GetTestParam_LeftLimit() , asnMsg_.GetAsnMsg().oid_limite_gauche    );
    }
    nLimit = pTarget_->GetTestParam_RightLimit();
    if( nLimit > 0 )
    {
        asnMsg_.GetAsnMsg().m.oid_limite_droitePresent = 1;
        ASN_Tools::CopyID   ( pTarget_->GetTestParam_RightLimit(), asnMsg_.GetAsnMsg().oid_limite_droite    );
    }
    ASN_Tools::CopyIDList   ( oid_limas                          , asnMsg_.GetAsnMsg().oid_limas            );
    ASN_Tools::CopyDirection( pTarget_->GetTestParam_Direction() , asnMsg_.GetAsnMsg().direction_dangereuse );

    delete &oid_limas;
}

// -----------------------------------------------------------------------------
// Name: Mission_Pawn_ABC::Send
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void Mission_Pawn_ABC::Send( const Scheduler& scheduler )
{
    // disengage automat first
    MOS_ASN_MsgSetAutomateMode msg;
    msg.GetAsnMsg().mode       = EnumAutomateState::debraye;
    msg.GetAsnMsg().unit_id    = pTarget_->GetAutomat()->GetId();
    msg.Send( 56 );

    // recomplete pawn if needed
    if( scheduler.MustRecomplete() )
    {
        pTarget_->SendMagicAction( T_MsgUnitMagicAction_action_recompletement_total );
        MT_LOG_INFO_MSG( "Pawn #" << pTarget_->GetId() << " - Magic recompletion" );
    }

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
