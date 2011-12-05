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
#include "Equipment.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Enums_Gen.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Equipments constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Equipments::Equipments( Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, PropertiesDictionary& dico
                       , const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver
                       , const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver
                       , const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : HierarchicExtension_ABC( automatResolver, formationResolver, teamResolver )
    , controller_( controller )
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
void Equipments::DoUpdate( const sword::UnitAttributes& message )
{
    if( !message.has_equipment_dotations() )
        return;

    std::vector< Equipment > differences;
    uint nSize = message.equipment_dotations().elem_size();
    while( nSize > 0 )
    {
        const sword::EquipmentDotations_EquipmentDotation& value = message.equipment_dotations().elem( --nSize );
        Equipment previous( resolver_.Get( value.type().id() ) );
        if( Equipment* equipment = Find( value.type().id() ) )
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
        {
            *equipment = *equipment + *it;
            if( equipment->Total() == 0 )
                Remove( it->type_.GetId() );
        }
    }
    if( const kernel::Entity_ABC* superior = GetSuperior() )
        if( Equipments* equipments = const_cast< Equipments* >( superior->Retrieve< Equipments >() ) )
            equipments->Update( differences );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Equipments::SetSuperior
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Equipments::SetSuperior( const kernel::Entity_ABC& automat )
{
    std::vector< Equipment > differences;
    differences.reserve( elements_.size() );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        differences.push_back( *it->second );
    if( Equipments* equipments = const_cast< Equipments* >( automat.Retrieve< Equipments >() ) )
        equipments->Update( differences );

    if( const kernel::Entity_ABC* previous = GetSuperior() )
        if( Equipments* equipments = const_cast< Equipments* >( previous->Retrieve< Equipments >() ) )
        {
            std::transform( differences.begin(), differences.end(), differences.begin(), std::negate< Equipment >() );
            equipments->Update( differences );
        }
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
    dico_.Register( *this, baseName + tools::translate( "Equipments", "On site fixable" ), equipment.onSiteFixable_ );
    dico_.Register( *this, baseName + tools::translate( "Equipments", "In maintenance" ), equipment.inMaintenance_ );
    dico_.Register( *this, baseName + tools::translate( "Equipments", "Prisoner" ), equipment.prisonners_ );
}
