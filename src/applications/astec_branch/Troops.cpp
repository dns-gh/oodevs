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
    // NOTHING
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
// Name: Troops::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Troops::Update( const ASN1T_MsgUnitDotations& message )
{
    if( ! message.m.dotation_eff_personnelPresent  )
        return;

    uint nSize = message.dotation_eff_personnel.n;
    while( nSize > 0 )
    {
        const ASN1T_DotationPersonnel& dot = message.dotation_eff_personnel.elem[ --nSize ];

        unsigned* table = troopers_;
        if( dot.rang == EnumHumanRank::officier )
            table = officers_;
        else if( dot.rang == EnumHumanRank::sous_officer )
            table = subOfficers_;

        table[ eTroopHealthStateTotal ]              = dot.nb_total;
        table[ eTroopHealthStateOperational ]        = dot.nb_operationnels;
        table[ eTroopHealthStateDead ]               = dot.nb_morts;
        table[ eTroopHealthStateWounded ]            = dot.nb_blesses;
        table[ eTroopHealthStateMentalWounds ]       = dot.nb_blesses_mentaux;
        table[ eTroopHealthStateContaminated ]       = dot.nb_contamines_nbc;
        table[ eTroopHealthStateInTreatment ]        = dot.nb_dans_chaine_sante;
        table[ eTroopHealthStateUsedForMaintenance ] = dot.nb_utilises_pour_maintenance;
    }

    controller_.Update( *this );
}

