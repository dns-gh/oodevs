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
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/UrbanObject.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/AccommodationType.h"
#include "clients_kernel/AccommodationTypes.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/InhabitantType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Usages_ABC.h"
#include "preparation/ResourceNetworkAttribute.h"
#include "preparation/StaticModel.h"
#include "ENT/ENT_Enums.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog constructor
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
ResourceNetworkDialog::ResourceNetworkDialog( QMainWindow* parent, const kernel::Model_ABC& model, Controllers& controllers, const ::StaticModel& staticModel, const EntityResolver_ABC& resolver )
    : gui::ResourceLinksDialog_ABC( parent, model, controllers, staticModel.objectTypes_ )
    , staticModel_( staticModel )
    , resolver_   ( resolver )
{
    // NOTHING
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
void ResourceNetworkDialog::DoValidate( kernel::Entity_ABC* element /*= 0*/ )
{
    if( selected_.size() != 1 )
        return;
    if( element )
    {
        auto& network = element->Get< gui::ResourceNetwork_ABC >();
        static_cast< ResourceNetworkAttribute& >( network ).Update( network.GetResourceNodes() );
    }
    else
        static_cast< ResourceNetworkAttribute& >( selected_.front()->Get< gui::ResourceNetwork_ABC >() ).Update( resourceNodes_ );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::DoGenerateProduction
// Created: JSR 2011-09-20
// -----------------------------------------------------------------------------
bool ResourceNetworkDialog::DoGenerateProduction()
{
    if( id_ == 0 )
        return false;
    std::string resource = dotationList_->currentItem()->text().toStdString();
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
    const Entity_ABC* FindObject( unsigned int id, const EntityResolver_ABC& resolver )
    {
        const Entity_ABC* object = resolver.FindUrbanObject( id );
        if( !object )
            object = resolver.FindObject( id );
        return object;
    }

    const gui::ResourceNode* FindNode( const Entity_ABC& object, const std::string& resource )
    {
        if( auto blockNodes = object.Retrieve< gui::ResourceNetwork_ABC >() )
            return blockNodes->FindResourceNode( resource );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::IsNetworkValid
// Created: JSR 2011-09-20
// -----------------------------------------------------------------------------
bool ResourceNetworkDialog::IsNetworkValid( const gui::ResourceNode& node, unsigned int id, const std::string& resource, std::set< unsigned int >& array )
{
    // Depth-first search algorithm to find back edges (cycles in graph)
    for( auto link = node.links_.begin(); link != node.links_.end(); ++link )
        if( const Entity_ABC* object = FindObject( link->id_, resolver_ ) )
        {
            if( array.insert( object->GetId() ).second == false )
            {
                // node already visited
                QMessageBox::warning( this, tools::translate( "ResourceNetworkDialog", "Warning" ),
                    tools::translate( "ResourceNetworkDialog", "A cycle has been detected in the resource network for the block %1.\n The network automatic generation cannot be computed." ).arg( object->GetId() ), QMessageBox::Ok, QMessageBox::NoButton );
                return false;
            }
            // Apply to children
            if( auto blockNode = FindNode( *object, resource ) )
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
    const Entity_ABC* object = FindObject( id, resolver_ );
    if( !object )
        return 0;
    int consumption = 0;
    if( inhabitantConsumption > 0)
        if( auto block = dynamic_cast< const gui::UrbanObject* >( object ) )
        {
            double tmp = inhabitantConsumption * block->GetNominalCapacity();
            if( tmp >= std::numeric_limits< int >::max() )
                return std::numeric_limits< int >::max();
            consumption = static_cast< int >( tmp );
        }
    if( auto node = FindNode( *object, resource ) )
    {
        consumption += node->consumption_;
        unsigned int childMaxConsumption = 0;
        for( auto link = node->links_.begin(); link != node->links_.end(); ++link )
            childMaxConsumption = std::max( ComputeConsumption( link->id_, resource, inhabitantConsumption ), childMaxConsumption );
        consumption = std::max( static_cast< int >( consumption + childMaxConsumption * node->links_.size() - node->production_ ), 0 );
    }
    return consumption;
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::NotifyModeChanged
// Created: ABR 2012-06-08
// -----------------------------------------------------------------------------
void ResourceNetworkDialog::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    ResourceLinksDialog_ABC::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
    if( newMode == eModes_Prepare )
    {
        generateProduction_->setVisible( true );
        stockBox_->setVisible( true );
    }
    else if( newMode == eModes_Terrain )
    {
        generateProduction_->setVisible( false );
        stockBox_->setVisible( false );
    }
}
