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
#include "Architecture.h"
#include "ConsistencyErrorTypes.h"
#include "InfrastructureAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "Model.h"
#include "ResourceNetworkAttribute.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "UrbanPositions.h"
#include "Usages.h"
#include "UrbanColor.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_gui/Usages.h"
#include "clients_gui/Architecture.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "tools/SchemaWriter_ABC.h"
#include <urban/PhysicalAttribute.h>
#include <urban/InfrastructureAttribute.h>
#include <urban/ResourceNetworkAttribute.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/GeometryAttribute.h>
#include <urban/ColorAttribute.h>
#include <urban/TerrainObjectVisitor_ABC.h>
#include <xeumeuleu/xml.hpp>

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
UrbanModel::UrbanModel( Controllers& controllers, const StaticModel& staticModel, const Resolver< Object_ABC >& objects )
    : controllers_        ( controllers )
    , objectTypes_        ( staticModel.objectTypes_ )
    , accommodationTypes_ ( staticModel.accommodationTypes_ )
    , objects_            ( objects )
    , urbanStateVersion_  ( ::defaultUrbanStateVersion )
    , urbanDisplayOptions_( new gui::UrbanDisplayOptions( controllers, accommodationTypes_ ) )
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
        UrbanSendingCreationVisitor( UrbanModel& urbanModel, Model& model, const ObjectTypes& objectTypes )
            : urbanModel_ ( urbanModel )
            , model_      ( model )
            , objectTypes_( objectTypes )
        {
            // NOTHING
        }

        ~UrbanSendingCreationVisitor()
        {
            // NOTHING
        }

        virtual void VisitBlock( urban::TerrainObject_ABC& urbanObject )
        {
            const urban::PhysicalAttribute* pPhysical = urbanObject.Retrieve< urban::PhysicalAttribute >();
            if( pPhysical && pPhysical->GetArchitecture() )
            {
                if( !objectTypes_.StringResolver< MaterialCompositionType >::Find( pPhysical->GetArchitecture()->GetMaterial() ) )
                {
                    model_.AppendLoadingError( eOthers, "Urban Bloc " + urbanObject.GetName() + ": Unknown Material " + pPhysical->GetArchitecture()->GetMaterial() );
                    return;
                }
                if( !objectTypes_.StringResolver< RoofShapeType >::Find( pPhysical->GetArchitecture()->GetRoofShape() ) )
                {
                    model_.AppendLoadingError( eOthers, "Urban Bloc " + urbanObject.GetName() + ": Unknown Roof shape " + pPhysical->GetArchitecture()->GetRoofShape() );
                    return;
                }
            }
            urbanModel_.SendCreation( urbanObject );
        }
    private:
        UrbanModel& urbanModel_;
        Model& model_;
        const ObjectTypes& objectTypes_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Load
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
void UrbanModel::Load( const std::string& directoryPath, urban::WorldParameters& world, ::Model& model )
{
    Purge();
    urban::Model::Load( directoryPath, world );
    UrbanSendingCreationVisitor visitor( *this, model, objectTypes_ );
    Accept( visitor );
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
    urban::Model::Purge();
    urbanStateVersion_ = ::defaultUrbanStateVersion;
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::SendCreation
// Created: JSR 2010-06-21
// -----------------------------------------------------------------------------
void UrbanModel::SendCreation( urban::TerrainObject_ABC& urbanObject )
{
    const ObjectType& type = objectTypes_.StringResolver< ObjectType >::Get( "urban block" );
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controllers_, urbanObject.GetName(), urbanObject.GetId(), type, *urbanDisplayOptions_, accommodationTypes_ );
    PropertiesDictionary& dictionary = pTerrainObject->Get< PropertiesDictionary >();
    pTerrainObject->Attach< StructuralStateAttribute_ABC >( *new StructuralStateAttribute( 100, dictionary ) );
    pTerrainObject->Attach< UrbanColor_ABC >( *new UrbanColor( urbanObject.Retrieve< urban::ColorAttribute >() ) );
    UrbanPositions_ABC* urbanPositions = 0;
    if( const urban::GeometryAttribute* pGeometryAttribute = urbanObject.Retrieve< urban::GeometryAttribute >() )
    {
        urbanPositions = new UrbanPositions( urbanObject, pTerrainObject->Retrieve< UrbanColor_ABC >() );
        pTerrainObject->Attach< UrbanPositions_ABC >( *urbanPositions );
        const urban::ResourceNetworkAttribute* resource = urbanObject.Retrieve< urban::ResourceNetworkAttribute >();
        pTerrainObject->Attach< ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, resource, pTerrainObject->Get< UrbanPositions_ABC >().Barycenter(), *this, objects_, objectTypes_ ) );
    }
    const urban::PhysicalAttribute* pPhysical = urbanObject.Retrieve< urban::PhysicalAttribute >();
    if( pPhysical && pPhysical->GetArchitecture() )
        pTerrainObject->Attach< Architecture_ABC >( *new Architecture( *pPhysical->GetArchitecture(), std::auto_ptr< Architecture_ABC >( new gui::Architecture( dictionary ) ) ) );
    pTerrainObject->Attach< Usages_ABC >( *new Usages( urbanObject, std::auto_ptr< Usages_ABC >( new gui::Usages( dictionary, accommodationTypes_, pTerrainObject->GetLivingSpace() ) ) ) );
    const urban::InfrastructureAttribute* infra = urbanObject.Retrieve< urban::InfrastructureAttribute >();
    if( infra && urbanPositions )
        if( const InfrastructureType* infraType = objectTypes_.StringResolver< InfrastructureType >::Find( infra->GetType() ) )
        {
            pTerrainObject->Attach< Infrastructure_ABC >( *new InfrastructureAttribute( urbanPositions->Barycenter(), *infraType, dictionary ) );
            if( infraType->FindCapacity( "medical" ) )
                pTerrainObject->Attach< MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( objectTypes_, dictionary ) );
            urbanPositions->SetInfrastructurePresent();
        }
    pTerrainObject->Polish();
    if( !Resolver< gui::TerrainObjectProxy >::Find( urbanObject.GetId() ) )
        Resolver< gui::TerrainObjectProxy >::Register( urbanObject.GetId(), *pTerrainObject );
}
