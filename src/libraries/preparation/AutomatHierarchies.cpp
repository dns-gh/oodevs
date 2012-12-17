// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AutomatHierarchies.h"
#include "TacticalHierarchies.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies constructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
AutomatHierarchies::AutomatHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : kernel::MergingTacticalHierarchies( controller, holder, 0 )
    , superior_( superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies destructor
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
AutomatHierarchies::~AutomatHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::ChangeSuperior
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void AutomatHierarchies::ChangeSuperior( kernel::Entity_ABC& superior )
{
    MergingTacticalHierarchies::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::SerializeAttributes
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void AutomatHierarchies::SerializeAttributes( xml::xostream& xos ) const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        const kernel::Entity_ABC* child = it->second;
        if( dynamic_cast< const kernel::Agent_ABC* >( child ) )
            xos << xml::start( "unit" );
        else if( dynamic_cast< const kernel::Ghost_ABC* >( child ) )
            xos << xml::start( "phantom" );
        else
            xos << xml::start( "automat" );
        it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-11-22
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const kernel::InstanciationComplete& ic )
{
    SetSuperior( superior_ );
    MergingTacticalHierarchies::DoUpdate( ic );
}

// $$$$ AGE 2006-11-23: Tout ce bazar est dégueu, a tous les points de vue

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::UpdateSymbolUpward
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void AutomatHierarchies::UpdateSymbolUpward()
{
    level_.clear();
    tools::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        if( const TacticalHierarchies* hierarchies = it.NextElement().Retrieve< TacticalHierarchies >() )
        {
            const std::string childLevel = hierarchies->GetLevel();
            if( level_.empty() && !childLevel.empty() )
                level_ = childLevel;
            else if( !childLevel.empty() )
                level_ = MergingTacticalHierarchies::MaxLevel( level_, childLevel );
        }
    }
    MergingTacticalHierarchies::UpdateSymbolUpward();
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::NotifySuperiorChanged
// Created: NPT 2012-12-14
// -----------------------------------------------------------------------------
void AutomatHierarchies::NotifySuperiorChanged( const kernel::Entity_ABC* newSuperior )
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
// Name: AutomatHierarchies::UpdateLevel
// Created: LDC 2012-04-12
// -----------------------------------------------------------------------------
void AutomatHierarchies::UpdateLevel()
{
    tools::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        if( const TacticalHierarchies* hierarchies = it.NextElement().Retrieve< TacticalHierarchies >() )
        {
            const std::string childLevel = hierarchies->GetLevel();
            if( level_.empty() && !childLevel.empty() )
                level_ = childLevel;
            else if( !childLevel.empty() )
                level_ = MergingTacticalHierarchies::MaxLevel( level_, childLevel );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::MergeSymbol
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
void AutomatHierarchies::MergeSymbol( const kernel::Entity_ABC& entity )
{
    if( const TacticalHierarchies* hierarchies = entity.Retrieve< TacticalHierarchies >() )
    {
        const std::string childLevel = hierarchies->GetLevel();
        if( level_.empty() && !childLevel.empty() )
            level_ = MergingTacticalHierarchies::IncreaseLevel( childLevel );
        else if( !childLevel.empty() )
            level_ = MergingTacticalHierarchies::MaxLevel( level_, MergingTacticalHierarchies::IncreaseLevel( childLevel ) );
    }
    MergingTacticalHierarchies::MergeSymbol( entity );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::GetLevel
// Created: AGE 2006-11-23
// -----------------------------------------------------------------------------
std::string AutomatHierarchies::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::GetSuperior
// Created: SBO 2008-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* AutomatHierarchies::GetSuperior() const
{
    const kernel::Entity_ABC* superior = kernel::MergingTacticalHierarchies::GetSuperior();
    return superior ? superior : superior_;
}
