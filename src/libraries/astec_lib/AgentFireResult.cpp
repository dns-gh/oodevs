// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentFireResult.h"
#include "Equipment.h"

// -----------------------------------------------------------------------------
// Name: AgentFireResult constructor
// Created: AGE 2006-03-10
// -----------------------------------------------------------------------------
AgentFireResult::AgentFireResult( const ASN1T_FireDamagesPion& message, const Resolver_ABC< Agent >& resolver,  const Resolver_ABC< EquipmentType >& equipmentResolver )
    : target_( resolver.Get( message.cible ) )
{
    for( unsigned i = 0; i < message.equipements.n; ++i )
    {
        const EquipmentType* type = & equipmentResolver.Get( message.equipements.elem[ i ].type_equipement );
        Equipment* equipment = new Equipment( *type );
        equipment->available_   = message.equipements.elem[ i ].nb_disponibles;
        equipment->unavailable_ = message.equipements.elem[ i ].nb_indisponibles;
        equipment->repairable_  = message.equipements.elem[ i ].nb_reparables;
        Register( message.equipements.elem[ i ].type_equipement, *equipment );
    }
    for( unsigned i = 0; i < eNbrHumanWound; ++i )
        casualties_[i].wound_ = E_HumanWound( i );

    for( uint i = 0; i < message.humains.n; ++i )
    {
        const ASN1T_FireDamagePionHuman& fire = message.humains.elem[ i ];

        unsigned Casualties::* table = & Casualties::troopers_;
         if( fire.rang == EnumHumanRank::officier )
            table = & Casualties::officers_;
        else if( fire.rang == EnumHumanRank::sous_officer )
            table = & Casualties::subOfficers_;

        casualties_[ eHumanWound_BlesseUrgence1 ].*table        = fire.nb_blesses_urgence_1;
        casualties_[ eHumanWound_BlesseUrgence2 ].*table        = fire.nb_blesses_urgence_2;
        casualties_[ eHumanWound_BlesseUrgence3 ].*table        = fire.nb_blesses_urgence_3;
        casualties_[ eHumanWound_BlesseUrgenceExtreme ].*table  = fire.nb_blesses_urgence_extreme;
        casualties_[ eHumanWound_Mort ].*table                  = fire.nb_morts;
        casualties_[ eHumanWound_NonBlesse ].*table             = fire.nb_non_blesses;
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
