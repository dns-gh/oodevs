// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ResourceNetworkDialog.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Usages_ABC.h"
#include "preparation/ResourceNetworkAttribute.h"
#include "preparation/StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog constructor
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
ResourceNetworkDialog::ResourceNetworkDialog( QMainWindow* parent, Controllers& controllers, const StaticModel& staticModel, const EntityResolver_ABC& resolver )
    : gui::ResourceLinksDialog_ABC( parent, controllers, staticModel.objectTypes_ )
    , staticModel_( staticModel )
    , resolver_   ( resolver )
{
    setObjectName( "ressources" );
    stockBox_->show();
    generateProduction_->show();
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog destructor
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
ResourceNetworkDialog::~ResourceNetworkDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::DoValidate
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
void ResourceNetworkDialog::DoValidate()
{
    static_cast< ResourceNetworkAttribute* >( const_cast< ResourceNetwork_ABC* >( selected_ ) )->Update( resourceNodes_ );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::DoGenerateProduction
// Created: JSR 2011-09-20
// -----------------------------------------------------------------------------
bool ResourceNetworkDialog::DoGenerateProduction()
{
    std::string resource = dotationList_->selectedItem()->text().toStdString();
    std::set< unsigned int > array;
    array.insert( id_ );
    if( !IsNetworkValid( resourceNodes_[ resource ], id_, resource, array ) )
        return false;
    resourceNodes_[ resource ].production_ = 0;
    DoValidate();
    const tools::Resolver< InhabitantType >& inhabitants = staticModel_.types_;
    tools::Iterator< const InhabitantType& > iterator = inhabitants.CreateIterator();
    double inhabitantConsumption = 0;
    while( iterator.HasMoreElements() )
        inhabitantConsumption = std::max( inhabitantConsumption, iterator.NextElement().GetConsumption( resource ) );
    resourceNodes_[ resource ].production_ = ComputeConsumption( id_, resource, inhabitantConsumption );
    return true;
}

namespace
{
    const Object_ABC* FindObject( unsigned int id, const EntityResolver_ABC& resolver )
    {
        const Object_ABC* object = resolver.FindUrbanObject( id );
        if( !object )
            object = resolver.FindObject( id );
        return object;
    }

    const ResourceNetwork_ABC::ResourceNode* FindNode( const Object_ABC& object, const std::string& resource )
    {
        if( const ResourceNetwork_ABC* blockNodes = object.Retrieve< ResourceNetwork_ABC >() )
            return blockNodes->FindResourceNode( resource );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::IsNetworkValid
// Created: JSR 2011-09-20
// -----------------------------------------------------------------------------
bool ResourceNetworkDialog::IsNetworkValid( const ResourceNetwork_ABC::ResourceNode& node, unsigned int id, const std::string& resource, std::set< unsigned int >& array )
{
    // Depth-first search algorithm to find back edges (cycles in graph)
    for( ResourceNetwork_ABC::CIT_ResourceLinks link = node.links_.begin(); link != node.links_.end(); ++link )
        if( const Object_ABC* object = FindObject( link->id_, resolver_ ) )
        {
            if( array.insert( object->GetId() ).second == false )
            {
                // node already visited
                QMessageBox::warning( this, tr( "Warning" ), tr( "A cycle has been detected in the resource network for the block %1.\n The network automatic generation cannot be computed." ).arg( object->GetId() ), QMessageBox::Ok, QMessageBox::NoButton );
                return false;
            }
            // Apply to children
            if( const ResourceNetwork_ABC::ResourceNode* blockNode = FindNode( *object, resource ) )
                if( !IsNetworkValid( *blockNode, object->GetId(), resource, array ) )
                    return false;
        }
    std::set< unsigned int >::iterator it = array.find( id );
    assert( it != array.end() );
    array.erase( it );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::ComputeConsumption
// Created: JSR 2011-09-22
// -----------------------------------------------------------------------------
unsigned int ResourceNetworkDialog::ComputeConsumption( unsigned int id, const std::string& resource, double inhabitantConsumption ) const
{
    const Object_ABC* object = FindObject( id, resolver_ );
    if( !object )
        return 0;
    int consumption = 0;
    if( inhabitantConsumption > 0)
        if( const gui::TerrainObjectProxy* block = dynamic_cast< const gui::TerrainObjectProxy* >( object ) )
        {
            double tmp = inhabitantConsumption * GetNominalOccupation( *block );
            if( tmp >= std::numeric_limits< int >::max() )
                return std::numeric_limits< int >::max();
            consumption = static_cast< int >( tmp );
        }
    if( const ResourceNetwork_ABC::ResourceNode* node = FindNode( *object, resource ) )
    {
        consumption += node->consumption_;
        unsigned int childMaxConsumption = 0;
        for( ResourceNetwork_ABC::CIT_ResourceLinks link = node->links_.begin(); link != node->links_.end(); ++link )
            childMaxConsumption = std::max( ComputeConsumption( link->id_, resource, inhabitantConsumption ), childMaxConsumption );
        consumption = std::max( static_cast< int >( consumption + childMaxConsumption * node->links_.size() - node->production_ ), 0 );
    }
    return consumption;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::GetNominalOccupation
// Created: JSR 2011-09-22
// -----------------------------------------------------------------------------
double ResourceNetworkDialog::GetNominalOccupation( const gui::TerrainObjectProxy& block ) const
{
    double occupation = 0;
    double livingSpace = block.GetLivingSpace();
    tools::Iterator< const AccommodationType& > it = staticModel_.accommodationTypes_.CreateIterator();
    double proportionSum = 0;
    const AccommodationType* defaultAcc = 0;
    while( it.HasMoreElements() )
    {
        const AccommodationType& acc = it.NextElement();
        if( acc.GetRole() == "default" )
        {
            defaultAcc = &acc;
            continue;
        }
        const Usages_ABC& usages = block.Get< Usages_ABC >();
        double proportion = usages.Find( acc.GetRole() ) * 0.01;
        proportionSum += proportion;
        occupation += livingSpace * proportion * acc.GetNominalCapacity();
    }
    if( defaultAcc && proportionSum < 1 )
        occupation += livingSpace * ( 1 - proportionSum ) * defaultAcc->GetNominalCapacity();        
    return occupation;
}
