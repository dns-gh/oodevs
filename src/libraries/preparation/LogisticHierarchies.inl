// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel/Automat_ABC.h"
#include "xeumeuleu/xml.h"

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
template< typename Superior >
LogisticHierarchies< Superior >::LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver )
    : kernel::EntityHierarchies< kernel::TacticalHierarchies >( controller, entity )
    , controller_( controller )
    , resolver_( resolver )
    , tc2_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: LogisticHierarchies destructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
template< typename Superior >
LogisticHierarchies< Superior >::~LogisticHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::GetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename Superior >
const kernel::Entity_ABC* LogisticHierarchies< Superior >::GetSuperior() const
{
    return (const kernel::Automat_ABC*)tc2_;
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SerializeLogistics
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename Superior >
void LogisticHierarchies< Superior >::SerializeLogistics( xml::xostream& xos ) const
{
    kernel::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = it.NextElement();
        xos << xml::start( "subordinate" )
                << xml::attribute( "id", long( entity.GetId() ) )
                << xml::attribute( "link", GetLinkType() )
            << xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename Superior >
template< typename ConcreteHierarchy >
void LogisticHierarchies< Superior >::SetSuperior( const Superior& automat )
{
    kernel::Automat_ABC* tc2 = const_cast< kernel::Automat_ABC* >( (const kernel::Automat_ABC*)tc2_ );
    if( tc2 )
        if( ConcreteHierarchy* superiorHierarchy = tc2->Retrieve< ConcreteHierarchy >() )
            superiorHierarchy->RemoveSubordinate( GetEntity() );
    tc2_ = automat;
    tc2 = const_cast< kernel::Automat_ABC* >( (const kernel::Automat_ABC*)tc2_ );
    if( tc2 )
        if( ConcreteHierarchy* superiorHierarchy = tc2->Retrieve< ConcreteHierarchy >() )
            superiorHierarchy->AddSubordinate( const_cast< kernel::Entity_ABC& >( GetEntity() ) );
}