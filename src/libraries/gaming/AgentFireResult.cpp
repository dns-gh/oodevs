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
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: AgentFireResult constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
AgentFireResult::AgentFireResult( const sword::UnitFireDamages& message, const tools::Resolver_ABC< kernel::Agent_ABC >& resolver, const tools::Resolver_ABC< kernel::EquipmentType >& equipmentResolver, const QDateTime& time, const kernel::Entity_ABC* firer, int id )
    : target_( resolver.Get( message.target().id() ) )
    , firer_( firer )
    , time_( time )
    , id_( id )
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
// Name: AgentFireResult constructor
// Created: LDC 2013-11-29
// -----------------------------------------------------------------------------
AgentFireResult::AgentFireResult( const kernel::Agent_ABC& target, const QDateTime& time, const kernel::Entity_ABC* firer, int id )
    : target_( target )
    , firer_( firer )
    , time_( time )
    , id_( id )
{
    for( int i = 0; i < eNbrHumanWound; ++i )
        casualties_[ i ].wound_ = E_HumanWound( i );
}

// -----------------------------------------------------------------------------
// Name: AgentFireResult destructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
AgentFireResult::~AgentFireResult()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentFireResult::IsMiss
// Created: LDC 2013-11-29
// -----------------------------------------------------------------------------
bool AgentFireResult::IsMiss() const
{
    for( int i = 0; i < eNbrHumanWound; ++i )
    {
        if( casualties_[ i ].troopers_ != 0 || casualties_[ i ].officers_ != 0 || casualties_[ i ].subOfficers_ != 0 )
            return false;
    }
    return Count() == 0;
}
