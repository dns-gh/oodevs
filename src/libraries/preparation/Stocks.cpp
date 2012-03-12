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
#include "clients_kernel/Tools.h"
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
// Name: Stocks::Clear
// Created: LGY 2012-03-05
// -----------------------------------------------------------------------------
void Stocks::Clear()
{
    item_->Clear();
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Stocks::ReadDotation
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void Stocks::ReadDotation( xml::xistream& xis, const tools::Resolver_ABC< DotationType, std::string >& resolver )
{
    Dotation* dotation = new Dotation( xis, resolver );
    item_->AddDotation( *dotation );
    Register( dotation->type_.GetId(), *dotation );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Stocks::SetDotation
// Created: MMC 2011-08-09
// -----------------------------------------------------------------------------
void Stocks::SetDotation( const kernel::DotationType& type, unsigned int quantity, bool add )
{
    Dotation* pDotation = Find( type.GetId() );
    if( pDotation )
    {
        if( add )
            pDotation->quantity_ += quantity;
        else
            pDotation->quantity_ = quantity;
    }
    else
    {
        Dotation* pDotation = new Dotation( type, quantity );
        item_->AddDotation( *pDotation );
        Register( pDotation->type_.GetId(), *pDotation );
        controller_.Update( *this );
    }
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

// -----------------------------------------------------------------------------
// Name: Stocks::HasDotations
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
bool Stocks::HasDotations() const
{
    return ( item_ ) ? item_->CountDotations() != 0 : false;
}

// -----------------------------------------------------------------------------
// Name: Stocks::ComputeWeightAndVolume
// Created: JSR 2012-03-08
// -----------------------------------------------------------------------------
void Stocks::ComputeWeightAndVolume( const std::string& dotationNature, double& weight, double& volume )
{
    weight = 0;
    volume = 0;
    if( item_ )
    {
        tools::Iterator< const Dotation& > it = item_->CreateIterator();
        while( it.HasMoreElements() )
        {
            const Dotation& dotation = it.NextElement();
            if( dotation.type_.GetNature() == dotationNature )
            {
                weight += dotation.quantity_ * dotation.type_.GetUnitWeight();
                volume += dotation.quantity_ * dotation.type_.GetUnitVolume();
            }
        }
    }
}
