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
#include "Overridable_ABC.h"
#include "ResourceNetworkAttribute.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanPositions.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/PropertiesDictionary.h"
#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )
#include <urban/Architecture.h>
#include <urban/ResourceNetworkAttribute.h>
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
        UrbanSendingCreationVisitor( UrbanModel& model, const StaticModel& staticModel, std::string& loadingErrors ) : model_( model ), static_( staticModel ), loadingErrors_( loadingErrors )
        {}
        ~UrbanSendingCreationVisitor()
        {}
        virtual void VisitBlock( urban::TerrainObject_ABC& urbanObject )
        {
            const urban::Architecture* architecture = urbanObject.Retrieve< urban::Architecture >();
            if( architecture && ( !static_.objectTypes_.StringResolver< kernel::MaterialCompositionType >::Find( architecture->GetMaterial() ) || !static_.objectTypes_.StringResolver< kernel::RoofShapeType >::Find( architecture->GetRoofShape() ) ) )
            {
                loadingErrors_ += "Urban Bloc : " + urbanObject.GetName() + "\n";
                return;
            }
            model_.SendCreation( urbanObject );
        }
    private:
        UrbanModel& model_;
        const StaticModel& static_;
        std::string& loadingErrors_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Load
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
bool UrbanModel::Load( const std::string& directoryPath, urban::WorldParameters& world, std::string& loadingErrors )
{
    Purge();
    bool ret = urban::Model::Load( directoryPath, world );
    if( ret )
    {
        UrbanSendingCreationVisitor visitor( *this, static_, loadingErrors );
        Accept( visitor );
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
        bool needsUpdate = false;
        it->second->Interface().Apply( & Overridable_ABC::SetOverriden, needsUpdate );
        if( needsUpdate )
        {
            xos << xml::start( "urban-object" )
                << xml::attribute( "id", it->second->GetId() );
            it->second->Interface().Apply( & kernel::Serializable_ABC::SerializeAttributes, xos );
            xos << xml::end;
        }
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
    // TODO faire �a proprement et de fa�on g�n�rique avec la factory d'objets quand elle sera impl�ment�e (pour l'instant, c'est une par Team)
    if( capacity == "structural-state" )
        UpdateCapacity< StructuralStateAttribute, kernel::StructuralStateAttribute_ABC >( xis, proxy );
    else if( capacity == "resources" )
        UpdateCapacity< ResourceNetworkAttribute, kernel::ResourceNetwork_ABC >( xis, proxy );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::UpdateCapacity
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
template< typename T, typename U >
void UrbanModel::UpdateCapacity( xml::xistream& xis, gui::TerrainObjectProxy& proxy )
{
    T* capacity = static_cast< T* >( proxy.Retrieve< U >() );
    if( capacity )
        capacity->Update( xis );
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
    pTerrainObject->Attach< kernel::Positions >( *new UrbanPositions( urbanObject ) );
    const urban::ResourceNetworkAttribute* resource = urbanObject.Retrieve< urban::ResourceNetworkAttribute >();
    if( resource )
        pTerrainObject->Attach< kernel::ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, *resource, pTerrainObject->GetId(), *this, static_.objectTypes_ ) );
    pTerrainObject->Polish();
    if( !Resolver< gui::TerrainObjectProxy >::Find( urbanObject.GetId() ) )
        Resolver< gui::TerrainObjectProxy >::Register( urbanObject.GetId(), *pTerrainObject );
}
