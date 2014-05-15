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
#include "clients_gui/PropertiesDictionary.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/Tools.h"
#include "ENT/ENT_Enums.h"
#include "protocol/Protocol.h"
#include <boost/bind.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Equipments constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Equipments::Equipments( kernel::Entity_ABC& entity, Controller& controller, const tools::Resolver_ABC< kernel::EquipmentType >& resolver, gui::PropertiesDictionary& dico
                       , const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver
                       , const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver
                       , const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver )
    : HierarchicExtension_ABC( automatResolver, formationResolver, teamResolver )
    , entity_    ( entity )
    , controller_( controller )
    , resolver_  ( resolver )
    , dico_      ( dico )
    , property_  ( tools::translate( "Equipments", "Equipments" ) )
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
// Name: Equipments::GetTotalWeight
// Created: NPT 2012-11-16
// -----------------------------------------------------------------------------
float Equipments::GetTotalWeight() const
{
    float weight = 0;
    tools::Iterator< const Equipment& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const Equipment& equipment = it.NextElement();
        weight += equipment.Total() * equipment.type_.GetWeight();
    }
    return weight;
}

// -----------------------------------------------------------------------------
// Name: Equipments::UpdateController
// Created: LDC 2013-01-31
// -----------------------------------------------------------------------------
void Equipments::UpdateController()
{
    controller_.Update( gui::DictionaryUpdated( entity_, property_ ) );
    controller_.Update( static_cast< const kernel::Equipments_ABC& >( *this ) );
}

// -----------------------------------------------------------------------------
// Name: Equipments::CreateMaintenanceConsign
// Created: LDC 2013-01-30
// -----------------------------------------------------------------------------
void Equipments::CreateMaintenanceConsign( const sword::LogMaintenanceHandlingCreation& message )
{
    Equipment* equipment = Find( message.equipement().id() );
    if( !equipment )
    {
        equipment = new Equipment( resolver_.Get( message.equipement().id() ) );
        AddToDictionary( *equipment );
        Register( message.equipement().id(), *equipment );
    }
    equipment->CreateMaintenanceConsign( message );
    UpdateController();
}

// -----------------------------------------------------------------------------
// Name: Equipments::DeleteMaintenanceConsign
// Created: LDC 2013-01-30
// -----------------------------------------------------------------------------
void Equipments::DeleteMaintenanceConsign( int id )
{
    Apply( boost::bind( &Equipment::DeleteMaintenanceConsign, _1, id ) );
    UpdateController();
}

// -----------------------------------------------------------------------------
// Name: Equipments::Update
// Created: LDC 2013-01-30
// -----------------------------------------------------------------------------
void Equipments::Update( const sword::LogMaintenanceHandlingUpdate& message )
{
    Apply( boost::bind( &Equipment::UpdateMaintenanceConsign, _1, boost::cref( message ) ) );
    UpdateController();
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
    const auto& elems = message.equipment_dotations().elem();
    for( auto it = elems.begin(); it != elems.end(); ++it )
    {
        Equipment previous( resolver_.Get( it->type().id() ) );
        if( Equipment* equipment = Find( it->type().id() ) )
            previous = *equipment;
        Equipment current( previous );
        current.Update( *it );
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
    for( auto it = differences.begin(); it != differences.end(); ++it )
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
            {
                RemoveFromDictionary( *equipment );
                Remove( it->type_.GetId() );
            }
        }
    }
    if( kernel::Entity_ABC* superior = GetSuperior() )
        if( Equipments* equipments = static_cast< Equipments* >( superior->Retrieve< Equipments_ABC >() ) )
            equipments->Update( differences );
    UpdateController();
}

// -----------------------------------------------------------------------------
// Name: Equipments::SetSuperior
// Created: SBO 2007-04-11
// -----------------------------------------------------------------------------
void Equipments::SetSuperior( const kernel::Entity_ABC& automat )
{
    std::vector< Equipment > differences;
    differences.reserve( elements_.size() );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        differences.push_back( *it->second );
    if( Equipments* equipments = static_cast< Equipments* >( const_cast< kernel::Entity_ABC& >( automat ).Retrieve< Equipments_ABC >() ) )
        equipments->Update( differences );
    if( kernel::Entity_ABC* previous = GetSuperior() )
        if( Equipments* equipments = static_cast< Equipments* >( previous->Retrieve< Equipments_ABC >() ) )
        {
            std::transform( differences.begin(), differences.end(), differences.begin(), std::negate< Equipment >() );
            equipments->Update( differences );
        }
}

// -----------------------------------------------------------------------------
// Name: Equipments::RemoveFromDictionary
// Created: AGE 2013-03-06
// -----------------------------------------------------------------------------
void Equipments::RemoveFromDictionary( const Equipment& equipment )
{
    dico_.Remove( property_ + "/" + equipment.GetName() );
}

// -----------------------------------------------------------------------------
// Name: Equipments::AddToDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Equipments::AddToDictionary( const Equipment& equipment )
{
    const QString baseName = property_ + "/" + equipment.GetName() + "/";
    dico_.Register( entity_, baseName + tools::translate( "Equipments", "Available" ), equipment.available_ );
    dico_.Register( entity_, baseName + tools::translate( "Equipments", "Unavailable" ), equipment.unavailable_ );
    dico_.Register( entity_, baseName + tools::translate( "Equipments", "Repairable" ), equipment.repairable_ );
    dico_.Register( entity_, baseName + tools::translate( "Equipments", "On site fixable" ), equipment.onSiteFixable_ );
    dico_.Register( entity_, baseName + tools::translate( "Equipments", "In maintenance" ), equipment.inMaintenance_ );
    dico_.Register( entity_, baseName + tools::translate( "Equipments", "Prisoner" ), equipment.prisonners_ );
}

bool Equipments::HasEquipment( const kernel::EquipmentType& type ) const
{
    for( auto it = CreateIterator(); it.HasMoreElements(); )
        if( it.NextElement().GetType().GetId() == type.GetId() )
            return true;
    return false;
}

void Equipments::Visit( const std::function< bool( const kernel::EquipmentType& ) >& functor ) const
{
    for( auto it = CreateIterator(); it.HasMoreElements(); /**/ )
        if( !functor( it.NextElement().type_ ) )
            return;
}