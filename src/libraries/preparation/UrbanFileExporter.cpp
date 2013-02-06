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
#include "clients_gui/Infrastructure_ABC.h"
#include "clients_gui/ResourceNetwork_ABC.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Usages_ABC.h"
#include "terrain/PointProjector_ABC.h"
#pragma warning( push, 0 )
#include <gdal/ogrsf_frmts.h>
#pragma warning( pop )
#include <boost/filesystem/operations.hpp>
#include <boost/noncopyable.hpp>

namespace bfs = boost::filesystem;

namespace
{

// =============================================================================
/** @class  UrbanFileExporter
    @brief  UrbanFileExporter
*/
// Created: CMA 2012-03-12
// =============================================================================
class UrbanFileExporter : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanFileExporter( const std::string& directory, const std::string& name, PointProjector_ABC& projector, const UrbanModel& model );
    virtual ~UrbanFileExporter();
    //@}

private:
    //! @name Helpers
    //@{
    void CreateStructure();
    void CreateField( const char* name, OGRFieldType type, int width, int precision = 0 );
    void Initialize();
    void Write();
    void WriteObject( const kernel::UrbanObject_ABC& object, unsigned int counter );
    //@}

private:
    //! @name Member data
    //@{
    const std::string   directory_;
    const std::string   name_;
    PointProjector_ABC& projector_;
    const UrbanModel&   urbanModel_;
    OGRDataSource*      source_;
    OGRLayer*           layer_;
    //@}
};


// -----------------------------------------------------------------------------
// Name: UrbanFileExporter constructor
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
UrbanFileExporter::UrbanFileExporter( const std::string& directory, const std::string& name, PointProjector_ABC& projector, const UrbanModel& model )
    : directory_ ( directory )
    , name_      ( name )
    , projector_ ( projector )
    , urbanModel_( model )
    , source_    ( 0 )
    , layer_     ( 0 )
{
    Initialize();
    CreateStructure();
    Write();
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter destructor
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
UrbanFileExporter::~UrbanFileExporter()
{
    OGRDataSource::DestroyDataSource( source_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter::Initialize
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanFileExporter::Initialize()
{
    bfs::create_directories( bfs::path( directory_ ) );
    OGRRegisterAll();
    OGRSFDriver* poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName( "ESRI Shapefile" );
    if( ! poDriver )
        throw MASA_EXCEPTION( "ESRI Shapefile driver not available" );
    const std::string filepath =  directory_ + "/" + name_ + ".shp";
    bfs::remove( filepath );
    source_ = poDriver->CreateDataSource( filepath.c_str(), 0 );
    if( !source_ )
        throw MASA_EXCEPTION( "gdal_ogr : write shape failed " + filepath );
    OGRSpatialReference newSpatialRef;
    newSpatialRef.SetWellKnownGeogCS( "EPSG:4326" );
    layer_ = source_->CreateLayer( filepath.c_str(), &newSpatialRef, wkbPolygon, 0 );
    if( !layer_ )
        throw MASA_EXCEPTION( "Layer creation failed" );
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter::CreateStructure
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanFileExporter::CreateStructure()
{
    CreateField( "ID", OFTInteger, 10 );
    CreateField( "NAME", OFTString, 255 );
    CreateField( "DIST_ID", OFTInteger, 10 );
    CreateField( "DIST_NAME", OFTString, 255 );
    CreateField( "TOWN_ID", OFTInteger, 10 );
    CreateField( "TOWN_NAME", OFTString, 255 );
    CreateField( "INFRAS", OFTString, 255 );
    CreateField( "FLOOR_NUM", OFTInteger, 10 );
    CreateField( "HEIGHT", OFTInteger, 10 );
    CreateField( "ROOF_SHAPE", OFTString, 255 );
    CreateField( "OCCUPATION", OFTReal, 10, 6 );
    CreateField( "MATERIAL", OFTString, 255 );
    CreateField( "TRAFFIC", OFTReal, 10, 6 );
    CreateField( "PARKING", OFTString, 255 );
    CreateField( "LEISURE", OFTReal, 10, 6 );
    CreateField( "HOUSING", OFTReal, 10, 6 );
    CreateField( "WORK", OFTReal, 10, 6 );
    CreateField( "RES_WATER", OFTString, 255 );
    CreateField( "RES_FOOD", OFTString, 255 );
    CreateField( "RES_ELEC", OFTString, 255 );
    CreateField( "RES_FUEL", OFTString, 255 );
}

// -----------------------------------------------------------------------------
// Name: UrbanFileExporter::CreateField
// Created: CMA 2012-03-12
// -----------------------------------------------------------------------------
void UrbanFileExporter::CreateField( const char* name, OGRFieldType type, int width, int precision )
{
    OGRFieldDefn def( name, type );
    def.SetWidth( width );
    def.SetPrecision( precision );
    if( layer_->CreateField( &def ) != OGRERR_NONE )
        throw MASA_EXCEPTION( "Creating Name field failed" );
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
        const UrbanHierarchies* urbanHierarchies = static_cast< const UrbanHierarchies* >( urbanObject.Retrieve< kernel::Hierarchies >() );
        if( urbanHierarchies && urbanHierarchies->GetLevel() == eUrbanLevelBlock )
        {
            WriteObject( urbanObject, counter );
            ++counter;
        }
    }
    layer_->SyncToDisk();
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
        const auto& infra = urbanObject.Get< gui::Infrastructure_ABC >();
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
        const auto& resourceNetwork = urbanObject.Get< gui::ResourceNetwork_ABC>();
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
        OGRLinearRing ring;
        ring.setNumPoints( numVertices );
        for( int i = 0; i < numVertices; i++ )
        {
            double latitude, longitude;
            geometry::Point2d point( static_cast< double >( vertices[ i ].X() ), static_cast< double >( vertices[ i ].Y() ) );
            projector_.Unproject( point, latitude, longitude );
            ring.setPoint( i, longitude, latitude );
        }
        OGRPolygon polygon;
        polygon.addRing( &ring );
        polygon.closeRings();
        pFeature->SetGeometry( &polygon );
    }
    // Ending
    layer_->CreateFeature( pFeature );
    if( counter % 50 == 0 )
        layer_->SyncToDisk();
    OGRFeature::DestroyFeature( pFeature );
}

}  // namespace


void ExportUrbanFiles( const std::string& directory, const std::string& name,
    PointProjector_ABC& projector, const UrbanModel& model)
{
    UrbanFileExporter( directory, name, projector, model);
}
