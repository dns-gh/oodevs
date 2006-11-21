// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel/Automat_ABC.h"
#include "DotationsItem.h"
#include "Dotation.h"
#include "clients_kernel/DotationType.h"
#include "xeumeuleu/xml.h"

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies constructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
template< typename Superior, typename ConcreteHierarchy >
LogisticHierarchies< Superior, ConcreteHierarchy >::LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver )
    : kernel::EntityHierarchies< kernel::TacticalHierarchies >( controller, entity, 0 )
    , controller_( controller )
    , resolver_( resolver )
    , tc2_( 0 )
    , item_( 0 )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: LogisticHierarchies destructor
// Created: SBO 2006-10-24
// -----------------------------------------------------------------------------
template< typename Superior, typename ConcreteHierarchy >
LogisticHierarchies< Superior, ConcreteHierarchy >::~LogisticHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::GetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename Superior, typename ConcreteHierarchy >
const kernel::Entity_ABC* LogisticHierarchies< Superior, ConcreteHierarchy >::GetSuperior() const
{
    return (const kernel::Automat_ABC*)tc2_;
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SerializeLogistics
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename Superior, typename ConcreteHierarchy >
void LogisticHierarchies< Superior, ConcreteHierarchy >::SerializeLogistics( xml::xostream& xos ) const
{
    kernel::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = it.NextElement();
        xos << xml::start( "subordinate" )
                << xml::attribute( "id", long( entity.GetId() ) )
                << xml::attribute( "link", GetLinkType() );
        kernel::Iterator< const Dotation& > itQuota = ((kernel::Resolver< Dotation >&)entity.Get< ConcreteHierarchy >()).CreateIterator();
        xos << xml::start( "quotas" );
        while( itQuota.HasMoreElements() )
        {
            xos << xml::start( "dotation" );
            itQuota.NextElement().SerializeAttributes( xos );
            xos << xml::end();
        }
        xos << xml::end();
        xos << xml::end();
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::Load
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
template< typename Superior, typename ConcreteHierarchy >
void LogisticHierarchies< Superior, ConcreteHierarchy >::Load( xml::xistream& xis, const Superior& superior )
{
    SetSuperior( superior );
    xis >> xml::start( "quotas" )
            >> xml::list( "dotation", *this, &LogisticHierarchies< Superior, ConcreteHierarchy >::ReadDotation )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::ReadDotation
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
template< typename Superior, typename ConcreteHierarchy >
void LogisticHierarchies< Superior, ConcreteHierarchy >::ReadDotation( xml::xistream& xis )
{
    Dotation* dotation = new Dotation( xis, resolver_ );
    item_->AddDotation( *dotation );
    kernel::Resolver< Dotation >::Register( dotation->type_->GetId(), *dotation );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SetSuperior
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
template< typename Superior, typename ConcreteHierarchy >
void LogisticHierarchies< Superior, ConcreteHierarchy >::SetSuperior( const Superior& automat )
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

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::CreateDictionary
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
template< typename Superior, typename ConcreteHierarchy >
void LogisticHierarchies< Superior, ConcreteHierarchy >::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner, const QString& name )
{
    item_ = new DotationsItem( controller_, owner, dico, name, *(kernel::Resolver< Dotation >*)this );
    dico.Register( *(const kernel::TacticalHierarchies*)this, name + tools::translate( "LogisticHierarchies", "/<Dotations>" ), item_ );
}
