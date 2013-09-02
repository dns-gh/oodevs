// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RawDataHazardContourGroup.h"
#include "AttributesUpdater.h"
#include "ObjectListenerComposite.h"
#include "SerializationTools.h"
#include "TacticalObject_ABC.h"
#include "FOM_Serializer_ABC.h"
#include "rpr/EntityType.h"
#include "rpr/EntityIdentifier.h"

#pragma warning( push, 0 )
#include "protocol/proto/common.pb.h"
#pragma warning( pop )

#include <geometry/Types.h>

#include <hla/Deserializer_ABC.h>
#include <hla/Serializer_ABC.h>
#include <hla/AttributeIdentifier.h>

#include <boost/bind.hpp>

#include <algorithm>
#include <numeric>
#include <sstream>

using namespace plugins::hla;

namespace
{

void ReadNothing( ::hla::Deserializer_ABC& /*deserializer*/, const std::string& /*identifier*/, ObjectListener_ABC& /*listener*/ )
{
    // NOTHING
}
void ReadMaterial( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, uint16_t& material)
{
    deserializer >> material;
    std::stringstream ss;
    ss << "4 0 225 1 120 " << material / 250 << " " << material % 250; // FIXME AHC : configure DIS prefix
    rpr::EntityType type( ss.str() );
    listener.TypeChanged( identifier, type );
}
void ReadTime( ::hla::Deserializer_ABC& deserializer, const std::string& , ObjectListener_ABC& , uint64_t& time)
{
    deserializer >> time;
}
void ReadHazardType(::hla::Deserializer_ABC& deserializer, const std::string& , ObjectListener_ABC& , uint8_t hazardType )
{
    deserializer >> hazardType;
}
void ReadContours( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, std::vector< RawDataHazardContour >& contours )
{
    contours.clear();
    uint32_t sz=0;
    deserializer >> sz;
    contours.resize( sz );
    if( contours.size() == 0 )
        return;
    uint32_t padd;
    deserializer >> padd;
    deserializer >> contours;
    std::vector< geometry::Polygon2d > polys( contours.size() );
    geometry::Rectangle2d box;
    for( std::size_t i=0;i<contours.size(); ++i)
    {
        const std::vector< rpr::WorldLocation >& locs=contours[i].locations;
        for( std::size_t j=0; j<locs.size(); ++j )
        {
            const rpr::WorldLocation& loc = locs[j];
            polys[i].Add( geometry::Point2d( loc.Longitude(), loc.Latitude() ) );
        }
        if( box.IsEmpty() ) // assumes first is largest
            box  = polys[i].BoundingBox();
    }
    const int mx = 100, my=100;
    const double incx = (box.Right() - box.Left())/mx;
    const double incy = (box.Top() - box.Bottom())/my;
    std::vector< ObjectListener_ABC::PropagationData > data;
    for( int i=0; i<my; ++i )
    {
		const double y = box.Top() - i * incy;
        for( int j=0; j<mx; ++j )
        {
			const double x = box.Left() + j * incx;
            bool found=false;
            for(int32_t j=static_cast<int32_t>(contours.size()-1); j>=0; --j)
            {
                const geometry::Point2d pt(x,y);
                if( polys[j].IsInside(pt) )
                {
                    data.push_back( ObjectListener_ABC::PropagationData( y, x, contours[j].exposureLevel ) );
                    found = true;
                    break;
                }
            }
            if( !found )
                data.push_back( ObjectListener_ABC::PropagationData(y,x,-9999));
        }
    }
    listener.PropagationChanged( identifier, data, (int)mx, (int)my, box.Left(), box.Bottom(), incx, incy );

}

}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup constructor
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
RawDataHazardContourGroup::RawDataHazardContourGroup( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier /*force*/, const rpr::EntityType& /*type*/,
        const rpr::EntityIdentifier& /*entityId*/, const std::string& rtiId )
    : object_( &object )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( rtiId )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
    object_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup constructor
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
RawDataHazardContourGroup::RawDataHazardContourGroup( const std::string& identifier, EntityIdentifierResolver_ABC& /*entityIdentifierResolver*/, FOM_Serializer_ABC& /*fomSerializer*/ )
    : object_( 0 )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( identifier )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
{
    RegisterAttributes();
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup destructor
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
RawDataHazardContourGroup::~RawDataHazardContourGroup()
{
    if( object_ )
        object_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Serialize
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Serialize( ::hla::UpdateFunctor_ABC& functor,  bool updateAll ) const
{
    attributes_->Serialize( functor, updateAll );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Deserialize
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Deserialize( const ::hla::AttributeIdentifier& identifier, ::hla::Deserializer_ABC& deserializer )
{
    attributes_->Deserialize( identifier.ToString(), deserializer );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::GetIdentifier
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
const std::string& RawDataHazardContourGroup::GetIdentifier() const
{
    return identifier_;
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Register
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Register( ObjectListener_ABC& listener )
{
    listeners_->Register( listener );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Unregister
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Unregister( ObjectListener_ABC& listener )
{
    listeners_->Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::RegisterAttributes
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::RegisterAttributes()
{
    attributes_->Register( "Time", boost::bind( &ReadTime, _1, _2, _3, boost::ref( time_ ) ), Wrapper< uint64_t >( time_ ) );
    attributes_->Register( "Material", boost::bind( &ReadMaterial, _1, _2, _3, boost::ref( material_ ) ), Wrapper< uint16_t >( material_ ) );
    attributes_->Register( "HazardType", boost::bind( &ReadHazardType, _1, _2, _3, boost::ref( hazardType_ ) ), Wrapper< uint8_t >( hazardType_ ) );
    attributes_->Register( "Contours", boost::bind( &ReadContours, _1, _2, _3, boost::ref( contours_ ) ), Wrapper< std::vector< RawDataHazardContour > >( contours_ ) );
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::SpatialChanged
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::SpatialChanged( const ObjectLocationEventListener_ABC::T_PositionVector& pos )
{
    if( pos.size() == 0 )
        return;
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::ResetAttributes
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::ResetAttributes()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::Attach
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::Attach( Agent_ABC* /*agent*/, unsigned long /*simId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RawDataHazardContourGroup::ResourcesChanged
// Created: AHC 2013-06-28
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::ResourcesChanged( const ObjectLocationEventListener_ABC::T_ResourceVector& /*res*/ )
{
    // NOTHING
}
