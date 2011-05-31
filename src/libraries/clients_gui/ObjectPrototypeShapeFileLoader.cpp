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
#include "Tools.h"
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
    dataSource_.reset( OGRSFDriverRegistrar::Open( filename.ascii(), FALSE ), OGRDataSource::DestroyDataSource );
    if( dataSource_ == NULL )
        throw std::runtime_error( parent->tr( "Cannot load shapefile %1" ).arg( filename ).ascii() );

    QStringList loadableLayerNames;
    QString unloadableLayerExplanations;
    for( int i=0, count=dataSource_->GetLayerCount(); i<count; ++i )
    {
        OGRLayer* layer = dataSource_->GetLayer( i );
        QString unloadableExplanation;
        if( !CanLoadLayer( *layer, objectType, unloadableExplanation ) )
            unloadableLayerExplanations += tools::translate( "gui::ObjectPrototypeShapeFileLoader", "Cannot load layer %1 : %2" ).arg( layer->GetName() ).arg( unloadableExplanation );
        else
            loadableLayerNames.push_back( layer->GetName() );
    }

    if( loadableLayerNames.empty() )
        throw std::runtime_error(  tools::translate( "gui::ObjectPrototypeShapeFileLoader", "No layer to load.\n%1" ).arg( unloadableLayerExplanations ).ascii() );

    QString layerName;
    if( loadableLayerNames.size() == 1 )
        layerName = *loadableLayerNames.begin();
    else
    {
        bool ok;
        layerName = QInputDialog::getItem( tools::translate( "gui::ObjectPrototypeShapeFileLoader", "Select layer" ), unloadableLayerExplanations, loadableLayerNames, 1, false, &ok, parent );
        if( !ok )
            throw LoadCancelledException();
    }

    currentLayer_ = dataSource_->GetLayerByName( layerName.ascii() );
    if( !currentLayer_ )
        throw std::runtime_error( tools::translate( "gui::ObjectPrototypeShapeFileLoader", "Cannot read layer %1" ).arg( layerName ).ascii() );

    for( int i=0, count=currentLayer_->GetLayerDefn()->GetFieldCount(); i<count; ++i )
        fields_.push_back( currentLayer_->GetLayerDefn()->GetFieldDefn( i )->GetNameRef() );
    fields_.sort();
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader destructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
ObjectPrototypeShapeFileLoader::~ObjectPrototypeShapeFileLoader()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::CanLoadLayer
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
bool ObjectPrototypeShapeFileLoader::CanLoadLayer( OGRLayer& layer, const kernel::ObjectType& objectType, QString& unloadableExplanation )
{
    if ( !layer.GetSpatialRef() )
    {
        unloadableExplanation = tools::translate( "gui::ObjectPrototypeShapeFileLoader", "no spatial reference" );
        return false;
    }
    if( layer.GetSpatialRef()->GetEPSGGeogCS() != 4326 )
    {
        unloadableExplanation = tools::translate( "gui::ObjectPrototypeShapeFileLoader", "spatial reference must be WGS84" );
        return false;
    }
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
// Name: ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsBool
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
bool ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsBool( const QString& fieldName ) const
{
    return currentFeature_->GetFieldAsInteger( fieldName ) != 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::LoadNext
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
bool ObjectPrototypeShapeFileLoader::LoadNext()
{
    QString cannotLoadStr = tools::translate( "gui::ObjectPrototypeShapeFileLoader", "Cannot load SHP feature %1 : %2"  );
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
                else
                    loadReports_.push_back( cannotLoadStr.arg( currentFeature_->GetFID() ).arg( "outside terrain boundaries" ) );
            }
            else
                loadReports_.push_back( cannotLoadStr.arg( currentFeature_->GetFID() ).arg( "object type cannot have point shape" ) );
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
                        loadReports_.push_back( cannotLoadStr.arg( currentFeature_->GetFID() ).arg( "outside terrain boundaries" ) );
                        break;
                    }
                }
                if( isInBoundaries )
                    return true;
            }
            else
                loadReports_.push_back( cannotLoadStr.arg( currentFeature_->GetFID() ).arg( "object type cannot have polygon shape" ) );
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

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::GetLoadReport
// Created: BCI 2011-05-19
// -----------------------------------------------------------------------------
QString ObjectPrototypeShapeFileLoader::GetLoadReport() const
{
    return loadReports_.join( "\n" );
}
