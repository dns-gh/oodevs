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
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: Stocks constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
Stocks::Stocks( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Stocks constructor
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
Stocks::Stocks( xml::xistream& xis, kernel::Controller& controller, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver )
    : controller_( controller )
{
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Stocks::Clear
// Created: LGY 2012-03-05
// -----------------------------------------------------------------------------
void Stocks::Clear()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: Stocks::ReadDotation
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void Stocks::ReadDotation( xml::xistream& xis, const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver )
{
    const std::string dotationName = xis.attribute< std::string >( "name" );
    const kernel::DotationType* pDotationType = resolver.Find( dotationName );
    if( pDotationType )
    {
        Dotation* dotation = new Dotation( xis, resolver );
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
    Dotation* pDotation = Find( type.GetId() );
    if( quantity == 0 && pDotation && !add )
        Remove( type.GetId() );
    else if( quantity > 0 )
    {
        if( pDotation )
            pDotation->quantity_ = quantity + ( add ? pDotation->quantity_ : 0 );
        else
        {
            Dotation* pDotation = new Dotation( type, quantity );
            Register( pDotation->type_.GetId(), *pDotation );
        }
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Stocks::DeleteAll
// Created: SBO 2006-11-27
// -----------------------------------------------------------------------------
void Stocks::DeleteAll()
{
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
// Name: Stocks::HasDotations
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
bool Stocks::HasDotations() const
{
    return Count() != 0;
}

// -----------------------------------------------------------------------------
// Name: Stocks::HasDotationType
// Created: ABR 2012-05-03
// -----------------------------------------------------------------------------
bool Stocks::HasDotationType( const kernel::DotationType& dotationType ) const
{
    auto dotationIt = CreateIterator();
    while( dotationIt.HasMoreElements() )
    {
        const Dotation& dotation = dotationIt.NextElement();
        if( dotation.type_.GetId() == dotationType.GetId() && dotation.quantity_ > 0 )
            return true;
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
    auto it = CreateIterator();
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
