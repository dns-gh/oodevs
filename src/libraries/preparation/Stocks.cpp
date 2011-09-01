// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Stocks.h"
#include "Dotation.h"
#include "DotationsItem.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Stocks constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
Stocks::Stocks( Controller& controller, Entity_ABC& entity, PropertiesDictionary& dico )
    : controller_( controller )
{
    CreateDictionary( entity, dico );
}

// -----------------------------------------------------------------------------
// Name: Stocks constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
Stocks::Stocks( xml::xistream& xis, Controller& controller, Entity_ABC& entity, const tools::Resolver_ABC< DotationType, std::string >& resolver, PropertiesDictionary& dico )
    : controller_( controller )
{
    CreateDictionary( entity, dico );
    xis >> xml::optional >> xml::start( "stocks" )
            >> xml::list( "resource", *this, &Stocks::ReadDotation, resolver )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: Stocks destructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
Stocks::~Stocks()
{
    delete item_;
}

// -----------------------------------------------------------------------------
// Name: Stocks::ReadDotation
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void Stocks::ReadDotation( xml::xistream& xis, const tools::Resolver_ABC< DotationType, std::string >& resolver )
{
    Dotation* dotation = new Dotation( xis, resolver );
    item_->AddDotation( *dotation );
    Register( dotation->type_->GetId(), *dotation );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Stocks::AddDotation
// Created: MMC 2011-08-09
// -----------------------------------------------------------------------------
void Stocks::AddDotation( Dotation* pNewDotation )
{
    if ( !pNewDotation )
        return;

    Dotation* pDotation = Find( pNewDotation->type_->GetId() );
    if ( pDotation )
    {
        *pDotation = *pNewDotation;
        delete pNewDotation;
        return;
    }

    item_->AddDotation( *pNewDotation );
    Register( pNewDotation->type_->GetId(), *pNewDotation );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Stocks::SerializeAttributes
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void Stocks::SerializeAttributes( xml::xostream& xos ) const
{
    if( elements_.empty() )
        return;
    xos << xml::start( "stocks" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << xml::start( "resource" );
        it->second->SerializeAttributes( xos );
        xos << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: Stocks::CreateDictionary
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void Stocks::CreateDictionary( Entity_ABC& entity, PropertiesDictionary& dico )
{
    item_ = new DotationsItem( controller_, entity, dico, tools::translate( "Stocks", "Stocks" ), *(Resolver< Dotation >*)this );
    dico.Register( entity, tools::translate( "Stocks", "Stocks/Stocks" ), item_ );
}
