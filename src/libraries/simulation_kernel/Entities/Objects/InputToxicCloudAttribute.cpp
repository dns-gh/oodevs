// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "InputToxicCloudAttribute.h"
#include "MIL_AgentServer.h"
#include "MIL_NbcAgentType.h"
#include "MIL_ToxicEffectManipulator.h"
#include "NBCAttribute.h"
#include "Object.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_Knowledge_ObjectAttributeInputToxicCloud.h"
#include "simulation_terrain/TER_World.h"
#include "protocol/protocol.h"
#include <geodata/Feature_ABC.h>
#include <geodata/FeatureHandler_ABC.h>
#include <geodata/Primitive_ABC.h>
#include <gdal_ogr/gdal_ogr_lib.h>
#include <xeumeuleu/xml.hpp>

#pragma warning( push, 1 )
#include <boost/date_time/posix_time/posix_time.hpp>
#pragma warning( pop )

namespace bpt = boost::posix_time;

BOOST_CLASS_EXPORT_IMPLEMENT( InputToxicCloudAttribute )

namespace
{
    geometry::Rectangle2d ConvertTo( const MT_Rect& rect )
    {
        return geometry::Rectangle2d( rect.GetLeft(), rect.GetBottom(), rect.GetRight(), rect.GetTop() );
    }
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute constructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
InputToxicCloudAttribute::InputToxicCloudAttribute()
    : extent_( ConvertTo( TER_World::GetWorld().GetExtent() ) )
    , quantities_( new T_Quantities( extent_ ) )
    , schedule_( new T_Schedule() )
    , bExport_( true )
{
     quantities_->SetRefinementPolicy( 20 );  // $$$$ JCR 2007-09-13: profiling?
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute constructor
// Created: JCR 2008-06-05
// -----------------------------------------------------------------------------
InputToxicCloudAttribute::InputToxicCloudAttribute( xml::xistream& xis )
    : extent_( ConvertTo( TER_World::GetWorld().GetExtent() ) )
    , quantities_( new T_Quantities( extent_ ) )
    , filename_( xis.attribute< std::string >( "source" ) )
    , dataField_( xis.attribute< std::string >( "data-field" ) )
    , schedule_( new T_Schedule() )
    , bExport_( xis.attribute< bool >( "export", true ) )
{
    quantities_->SetRefinementPolicy( 20 );  // $$$$ JCR 2007-09-13: profiling?
    LoadConfig();
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute destructor
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
InputToxicCloudAttribute::~InputToxicCloudAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::load
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::load( MIL_CheckPointInArchive& ar, const unsigned int )
{
    ar >> boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar >> filename_
       >> field_
       >> dataField_
       >> bExport_;
    LoadConfig();
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::save
// Created: JCR 2008-07-03
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::save( MIL_CheckPointOutArchive& ar, const unsigned int ) const
{
    ar << boost::serialization::base_object< ObjectAttribute_ABC >( *this );
    ar << filename_
       << field_
       << dataField_
       << bExport_;
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::operator=
// Created: JCR 2008-05-30
// -----------------------------------------------------------------------------
InputToxicCloudAttribute& InputToxicCloudAttribute::operator=( const InputToxicCloudAttribute& rhs )
{
    filename_ = rhs.filename_;
    field_ = rhs.field_;
    dataField_ = rhs.dataField_;
    bExport_ = rhs.bExport_;
    extent_ = rhs.extent_;
    schedule_ = rhs.schedule_;
    quantities_ = rhs.quantities_;
    NotifyAttributeUpdated( eOnCreation );
    return *this;
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::LoadConfig
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::LoadConfig()
{
    if( !filename_.empty() )
    {
        xml::xifstream fxis( filename_ );
        fxis >> xml::start( "�tude" )
                >> xml::start( "r�sultats" )
                    >> xml::content( dataField_, field_ )
                    >> xml::start( "�ch�ances" )
                        >> xml::list( "heure", *this, &InputToxicCloudAttribute::ReadFiles )
                    >> xml::end
                >> xml::end
            >> xml::end;
    }
}

namespace
{
    unsigned int ReadGDH( const std::string& gdh )
    {
        bpt::ptime time = bpt::from_iso_string( gdh );
        return ( time - bpt::from_time_t( 0 ) ).total_seconds();
    }

    std::string ConvertDateFrToUs( const std::string& date, const std::string& time ) // DDMMYYYYTHHMMSS -> YYYYMMDDTHHMMSS
    {
        assert( date.size() == 8 );
        std::string new_date = std::string( date.begin() + 4, date.end() ) +
                               std::string( date.begin() + 2, date.begin() + 4 ) +
                               std::string( date.begin(), date.begin() + 2 );
        return new_date + "T" + time;
    }

    bool CanConvertWeirdFormat( const std::string& schedule, std::string& eventGDH )
    {
        std::string::size_type time_pos = schedule.find_last_of( "_" );
        if( time_pos == std::string::npos || time_pos >= schedule.size() )
            return false;
        const std::string event_time( schedule, time_pos + 1, schedule.size() );

        std::string::size_type date_pos = schedule.find_last_of( "_", time_pos - 1 );
        if( date_pos == std::string::npos || date_pos >= schedule.size() )
            return false;
        const std::string event_date( schedule, date_pos + 1, time_pos - 1 - date_pos );
        eventGDH = ConvertDateFrToUs( event_date, event_time );
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::ReadFiles
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::ReadFiles( xml::xistream& xis )
{
    std::string schedule;
    std::string eventGDH;
    xis >> schedule;

    if( CanConvertWeirdFormat( schedule, eventGDH ) )
    {
        unsigned int eventTime = ReadGDH( eventGDH );
        unsigned int simTime = MIL_AgentServer::GetWorkspace().RealTimeToTick( eventTime );
        (*schedule_)[ simTime ] = schedule;
    }
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::Instanciate
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::Instanciate( DEC_Knowledge_Object& /*object*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::Register
// Created: JSR 2010-03-15
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::Register( MIL_Object_ABC& object ) const
{
    object.SetAttribute< InputToxicCloudAttribute, InputToxicCloudAttribute >( *this );
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::SendFullState
// Created: JCR 2008-06-18
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::SendFullState( Common::MsgObjectAttributes& asn ) const
{
    if( bExport_ )
    {
        unsigned int i = 0;
        for (CIT_QuantityContainer it(export_.begin()); it != export_.end(); ++it, i++)
        {
            asn.mutable_toxic_cloud()->mutable_quantities()->mutable_elem( i )->mutable_coordinate()->set_latitude( (*it).first.rX_ );
            asn.mutable_toxic_cloud()->mutable_quantities()->mutable_elem( i )->mutable_coordinate()->set_longitude( (*it).first.rY_ );
            asn.mutable_toxic_cloud()->mutable_quantities()->mutable_elem( i )->set_quantity( float( (*it).second ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::Send
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::SendUpdate( Common::MsgObjectAttributes& asn ) const
{
    if( bExport_ && NeedUpdate() )
    {
        SendFullState( asn );
        Reset();
    }
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::WriteODB
// Created: JCR 2008-06-09
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "input-toxic-cloud" )
            << xml::attribute( "source", filename_ )
            << xml::attribute( "data-field", dataField_ )
            << xml::attribute( "export", bExport_ )
        << xml::end;
}

namespace
{
    struct Handler : public geodata::FeatureHandler_ABC
    {
        typedef InputToxicCloudAttribute::T_Quantity    T_Quantity;
        typedef InputToxicCloudAttribute::T_Quantities  T_Quantities;
        Handler( const std::string& field, T_Quantities& quantities, std::vector< T_Quantity >& e )
            : field_ ( field ), quantities_ ( quantities ), export_ ( e ) {}

        virtual void Handle( geodata::Feature_ABC* feature )
        {
            const geometry::Point2d geoPos( feature->GetPrimitive().GetPosition() );
            if( feature->HasAttribute( field_ ) )
            {
                const double quantity = boost::get< double >( feature->GetAttribute( field_ ) );
                if( quantity > 0 )
                {
                    MT_Vector2D position;
                    TER_World::GetWorld().MosToSimMgrsCoord( geoPos.Y(), geoPos.X(), position );
                    quantities_.Insert( std::make_pair( position, quantity ) );

                    // switch long/lat -> lat/long
                    MT_Vector2D latlong( position.rY_, position.rX_ );
                    export_.push_back( std::make_pair( latlong, quantity ) );
                }
            }
            delete feature; // must be deleted
        }

        const std::string& field_;
        T_Quantities& quantities_;
        std::vector< T_Quantity >& export_;
    };
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::LoadShape
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::LoadShape( const std::string& name )
{
    const std::string fdir( filename_, 0, filename_.find_last_of( '/' ) );
    gdal_ogr::OGR_Directory dir( "", fdir + "/propagation" );
    Handler handler( field_, *quantities_, export_ );
    quantities_->Clear();
    export_.clear();
    dir.Retrieve( handler, name, extent_ );
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::BuildConvexHull
// Created: JCR 2008-06-12
// -----------------------------------------------------------------------------
void InputToxicCloudAttribute::BuildConvexHull( TER_Polygon& polygon ) const
{
    T_PointVector vector( export_.size() );
    IT_PointVector to = vector.begin();
    for ( CIT_QuantityContainer it = export_.begin(); it != export_.end();  ++it )
        *(to++) = MT_Vector2D( it->first.rY_, it->first.rX_ );
    polygon.Reset( vector, true /*convexhull*/ );
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::Update
// Created: JCR 2008-06-11
// -----------------------------------------------------------------------------
bool InputToxicCloudAttribute::Update( unsigned int time, TER_Polygon& polygon )
{
    CIT_Schedule it = schedule_->find( time );
    if( it != schedule_->end() )
    {
        LoadShape( it->second );
        if( ! export_.empty() )
            BuildConvexHull( polygon );
        NotifyAttributeUpdated( eOnUpdate );
        return true;
    }
    return false;
}

namespace
{
    class Intersecter
    {
    public:
        explicit Intersecter( const MT_Vector2D& position ) : position_ ( position ) {}

        int CompareOnX( double value ) const
        {
            return Compare( position_.rX_ , value );
        }

        int CompareOnY( double value ) const
        {
            return Compare( position_.rY_, value );
        }

        int Compare( double lhs, double rhs ) const
        {
            if( lhs < rhs )
                return -1;
            if( lhs > rhs )
                return 1;
            return 0;
        }
    private:
         const MT_Vector2D& position_;
    };

    class AssessQuantity
    {
    public:
        explicit AssessQuantity( MT_Float& quantity ) : n_( 0 ), quantity_ ( quantity ) {}

        bool operator()( const InputToxicCloudAttribute::T_Quantity& quantity )
        {
            quantity_ += quantity.second;
            return false;
        }
    private:
        MT_Float& quantity_;
        unsigned int      n_;
    };
}

// -----------------------------------------------------------------------------
// Name: InputToxicCloudAttribute::GetContaminationEffect
// Created: JCR 2008-06-17
// -----------------------------------------------------------------------------
MIL_ToxicEffectManipulator InputToxicCloudAttribute::GetContaminationEffect( const NBCAttribute& nbc, const MT_Vector2D& position ) const
{
    MT_Float        quantity = 0.;
    Intersecter     intersecter( position );
    AssessQuantity  functor( quantity );
    quantities_->Apply( intersecter, functor );

    return MIL_ToxicEffectManipulator( nbc.GetNBCAgents(), quantity );
}
