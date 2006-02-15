// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Equipments.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: Equipments constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Equipments::Equipments( Controller& controller )
    : controller_( controller )
{

}
    
// -----------------------------------------------------------------------------
// Name: Equipments destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Equipments::~Equipments()
{

}

// -----------------------------------------------------------------------------
// Name: Equipments::DoUpdate
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void Equipments::DoUpdate( const ASN1T_MsgUnitDotations& message )
{
    if ( message.m.dotation_eff_materielPresent != 1 )
        return;

    uint nSize = message.dotation_eff_materiel.n;
    while ( nSize > 0 )
    {
        const ASN1T_DotationEquipement& value = message.dotation_eff_materiel.elem[ --nSize ];
        unsigned long equipId = value.type_equipement;
        equipments_[ equipId ].nNbrAvailable_       = value.nb_disponibles;
        equipments_[ equipId ].nNbrUnavailable_     = value.nb_indisponibles;
        equipments_[ equipId ].nNbrReparable_       = value.nb_reparables;
        equipments_[ equipId ].nNbrInMaintenance_   = value.nb_dans_chaine_maintenance;
    }

    controller_.Update( *this );
}
