// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanModel.h"
#include "InfrastructureAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "ResourceNetworkAttribute.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanFactory.h"
#include "UrbanHierarchies.h"
#include "UrbanMenuManager.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/UrbanObject.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "ENT/ENT_Enums_Gen.h"
#include "geometry/Types.h"
#include "geostore/GeoStoreManager.h"
#include "tools/SchemaWriter_ABC.h"
#include "tools/ExerciseConfig.h"
#include <boost/filesystem.hpp>
#include <xeumeuleu/xml.hpp>

#pragma warning( disable : 4355 )

namespace bfs = boost::filesystem;

namespace
{
    spatialcontainer::SegmentIntersecter< float > Intersector( const geometry::Rectangle2f& boundingBox )
    {
        return spatialcontainer::SegmentIntersecter< float >( boundingBox.BottomLeft(), boundingBox.TopRight() );
    };

    template< typename ConcretType, typename AbstractType >
    void SerializeIFN( const kernel::Entity_ABC& entity, xml::xostream& xos )
    {
        const AbstractType* abstractExtension = entity.Retrieve< AbstractType >();
        if( !abstractExtension )
            return;
        const ConcretType& extension = static_cast< const ConcretType& >( *abstractExtension );
        if( extension.IsOverriden() )
            extension.SerializeAttributes( xos );
    }

    template< typename CollisionChecker >
    struct DataExtractor
    {
        explicit DataExtractor( const CollisionChecker& wrapper ) : wrapper_ ( wrapper ) {}
        bool operator()( const kernel::UrbanObject_ABC* key )
        {
            if( key && key->Retrieve< kernel::UrbanPositions_ABC >() && wrapper_.Check( key->Get< kernel::UrbanPositions_ABC >().Polygon() ) )
            {
                data_.push_back( key );
                return wrapper_.DoContinue();
            }
            return true;
        }
        std::vector< const kernel::UrbanObject_ABC* > data_;
        const CollisionChecker wrapper_;
    };

    class PointChecker
    {
    public:
        explicit PointChecker( const geometry::Point2f& pick ) : pick_ ( pick ) {}
        bool Check( const geometry::Polygon2f& geometry ) const
        {
            return geometry.IsInside( pick_ );
        }
        bool DoContinue() const { return false; }
    private:
        PointChecker& operator=( const PointChecker& );
    private:
        const geometry::Point2f& pick_;
    };

    class CircleChecker
    {
    public:
        explicit CircleChecker( const geometry::Point2f& center, float radius ) : center_ ( center ), radius_ ( radius ) {}
        bool Check( const geometry::Polygon2f& geometry ) const
        {
            return geometry.Intersect( center_, radius_ );
        }
        bool DoContinue() const { return true; }
    private:
        CircleChecker& operator=( const CircleChecker& );
    private:
        const geometry::Point2f& center_;
        float radius_;
    };

    class SegmentChecker
    {
    public:
        explicit SegmentChecker( const geometry::Segment2f& segment ) : segment_ ( segment ) {}
        bool Check( const geometry::Polygon2f& geometry ) const
        {
            return !geometry.Intersect( segment_ ).empty();
        }
        bool DoContinue() const { return true; }
    private:
        SegmentChecker& operator=( const SegmentChecker& );
    private:
        const geometry::Segment2f& segment_;
    };
}

struct UrbanModel::QuadTreeTraits
{
    int CompareOnX( float value, const kernel::UrbanObject_ABC* key ) const
    {
        if( const kernel::UrbanPositions_ABC* geom = key->Retrieve< kernel::UrbanPositions_ABC >() )
            return Intersector( geom->Polygon().BoundingBox() ).CompareOnX( value );
        return 0;
    }

