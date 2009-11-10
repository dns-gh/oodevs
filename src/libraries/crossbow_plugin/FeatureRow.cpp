// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "crossbow_plugin_pch.h"
#include "FeatureRow.h"
#include "Point.h"
#include "Line.h"
#include "Area.h"
#include <boost/lexical_cast.hpp>

using namespace plugins;
using namespace plugins::crossbow;

// -----------------------------------------------------------------------------
// Name: FeatureRow constructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
FeatureRow::FeatureRow( ISpatialReferencePtr spatialReference )
    : spatialReference_( spatialReference )
    , shape_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FeatureRow destructor
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
FeatureRow::~FeatureRow()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FeatureRow::SetShape
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void FeatureRow::SetShape( const Shape_ABC& value )
{
    IGeometryPtr geometry;
    feature_->get_Shape( &geometry );    
    value.Serialize( geometry, spatialReference_  );
    feature_->putref_Shape( geometry );
}

// -----------------------------------------------------------------------------
// Name: FeatureRow::GetShape
// Created: SBO 2007-09-26
// -----------------------------------------------------------------------------
Shape_ABC& FeatureRow::GetShape() const
{
    if( shape_.get() )
        return *shape_;

    IGeometryPtr geometry;
    feature_->get_Shape( &geometry );
    esriGeometryType shapeType;
    geometry->get_GeometryType( &shapeType );
    if( shapeType == esriGeometryPoint )
        shape_.reset( new Point( geometry ) );
    else if( shapeType == esriGeometryLine || shapeType == esriGeometryPolyline )
        shape_.reset( new Line( geometry ) );
    else if( shapeType == esriGeometryPolygon )
        shape_.reset( new Area( geometry ) );    
    else
        throw std::runtime_error( "Unsupported feature geometry type" ); // $$$$ SBO 2007-09-26: 
    return *shape_;
}

// -----------------------------------------------------------------------------
// Name: FeatureRow::BindFeature
// Created: SBO 2007-08-30
// -----------------------------------------------------------------------------
void FeatureRow::BindFeature( IFeaturePtr feature, long id )
{
    feature_ = feature;
    shape_.reset();
    IRowPtr row;
    feature.QueryInterface( IID_IRow, &row ); // $$$$ SBO 2007-08-30: check
    crossbow::Row::BindRow( row, id );
}

namespace
{
    void ThrowError()
    {
        IErrorInfoPtr ipError;
        BSTR strError;
        ::GetErrorInfo( 0, &ipError );
        ipError->GetDescription( &strError );
        const std::string error = boost::lexical_cast< std::string >( _bstr_t( strError ) );
        MT_LOG_ERROR_MSG( error ); // $$$$ SBO 2008-05-15: should throw
    }
}

// -----------------------------------------------------------------------------
// Name: FeatureRow::Commit
// Created: SBO 2007-08-31
// -----------------------------------------------------------------------------
void FeatureRow::Commit( IFeatureCursorPtr cursor /*= NULL*/ )
{
    if( cursor != NULL )
    {
        CComVariant value;
        // HRESULT res = cursor->InsertFeature( feature_, &value );
        HRESULT res = cursor->UpdateFeature( feature_ );
        if( FAILED( res ) )
            ThrowError();
    }
}
