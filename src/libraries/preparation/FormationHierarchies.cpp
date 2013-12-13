// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationHierarchies.h"
#include "TacticalHierarchies.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/SymbolFactory.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: FormationHierarchies constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationHierarchies::FormationHierarchies( kernel::Controller& controller, kernel::Formation_ABC& holder, kernel::Entity_ABC* superior, kernel::SymbolFactory& factory )
    : gui::MergingTacticalHierarchies( controller, holder, 0 )
    , superior_( superior )
    , level_   ( factory.CreateLevelSymbol( ENT_Tr::ConvertFromNatureLevel( holder.GetLevel() ) ) )
    , symbolFactory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationHierarchies::~FormationHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string FormationHierarchies::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::SetLevel
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
void FormationHierarchies::SetLevel( E_NatureLevel level )
{
    level_ = symbolFactory_.CreateLevelSymbol( ENT_Tr::ConvertFromNatureLevel( level ) );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::ChangeSuperior
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void FormationHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    gui::MergingTacticalHierarchies::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::UnregisterParent
// Created: MMC 2011-05-30
// -----------------------------------------------------------------------------
void FormationHierarchies::UnregisterParent()
{
    superior_ = 0;
    MergingTacticalHierarchies::UnregisterParent();
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::NotifySuperiorChanged
// Created: NPT 2012-12-14
// -----------------------------------------------------------------------------
void FormationHierarchies::NotifySuperiorChanged( const kernel::Entity_ABC* newSuperior )
{
    if( newSuperior && newSuperior->Retrieve< kernel::TacticalHierarchies >() )
    {
        tools::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::Entity_ABC& entity = it.NextElement();
            if( const ::TacticalHierarchies* hierarchies = static_cast< const ::TacticalHierarchies* >( entity.Retrieve< kernel::TacticalHierarchies >() ) )
                const_cast< ::TacticalHierarchies* >( hierarchies )->NotifySuperiorChanged( hierarchies->GetSuperior() );
        }
        UpdateSymbolUpward();
    }
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::SerializeAttributes
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void FormationHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        const kernel::Entity_ABC* child = it->second;
        if( dynamic_cast< const kernel::Ghost_ABC* >( child ) )
            xos << xml::start( "phantom" );
        else if( dynamic_cast< const kernel::Automat_ABC* >( child ) )
            xos << xml::start( "automat" );
        else
            xos << xml::start( "formation" );
        it->second->GetInterfaces().Apply( &Serializable_ABC::SerializeAttributes, xos );
        it->second->GetInterfaces().Apply( &kernel::Ghost_ABC::SerializeGhostAttributes, xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::SerializeLogistics
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void FormationHierarchies::SerializeLogistics( xml::xostream& xos ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        it->second->GetInterfaces().Apply( & Serializable_ABC::SerializeLogistics, xos );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::DoUpdate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void FormationHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    SetSuperior( superior_ );
    gui::MergingTacticalHierarchies::DoUpdate( ic );
}

// -----------------------------------------------------------------------------
// Name: FormationHierarchies::GetSuperior
// Created: SBO 2008-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* FormationHierarchies::GetSuperior() const
{
    const kernel::Entity_ABC* superior = gui::MergingTacticalHierarchies::GetSuperior();
    return superior ? superior : superior_;
}
