// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Troops.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: Troops constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Troops::Troops( Controller& controller )
    : controller_( controller )
{
    for( int i = 0; i < eTroopHealthStateNbrStates; ++i )
        humans_[ i ].state_ = E_TroopHealthState( i );
}

// -----------------------------------------------------------------------------
// Name: Troops destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Troops::~Troops()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Troops::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Troops::DoUpdate( const ASN1T_MsgUnitDotations& message )
{
    if( ! message.m.dotation_eff_personnelPresent  )
        return;

    uint nSize = message.dotation_eff_personnel.n;
    while( nSize > 0 )
    {
        const ASN1T_DotationPersonnel& dot = message.dotation_eff_personnel.elem[ --nSize ];
        unsigned Humans::* table = & Humans::troopers_;
        if( dot.rang == EnumHumanRank::officier )
            table = & Humans::officers_;
        else if( dot.rang == EnumHumanRank::sous_officer )
            table = & Humans::subOfficers_;

        humans_[ eTroopHealthStateTotal ].*table              = dot.nb_total;
        humans_[ eTroopHealthStateOperational ].*table        = dot.nb_operationnels;
        humans_[ eTroopHealthStateDead ].*table               = dot.nb_morts;
        humans_[ eTroopHealthStateWounded ].*table            = dot.nb_blesses;
        humans_[ eTroopHealthStateMentalWounds ].*table       = dot.nb_blesses_mentaux;
        humans_[ eTroopHealthStateContaminated ].*table       = dot.nb_contamines_nbc;
        humans_[ eTroopHealthStateInTreatment ].*table        = dot.nb_dans_chaine_sante;
        humans_[ eTroopHealthStateUsedForMaintenance ].*table = dot.nb_utilises_pour_maintenance;
    }

    controller_.Update( *this );
}

