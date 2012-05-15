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
#include "UrbanFactory_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "tools/SchemaWriter_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>

#pragma warning( disable : 4355 )

namespace bfs = boost::filesystem;

using namespace kernel;
using namespace tools;

namespace
{
    const std::string defaultUrbanStateVersion = "4.2.3";
}

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( Controllers& controllers, const ::StaticModel& staticModel, const Resolver< Object_ABC >& objects )
    : controllers_        ( controllers )
    , objectTypes_        ( staticModel.objectTypes_ )
    , accommodationTypes_ ( staticModel.accommodationTypes_ )
    , objects_            ( objects )
    , urbanStateVersion_  ( ::defaultUrbanStateVersion )
    , urbanDisplayOptions_( new gui::UrbanDisplayOptions( controllers, accommodationTypes_ ) )
    , factory_            ( new UrbanFactory( controllers_, objectTypes_, objects_,
                                              *urbanDisplayOptions_, *this, accommodationTypes_, staticModel.coordinateConverter_ ) )
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

// -----------------------------------------------------------------------------
// Name: UrbanModel::Load
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
void UrbanModel::Load( const std::string& directoryPath )
{
    Purge();
    const bfs::path fullPath = bfs::path( directoryPath, bfs::native ) / "urban" / "urban.xml";
    if( bfs::exists( fullPath ) )
    {
        xml::xifstream input( fullPath.native_file_string() );
        input >> xml::start( "urban" )
                >> xml::start( "urban-objects" )
                   >> xml::list( "urban-object", *this, &UrbanModel::ReadCity )
                >> xml::end
              >> xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadCity
// Created: LGY 2012-04-10
// -----------------------------------------------------------------------------
void UrbanModel::ReadCity( xml::xistream& xis )
{
    std::auto_ptr< gui::TerrainObjectProxy > pTerrainObject( factory_->Create( xis, 0 ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        gui::TerrainObjectProxy* ptr = pTerrainObject.release();
        ptr->Polish();
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
void UrbanModel::ReadDistrict( xml::xistream& xis, gui::TerrainObjectProxy* parent )
{
    std::auto_ptr< gui::TerrainObjectProxy > pTerrainObject( factory_->Create( xis, parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        gui::TerrainObjectProxy* ptr = pTerrainObject.release();
        ptr->Polish();
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
void UrbanModel::ReadBlock( xml::xistream& xis, gui::TerrainObjectProxy* parent )
{
    std::auto_ptr< gui::TerrainObjectProxy > pTerrainObject( factory_->Create( xis, parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        gui::TerrainObjectProxy* ptr = pTerrainObject.release();
        ptr->Polish();
        Register( ptr->GetId(), *ptr );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Serialize
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::Serialize( const std::string& filename, const SchemaWriter_ABC& schemaWriter ) const
{
    if( filename.empty() )
        return;
    xml::xofstream xos( filename, xml::encoding( "UTF-8" ) );
    xos << xml::start( "urban-state" );
    schemaWriter.WriteExerciseSchema( xos, "urbanstate" );
    xos << xml::attribute( "model-version", urbanStateVersion_ )
            << xml::start( "urban-objects" );
    for( Resolver< gui::TerrainObjectProxy >::CIT_Elements it = Resolver< gui::TerrainObjectProxy >::elements_.begin(); it != Resolver< gui::TerrainObjectProxy >::elements_.end(); ++it )
    {
        bool needsUpdate = false;
        it->second->Interface().Apply( & Overridable_ABC::SetOverriden, needsUpdate );// Temp pour serializer l'attribut
        if( needsUpdate )
        {
            xos << xml::start( "urban-object" )
                << xml::attribute( "id", it->second->GetId() );
            it->second->Interface().Apply( & Serializable_ABC::SerializeAttributes, xos );
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
    gui::TerrainObjectProxy* proxy = Resolver< gui::TerrainObjectProxy >::Find( xis.attribute< unsigned int >( "id" ) );
    if( proxy )
        xis >> xml::list( *this, &UrbanModel::ReadCapacity, *proxy );
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::ReadCapacity
// Created: JSR 2010-06-22
// -----------------------------------------------------------------------------
void UrbanModel::ReadCapacity( const std::string& capacity, xml::xistream& xis, gui::TerrainObjectProxy& proxy )
{
    // TODO faire ça proprement et de façon générique avec la factory d'objets quand elle sera implémentée (pour l'instant, c'est une par Team)
    if( capacity == "structural-state" )
        UpdateCapacity< StructuralStateAttribute, StructuralStateAttribute_ABC >( xis, proxy );
    else if( capacity == "resources" )
        UpdateCapacity< ResourceNetworkAttribute, ResourceNetwork_ABC >( xis, proxy );
    else if( capacity == "medical-treatment" )
        UpdateCapacity< MedicalTreatmentAttribute, MedicalTreatmentAttribute_ABC >( xis, proxy );
    else if( capacity == "infrastructure" )
        UpdateCapacity< InfrastructureAttribute, Infrastructure_ABC >( xis, proxy );
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
    urbanStateVersion_ = ::defaultUrbanStateVersion;
}
