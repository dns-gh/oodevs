// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentFireResult.h"
#include "Equipment.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentFireResult constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
AgentFireResult::AgentFireResult( const sword::UnitFireDamages& message, const tools::Resolver_ABC< Agent_ABC >& resolver, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver, const QDateTime& time, const kernel::Entity_ABC* firer )
    : target_( resolver.Get( message.target().id() ) )
    , firer_( firer )
    , time_( time )
{
    for( int i = 0; i < message.equipments().elem_size(); ++i )
    {
        const kernel::EquipmentType* type = & equipmentResolver.Get( message.equipments().elem( i ).equipement().id() );
        Equipment* equipment = new Equipment( *type );
        equipment->available_   = message.equipments().elem( i ).available();
        equipment->unavailable_ = message.equipments().elem( i ).unavailable();
        equipment->repairable_  = message.equipments().elem( i ).repairable();
        Register( message.equipments().elem( i ).equipement().id(), *equipment );
    }
    for( int i = 0; i < eNbrHumanWound; ++i )
        casualties_[i].wound_ = E_HumanWound( i );

    for( int i = 0; i < message.humans().elem_size(); ++i )
    {
        const sword::UnitHumanFireDamage& fire = message.humans().elem( i );

        int Casualties::* table = & Casualties::troopers_;
        if( fire.rank() == sword::officer )
            table = & Casualties::officers_;
        else if( fire.rank() == sword::sub_officer )
            table = & Casualties::subOfficers_;

        casualties_[ eHumanWound_BlesseUrgence1 ].*table        = fire.wounded_u1();
        casualties_[ eHumanWound_BlesseUrgence2 ].*table        = fire.wounded_u2();
        casualties_[ eHumanWound_BlesseUrgence3 ].*table        = fire.wounded_u3();
        casualties_[ eHumanWound_BlesseUrgenceExtreme ].*table  = fire.wounded_ue();
        casualties_[ eHumanWound_Mort ].*table                  = fire.dead();
        casualties_[ eHumanWound_NonBlesse ].*table             = fire.alive();
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
