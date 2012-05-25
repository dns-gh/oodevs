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
#include "UrbanHierarchies.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "ENT/ENT_Enums_Gen.h"
#include "tools/SchemaWriter_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/filesystem.hpp>

#pragma warning( disable : 4355 )

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: UrbanModel constructor
// Created: SLG 2009-10-20
// -----------------------------------------------------------------------------
UrbanModel::UrbanModel( kernel::Controllers& controllers, const ::StaticModel& staticModel, const tools::Resolver< kernel::Object_ABC >& objects )
    : controllers_        ( controllers )
    , objectTypes_        ( staticModel.objectTypes_ )
    , accommodationTypes_ ( staticModel.accommodationTypes_ )
    , objects_            ( objects )
    , urbanDisplayOptions_( new kernel::UrbanDisplayOptions( controllers, accommodationTypes_ ) )
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
    DeleteAll();
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
        kernel::UrbanObject_ABC* ptr = pTerrainObject.release();
        Register( ptr->GetId(), *ptr );
        ptr->Polish();
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
    std::auto_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( xis, parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        kernel::UrbanObject_ABC* ptr = pTerrainObject.release();
        Register( ptr->GetId(), *ptr );
        ptr->Polish();
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
    std::auto_ptr< kernel::UrbanObject_ABC > pTerrainObject( factory_->Create( xis, parent ) );
    if( !Find( pTerrainObject->GetId() ) )
    {
        kernel::UrbanObject_ABC* ptr = pTerrainObject.release();
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

namespace
{
    template< typename ConcretType, typename AbstractType >
    void SerializeIFN( const kernel::Entity_ABC& entity, xml::xostream& xos )
    {
        const ConcretType& extension = static_cast< const ConcretType& >( entity.Get< AbstractType >() );
        if( extension.IsOverriden() )
            extension.SerializeAttributes( xos );
    }
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
        const UrbanHierarchies& hierarchy = *static_cast< const UrbanHierarchies* >( it->second->Retrieve< kernel::Hierarchies >() );
        if( hierarchy.GetLevel() != eUrbanLevelBlock ) // only city here, UrbanHierarchy proceed the recursion
            continue;
        xos << xml::start( "urban-object" )
            << xml::attribute( "id", it->second->GetId() );

        SerializeIFN< StructuralStateAttribute, kernel::StructuralStateAttribute_ABC >( *it->second, xos );
        SerializeIFN< ResourceNetworkAttribute, kernel::ResourceNetwork_ABC >( *it->second, xos );
        SerializeIFN< InfrastructureAttribute, kernel::Infrastructure_ABC >( *it->second, xos );

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
