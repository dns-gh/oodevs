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
#include "tools/ContourComputer.h"

#include <boost/ref.hpp>
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
void ReadMaterial( ::hla::Deserializer_ABC& deserializer, const std::string& identifier, ObjectListener_ABC& listener, uint16_t& agent )
{
    deserializer >> agent;
    std::stringstream ss;
    ss << "100 0 0 0 0 " << (agent >> 8) << " " << (agent % 256); // FIXME AHC : configure DIS prefix
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
void ReadUniqueId(::hla::Deserializer_ABC& deserializer, const std::string&, ObjectListener_ABC& , NETN_UUID& uuid )
{
    deserializer >> uuid;
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
    double bottom = 0., top = 0., left = 0., right = 0.;
    bool first = true;
    for( std::size_t i=0;i<contours.size(); ++i)
    {
        const std::vector< rpr::WorldLocation >& locs=contours[i].locations;
        for( std::size_t j=0; j<locs.size(); ++j )
        {
            const rpr::WorldLocation& loc = locs[j];
            polys[i].Add( geometry::Point2d( loc.Longitude(), loc.Latitude() ) );
        }
        geometry::Rectangle2d bbox = polys[i].BoundingBox();
        if( first )
        {
            bottom = bbox.Bottom();
            top = bbox.Top();
            left = bbox.Left();
            right = bbox.Right();
            first = false;
        }
        else
        {
            bottom = std::min( bottom, bbox.Bottom());
            top = std::max( top, bbox.Top());
            left = std::min( left, bbox.Left());
            right = std::max( right, bbox.Right());
        }
    }
    const geometry::Rectangle2d box(left,bottom,right,top);
    const double width = box.Right() - box.Left();
    const double height = box.Top() - box.Bottom();
    double incx = 0.005;
    double incy = 0.005;
    uint32_t mx = static_cast< uint32_t >( ( width ) / incx ) +1;
    uint32_t my = static_cast< uint32_t >( ( height ) / incy ) +1;
    const uint32_t MIN_SIZE = 10;
    if( mx < MIN_SIZE || my < MIN_SIZE )
    {
        if( width < height )
        {
            mx = MIN_SIZE;
            incy = incx = width / mx;
            my = static_cast< uint32_t >( ( height ) / incy );
        }
        else
        {
            my = MIN_SIZE;
            incy = incx = height / my;
            mx = static_cast< uint32_t >( ( width ) / incx );
        }
    }

    std::vector< ObjectListener_ABC::PropagationData > data;
    for( uint32_t i=0; i<my; ++i )
    {
        const double y = box.Top() - i * incy;
        for( uint32_t j=0; j<mx; ++j )
        {
            const double x = box.Left() + j * incx;
            bool found=false;
            for(int32_t k=static_cast<int32_t>(contours.size()-1); k>=0; --k)
            {
                const geometry::Point2d pt(x,y);
                const bool isInside = polys[k].IsInside(pt);
                if( isInside )
                {
                    data.push_back( ObjectListener_ABC::PropagationData( y, x, contours[k].exposureLevel ) );
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
RawDataHazardContourGroup::RawDataHazardContourGroup( TacticalObject_ABC& object, unsigned int /*identifier*/, const std::string& /*name*/, rpr::ForceIdentifier /*force*/,
        const rpr::EntityType& type, const rpr::EntityIdentifier& /*entityId*/, const std::string& rtiId )
    : object_( &object )
    , listeners_ ( new ObjectListenerComposite() )
    , identifier_( rtiId )
    , attributes_( new AttributesUpdater( identifier_, *listeners_ ) )
    , agent_( type.Specific()*256 +  type.Extra() )
    , hazardType_( 1 ) // aTP45HazardAreaTypeEnum8_Detailed
    , uniqueId_( UniqueIdSerializer::GenerateUniqueId( 16 ) )
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
    , agent_( 1000 ) // agentTypeEnum16_Chemical
    , hazardType_( 1 ) // aTP45HazardAreaTypeEnum8_Detailed
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
    attributes_->Register( "Agent", boost::bind( &ReadMaterial, _1, _2, _3, boost::ref( agent_ ) ), Wrapper< uint16_t >( agent_ ) );
    attributes_->Register( "HazardType", boost::bind( &ReadHazardType, _1, _2, _3, boost::ref( hazardType_ ) ), Wrapper< uint8_t >( hazardType_ ) );
    attributes_->Register( "Contours", boost::bind( &ReadContours, _1, _2, _3, boost::ref( contours_ ) ), Wrapper< std::vector< RawDataHazardContour > >( contours_ ) );
    attributes_->Register( "UniqueID", boost::bind( &ReadUniqueId, _1, _2, _3, boost::ref( uniqueId_ ) ), Wrapper< NETN_UUID >( uniqueId_ ) );
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
// Name: RawDataHazardContourGroup::PropagationChanged
// Created: AHC 2014-01-23
// -----------------------------------------------------------------------------
void RawDataHazardContourGroup::PropagationChanged( const ObjectPropagationEventListener_ABC::T_DataVector& data )
{
    if( data.size() == 0 )
        return;

    contours_.clear();
    const short MAX = 100;
    const int HEIGHT = 10;
    for( ObjectPropagationEventListener_ABC::T_DataVector::const_iterator it = data.begin(); data.end() != it; ++it )
    {
        const ObjectPropagationEventListener_ABC::Data& propagationData = *it;
        const float cellsize = static_cast< float >( ( propagationData.extent.Right() - propagationData.extent.Left() ) / propagationData.cols );

        tools::ComputeContour( propagationData.cols, propagationData.rows, HEIGHT, cellsize,
            [&]( int c, int l )->short { return static_cast< short >( propagationData.concentrations[ l * propagationData.cols + c] / propagationData.maxValue * MAX ) ; }, // data
            []( short ) {}, // progress
            []() -> bool { return true; }, // valid
            [&]( boost::shared_ptr< tools::T_PointVector > pts, int k, bool closed ) // loop
                {
                    if( closed && pts->size() != 0 )
                    {
                        float v = static_cast< float >( k * propagationData.maxValue / HEIGHT );
                        RawDataHazardContour contour;
                        contour.exposureLevel = v;
                        std::for_each( pts->begin(), pts->end(), [&]( const geometry::Point2f& pt )
                            {
                                contour.locations.push_back( rpr::WorldLocation( propagationData.extent.Bottom() + pt.Y() * cellsize, propagationData.extent.Left() + pt.X() * cellsize, 0 ) );
                            });
                        contours_.push_back( contour );
                    }
                },
            []() -> bool { return false; }, // checkStop
            MAX / HEIGHT );
    }
    attributes_->Update( "Contours", Wrapper< std::vector< RawDataHazardContour > >( contours_ ) );
}

Agent_ABC* const RawDataHazardContourGroup::GetAgent() const
{
    return 0;
}
