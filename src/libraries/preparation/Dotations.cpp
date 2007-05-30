// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Dotations.h"
#include "Dotation.h"
#include "DotationsItem.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: Dotations constructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotations::Dotations( Controller& controller, Entity_ABC& entity, PropertiesDictionary& dico )
    : controller_( controller )
{
    CreateDictionary( entity, dico );
}

// -----------------------------------------------------------------------------
// Name: Dotations constructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotations::Dotations( xml::xistream& xis, Controller& controller, Entity_ABC& entity, const Resolver_ABC< DotationType, QString >& resolver, PropertiesDictionary& dico )
    : controller_( controller )
{
    CreateDictionary( entity, dico );
    xis >> optional() >> start( "dotations" )
            >> list( "dotation", *this, &Dotations::ReadDotation, resolver )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: Dotations destructor
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
Dotations::~Dotations()
{
    delete item_;
}

// -----------------------------------------------------------------------------
// Name: Dotations::ReadDotation
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void Dotations::ReadDotation( xml::xistream& xis, const Resolver_ABC< DotationType, QString >& resolver )
{
    Dotation* dotation = new Dotation( xis, resolver );
    item_->AddDotation( *dotation );
    Register( dotation->type_->GetId(), *dotation );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Dotations::DoSerialize
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void Dotations::SerializeAttributes( xml::xostream& xos ) const
{
    if( elements_.empty() )
        return;
    xos << start( "dotations" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        xos << start( "dotation" );
        it->second->SerializeAttributes( xos );
        xos << end();
    }
    xos << end();
}

// -----------------------------------------------------------------------------
// Name: Dotations::CreateDictionary
// Created: SBO 2006-11-10
// -----------------------------------------------------------------------------
void Dotations::CreateDictionary( Entity_ABC& entity, PropertiesDictionary& dico )
{
    item_ = new DotationsItem( controller_, entity, dico, tools::translate( "Dotations", "Dotations" ), *(Resolver< Dotation >*)this );
    dico.Register( entity, tools::translate( "Dotations", "Dotations/Dotations" ), item_ );
}
