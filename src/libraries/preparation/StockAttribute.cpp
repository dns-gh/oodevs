// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "StockAttribute.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>
#include <boost/ref.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StockAttribute constructor
// Created: BCI 2011-02-04
// -----------------------------------------------------------------------------
StockAttribute::StockAttribute( kernel::PropertiesDictionary& dico )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute constructor
// Created: BCI 2011-02-04
// -----------------------------------------------------------------------------
StockAttribute::StockAttribute( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, kernel::PropertiesDictionary& dico )
{
    xis >> xml::list( "resource", *this, &StockAttribute::ReadResource, boost::cref( resolver ) );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute destructor
// Created: BCI 2011-02-04
// -----------------------------------------------------------------------------
StockAttribute::~StockAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::SerializeAttributes
// Created: BCI 2011-02-04
// -----------------------------------------------------------------------------
void StockAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "stock" );
    for( StockDotations::const_iterator it = stockDotations_.begin(); it != stockDotations_.end(); ++it )
    {
        xos << xml::start( "resource" )
                << xml::attribute( "type", it->first->GetName() )
                << xml::attribute( "current", it->second.stock_ )
                << xml::attribute( "max", it->second.maxStock_ )
            << xml::end;
    }
    xos << xml::end;
}
// -----------------------------------------------------------------------------
// Name: StockAttribute::ReadResource
// Created: BCI 2011-02-04
// -----------------------------------------------------------------------------
void StockAttribute::ReadResource( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver )
{
    kernel::DotationType* dotationType = resolver.Find( xis.attribute< std::string >( "type" ) );
    if( dotationType )
    {
        StockDotation& stockDotation = stockDotations_[ dotationType ];
        kernel::ReaderHelper( stockDotation.maxStock_, "max", xis );
        kernel::ReaderHelper( stockDotation.stock_, "current", xis );
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::CreateDictionary
// Created: BCI 2011-02-04
// -----------------------------------------------------------------------------
void StockAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    for( StockDotations::const_iterator it = stockDotations_.begin(); it != stockDotations_.end(); ++it )
    {
        dico.Register( *this, tools::translate( "StockAttribute", std::string( "Info/Stock attributes/" + it->first->GetName() + "/Current" ).c_str() ), it->second.stock_ );
        dico.Register( *this, tools::translate( "StockAttribute", std::string( "Info/Stock attributes/" + it->first->GetName() + "/Max" ).c_str() ), it->second.maxStock_ );
    }
}