    int CompareOnY( float value, const kernel::UrbanObject_ABC* key ) const
    {
        if( const kernel::UrbanPositions_ABC* geom = key->Retrieve< kernel::UrbanPositions_ABC >() )
            return Intersector( geom->Polygon().BoundingBox() ).CompareOnY( value );
        return 0;
    }
};

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controllers& controllers, const ::StaticModel& staticModel, const tools::Resolver< kernel::Object_ABC >& objects, IdManager& idManager )
    : controllers_        ( controllers )
    , staticModel_        ( staticModel )
    , objects_            ( objects )
    , urbanDisplayOptions_( new kernel::UrbanDisplayOptions( controllers, staticModel.accommodationTypes_ ) )
    , factory_            ( new UrbanFactory( controllers_, *this, staticModel, idManager, objects_, *urbanDisplayOptions_ ) )
    , geostore_           ( new geostore::GeoStoreManager( *this ) )
    , menuManager_        ( new UrbanMenuManager( controllers, *this ) )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::~UrbanModel()
{
    Purge();
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetFactory
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
UrbanFactory_ABC& UrbanModel::GetFactory() const
{
    assert( factory_.get() );
    return *factory_;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Purge
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::Purge()
{
    quadTree_.reset();
    geostore_.reset( new geostore::GeoStoreManager( *this ) );
    DeleteAll();
    menuManager_->Unregister();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Load
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanModel::Load()
{
    menuManager_->Register();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::NotifyUpdated
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::NotifyUpdated( const kernel::ModelLoaded& model )
{
    CreateQuadTree( model.config_.GetTerrainWidth(), model.config_.GetTerrainHeight() );
    geostore_->Initialize( model.config_.GetTerrainDir( model.config_.GetTerrainName() ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::LoadUrban
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
void UrbanModel::LoadUrban( xml::xistream& xis )
{
    DeleteAll();
    xis >> xml::start( "urban" )
            >> xml::start( "urban-objects" )
                >> xml::list( "urban-object", *this, &UrbanModel::ReadCity )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadCity
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
void UrbanModel::ReadCity( xml::xistream& xis )
{
    std::auto_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( xis, 0 ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        kernel::UrbanObject* ptr = static_cast< kernel::UrbanObject* >( pTerrainObject.release() );
        Register( ptr->GetId(), *ptr );
        ptr->Polish();
        xis >> xml::optional
                >> xml::start( "urban-objects" )
                    >> xml::list( "urban-object", *this, &UrbanModel::ReadDistrict, ptr )
               >> xml::end;
        //ptr->ComputeConvexHull(); // needed ?
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadDistrict
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
void UrbanModel::ReadDistrict( xml::xistream& xis, kernel::UrbanObject_ABC* parent )
{
    std::auto_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( xis, parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        kernel::UrbanObject* ptr = static_cast< kernel::UrbanObject* >( pTerrainObject.release() );
        Register( ptr->GetId(), *ptr );
        ptr->Polish();
        xis >> xml::optional
            >> xml::start( "urban-objects" )
                >> xml::list( "urban-object", *this, &UrbanModel::ReadBlock, ptr )
            >> xml::end;
        //ptr->ComputeConvexHull(); // needed ?
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadBlock
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
void UrbanModel::ReadBlock( xml::xistream& xis, kernel::UrbanObject_ABC* parent )
{
    std::auto_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( xis, parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        kernel::UrbanObject* ptr = static_cast< kernel::UrbanObject* >( pTerrainObject.release() );
        Register( ptr->GetId(), *ptr );
        ptr->Polish();
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Serialize
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    if( filename.empty() )
        return;
    assert( controllers_.modes_ != 0 );
    if( controllers_.modes_->GetCurrentMode() == ePreparationMode_Exercise )
        SerializeExercise( filename, schemaWriter );
    else
        SerializeTerrain( filename, schemaWriter );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::SerializeExercise
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanModel::SerializeExercise( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xml::xofstream xos( filename, xml::encoding( "UTF-8" ) );
    xos << xml::start( "urban-state" );
    schemaWriter.WriteExerciseSchema( xos, "urbanstate" );
    xos << xml::start( "urban-objects" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const UrbanHierarchies& hierarchy = static_cast< const UrbanHierarchies& >( it->second->Get< kernel::Hierarchies >() );
        if( hierarchy.GetLevel() != eUrbanLevelBlock ) // only city here, UrbanHierarchy proceed the recursion
            continue;
        xos << xml::start( "urban-object" )
            << xml::attribute( "id", it->second->GetId() );

        SerializeIFN< StructuralStateAttribute, kernel::StructuralStateAttribute_ABC >( *it->second, xos );
        SerializeIFN< ResourceNetworkAttribute, kernel::ResourceNetwork_ABC >( *it->second, xos );
        SerializeIFN< InfrastructureAttribute, kernel::Infrastructure_ABC >( *it->second, xos );
        SerializeIFN< MedicalTreatmentAttribute, kernel::MedicalTreatmentAttribute_ABC >( *it->second, xos );

        xos << xml::end;
    }
    xos << xml::end  // urban-objects
        << xml::end; // urban-state
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::SerializeTerrain
// Created: ABR 2012-05-22
// -----------------------------------------------------------------------------
void UrbanModel::SerializeTerrain( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xml::xofstream xos( filename, xml::encoding( "UTF-8" ) );
    xos << xml::start( "urban" );
    schemaWriter.WriteSchema( xos, "terrain", "urban" );
    xos<< xml::start( "urban-objects" );
    for( CIT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        const UrbanHierarchies& hierarchy = *static_cast< const UrbanHierarchies* >( it->second->Retrieve< kernel::Hierarchies >() );
        if( hierarchy.GetLevel() == eUrbanLevelCity ) // only city here, UrbanHierarchy proceed the recursion
        {
            xos << xml::start( "urban-object" );
            it->second->Interface().Apply( &kernel::Serializable_ABC::SerializeAttributes, xos );
            xos << xml::end; // urban-object
        }
    }
    xos << xml::end  // urban-objects
        << xml::end; // urban
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::LoadUrbanState
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::LoadUrbanState( xml::xistream& xis )
{
    xis >> xml::start( "urban-state" )
            >> xml::start( "urban-objects" )
                >> xml::list( "urban-object", *this, &UrbanModel::ReadUrbanObject )
            >> xml::end
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadUrbanObject
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::ReadUrbanObject( xml::xistream& xis )
{
    kernel::UrbanObject_ABC* object = Resolver< kernel::UrbanObject_ABC >::Find( xis.attribute< unsigned int >( "id" ) );
    if( object )
        xis >> xml::list( *this, &UrbanModel::ReadCapacity, *object );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadCapacity
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::ReadCapacity( const std::string& capacity, xml::xistream& xis, kernel::UrbanObject_ABC& object )
{
    // TODO faire ça proprement et de façon générique avec la factory d'objets quand elle sera implémentée (pour l'instant, c'est une par Team)
    if( capacity == "structural-state" )
        UpdateCapacity< StructuralStateAttribute, kernel::StructuralStateAttribute_ABC >( xis, object );
    else if( capacity == "resources" )
        UpdateCapacity< ResourceNetworkAttribute, kernel::ResourceNetwork_ABC >( xis, object );
    else if( capacity == "medical-treatment" )
        UpdateCapacity< MedicalTreatmentAttribute, kernel::MedicalTreatmentAttribute_ABC >( xis, object );
    else if( capacity == "infrastructure" )
        UpdateCapacity< InfrastructureAttribute, kernel::Infrastructure_ABC >( xis, object );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::UpdateCapacity
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
template< typename T, typename U >
void UrbanModel::UpdateCapacity( xml::xistream& xis, kernel::UrbanObject_ABC& object )
{
    T* capacity = static_cast< T* >( object.Retrieve< U >() );
    if( capacity )
        capacity->Update( xis );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::CreateDistrict
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanModel::CreateCityOrDistrict( kernel::Entity_ABC* parent )
{
    std::auto_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        kernel::UrbanObject* ptr = static_cast< kernel::UrbanObject* >( pTerrainObject.release() );
        Register( ptr->GetId(), *ptr );
        ptr->Polish();
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::CreateUrbanBlocs
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::CreateUrbanBlocks( const kernel::Location_ABC& location, kernel::UrbanObject_ABC& parent, bool isAuto )
{
    T_PointVector points = static_cast< const kernel::Polygon& >( location ).GetPoints();
    if( points.front() == points.back() )
        points.pop_back();
    if( points.size() < 3 || !geostore_.get() || !geostore_->IsInitialized() )
        return;
    const geometry::Polygon2f polygon( points );
    if( !isAuto )
    {
        if( geostore_->BlockAutoProcess( polygon ) )
            if( const kernel::UrbanObject_ABC* block = factory_->Create( polygon, &parent ) )
                controllers_.controller_.Create< kernel::UrbanObject_ABC >( *block );
    }
    else
    {
        std::vector< kernel::UrbanObject_ABC* > newBlocks;
        geostore_->CreateUrbanBlocksOnCities( polygon, parent, newBlocks ) ;
        for( int i = 0; i < newBlocks.size(); ++i )
            controllers_.controller_.Create< kernel::UrbanObject_ABC >( *newBlocks[ i ] );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::DeleteBlocs
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::DeleteBlocks( const kernel::UrbanObject_ABC& urbanObject )
{
    const UrbanHierarchies& hierarchy = *static_cast< const UrbanHierarchies* >( urbanObject.Retrieve< kernel::Hierarchies >() );
    tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& child = it.NextElement();
        DeleteBlocks( static_cast< const kernel::UrbanObject_ABC& >( child ) );
    }
    controllers_.controller_.Delete< kernel::UrbanObject_ABC >( urbanObject );
    Remove( urbanObject.GetId() );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::DeleteBlocs
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::DeleteBlocks( int minimumArea )
{
    tools::Iterator< const kernel::UrbanObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::UrbanObject_ABC& urbanObject = it.NextElement();
        if( const UrbanHierarchies* hierarchy = static_cast< const UrbanHierarchies* >( urbanObject.Retrieve< kernel::Hierarchies >() ) )
            if( hierarchy->GetLevel() == eUrbanLevelBlock )
                if( const kernel::UrbanPositions_ABC* position = urbanObject.Retrieve< kernel::UrbanPositions_ABC >() )
                    if( position->ComputeArea() < minimumArea )
                        DeleteBlocks( urbanObject );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::CreateQuadTree
// Created: JSR 2010-03-24
// -----------------------------------------------------------------------------
void UrbanModel::CreateQuadTree( float width, float height )
{
    const geometry::Rectangle2f rect( geometry::Point2f( 0, 0 ), geometry::Point2f( width, height ) );
    quadTree_.reset( new T_QuadTree( rect ) );
    quadTree_->SetRefinementPolicy( 30 );
    // $$$$ _RC_ JSR 2010-09-17: TODO Optimiser le quadtree en utilisant aussi les villes et quartiers pour accélérer la recherche

    precision_ = sqrt( ( rect.Right() - rect.Left() ) * ( rect.Top() - rect.Bottom() ) * 1e-16f );
    maxElementSize_ = 0;

    tools::Iterator< const kernel::UrbanObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::UrbanObject_ABC& urbanObject = it.NextElement();
        if( const UrbanHierarchies* urbanHierarchies = static_cast< const UrbanHierarchies* >( urbanObject.Retrieve< kernel::Hierarchies >() ) )
        {
            if( urbanHierarchies->GetLevel() != eUrbanLevelBlock )
                continue;

            const kernel::UrbanPositions_ABC* pAttribute = urbanObject.Retrieve< kernel::UrbanPositions_ABC >();
            if( pAttribute )
            {
                quadTree_->Insert( &urbanObject );
                geometry::Rectangle2f boundingBox = pAttribute->Polygon().BoundingBox();
                float size = 1.1f * std::max( boundingBox.Width(), boundingBox.Height() );
                if( maxElementSize_ < size )
                    maxElementSize_ = size;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::FindBlock
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
const kernel::UrbanObject_ABC* UrbanModel::FindBlock( const geometry::Point2f& center ) const
{
    if( quadTree_.get() )
    {
        spatialcontainer::PointIntersecter< float > intersecter( center, precision_, maxElementSize_ );
        PointChecker check( center );
        DataExtractor< PointChecker > extractor( check );

        quadTree_->Apply( intersecter, extractor );
        if( !extractor.data_.empty() )
            return *extractor.data_.begin();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::GetListWithinCircle
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::GetListWithinCircle( const geometry::Point2f& center, float radius, std::vector< const kernel::UrbanObject_ABC* >& result ) const
{
    if( quadTree_.get() )
    {
        spatialcontainer::SegmentIntersecter< float > intersecter( geometry::Point2f( center.X() - radius, center.Y() - radius ),
                                                                   geometry::Point2f( center.X() + radius, center.Y() + radius ), maxElementSize_ );
        CircleChecker checker( center, radius );
        DataExtractor< CircleChecker > extractor( checker );

        quadTree_->Apply( intersecter, extractor );
        std::swap( extractor.data_, result );
    }
}
