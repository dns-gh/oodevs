// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "OGR_FeatureRow.h"
#include "Point.h"
#include "Area.h"
#include "Line.h"

#include <gdal/ogr_core.h>
#include <gdal/ogr_feature.h>
#include <gdal/ogrsf_frmts.h>
#include <boost/noncopyable.hpp>
#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow constructor
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
OGR_FeatureRow::OGR_FeatureRow( OGRSpatialReference* spatialReference )
    : spatialReference_ ( spatialReference )
    , log_ ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow destructor
// Created: JCR 2010-02-25
// -------- ---------------------------------------------------------------------
OGR_FeatureRow::~OGR_FeatureRow()
{
    // NOTHING
}

namespace
{
    class FieldConverter : public boost::static_visitor<>, public boost::noncopyable
    {
    public:
        FieldConverter( OGRFeature& feature, int index )
            : feature_ ( feature ), index_ ( index )
        {
        }

        void operator()( int value ) const
        {
            feature_.SetField( index_, value );
        }

        void operator()( long value ) const
        {
            feature_.SetField( index_, static_cast< int >( value ) );
        }

        void operator()( double value ) const
        {
            feature_.SetField( index_, value );
        }

        void operator()( bool value ) const
        {
            feature_.SetField( index_, ( value ) ? 1 : 0 );
        }

        void operator()( const std::string& value ) const
        {
            feature_.SetField( index_, value.c_str() );
        }

        crossbow::FieldVariant ConvertVariant( OGRFieldDefn* def )
        {
            switch( def->GetType() )
            {
            case OFTInteger:
                return crossbow::FieldVariant( feature_.GetFieldAsInteger( index_ ) );
            case OFTReal:
                return crossbow::FieldVariant( feature_.GetFieldAsDouble( index_ ) );
            case OFTString:
                return crossbow::FieldVariant( std::string( feature_.GetFieldAsString( index_ ) ) );
            default:
                throw;
            }
        }
    private:
        OGRFeature& feature_;
        const int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::SetField
// Created: JCR 2010-02-26
// -----------------------------------------------------------------------------
void OGR_FeatureRow::SetField( const std::string& field, const FieldVariant& value )
{
    const int index = feature_->GetFieldIndex( field.c_str() );
    if( index > -1 )
        boost::apply_visitor( FieldConverter( *feature_, index ), value );
}

namespace
{
    void Log( const OGRFeature& /*feature*/ )
    {
        /* char* value;
        OGRGeometry* result = feature.GetGeometryRef();

        if( result )
        {
            result->exportToWkt( &value );
            MT_LOG_INFO_MSG( "OGR_FeatureRow: IsValid:" << boost::lexical_cast<std::string>( result->IsValid() ) << " "
                                       << " Surface:" << result->getGeometryName() << " "
                                       << " Value: " << value );
        }
        else
            MT_LOG_ERROR_MSG( "OGR_FeatureRow: geometry not registered" );
        */
    }
}
// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::SetShape
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
void OGR_FeatureRow::SetGeometry( const Shape_ABC& shape )
{
    shape.Serialize( *feature_, spatialReference_ );
    if( log_ )
        Log( *feature_ );
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::GetField
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
FieldVariant OGR_FeatureRow::GetField( const std::string& name ) const
{
    int index = feature_->GetFieldIndex( name.c_str() );
    if( index == -1 )
        throw std::runtime_error( "Cannot retrieve field " + name );

    OGRFieldDefn* def = feature_->GetFieldDefnRef( index );

    FieldConverter converter( *feature_, index );
    return converter.ConvertVariant( def );
}

namespace
{
    std::auto_ptr< Shape_ABC > CreateShape( const OGRGeometry* geometry )
    {
        OGRwkbGeometryType type = geometry->getGeometryType();
        switch ( type )
        {
        case wkbPoint:
            return std::auto_ptr< Shape_ABC >( new crossbow::Point( *static_cast< const OGRPoint* >( geometry ) ) );
            break;
        case wkbPolygon:
            return std::auto_ptr< Shape_ABC >( new crossbow::Area( *static_cast< const OGRPolygon* >( geometry ) ) );
            break;
        case wkbLineString:
            return std::auto_ptr< Shape_ABC >( new crossbow::Line( *static_cast< const OGRLineString* >( geometry ) ) );
            break;
        default:
            throw std::runtime_error( "unsupported geometry" );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::GetShape
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
Shape_ABC& OGR_FeatureRow::GetGeometry() const
{
    if( ! shape_.get() )
    {
        const OGRGeometry* geometry = feature_->GetGeometryRef();
        shape_ = CreateShape( geometry );
    }
    return *shape_;
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::BindFeature
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
void OGR_FeatureRow::BindFeature( boost::shared_ptr< OGRFeature > feature, long /*id*/ )
{
    feature_ = feature;
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::GetID
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
long OGR_FeatureRow::GetID() const
{
    return feature_->GetFID();
}

#include <fstream>

namespace 
{
    std::string Dump( OGRFeature* feature, OGRLayer& layer )
    {
       std::stringstream ss;
       
       for( int iField = 0; iField < layer.GetLayerDefn()->GetFieldCount(); iField++ )
       {
            OGRFieldDefn *poFieldDefn = layer.GetLayerDefn()->GetFieldDefn( iField );
            ss << "Name '" << poFieldDefn->GetNameRef() << "': ";
            if( poFieldDefn->GetType() == OFTInteger )
                ss << feature->GetFieldAsInteger(iField) << ", ";
            else if( poFieldDefn->GetType() == OFTReal )
                ss << feature->GetFieldAsDouble(iField) << ", ";
            else 
                ss << feature->GetFieldAsString(iField) << ", ";
       }
       ss << std::endl;
       return ss.str();
    }
}
// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::Insert
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
void OGR_FeatureRow::Insert( OGRLayer& layer ) const
{
    if( feature_.get() )
        feature_->SetFID( OGRNullFID );
    OGRErr err = layer.CreateFeature( feature_.get() );
    if( feature_.get() && err != OGRERR_NONE )
    {
        throw std::runtime_error( "unable to insert feature to the layer : " + Dump( feature_.get(), layer ) );
    }
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::Update
// Created: JCR 2010-04-09
// -----------------------------------------------------------------------------
void OGR_FeatureRow::Update( OGRLayer& layer ) const
{
    OGRErr err = layer.SetFeature( feature_.get() );
    if( feature_.get() && err != OGRERR_NONE )
    {
        Dump( feature_.get(), layer );
        throw std::runtime_error( "unable to update feature to the layer" );
    }
}
