// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanModel.h"
#include "ResourceNetworkAttribute.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanPositions.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PropertiesDictionary.h"
#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
#include <urban/TerrainObject_ABC.h>
#include <urban/TerrainObjectVisitor_ABC.h>
#include <xeumeuleu/xml.hpp>

namespace bfs = boost::filesystem;
using namespace tools;

namespace
{
    const std::string defaultUrbanStateVersion = "4.2.3";
}

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controllers& controllers, const StaticModel& staticModel )
    : controllers_( controllers )
    , static_( staticModel )
    , urbanStateVersion_( ::defaultUrbanStateVersion )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanModel destructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::~UrbanModel()
{
    Purge();
}

namespace
{
    class UrbanSendingCreationVisitor : public urban::TerrainObjectVisitor_ABC
    {
    public:
        UrbanSendingCreationVisitor( UrbanModel& model ) : model_( model )
        {}
        ~UrbanSendingCreationVisitor()
        {}
        virtual void VisitBlock( urban::TerrainObject_ABC& urbanObject )
        {
            model_.SendCreation( urbanObject );
        }
    private:
        UrbanModel& model_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Load
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
bool UrbanModel::Load( const std::string& directoryPath, urban::WorldParameters& world )
{
    Purge();
    bool ret = urban::Model::Load( directoryPath, world );
    if( ret )
    {
        UrbanSendingCreationVisitor visitor( *this );
        Accept( visitor );
        const bfs::path fullPath = bfs::path( directoryPath, bfs::native ) / "urban" / "urban.xml";
        if( bfs::exists( fullPath ) )
            LoadInfrastructures( fullPath.native_file_string() );
    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Serialize
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::Serialize( const std::string& filename ) const
{
    if( filename.empty() )
        return;
    xml::xofstream xos( filename, xml::encoding( "UTF-8" ) );
    xos << xml::start( "urban-state" )
        << xml::attribute( "xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance" )
        << xml::attribute( "xsi:noNamespaceSchemaLocation", "schemas/exercise/urbanstate.xsd" )
        << xml::attribute( "model-version", urbanStateVersion_ )
            << xml::start( "urban-objects" );
    for( Resolver< gui::TerrainObjectProxy >::CIT_Elements it = Resolver< gui::TerrainObjectProxy >::elements_.begin(); it != Resolver< gui::TerrainObjectProxy >::elements_.end(); ++it )
    {
        xos << xml::start( "urban-object" )
                << xml::attribute( "id", it->second->GetId() );
        // TODO appliquer serialize a toutes les capacit�s
        StructuralStateAttribute* attribute = static_cast< StructuralStateAttribute* >( it->second->Retrieve< kernel::StructuralStateAttribute_ABC >() );
        if( attribute )
        {
            xos     << xml::start( "structural-state" )
                        << xml::attribute( "value", attribute->structuralState_ )
                    << xml::end;
        }
        xos << xml::end;
    }
    xos     << xml::end // urban-objects
        << xml::end;  // urban-state
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::LoadUrbanState
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::LoadUrbanState( xml::xistream& xis )
{
    xis >> xml::start( "urban-state" )
        >> xml::optional >> xml::attribute( "model-version", urbanStateVersion_ );
    if( urbanStateVersion_.empty() )
        urbanStateVersion_ = ::defaultUrbanStateVersion;
    xis     >> xml::start( "urban-objects" )
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
    unsigned int id;
    xis >> xml::attribute( "id", id );
    gui::TerrainObjectProxy* proxy = Resolver< gui::TerrainObjectProxy >::Find( id );
    if( proxy )
        xis >> xml::list( *this, &UrbanModel::ReadCapacity, *proxy );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadCapacity
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::ReadCapacity( const std::string& capacity, xml::xistream& xis, gui::TerrainObjectProxy& proxy )
{
    // TODO faire �a de fa�on g�n�rique
    if( capacity == "structural-state" )
    {
        // Temp -> utiliser la s�rialization
        unsigned int value;
        xis >> xml::attribute( "value", value );
        StructuralStateAttribute* structural = static_cast< StructuralStateAttribute* >( proxy.Retrieve< kernel::StructuralStateAttribute_ABC >() );
        if( structural )
            structural->structuralState_ = value;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::LoadInfrastructures
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanModel::LoadInfrastructures( const std::string& directoryPath )
{
    xml::xifstream xis( directoryPath );
    xis >> xml::start( "urban" )
            >> xml::start( "urban-objects" )
             >> xml::list( "urban-object", *this, &UrbanModel::ReadInfrastructures )
            >> xml::end()
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadInfrastructures
// Created: JSR 2010-09-07
// -----------------------------------------------------------------------------
void UrbanModel::ReadInfrastructures( xml::xistream& xis )
{
    unsigned int id = xis.attribute< unsigned int >( "id" );
    gui::TerrainObjectProxy* proxy = Resolver< gui::TerrainObjectProxy >::Find( id );
    if( proxy )
    {
        if( xis.has_child( "resources" ) )
        {
            xis >> xml::start( "resources" );
                proxy->Attach< kernel::ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, xis, id, *this, static_.objectTypes_ ) );
            xis >> xml::end();
        }
        // $$$$ JSR 2010-08-12: TODO read infrastructures
        xis >> xml::optional >> xml::start( "infrastructures" )
            >> xml::end();
    }
    xis >> xml::start( "urban-objects" )
            >> xml::list( "urban-object", *this, &UrbanModel::ReadInfrastructures )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Purge
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
void UrbanModel::Purge()
{
    Resolver< gui::TerrainObjectProxy >::DeleteAll();
    urban::Model::Purge();
    urbanStateVersion_ = ::defaultUrbanStateVersion;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::SendCreation
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
void UrbanModel::SendCreation( urban::TerrainObject_ABC& urbanObject )
{
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controllers_.controller_, urbanObject );
    kernel::PropertiesDictionary& dico = pTerrainObject->Get< kernel::PropertiesDictionary >();
    pTerrainObject->Attach< kernel::StructuralStateAttribute_ABC >( *new StructuralStateAttribute( 100, dico ) );
    pTerrainObject->Attach< kernel::Positions >( *new UrbanPositions( urbanObject/*, static_.coordinateConverter_*/ ) );
    pTerrainObject->Polish();
    if( !Resolver< gui::TerrainObjectProxy >::Find( urbanObject.GetId() ) )
        Resolver< gui::TerrainObjectProxy >::Register( urbanObject.GetId(), *pTerrainObject );
}
