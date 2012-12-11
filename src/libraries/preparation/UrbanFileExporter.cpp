// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanFileExporter.h"
#include "UrbanModel.h"
#include "UrbanHierarchies.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/ResourceNetwork_ABC.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Usages_ABC.h"
#include "terrain/PointProjector_ABC.h"
#pragma warning( push, 0 )
#include <gdal/ogrsf_frmts.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter constructor
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
UrbanFileExporter::UrbanFileExporter( const std::string& directory, const std::string& name, PointProjector_ABC& projector, const UrbanModel& model )
    : directory_( directory )
    , name_( name )
    , projector_( projector )
    , urbanModel_( model )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter destructor
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
UrbanFileExporter::~UrbanFileExporter()
{
    if( source_ != NULL )
        OGRDataSource::DestroyDataSource( source_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter::Initialize
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanFileExporter::Initialize()
{
    OGRRegisterAll();
    const char* pszDriverName = "ESRI Shapefile";
    OGRSFDriver* poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName( pszDriverName );
    if( poDriver == NULL )
        throw MASA_EXCEPTION(("%s driver not available.\n", pszDriverName ));

    std::string filename = name_ + ".shp";
    std::string filepath =  directory_ + "/" + filename;
    source_ = poDriver->CreateDataSource( filepath.c_str(), NULL );
    if( source_ == NULL )
        throw MASA_EXCEPTION( ( "gdal_ogr : write shape failed %s",directory_.c_str() ) );

    OGRSpatialReference newSpatialRef;
    newSpatialRef.SetWellKnownGeogCS( "EPSG:4326" );
    layer_ = source_->CreateLayer( filepath.c_str(), &newSpatialRef, wkbPolygon, NULL );
    if( layer_ == NULL )
        throw MASA_EXCEPTION( "Layer creation failed.\n" );

    CreateStructure();
    Write();
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter::CreateStructure
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanFileExporter::CreateStructure()
{
    OGRFieldDefn* oFieldId = new OGRFieldDefn( "ID", OFTInteger );
    oFieldId->SetWidth( 10 );
    CreateField( oFieldId );

    OGRFieldDefn* oFieldName = new OGRFieldDefn( "NAME", OFTString );
    oFieldName->SetWidth( 255 );
    CreateField( oFieldName );

    OGRFieldDefn* oFieldDistrictId = new OGRFieldDefn( "DIST_ID", OFTInteger );
    oFieldDistrictId->SetWidth( 10 );
    CreateField( oFieldDistrictId );

    OGRFieldDefn* oFieldDistrictName = new OGRFieldDefn( "DIST_NAME", OFTString );
    oFieldDistrictName->SetWidth( 255 );
    CreateField( oFieldDistrictName );

    OGRFieldDefn* oFieldTownId = new OGRFieldDefn( "TOWN_ID", OFTInteger );
    oFieldTownId->SetWidth( 10 );
    CreateField( oFieldTownId );

    OGRFieldDefn* oFieldTownName = new OGRFieldDefn( "TOWN_NAME", OFTString );
    oFieldTownName->SetWidth( 255 );
    CreateField( oFieldTownName );

    OGRFieldDefn* oFieldInfraName = new OGRFieldDefn( "INFRAS", OFTString );
    oFieldInfraName->SetWidth( 255 );
    CreateField( oFieldInfraName );

    OGRFieldDefn* oFieldFloorNumber = new OGRFieldDefn( "FLOOR_NUM", OFTInteger );
    oFieldFloorNumber->SetWidth( 10 );
    CreateField( oFieldFloorNumber );

    OGRFieldDefn* oFieldHeight = new OGRFieldDefn( "HEIGHT", OFTInteger );
    oFieldHeight->SetWidth( 10 );
    CreateField( oFieldHeight );

    OGRFieldDefn* oFieldRoofShape = new OGRFieldDefn( "ROOF_SHAPE", OFTString );
    oFieldRoofShape->SetWidth( 255 );
    CreateField( oFieldRoofShape );

    OGRFieldDefn* oFieldOccupation = new OGRFieldDefn( "OCCUPATION", OFTReal );
    oFieldOccupation->SetWidth( 10 );
    oFieldOccupation->SetPrecision( 6 );
    CreateField( oFieldOccupation );

    OGRFieldDefn* oFieldMaterial = new OGRFieldDefn( "MATERIAL", OFTString );
    oFieldMaterial->SetWidth( 255 );
    CreateField( oFieldMaterial );
   
    OGRFieldDefn* oFieldTrafficability = new OGRFieldDefn( "TRAFFIC", OFTReal );
    oFieldTrafficability->SetWidth( 10 );
    oFieldTrafficability->SetPrecision( 6 );
    CreateField( oFieldTrafficability );

    OGRFieldDefn* oFieldParking = new OGRFieldDefn( "PARKING", OFTString );
    oFieldParking->SetWidth( 255 );
    CreateField( oFieldParking );

    OGRFieldDefn* oFieldLeisure = new OGRFieldDefn( "LEISURE", OFTReal );
    oFieldLeisure->SetWidth( 10 );
    oFieldLeisure->SetPrecision( 6 );
    CreateField( oFieldLeisure );

    OGRFieldDefn* oFieldHousing = new OGRFieldDefn( "HOUSING", OFTReal );
    oFieldHousing->SetWidth( 10 );
    oFieldHousing->SetPrecision( 6 );
    CreateField( oFieldHousing );

    OGRFieldDefn* oFieldWork = new OGRFieldDefn( "WORK", OFTReal );
    oFieldWork->SetWidth( 10 );
    oFieldWork->SetPrecision( 6 );
    CreateField( oFieldWork );

    OGRFieldDefn* oFieldResWater = new OGRFieldDefn( "RES_WATER", OFTString );
    oFieldResWater->SetWidth( 255 );
    CreateField( oFieldResWater );

    OGRFieldDefn* oFieldResFood = new OGRFieldDefn( "RES_FOOD", OFTString );
    oFieldResFood->SetWidth( 255 );
    CreateField( oFieldResFood );

    OGRFieldDefn* oFieldResElec = new OGRFieldDefn( "RES_ELEC", OFTString );
    oFieldResElec->SetWidth( 255 );
    CreateField( oFieldResElec );

    OGRFieldDefn* oFieldResFuel = new OGRFieldDefn( "RES_FUEL", OFTString );
    oFieldResFuel->SetWidth( 255 );
    CreateField( oFieldResFuel );
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter::CreateField
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanFileExporter::CreateField( OGRFieldDefn* field )
{
    if( layer_->CreateField( field ) != OGRERR_NONE )
        throw MASA_EXCEPTION( "Creating Name field failed.\n" );
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter::Write
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanFileExporter::Write()
{
    unsigned int counter = 0;
    tools::Iterator< const kernel::UrbanObject_ABC& > it = urbanModel_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::UrbanObject_ABC& urbanObject = it.NextElement();
        if( const UrbanHierarchies* urbanHierarchies = static_cast< const UrbanHierarchies* >( urbanObject.Retrieve< kernel::Hierarchies >() ) )
            if( urbanHierarchies->GetLevel() == eUrbanLevelBlock )
            {
                WriteObject( urbanObject, counter );
                ++counter;
            }
    }
    layer_->SyncToDisk();
    OGRDataSource::DestroyDataSource( source_ );
}


// -----------------------------------------------------------------------------
// Name: UrbanFileExporter::WriteObject
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanFileExporter::WriteObject( const kernel::UrbanObject_ABC& urbanObject, unsigned int counter )
{
    // Initializing
    OGRFeature* pFeature = OGRFeature::CreateFeature( layer_->GetLayerDefn() );
    pFeature->SetField( "ID", static_cast< int >( urbanObject.GetId() ) );
    pFeature->SetField( "NAME", urbanObject.GetName().toStdString().c_str() );

    // Infrastructure
    {
        const kernel::Infrastructure_ABC& infra = urbanObject.Get< kernel::Infrastructure_ABC >();
        if( infra.GetType() )
            pFeature->SetField( "INFRAS", infra.GetType()->GetName().c_str() );
    }

    // Hierarchy
    {
        const UrbanHierarchies& urbanHierarchies = static_cast< const UrbanHierarchies& >( urbanObject.Get< kernel::Hierarchies >() );
        assert( urbanHierarchies.GetLevel() == eUrbanLevelBlock );
        const kernel::UrbanObject_ABC* districtObject = static_cast< const kernel::UrbanObject_ABC* >( urbanHierarchies.GetSuperior() );
        assert( districtObject );
        pFeature->SetField( "DIST_ID", static_cast< int >( districtObject->GetId() ) );
        pFeature->SetField( "DIST_NAME", districtObject->GetName().toStdString().c_str() );
        const UrbanHierarchies& districtHierarchies = static_cast< const UrbanHierarchies& >( districtObject->Get< kernel::Hierarchies >() );
        assert( districtHierarchies.GetLevel() == eUrbanLevelDistrict );
        const kernel::UrbanObject_ABC* cityObject = static_cast< const kernel::UrbanObject_ABC* >( districtHierarchies.GetSuperior() );
        assert( cityObject );
        pFeature->SetField( "TOWN_ID", static_cast< int >( cityObject->GetId() ) );
        pFeature->SetField( "TOWN_NAME", cityObject->GetName().toStdString().c_str() );
    }

    // Physical
    const kernel::PhysicalAttribute_ABC& physicalAttribute = urbanObject.Get< kernel::PhysicalAttribute_ABC >();
    {
        const kernel::Architecture_ABC& architecture = physicalAttribute.GetArchitecture();
        pFeature->SetField( "TRAFFIC", architecture.GetTrafficability() );
        pFeature->SetField( "FLOOR_NUM", static_cast < int >( architecture.GetFloorNumber() ) );
        pFeature->SetField( "PARKING", static_cast < int >( architecture.GetParkingFloors() ) );
        pFeature->SetField( "OCCUPATION", static_cast < int >( architecture.GetOccupation() ) );
        pFeature->SetField( "MATERIAL", architecture.GetMaterial().GetName().c_str() );
        pFeature->SetField( "HEIGHT", static_cast < int >( architecture.GetHeight() ) );
        pFeature->SetField( "ROOF_SHAPE", architecture.GetRoofShape().GetName().c_str() );
    }
    {
        const kernel::Usages_ABC& usage = physicalAttribute.GetUsages();
        unsigned int proportion = usage.Find( "Logement" );
        if( proportion > 0 ) pFeature->SetField( "HOUSING", static_cast< float >( proportion ) / 100.f );
        proportion = usage.Find( "Loisir" );
        if( proportion > 0 ) pFeature->SetField( "LEISURE", static_cast< float >( proportion ) / 100.f );
        proportion = usage.Find( "Travail" );
        if( proportion > 0 ) pFeature->SetField( "WORK", static_cast< float >( proportion ) / 100.f );
    }

    // Resource network
    {
        const kernel::ResourceNetwork_ABC& resourceNetwork = urbanObject.Get< kernel::ResourceNetwork_ABC>();
        if ( resourceNetwork.FindResourceNode( "carburant" ) )
            pFeature->SetField( "RES_FUEL", "Yes" );
        if ( resourceNetwork.FindResourceNode( "eau courante" ) )
            pFeature->SetField( "RES_WATER", "Yes" );
        if ( resourceNetwork.FindResourceNode( "electricite" ) )
            pFeature->SetField( "RES_ELEC", "Yes" );
        if ( resourceNetwork.FindResourceNode( "nourriture" ) )
            pFeature->SetField( "RES_FOOD", "Yes" );
    }

    // Position
    {
        const kernel::UrbanPositions_ABC& position = urbanObject.Get< kernel::UrbanPositions_ABC >();
        const std::vector< geometry::Point2f >& vertices = position.Vertices();
        const int numVertices = static_cast< int >( vertices.size() );
        OGRPolygon* polygon = new OGRPolygon();
        OGRLinearRing* ring = new OGRLinearRing();
        ring->setNumPoints( numVertices );
        for( int i = 0; i < numVertices; i++ )
        {
            double latitude, longitude;
            geometry::Point2d point( static_cast< double >( vertices[ i ].X() ), static_cast< double >( vertices[ i ].Y() ) );
            projector_.Unproject( point, latitude, longitude );
            ring->setPoint( i, longitude, latitude );
        }
        polygon->addRingDirectly( ring );
        polygon->closeRings();
        pFeature->SetGeometry( polygon );
    }

    // Ending
    layer_->CreateFeature( pFeature );
    if( counter % 50 == 0 )
        layer_->SyncToDisk();
    OGRFeature::DestroyFeature( pFeature );
}
