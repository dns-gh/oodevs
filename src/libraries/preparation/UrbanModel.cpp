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
#include "ResourceNetworkAttribute.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanFactory.h"
#include "UrbanHierarchies.h"
#include "UrbanMenuManager.h"

#include "clients_gui/UrbanObject.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Polygon.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "ENT/ENT_Enums.h"
#include "geostore/Geostore.h"
#include "tools/ExerciseConfig.h"
#include "tools/SchemaWriter_ABC.h"
#include "tools/IdManager.h"

#include <geometry/Types.h>
#include <boost/ref.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <QtGui/qmessagebox.h>
#include <xeumeuleu/xml.hpp>

#pragma warning( disable : 4355 )

namespace
{
    spatialcontainer::SegmentIntersecter< float > Intersector( const geometry::Rectangle2f& boundingBox )
    {
        return spatialcontainer::SegmentIntersecter< float >( boundingBox.BottomLeft(), boundingBox.TopRight() );
    };

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
UrbanModel::UrbanModel( kernel::Controllers& controllers, const ::StaticModel& staticModel, const tools::Resolver< kernel::Object_ABC >& objects, tools::IdManager& idManager )
    : controllers_        ( controllers )
    , staticModel_        ( staticModel )
    , idManager_          ( idManager )
    , objects_            ( objects )
    , urbanDisplayOptions_( new gui::UrbanDisplayOptions( controllers, staticModel.accommodationTypes_ ) )
    , factory_            ( new UrbanFactory( controllers_, *this, staticModel, idManager, objects_, *urbanDisplayOptions_ ) )
    , menuManager_        ( new UrbanMenuManager( controllers, *this, staticModel_ ) )
    , cleanedLinks_       ( false )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanModel destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::~UrbanModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Create
// Created: LGY 2012-08-27
// -----------------------------------------------------------------------------
kernel::UrbanObject_ABC* UrbanModel::Create( const geometry::Polygon2f& location, kernel::Entity_ABC* parent )
{
    kernel::UrbanObject_ABC* ptr = factory_->Create( location, parent );
    if( ptr )
    {
        Register( ptr->GetId(), *ptr );
        InsertIntoQuadTree( *ptr );
    }
    return ptr;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Purge
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::Purge()
{
    quadTree_.reset();
    geostore_.reset();
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
    tools::Iterator< const kernel::UrbanObject_ABC& > it = CreateIterator();
    while( it.HasMoreElements() )
    {
        kernel::UrbanObject_ABC& block = const_cast< kernel::UrbanObject_ABC& >( it.NextElement() );
        block.UpdateTemplate( staticModel_.objectTypes_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::NotifyUpdated
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::CreateGeostoreManager( const tools::ExerciseConfig& config )
{
    CreateQuadTree( config.GetTerrainWidth(), config.GetTerrainHeight() );
    geostore_.reset( new geostore::Geostore( config.GetTerrainDir( config.GetTerrainName() ), *this ) );
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
    CleanLinks();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadCity
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
void UrbanModel::ReadCity( xml::xistream& xis )
{
    std::unique_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( xis, 0 ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        auto ptr = static_cast< gui::UrbanObject* >( pTerrainObject.release() );
        Register( ptr->GetId(), *ptr );
        xis >> xml::optional
                >> xml::start( "urban-objects" )
                    >> xml::list( "urban-object", *this, &UrbanModel::ReadDistrict, ptr )
               >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadDistrict
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
void UrbanModel::ReadDistrict( xml::xistream& xis, kernel::UrbanObject_ABC* parent )
{
    std::unique_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( xis, parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        auto ptr = static_cast< gui::UrbanObject* >( pTerrainObject.release() );
        Register( ptr->GetId(), *ptr );
        xis >> xml::optional
            >> xml::start( "urban-objects" )
                >> xml::list( "urban-object", *this, &UrbanModel::ReadBlock, ptr )
            >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadBlock
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
void UrbanModel::ReadBlock( xml::xistream& xis, kernel::UrbanObject_ABC* parent )
{
    std::unique_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( xis, parent ) );
    if( pTerrainObject.get() && !Find( pTerrainObject->GetId() ) )
    {
        auto ptr = static_cast< gui::UrbanObject* >( pTerrainObject.release() );
        Register( ptr->GetId(), *ptr );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Serialize
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::Serialize( const tools::Path& filename, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    if( filename.IsEmpty() )
        return;
    tools::Path directory = filename.Parent();
    try
    {
        if( !directory.Exists() )
            directory.CreateDirectories();
    }
    catch( std::exception& )
    {
        directory.CreateDirectories();
    }
    tools::Xofstream xos( filename );
    xos << xml::start( "urban" );
    schemaWriter.WriteSchema( xos, "exercise", "urban" );
    xos<< xml::start( "urban-objects" );
    for( auto it = elements_.begin(); it != elements_.end(); ++it )
    {
        const UrbanHierarchies& hierarchy = *static_cast< const UrbanHierarchies* >( it->second->Retrieve< kernel::Hierarchies >() );
        if( hierarchy.GetLevel() == eUrbanLevelCity ) // only city here, UrbanHierarchy proceed the recursion
        {
            xos << xml::start( "urban-object" );
            it->second->GetInterfaces().Apply( &kernel::Serializable_ABC::SerializeAttributes, xos );
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
    CleanLinks();
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
        UpdateCapacity< ResourceNetworkAttribute, gui::ResourceNetwork_ABC >( xis, object );
    else if( capacity == "infrastructure" )
        UpdateCapacity< InfrastructureAttribute, gui::Infrastructure_ABC >( xis, object );
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
    std::unique_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        auto ptr = static_cast< gui::UrbanObject* >( pTerrainObject.release() );
        Register( ptr->GetId(), *ptr );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::CreateAutoUrbanBlocks
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::CreateAutoUrbanBlocks( const kernel::Location_ABC& location, kernel::UrbanObject_ABC& parent, double roadWidth )
{
    if( !geostore_.get() )
        return;

    T_PointVector points = static_cast< const kernel::Polygon& >( location ).GetPoints();
    if( points.front() == points.back() )
        points.pop_back();

    if( points.size() < 3 )
        return;

    const geometry::Polygon2f polygon( points );
    std::vector< geometry::Polygon2f > blocks;
    geostore_->CreateUrbanBlocksOnCities( polygon, roadWidth, blocks );
    // Create the blocks
    for( auto it = blocks.begin(); it != blocks.end(); ++it )
        Create( *it, &parent );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::CreateManualUrbanBlock
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::CreateManualUrbanBlock( const kernel::Location_ABC& location, kernel::UrbanObject_ABC& parent, std::vector< const kernel::UrbanObject_ABC* >& intersectedBlocks )
{
    if( !geostore_.get() )
        return;

    T_PointVector points = static_cast< const kernel::Polygon& >( location ).GetPoints();
    if( points.front() == points.back() )
        points.pop_back();

    if( points.size() < 3 )
        return;

    const geometry::Polygon2f polygon( points );
    intersectedBlocks = geostore_->IntersectedBlocks( polygon );
    if( intersectedBlocks.empty() )
        Create( polygon, &parent );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ChangeShape
// Created: JSR 2013-01-07
// -----------------------------------------------------------------------------
void UrbanModel::ChangeShape( const kernel::Location_ABC& location, kernel::UrbanObject_ABC& block, std::vector< const kernel::UrbanObject_ABC* >& intersectedBlocks )
{
    if( !geostore_.get() )
        return;

    T_PointVector points = static_cast< const kernel::Polygon& >( location ).GetPoints();
    if( points.front() == points.back() )
        points.pop_back();

    if( points.size() < 3 )
        return;

    quadTree_->ForceErase( &block );
    const geometry::Polygon2f polygon( points );
    intersectedBlocks = geostore_->IntersectedBlocks( polygon );
    if( intersectedBlocks.empty() )
        block.Get< kernel::UrbanPositions_ABC >().ChangeGeometry( points );
    InsertIntoQuadTree( block );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::DeleteBlock
// Created: ABR 2012-06-01
// -----------------------------------------------------------------------------
void UrbanModel::DeleteBlock( const kernel::UrbanObject_ABC& urbanObject )
{
    const UrbanHierarchies& hierarchy = static_cast< const UrbanHierarchies& >( urbanObject.Get< kernel::Hierarchies >() );
    if( hierarchy.GetSuperior() )
        const_cast< kernel::Entity_ABC* >( hierarchy.GetSuperior() )->Get< kernel::UrbanPositions_ABC >().ResetConvexHull();
    tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
    while( it.HasMoreElements() )
        DeleteBlock( static_cast< const kernel::UrbanObject_ABC& >( it.NextElement() ) );
    if( hierarchy.GetLevel() == eUrbanLevelBlock )
        quadTree_->ForceErase( &urbanObject );
    Remove( urbanObject.GetId() );
    delete &urbanObject;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::DeleteBlocks
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void UrbanModel::DeleteBlocks( const std::vector< const kernel::UrbanObject_ABC* >& urbanObjects )
{
    if( QMessageBox::warning( 0, tools::translate( "UrbanModel", "Warning" ), tools::translate( "UrbanModel", "Delete all selected elements?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::No )
        return;
    std::set< const kernel::UrbanObject_ABC* > toDelete;
    for( std::vector< const kernel::UrbanObject_ABC* >::const_iterator it = urbanObjects.begin(); it != urbanObjects.end(); ++it )
    {
        const kernel::UrbanObject_ABC* object = *it;
        if( !object )
            continue;
        const UrbanHierarchies* hierarchy = static_cast< const UrbanHierarchies* >( object->Retrieve< kernel::Hierarchies >() );
        if( !hierarchy )
            return;
        if( hierarchy->GetLevel() == eUrbanLevelCity )
            toDelete.insert( object );
        else if( hierarchy->GetLevel() == eUrbanLevelDistrict )
        {
            if( hierarchy->GetSuperior() && std::find( urbanObjects.begin(), urbanObjects.end(), hierarchy->GetSuperior() ) == urbanObjects.end() )
                toDelete.insert( object );
        }
        else if( hierarchy->GetLevel() == eUrbanLevelBlock )
        {
            if( hierarchy->GetSuperior() && std::find( urbanObjects.begin(), urbanObjects.end(), hierarchy->GetSuperior() ) == urbanObjects.end() )
            {
                const kernel::Hierarchies* district = hierarchy->GetSuperior()->Retrieve< kernel::Hierarchies >();
                if( district && district->GetSuperior() && std::find( urbanObjects.begin(), urbanObjects.end(), district->GetSuperior() ) == urbanObjects.end() )
                    toDelete.insert( object );
            }
        }
    }
    for( std::set< const kernel::UrbanObject_ABC* >::const_iterator itDelete = toDelete.begin(); itDelete != toDelete.end(); ++itDelete )
        DeleteBlock( **itDelete );
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
            if( urbanHierarchies->GetLevel() == eUrbanLevelBlock )
                InsertIntoQuadTree( urbanObject );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::InsertIntoQuadTree
// Created: JSR 2013-01-10
// -----------------------------------------------------------------------------
void UrbanModel::InsertIntoQuadTree( const kernel::UrbanObject_ABC& urbanObject )
{
    if( const kernel::UrbanPositions_ABC* pAttribute = urbanObject.Retrieve< kernel::UrbanPositions_ABC >() )
    {
        quadTree_->Insert( &urbanObject );
        geometry::Rectangle2f boundingBox = pAttribute->Polygon().BoundingBox();
        maxElementSize_ = std::max( maxElementSize_, 1.1f * std::max( boundingBox.Width(), boundingBox.Height() ) );
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

// -----------------------------------------------------------------------------
// Name: UrbanModel::CleanLinks
// Created: LDC 2012-08-08
// -----------------------------------------------------------------------------
void UrbanModel::CleanLinks()
{
    cleanedLinks_ = false;
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
    {
        kernel::Entity_ABC& entity = *it->second;
        auto* abstractExtension = entity.Retrieve< gui::ResourceNetwork_ABC >();
        if( !abstractExtension )
            continue;
        ResourceNetworkAttribute* extension = static_cast< ResourceNetworkAttribute* >( abstractExtension );
        cleanedLinks_ = extension->CleanLinksToDeletedUrbanBlocks() | cleanedLinks_;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::TakeLinkErrors
// Created: LDC 2012-09-07
// -----------------------------------------------------------------------------
bool UrbanModel::TakeLinkErrors()
{
    bool result = cleanedLinks_;
    cleanedLinks_ = false;
    return result;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::CheckIdsConflict
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
void UrbanModel::CheckIdConflict( kernel::UrbanObject_ABC& urban )
{
    if( idManager_.HasBeenKept( urban.GetId() ) )
    {
        UrbanIdReplacement& urbanNewId = urbanConflictIds_[ urban.GetId() ];
        urbanNewId.pUrban_ = &urban;
        urbanNewId.newId_ = idManager_.GetNextId( true );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReplaceLinkId
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
void UrbanModel::ReplaceLinkId( kernel::UrbanObject_ABC& urban )
{
    for( auto it = urbanConflictIds_.begin();  it != urbanConflictIds_.end(); ++it )
    {
        auto pResource = dynamic_cast< ResourceNetworkAttribute* >( urban.Retrieve< gui::ResourceNetwork_ABC >() );
        if( pResource )
            pResource->ReplaceLinksUrbanId( it->first, it->second.newId_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ChangeUrbanObjectId
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
void UrbanModel::ChangeUrbanObjectId( kernel::UrbanObject_ABC& urban, unsigned long newId )
{
    Remove( urban.GetId() );
    static_cast< UrbanHierarchies* >( urban.Retrieve< kernel::Hierarchies >() )->ForceEntityNewId( newId );
    Register( urban.GetId(), urban );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ManageIdConflicts
// Created: MMC 2013-05-29
// -----------------------------------------------------------------------------
bool UrbanModel::ManageIdConflicts()
{
    bool bConflicts = false;
    urbanConflictIds_.clear();
    Apply( boost::bind( &UrbanModel::CheckIdConflict, this, _1 ) );
    if( !urbanConflictIds_.empty() )
    {
        bConflicts = true;
        Apply( boost::bind( &UrbanModel::ReplaceLinkId, this, _1 ) );
    }
    for( auto it = urbanConflictIds_.begin();  it != urbanConflictIds_.end(); ++it )
        ChangeUrbanObjectId( *it->second.pUrban_, it->second.newId_ );
    urbanConflictIds_.clear();
    return bConflicts;
}
