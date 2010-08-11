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

using namespace plugins;
using namespace crossbow;

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow constructor
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
OGR_FeatureRow::OGR_FeatureRow( OGRSpatialReference* spatialReference )
    : spatialReference_ ( spatialReference )
    , feature_ ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow destructor
// Created: JCR 2010-02-25
// -------- ---------------------------------------------------------------------
OGR_FeatureRow::~OGR_FeatureRow()
{
    Release();
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::Release
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
void OGR_FeatureRow::Release()
{
    if( feature_ != NULL )
        OGRFeature::DestroyFeature( feature_ );
}

namespace
{
    class FieldConverter : public boost::static_visitor<>, public boost::noncopyable
    {
    public:
        FieldConverter( OGRFeature& feature, int index ) 
            : feature_( feature )
            , index_  ( index ) 
        {
            // NOTHING
        }
        
        void operator()( int value ) const
        {
            feature_.SetField( index_, value );
        }

        void operator()( bool value ) const
        {
            feature_.SetField( index_, ( value ) ? 0 : -1 );
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
            // case OFTReal:
            //     return crossbow::FieldVariant( feature_.GetFieldAsDouble( index_ ) );
            case OFTString:
                return crossbow::FieldVariant( std::string( feature_.GetFieldAsString( index_ ) ) );
            default:
                throw;
            }
        }
    private:
        OGRFeature& feature_;
        int index_;
    };
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::SetField
// Created: JCR 2010-02-26
// -----------------------------------------------------------------------------
void OGR_FeatureRow::SetField( const std::string& field, const FieldVariant& value )
{
    const int index = feature_->GetFieldIndex( field.c_str() );
    boost::apply_visitor( FieldConverter( *feature_, index ), value );
}
    
// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::SetShape
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
void OGR_FeatureRow::SetGeometry( const Shape_ABC& shape )
{
    // $$$$ TODO ! OGRGeometry* geometry = feature_->GetGeometryRef();
    // geometry->Update();
    feature_->SetGeometryDirectly( shape.Extract( spatialReference_ ) );
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::GetField
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
FieldVariant OGR_FeatureRow::GetField( const std::string& name ) const
{
    int index = feature_->GetFieldIndex( name.c_str() );
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
void OGR_FeatureRow::BindFeature( OGRFeature& feature, long /*id*/ )
{
    Release();
    feature_ = &feature;
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::GetID
// Created: JCR 2010-02-24
// -----------------------------------------------------------------------------
long OGR_FeatureRow::GetID() const
{
    return feature_->GetFID();
}

// -----------------------------------------------------------------------------
// Name: OGR_FeatureRow::Insert
// Created: JCR 2010-02-25
// -----------------------------------------------------------------------------
void OGR_FeatureRow::Insert( OGRLayer& layer ) const
{
    layer.CreateFeature( feature_ );
}
