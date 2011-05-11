// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "clients_gui_pch.h"
#include "ObjectPrototypeShapeFileLoader.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include <gdal/ogrsf_frmts.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader constructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
ObjectPrototypeShapeFileLoader::ObjectPrototypeShapeFileLoader(  const kernel::CoordinateConverter_ABC& coordinateConverter, QWidget* parent, const QString& filename, const kernel::ObjectType& objectType )
: currentFeature_( 0 )
, objectType_( objectType )
, coordinateConverter_( coordinateConverter )
{
    OGRRegisterAll();
    dataSource_ = OGRSFDriverRegistrar::Open( filename.ascii(), FALSE );
    if( dataSource_ == NULL )
        throw std::runtime_error( parent->tr( "Cannot load shapefile %1" ).arg( filename ).ascii() );

    QStringList layerNames;
    for( int i=0, count=dataSource_->GetLayerCount(); i<count; ++i )
    {
        OGRLayer* layer = dataSource_->GetLayer( i );
        if( !CanLoadLayer( *layer, objectType ) )
            continue;
        layerNames.push_back( layer->GetName() );
    }

    bool ok;
    QString layerName = QInputDialog::getItem( parent->tr( "Select layer" ), parent->tr( "Loadable layer:" ), layerNames, 1, false, &ok, parent );
    if( !ok )
    {
        OGRDataSource::DestroyDataSource( dataSource_ );
        throw LoadCancelledException();
    }

    currentLayer_ = dataSource_->GetLayerByName( layerName.ascii() );
    if( !currentLayer_ )
        throw std::runtime_error( parent->tr( "Cannot read layer %1" ).arg( layerName ).ascii() );

    for( int i=0, count=currentLayer_->GetLayerDefn()->GetFieldCount(); i<count; ++i )
        fields_.push_back( currentLayer_->GetLayerDefn()->GetFieldDefn( i )->GetNameRef() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader destructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
ObjectPrototypeShapeFileLoader::~ObjectPrototypeShapeFileLoader()
{
    OGRDataSource::DestroyDataSource( dataSource_ );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::CanLoadLayer
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
bool ObjectPrototypeShapeFileLoader::CanLoadLayer( OGRLayer& layer, const kernel::ObjectType& objectType )
{
    if ( !layer.GetSpatialRef() || layer.GetSpatialRef()->GetEPSGGeogCS() != 4326 ) //Only WGS84
        return false;
    return true;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::GetFields
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
QStringList ObjectPrototypeShapeFileLoader::GetFields() const
{
    return fields_;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsString
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
QString ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsString( const QString& fieldName ) const
{
    return currentFeature_->GetFieldAsString( fieldName );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsInt
// Created: BCI 2011-05-11
// -----------------------------------------------------------------------------
int ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsInt( const QString& fieldName ) const
{
    return currentFeature_->GetFieldAsInteger( fieldName );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::LoadNext
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
bool ObjectPrototypeShapeFileLoader::LoadNext()
{
    while( ( currentFeature_ = currentLayer_->GetNextFeature() ) != 0 )
    {
        switch( currentFeature_->GetGeometryRef()->getGeometryType() )
        {
        case wkbPoint:
        case wkbPoint25D:
            if( objectType_.CanBePoint() )
            {
                OGRPoint* point = static_cast< OGRPoint* >( currentFeature_->GetGeometryRef() );
                geometry::Point2d latlong( point->getX(), point->getY() );
                geometry::Point2f xy = coordinateConverter_.ConvertFromGeo( latlong );
                if( coordinateConverter_.IsInBoundaries( xy ) )
                {
                    currentLocation_.reset( new kernel::Point() );
                    currentLocation_->AddPoint( xy );
                    return true;
                }
            }
            break;
        case wkbPolygon:
            if( objectType_.CanBePolygon() )
            {
                currentLocation_.reset( new kernel::Polygon() );
                OGRPolygon* polygon = static_cast< OGRPolygon* >( currentFeature_->GetGeometryRef() );
                bool isInBoundaries = true;
                for( int i=0, count = polygon->getExteriorRing()->getNumPoints(); i<count; ++i )
                {
                    OGRPoint point;
                    polygon->getExteriorRing()->getPoint( i, &point );
                    geometry::Point2d latlong( point.getX(), point.getY() );
                    geometry::Point2f xy = coordinateConverter_.ConvertFromGeo( latlong );
                    if( coordinateConverter_.IsInBoundaries( xy ) )
                        currentLocation_->AddPoint( xy );
                    else
                    {
                        isInBoundaries = false;
                        break;
                    }
                }
                if( isInBoundaries )
                    return true;
            }
            break;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::StartLoad
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
void ObjectPrototypeShapeFileLoader::StartLoad()
{
    currentLayer_->ResetReading();
    currentFeature_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::GetCurrentLocation
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
const kernel::Location_ABC& ObjectPrototypeShapeFileLoader::GetCurrentLocation() const
{
    return *currentLocation_;
}
