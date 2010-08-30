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
#include "Entities\Agents\Units\Dotations\PHY_DotationType.h"
#include "Entities\Agents\Units\Dotations\PHY_DotationCategory.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeConstruction.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "HLA/HLA_UpdateFunctor.h"
#include <hla/AttributeIdentifier.h>
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( StockAttribute )

using namespace hla;

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
    xis >> xml::list( "dotation", *this, &StockAttribute::LoadDotation );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute constructor
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
StockAttribute::StockAttribute( const PHY_DotationCategory& dotation, unsigned int nFullNbrDotation )
{
    stock_[ &dotation ] = std::make_pair( 0, nFullNbrDotation );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::LoadDotation
// Created: JCR 2009-06-03
// -----------------------------------------------------------------------------
void StockAttribute::LoadDotation( xml::xistream& xis )
{
    const PHY_DotationCategory* dotation = PHY_DotationType::FindDotationCategory( xis.attribute< std::string >( "type" ) );
    if( dotation )
    {
        unsigned int current = xis.attribute< unsigned int >( "current" );
        unsigned int max = xis.attribute< unsigned int >( "max" );
        stock_[ dotation ] = std::make_pair( current, max );
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
    // std::copy( rhs.stock_.begin(), rhs.stock_.end(), stock_.begin() );
    stock_ = rhs.stock_;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::load
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void StockAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    unsigned size = 0;
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this )
       >> size;
    while ( size-- )
    {
        std::string type;
        uint current, max;
        ar >> type
           >> current >> max;
        const PHY_DotationCategory* dotation = PHY_DotationType::FindDotationCategory( type );
        if( !dotation )
           throw std::runtime_error( "Unknown dotation category - " + type + " - " );
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::save
// Created: JCR 2008-09-15
// -----------------------------------------------------------------------------
void StockAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    unsigned size = stock_.size();
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << size;
    for ( CIT_DotationProgress it = stock_.begin(); it != stock_.end(); ++it )
    {
        ar << it->first->GetName()
           << it->second.first << it->second.second;
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::WriteODB
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
void StockAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "stock" );
    for ( CIT_DotationProgress it = stock_.begin(); it != stock_.end(); ++it )
    {
        xos << xml::start( "dotation" )
            << xml::attribute( "type", it->first->GetName() )
            << xml::attribute( "current", it->second.first )
            << xml::attribute( "max", it->second.second )
            << xml::end;
    }
    xos << xml::end;
}


// -----------------------------------------------------------------------------
// Name: StockAttribute::Send
// Created: JCR 2009-06-05
// -----------------------------------------------------------------------------
void StockAttribute::Send( Common::MsgObjectAttributeStock& attribute, bool send_max ) const
{
    int i = 0;
    for( CIT_DotationProgress it = stock_.begin(); it != stock_.end(); ++it, ++i )
    {
        Common::StockResource* resource = attribute.add_resources();
        resource->mutable_resource()->set_id( it->first->GetType().GetID() );
        resource->set_current( it->second.first );
        if( send_max )
            resource->set_maximum( it->second.second );
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void StockAttribute::SendFullState( Common::MsgObjectAttributes& asn ) const
{
    if( stock_.size() > 0 )
        Send( *asn.mutable_stock(), true );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void StockAttribute::SendUpdate( Common::MsgObjectAttributes& asn ) const
{
    if( NeedUpdate( eOnCreation ) | NeedUpdate( eOnUpdate) )
    {
        Send( *asn.mutable_stock(), false );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::OnMagicActionUpdate
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void StockAttribute::OnUpdate( const Common::MsgObjectAttributes& asn )
{
    if( asn.has_stock() )
    {
        // Shold not be called
        NotifyAttributeUpdated( eOnUpdate );
    }
}

namespace
{
    struct IsFullPredicate
    {
        template< typename T >
        bool operator()( const T& it ) const
        {
            return it.second.first < it.second.second;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::UpdateConstruction
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
bool StockAttribute::IsFull() const
{
    return std::find_if( stock_.begin(), stock_.end(), IsFullPredicate() ) == stock_.end();
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Fill
// Created: JCR 2009-06-03
// -----------------------------------------------------------------------------
unsigned int StockAttribute::Supply( const PHY_DotationCategory& category, unsigned int quantity )
{
    T_pair stock = stock_[ &category ];
    unsigned int filled = std::max( stock.first + quantity, stock.second );
    stock_[ &category ].first = filled;
    return filled - stock.first;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Distribute
// Created: JCR 2009-06-04
// -----------------------------------------------------------------------------
unsigned int StockAttribute::Distribute( const PHY_DotationCategory& category, unsigned int quantity )
{
    IT_DotationProgress it = stock_.find( &category );
    if( it == stock_.end() )
        return 0;
    unsigned int stock = it->second.first;
    it->second.first = stock - std::min( quantity, stock );
    return it->second.first - stock;
}

namespace
{
    struct SelectPredicate
    {
        SelectPredicate( std::vector< std::pair< const PHY_DotationCategory*, unsigned int > >& selection, bool select_full )
            : selection_  ( selection )
            , select_full_( select_full )
        {
            // NOTHING
        }
        template< typename T >
        void operator()( const T& it ) const
        {
            IsFullPredicate is_full;
            if( select_full_ || ! is_full( it ) )
                selection_.push_back( std::make_pair( it.first, it.second.second - it.second.first ) );
        }
        std::vector< std::pair< const PHY_DotationCategory*, unsigned int > >& selection_;
        bool select_full_;
    };
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::SelectDotations
// Created: JCR 2009-06-04
// -----------------------------------------------------------------------------
void StockAttribute::SelectDotations( std::vector< std::pair< const PHY_DotationCategory*, unsigned int > >& selection, bool select_full ) const
{
    std::for_each( stock_.begin(), stock_.end(), SelectPredicate( selection, select_full ) );
}
