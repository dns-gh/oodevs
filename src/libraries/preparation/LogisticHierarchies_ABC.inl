// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Dotation.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "DotationsItem.h"
#include "xeumeuleu/xml.h"

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies_ABC constructor
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename I >
LogisticHierarchies_ABC< I >::LogisticHierarchies_ABC( kernel::Controller& controller, kernel::Entity_ABC& entity, const kernel::Resolver_ABC< kernel::DotationType, QString >& resolver )
    : kernel::EntityHierarchies< I >( controller, entity, 0 )
    , controller_( controller )
    , resolver_( resolver )
    , item_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies_ABC destructor
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename I >
LogisticHierarchies_ABC< I >::~LogisticHierarchies_ABC()
{
    kernel::Resolver< Dotation >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies_ABC::Load
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename I >
void LogisticHierarchies_ABC< I >::Load( xml::xistream& xis )
{
    xis >> xml::start( "quotas" )
            >> xml::list( "dotation", *this, &LogisticHierarchies_ABC::ReadDotation )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies_ABC::ReadDotation
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename I >
void LogisticHierarchies_ABC< I >::ReadDotation( xml::xistream& xis )
{
    Dotation* dotation = new Dotation( xis, resolver_ );
    item_->AddDotation( *dotation );
    kernel::Resolver< Dotation >::Register( dotation->type_->GetId(), *dotation );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies_ABC::SerializeLogistics
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename I >
void LogisticHierarchies_ABC< I >::SerializeLogistics( xml::xostream& xos ) const
{
    kernel::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = it.NextElement();
        xos << xml::start( "subordinate" )
                << xml::attribute( "id", long( entity.GetId() ) )
                << xml::attribute( "link", GetLinkType() );
        const kernel::Resolver< Dotation >& resolver = static_cast< const LogisticHierarchies_ABC& >( entity.Get< I >() );
        kernel::Iterator< const Dotation& > itQuota = resolver.CreateIterator();
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
// Name: LogisticHierarchies_ABC::CreateDictionary
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
template< typename I >
void LogisticHierarchies_ABC< I >::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& owner, const QString& name )
{
    item_ = new DotationsItem( controller_, owner, dico, name, *(kernel::Resolver< Dotation >*)this );
    dico.Register( *(const kernel::TacticalHierarchies*)this, name + tools::translate( "LogisticHierarchies", "/<Dotations>" ), item_ );
}
