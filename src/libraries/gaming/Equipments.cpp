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
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Equipments constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Equipments::Equipments( Controller& controller, const Resolver_ABC< EquipmentType >& resolver, PropertiesDictionary& dico, kernel::Entity_ABC& holder )
    : controller_( controller )
    , resolver_( resolver )
    , dico_( dico )
    , holder_( holder )
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

    std::vector< Equipment > differences;
    uint nSize = message.dotation_eff_materiel.n;
    while ( nSize > 0 )
    {
        const ASN1T_DotationEquipement& value = message.dotation_eff_materiel.elem[ --nSize ];
        Equipment previous( resolver_.Get( value.type_equipement ) );
        if( Equipment* equipment = Find( value.type_equipement ) )
            previous = *equipment;
        Equipment current( previous );
        current.Update( value );
        differences.push_back( current - previous );
    }
    Update( differences );
}

// -----------------------------------------------------------------------------
// Name: Equipments::Update
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Equipments::Update( const std::vector< Equipment >& differences )
{
    for( std::vector< Equipment >::const_iterator it = differences.begin(); it != differences.end(); ++it )
    {
        Equipment* equipment = Find( it->type_.GetId() );
        if( !equipment )
        {
            equipment = new Equipment( *it );
            AddToDictionary( *equipment );
            Register( it->type_.GetId(), *equipment );
        }
        else
            *equipment = *equipment + *it;
    }
    if( const kernel::Entity_ABC* superior = holder_.Get< kernel::TacticalHierarchies >().GetSuperior() )
        if( Equipments* equipments = const_cast< Equipments* >( superior->Retrieve< Equipments >() ) )
            equipments->Update( differences );
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
