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
#include "moc_ResourceNetworkDialog.cpp"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/ResourceNetworkType.h"
#include "preparation/ResourceNetworkAttribute.h"

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog constructor
// Created: JSR 2010-09-08
// -----------------------------------------------------------------------------
ResourceNetworkDialog::ResourceNetworkDialog( QMainWindow* parent, kernel::Controllers& controllers, tools::StringResolver< kernel::ResourceNetworkType >& resources )
    : gui::ResourceLinksDialog_ABC( parent, controllers )
    , resources_   ( resources )
    , linkToChange_( 0 )
{
    stockBox_->show();
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
    static_cast< ResourceNetworkAttribute* >( const_cast< kernel::ResourceNetwork_ABC* >( selected_ ) )->Update( resourceNodes_ );
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::NotifyContextMenu
// Created: JSR 2011-02-24
// -----------------------------------------------------------------------------
void ResourceNetworkDialog::NotifyContextMenu( const kernel::Object_ABC& object, kernel::ContextMenu& menu )
{
    if( resources_.Count() == 0 )
        return;
    const kernel::ResourceNetwork_ABC* node = object.Retrieve< kernel::ResourceNetwork_ABC >();
    if( !node || !selected_ || selected_ == node )
        return;
    linkToChange_ = &object;
    QPopupMenu* subMenu = menu.SubMenu( "Resource", tr( "Resource networks" ) );
    tools::Iterator< const kernel::ResourceNetworkType& > it = resources_.CreateIterator();
    int resourceId = 0;
    while( it.HasMoreElements() )
    {
        const kernel::ResourceNetworkType& resource = it.NextElement();
        subMenu->insertItem( resource.GetName().c_str() , this , SLOT( OnChangeLink( int ) ), 0, resourceId++ );
    }
}

// -----------------------------------------------------------------------------
// Name: ResourceNetworkDialog::OnChangeLink
// Created: JSR 2011-02-24
// -----------------------------------------------------------------------------
void ResourceNetworkDialog::OnChangeLink( int resourceId )
{
    typedef kernel::ResourceNetwork_ABC::ResourceNode ResourceNode;
    typedef kernel::ResourceNetwork_ABC::ResourceLink ResourceLink;
    typedef kernel::ResourceNetwork_ABC::T_ResourceNodes T_ResourceNodes;
    tools::Iterator< const kernel::ResourceNetworkType& > it = resources_.CreateIterator();
    int index = 0;
    while( it.HasMoreElements() )
    {
        const kernel::ResourceNetworkType& resource = it.NextElement();
        if( index++ == resourceId )
        {
            bool destUrban = linkToChange_->GetType().IsUrban();
            unsigned long destId = linkToChange_->GetId();
            ResourceNode& sourceNode = selected_->FindOrCreateResourceNode( resource.GetName() );
            const_cast< kernel::ResourceNetwork_ABC& >( linkToChange_->Get< kernel::ResourceNetwork_ABC >() ).FindOrCreateResourceNode( resource.GetName() ); // necessary to create a node for this ressource
            std::vector< ResourceLink >::iterator itLink;
            for( itLink = sourceNode.links_.begin(); itLink != sourceNode.links_.end(); ++itLink )
                if( itLink->urban_ == destUrban && itLink->id_ == destId )
                    break;
            if( itLink == sourceNode.links_.end() )
                sourceNode.links_.push_back( ResourceLink( destUrban, destId ) );
            else
                sourceNode.links_.erase( itLink );
            controllers_.controller_.Update( *selected_ );
            break;
        }
    }
    linkToChange_ = 0;
}
