// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "StockAttribute.h"
#include "Object.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "protocol/Protocol.h"
#include <boost/lexical_cast.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( StockAttribute )

// -----------------------------------------------------------------------------
// Name: StockAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
StockAttribute::StockAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockAttribute constructor
// Created: JCR 2009-06-04
// -----------------------------------------------------------------------------
StockAttribute::StockAttribute( xml::xistream& xis )
{
    xis >> xml::list( "resource", *this, &StockAttribute::LoadDotation );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::StockAttribute
// Created: BCI 2011-03-31
// -----------------------------------------------------------------------------
StockAttribute::StockAttribute( const sword::MissionParameter_Value& attributes )
{
    const sword::MissionParameter_Value& parameterDotations = attributes.list( 1 );
    for( int i=0; i<parameterDotations.list_size(); ++i )
    {
        const sword::MissionParameter_Value& parameterDotation = parameterDotations.list( i );
        const PHY_DotationCategory* dotationType = PHY_DotationType::FindDotationCategory( parameterDotation.list( 0 ).identifier() );
        if( !dotationType )
            throw MASA_EXCEPTION( "Unknow dotation type id " + boost::lexical_cast< std::string >( parameterDotation.list( 0 ).identifier() ) );
        StockDotation& dotation = stockDotations_[ dotationType ];
        dotation.stock_ = parameterDotation.list( 1 ).quantity();
        dotation.maxStock_ = parameterDotation.list( 2 ).quantity();
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::LoadDotation
// Created: JCR 2009-06-03
// -----------------------------------------------------------------------------
void StockAttribute::LoadDotation( xml::xistream& xis )
{
    const PHY_DotationCategory* dotationType = PHY_DotationType::FindDotationCategory( xis.attribute< std::string >( "type" ) );
    if( dotationType )
    {
        StockDotation& dotation = stockDotations_[ dotationType ];
        xis >> xml::attribute( "current", dotation.stock_  );
        xis >> xml::attribute( "max", dotation.maxStock_  );
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
StockAttribute::~StockAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Instanciate
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void StockAttribute::Instanciate( DEC_Knowledge_Object& /*object*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void StockAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< StockAttribute, StockAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
StockAttribute& StockAttribute::operator=( const StockAttribute& rhs )
{
    stockDotations_ = rhs.stockDotations_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::load
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void StockAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    std::size_t size;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> size;
    while( size-- )
    {
        const PHY_DotationCategory* dotation;
        ar >> dotation;
        StockDotation& stockDotation = stockDotations_[ dotation ];
        ar >> stockDotation.stock_;
        ar >> stockDotation.maxStock_;
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void StockAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    std::size_t size = stockDotations_.size();
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << size;
    for( auto it = stockDotations_.begin(); it != stockDotations_.end(); ++it )
    {
        ar << it->first;
        ar << it->second.stock_;
        ar << it->second.maxStock_;
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::WriteODB
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void StockAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "stock" );
    for( auto it = stockDotations_.begin(); it != stockDotations_.end(); ++it )
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
// Name: StockAttribute::Send
// Created: JCR 2009-06-05
// -----------------------------------------------------------------------------
void StockAttribute::Send( sword::ObjectAttributeStock& attribute ) const
{
    for( auto it = stockDotations_.begin(); it != stockDotations_.end(); ++it )
    {
        sword::StockResource* resource = attribute.add_resources();
        resource->mutable_resource()->set_id( it->first->GetMosID() );
        resource->set_current( static_cast< int >( it->second.stock_ ) );
        resource->set_maximum( static_cast< int >( it->second.maxStock_ ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void StockAttribute::SendFullState( sword::ObjectAttributes& asn ) const
{
    if( !stockDotations_.empty() )
        Send( *asn.mutable_stock() );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::SendUpdate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
bool StockAttribute::SendUpdate( sword::ObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnUpdate) )
    {
        Send( *asn.mutable_stock() );
        Reset( eOnUpdate );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::OnUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void StockAttribute::OnUpdate( const sword::ObjectAttributes& asn )
{
    if( asn.has_stock() )
    {
        // Shold not be called
        NotifyAttributeUpdated( eOnUpdate );
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::IsFull
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
bool StockAttribute::IsFull() const
{
    for( auto it = stockDotations_.begin(); it != stockDotations_.end(); ++it )
        if( it->second.stock_ < it->second.maxStock_ )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::IsEmpty
// Created: BCI 2011-02-08
// -----------------------------------------------------------------------------
bool StockAttribute::IsEmpty() const
{
    for( auto it = stockDotations_.begin(); it != stockDotations_.end(); ++it )
        if( it->second.stock_ > 0 )
            return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Fill
// Created: JCR 2009-06-03
// -----------------------------------------------------------------------------
double StockAttribute::Supply( const PHY_DotationCategory& category, double quantity )
{
    StockDotation& stockDotation = stockDotations_[ &category ];
    double current = stockDotation.stock_;
    double filled = std::min( current + quantity, stockDotation.maxStock_ );
    if( stockDotation.stock_ != filled )
    {
        stockDotation.stock_ = filled;
        NotifyAttributeUpdated( eOnUpdate );
    }
    return filled - current;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Distribute
// Created: JCR 2009-06-04
// -----------------------------------------------------------------------------
double StockAttribute::Distribute( const PHY_DotationCategory& category, double quantity )
{
    auto it = stockDotations_.find( &category );
    if( it == stockDotations_.end() )
        return 0;
    double& oldStock = it->second.stock_;
    double diff = std::min( quantity, oldStock );
    if( diff != 0 )
    {
        oldStock -= diff;
        NotifyAttributeUpdated( eOnUpdate );
        return diff;
    }
    else
        return 0;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::CanDistribute
// Created: BCI 2011-02-10
// -----------------------------------------------------------------------------
bool StockAttribute::CanDistribute( const PHY_DotationCategory& category ) const
{
    auto it = stockDotations_.find( &category );
    if( it == stockDotations_.end() )
        return false;
    else
        return it->second.stock_ > 0;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::CanBeSuppliedWith
// Created: BCI 2011-02-10
// -----------------------------------------------------------------------------
bool StockAttribute::CanBeSuppliedWith( const PHY_DotationCategory& category ) const
{
    auto it = stockDotations_.find( &category );
    if( it == stockDotations_.end() )
        return false;
    else
        return it->second.stock_ < it->second.maxStock_;
}

namespace
{
    struct SelectPredicate
    {
        SelectPredicate( std::vector< std::pair< const PHY_DotationCategory*, double > >& selection, bool select_full )
            : selection_  ( &selection )
            , select_full_( select_full )
        {
            // NOTHING
        }
        template< typename T >
        void operator()( const T& it ) const
        {
            if( select_full_ || it.second.stock_ < it.second.maxStock_ )
                selection_->push_back( std::make_pair( it.first, it.second.maxStock_ - it.second.stock_ ) );
        }
        std::vector< std::pair< const PHY_DotationCategory*, double > >* selection_;
        bool select_full_;
    };
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::SelectDotations
// Created: JCR 2009-06-04
// $$$$ BCI 2011-02-08: renommé en DeprecatedSelectDotations, je ne vois pas bien quel est l'usage cette méthode...
// -----------------------------------------------------------------------------
void StockAttribute::DeprecatedSelectDotations( std::vector< std::pair< const PHY_DotationCategory*, double > >& selection, bool select_full ) const
{
    std::for_each( stockDotations_.begin(), stockDotations_.end(), SelectPredicate( selection, select_full ) );
}
