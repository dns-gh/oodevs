// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Equipments.h"
#include "clients_kernel/Controller.h"
#include "Equipment.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Equipments constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Equipments::Equipments( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, PropertiesDictionary& dico )
    : controller_( controller )
    , resolver_( resolver )
    , dico_( dico )
{
    // NOTHING   
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
void Equipments::DoUpdate( const ASN1T_MsgUnitAttributes& message )
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
            equipment->Update( value );
            AddToDictionary( *equipment );
            Register( value.type_equipement, *equipment );
        }
        equipment->Update( value );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Equipments::AddToDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Equipments::AddToDictionary( const Equipment& equipment )
{
    const QString baseName = tools::translate( "Equipments", "Equipments" ) + "/" + equipment.GetName() + "/";

    dico_.Register( *this, baseName + tools::translate( "Equipments", "Available" ), equipment.available_ );
    dico_.Register( *this, baseName + tools::translate( "Equipments", "Unavailable" ), equipment.unavailable_ );
    dico_.Register( *this, baseName + tools::translate( "Equipments", "Repairable" ), equipment.repairable_ );
    dico_.Register( *this, baseName + tools::translate( "Equipments", "In maintenance" ), equipment.inMaintenance_ );
    dico_.Register( *this, baseName + tools::translate( "Equipments", "Prisoner" ), equipment.prisonners_ );
}
