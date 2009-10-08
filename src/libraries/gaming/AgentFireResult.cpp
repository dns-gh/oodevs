// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentFireResult.h"
#include "Equipment.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentFireResult constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
AgentFireResult::AgentFireResult( const ASN1T_UnitFireDamages& message, const tools::Resolver_ABC< Agent_ABC >& resolver,  const tools::Resolver_ABC< EquipmentType >& equipmentResolver, const QDateTime& time )
    : target_( resolver.Get( message.target ) )
    , time_( time )
{
    for( unsigned i = 0; i < message.equipments.n; ++i )
    {
        const EquipmentType* type = & equipmentResolver.Get( message.equipments.elem[ i ].equipement_type );
        Equipment* equipment = new Equipment( *type );
        equipment->available_   = message.equipments.elem[ i ].available_nbr;
        equipment->unavailable_ = message.equipments.elem[ i ].unavailable_nbr;
        equipment->repairable_  = message.equipments.elem[ i ].repairable_nbr;
        Register( message.equipments.elem[ i ].equipement_type, *equipment );
    }
    for( unsigned i = 0; i < eNbrHumanWound; ++i )
        casualties_[i].wound_ = E_HumanWound( i );

    for( uint i = 0; i < message.humans.n; ++i )
    {
        const ASN1T_UnitHumanFireDamage& fire = message.humans.elem[ i ];

        int Casualties::* table = & Casualties::troopers_;
        if( fire.rank == EnumHumanRank::officier )
            table = & Casualties::officers_;
        else if( fire.rank == EnumHumanRank::sous_officer )
            table = & Casualties::subOfficers_;

        casualties_[ eHumanWound_BlesseUrgence1 ].*table        = fire.wounded_u1_nbr;
        casualties_[ eHumanWound_BlesseUrgence2 ].*table        = fire.wounded_u2_nbr;
        casualties_[ eHumanWound_BlesseUrgence3 ].*table        = fire.wounded_u3_nbr;
        casualties_[ eHumanWound_BlesseUrgenceExtreme ].*table  = fire.wounded_ue_nbr;
        casualties_[ eHumanWound_Mort ].*table                  = fire.dead_nbr;
        casualties_[ eHumanWound_NonBlesse ].*table             = fire.alive_nbr;
    }
}

// -----------------------------------------------------------------------------
// Name: AgentFireResult destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
AgentFireResult::~AgentFireResult()
{
    DeleteAll();
}
