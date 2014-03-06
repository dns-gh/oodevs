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
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Stocks constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
Stocks::Stocks( Controller& controller, Entity_ABC& entity, gui::PropertiesDictionary& dico )
    : controller_( controller )
{
    CreateDictionary( entity, dico );
}

// -----------------------------------------------------------------------------
// Name: Stocks constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
Stocks::Stocks( xml::xistream& xis, Controller& controller, Entity_ABC& entity, const tools::Resolver_ABC< DotationType, std::string >& resolver, gui::PropertiesDictionary& dico )
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
    const std::string dotationName = xis.attribute< std::string >( "name" );
    const kernel::DotationType* pDotationType = resolver.Find( dotationName );
    if( pDotationType )
    {
        Dotation* dotation = new Dotation( xis, resolver );
        item_->AddDotation( *dotation );
        Register( dotation->type_.GetId(), *dotation );
        controller_.Update( *this );
    }
    else
        invalidDotations_.push_back( dotationName );
}

// -----------------------------------------------------------------------------
// Name: Stocks::SetDotation
// Created: MMC 2011-08-09
// -----------------------------------------------------------------------------
void Stocks::SetDotation( const kernel::DotationType& type, unsigned int quantity, bool add )
{
    if( !item_ )
        return;

    Dotation* pDotation = Find( type.GetId() );
    if( pDotation )
    {
        if( add )
            pDotation->quantity_ += quantity;
        else
        {
            pDotation->quantity_ = quantity;
            if( quantity == 0 )
            {
                item_->RemoveDotation( type );
                Remove( type.GetId() );
            }
        }
    }
    else if( quantity > 0 )
    {
        Dotation* pDotation = new Dotation( type, quantity );
        item_->AddDotation( *pDotation );
        Register( pDotation->type_.GetId(), *pDotation );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Stocks::DeleteAll
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void Stocks::DeleteAll()
{
    item_->Clear();
    tools::Resolver< Dotation >::Clear();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Stocks::SerializeAttributes
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void Stocks::SerializeAttributes( xml::xostream& xos ) const
{
    invalidDotations_.clear();
    if( !IsToSerialize() )
        return;
    xos << xml::start( "stocks" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->quantity_ > 0 )
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
void Stocks::CreateDictionary( Entity_ABC& entity, gui::PropertiesDictionary& dico )
{
    item_ = new DotationsItem( controller_, entity, dico, tools::translate( "Stocks", "Stocks" ), *static_cast< Resolver< Dotation >* >( this ), true );
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
// Name: Stocks::HasDotationType
// Created: ABR 2012-05-03
// -----------------------------------------------------------------------------
bool Stocks::HasDotationType( const kernel::DotationType& dotationType ) const
{
    if( item_ )
    {
        tools::Iterator< const Dotation& > dotationIt = item_->CreateIterator();
        while( dotationIt.HasMoreElements() )
        {
            const Dotation& dotation = dotationIt.NextElement();
            if( dotation.type_.GetId() == dotationType.GetId() && dotation.quantity_ > 0 )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: Stocks::ComputeWeightAndVolume
// Created: JSR 2012-03-08
// -----------------------------------------------------------------------------
void Stocks::ComputeWeightAndVolume( const std::string& dotationNature, double& weight, double& volume ) const
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

// -----------------------------------------------------------------------------
// Name: Stocks::IsToSerialize
// Created: MMC 2012-03-23
// -----------------------------------------------------------------------------
bool Stocks::IsToSerialize() const
{
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
        if( it->second->quantity_ > 0 )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: Stocks::GetInvalidDotations
// Created: MMC 2012-05-31
// -----------------------------------------------------------------------------
const std::vector< std::string >& Stocks::GetInvalidDotations() const
{
    return invalidDotations_;
}
