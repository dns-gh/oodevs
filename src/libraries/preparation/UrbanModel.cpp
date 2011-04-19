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
#include "MedicalTreatmentAttribute.h"
#include "Overridable_ABC.h"
#include "ResourceNetworkAttribute.h"
#include "StaticModel.h"
#include "StructuralStateAttribute.h"
#include "InfrastructureAttribute.h"
#include "UrbanPositions.h"
#include "Usages.h"
#include "UrbanColor.h"
#include "Architecture.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_gui/Usages.h"
#include "clients_gui/Architecture.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/MaterialCompositionType.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/RoofShapeType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Usages_ABC.h"
#include "clients_kernel/Architecture_ABC.h"
#include "tools/SchemaWriter_ABC.h"
#include <urban/PhysicalAttribute.h>
#include <urban/InfrastructureAttribute.h>
#include <urban/ResourceNetworkAttribute.h>
#include <urban/TerrainObject_ABC.h>
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
UrbanModel::UrbanModel( Controllers& controllers, const StaticModel& staticModel, const tools::Resolver< Object_ABC >& objects )
    : controllers_        ( controllers )
    , objectTypes_        ( staticModel.objectTypes_ )
    , objects_            ( objects )
    , urbanStateVersion_  ( ::defaultUrbanStateVersion )
    , urbanDisplayOptions_( new gui::UrbanDisplayOptions( controllers, staticModel.accommodationTypes_ ) )
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
        UrbanSendingCreationVisitor( UrbanModel& model, const ObjectTypes& objectTypes, std::string& loadingErrors )
            : model_        ( model )
            , objectTypes_  ( objectTypes )
            , loadingErrors_( loadingErrors )
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
                    loadingErrors_ += "Urban Bloc " + urbanObject.GetName() + ": Unknown Material " + pPhysical->GetArchitecture()->GetMaterial() + "\n";
                    return;
                }
                if( !objectTypes_.StringResolver< RoofShapeType >::Find( pPhysical->GetArchitecture()->GetRoofShape() ) )
                {
                    loadingErrors_ += "Urban Bloc " + urbanObject.GetName() + ": Unknown Roof shape " + pPhysical->GetArchitecture()->GetRoofShape() + "\n";
                    return;
                }
            }
            model_.SendCreation( urbanObject );
        }
    private:
        UrbanModel& model_;
        const ObjectTypes& objectTypes_;
        std::string& loadingErrors_;
    };
}

// -----------------------------------------------------------------------------
// Name: UrbanModel::Load
// Created: SBO 2010-06-10
// -----------------------------------------------------------------------------
void UrbanModel::Load( const std::string& directoryPath, urban::WorldParameters& world, std::string& loadingErrors )
{
    Purge();
    urban::Model::Load( directoryPath, world );
    UrbanSendingCreationVisitor visitor( *this, objectTypes_, loadingErrors );
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
    // TODO faire �a proprement et de fa�on g�n�rique avec la factory d'objets quand elle sera impl�ment�e (pour l'instant, c'est une par Team)
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
    gui::TerrainObjectProxy* pTerrainObject = new gui::TerrainObjectProxy( controllers_, urbanObject.GetName(), urbanObject.GetId(), objectTypes_.StringResolver< ObjectType >::Get( "urban block" ), *urbanDisplayOptions_ );
    PropertiesDictionary& dictionary = pTerrainObject->Get< PropertiesDictionary >();
    pTerrainObject->Attach< StructuralStateAttribute_ABC >( *new StructuralStateAttribute( 100, dictionary ) );
    pTerrainObject->Attach< kernel::UrbanColor_ABC >( *new UrbanColor( urbanObject ) );
    const kernel::UrbanColor_ABC& color = pTerrainObject->Get< kernel::UrbanColor_ABC >();
    pTerrainObject->Attach< kernel::UrbanPositions_ABC >( *new UrbanPositions( urbanObject, color ) );
    pTerrainObject->Attach< kernel::Usages_ABC >( *new Usages( urbanObject, std::auto_ptr< Usages_ABC >( new gui::Usages( dictionary ) ) ) );
    pTerrainObject->Attach< kernel::Architecture_ABC >( *new Architecture( urbanObject, std::auto_ptr< Architecture_ABC >( new gui::Architecture( dictionary ) ) ) );
    const urban::ResourceNetworkAttribute* resource = urbanObject.Retrieve< urban::ResourceNetworkAttribute >();
    pTerrainObject->Attach< ResourceNetwork_ABC >( *new ResourceNetworkAttribute( controllers_, resource, pTerrainObject->Get< kernel::UrbanPositions_ABC >().Barycenter(), *this, objects_, objectTypes_ ) );
    if( const urban::InfrastructureAttribute* infra = urbanObject.Retrieve< urban::InfrastructureAttribute >() )
        if( const InfrastructureType* infraType = objectTypes_.StringResolver< InfrastructureType >::Find( infra->GetType() ) )
        {
            pTerrainObject->Attach< Infrastructure_ABC >( *new InfrastructureAttribute( controllers_, *pTerrainObject, *infraType, dictionary ) );
            if( infraType->FindCapacity( "medical" ) )
                pTerrainObject->Attach< MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( objectTypes_, dictionary ) );
        }
    pTerrainObject->Polish();
    if( !Resolver< gui::TerrainObjectProxy >::Find( urbanObject.GetId() ) )
        Resolver< gui::TerrainObjectProxy >::Register( urbanObject.GetId(), *pTerrainObject );
}
