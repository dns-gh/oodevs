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
#include "Equipment.h"

// -----------------------------------------------------------------------------
// Name: Equipments constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Equipments::Equipments( Controller& controller, const Resolver_ABC< EquipmentType >& resolver )
    : controller_( controller )
    , resolver_( resolver )
{

}
    
// -----------------------------------------------------------------------------
// Name: Equipments destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Equipments::~Equipments()
{
    DeleteAll();
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
        Equipment* equipment = Find( value.type_equipement );
        if( ! equipment )
        {
            equipment = new Equipment( resolver_.Get( value.type_equipement ) );
            Register( value.type_equipement, *equipment );
        }
        equipment->Update( value );
    }
    controller_.Update( *this );
}
