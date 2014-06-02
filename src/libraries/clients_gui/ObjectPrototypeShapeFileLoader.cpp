// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ObjectPrototypeShapeFileLoader.h"
#include "RichLabel.h"

#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Lines.h"
#include "clients_kernel/Tools.h"

#pragma warning( push, 0 )
#include <gdal/ogrsf_frmts.h>
#pragma warning( pop )

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader constructor
// Created: BCI 2011-05-09
// -----------------------------------------------------------------------------
ObjectPrototypeShapeFileLoader::ObjectPrototypeShapeFileLoader(  const kernel::CoordinateConverter_ABC& coordinateConverter, QWidget* parent, const tools::Path& filename, const kernel::ObjectType& objectType )
    : currentFeature_     ( 0 )
    , objectType_         ( objectType )
    , coordinateConverter_( coordinateConverter )
    , unsupportedShapes_  ( 0 )
    , unsupportedPoints_  ( 0 )
{
    OGRRegisterAll();
    dataSource_.reset( OGRSFDriverRegistrar::Open( filename.ToUTF8().c_str(), FALSE ), OGRDataSource::DestroyDataSource );
    if( ! dataSource_ )
        throw MASA_EXCEPTION( tools::translate( "gui::ObjectPrototypeShapeFileLoader", "Cannot load shapefile %1" ).arg( filename.ToUTF8().c_str() ).toStdString() );

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
        throw MASA_EXCEPTION( tools::translate( "gui::ObjectPrototypeShapeFileLoader", "No layer to load.\n%1" ).arg( unloadableLayerExplanations ).toStdString() );

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

    currentLayer_ = dataSource_->GetLayerByName( layerName.toStdString().c_str() );
    if( !currentLayer_ )
        throw MASA_EXCEPTION( tools::translate( "gui::ObjectPrototypeShapeFileLoader", "Cannot read layer %1" ).arg( layerName ).toStdString() );

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
bool ObjectPrototypeShapeFileLoader::CanLoadLayer( OGRLayer& layer, const kernel::ObjectType&, QString& unloadableExplanation )
{
    if( !layer.GetSpatialRef() )
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
    return currentFeature_->GetFieldAsString( fieldName.toStdString().c_str() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsInt
// Created: BCI 2011-05-11
// -----------------------------------------------------------------------------
int ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsInt( const QString& fieldName ) const
{
    return currentFeature_->GetFieldAsInteger( fieldName.toStdString().c_str() );
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsBool
// Created: BCI 2011-05-12
// -----------------------------------------------------------------------------
bool ObjectPrototypeShapeFileLoader::GetCurrentFieldValueAsBool( const QString& fieldName ) const
{
    return currentFeature_->GetFieldAsInteger( fieldName.toStdString().c_str() ) != 0;
}

namespace
{
    bool AddPoint( const kernel::CoordinateConverter_ABC& converter, OGRPoint& point,
                   kernel::Location_ABC& location, unsigned int& unsupportedPoints )
    {
        const geometry::Point2d latlong( point.getX(), point.getY() );
        const geometry::Point2f xy = converter.ConvertFromGeo( latlong );
        if( !converter.IsInBoundaries( xy ) )
        {
            ++unsupportedPoints;
            return false;
        }
        location.AddPoint( xy );
        return true;
    }

    bool IsMatch( bool valid, unsigned int& unmatchedShapes )
    {
        if( !valid )
            ++unmatchedShapes;
        return valid;
    }
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
            if( IsMatch( objectType_.CanBePoint(), unsupportedShapes_ ) )
            {
                currentLocation_.reset( new kernel::Point() );
                if( OGRPoint* point = static_cast< OGRPoint* >( currentFeature_->GetGeometryRef() ) )
                    AddPoint( coordinateConverter_, *point, *currentLocation_, unsupportedPoints_ );
            }
            break;
        case wkbPolygon:
            if( IsMatch( objectType_.CanBePolygon(), unsupportedShapes_ ) )
            {
                currentLocation_.reset( new kernel::Polygon() );
                if( OGRPolygon* polygon = static_cast< OGRPolygon* >( currentFeature_->GetGeometryRef() ) )
                    for( auto i = 0, count = polygon->getExteriorRing()->getNumPoints(); i < count; ++i )
                    {
                        OGRPoint point;
                        polygon->getExteriorRing()->getPoint( i, &point );
                        AddPoint( coordinateConverter_, point, *currentLocation_, unsupportedPoints_ );
                    }
            }
            break;
        case wkbLineString:
            if( IsMatch(  objectType_.CanBeLine(), unsupportedShapes_ ) )
                if( OGRLineString* line = static_cast< OGRLineString* >( currentFeature_->GetGeometryRef() ) )
                {
                    currentLocation_.reset( new kernel::Lines() );
                    OGRPoint point;
                    for( auto i = 0; i < line->getNumPoints(); ++i )
                    {
                        line->getPoint( i, &point );
                        AddPoint( coordinateConverter_, point, *currentLocation_, unsupportedPoints_ );
                    }
                }
            break;
        default:
            ++unsupportedShapes_;
            break;
        }
        return true;
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
    unsupportedShapes_ = 0;
    unsupportedPoints_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectPrototypeShapeFileLoader::GetCurrentLocation
// Created: BCI 2011-05-10
// -----------------------------------------------------------------------------
const kernel::Location_ABC& ObjectPrototypeShapeFileLoader::GetCurrentLocation() const
{
    return *currentLocation_;
}

bool ObjectPrototypeShapeFileLoader::CheckValidity( RichLabel& result ) const
{
    if( unsupportedShapes_ != 0 )
    {
        result.setText(
            tools::translate( "gui::ObjectPrototypeShapeFileLoader", "Error: cannot load shapefile, \n %1 unsupported/unmatched shape(s)" ).
            arg( QString::number( unsupportedShapes_ ) ) );
        result.Warn();
    }
    else if( unsupportedPoints_ != 0 )
        result.setText(
            tools::translate( "gui::ObjectPrototypeShapeFileLoader", "Warning: %1 unsupported point(s)" ).
            arg( QString::number( unsupportedPoints_ ) ) );
    return unsupportedShapes_ == 0;
}
