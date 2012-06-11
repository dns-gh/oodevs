// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "UrbanMenuManager.h"
#include "moc_UrbanMenuManager.cpp"
#include "UrbanHierarchies.h"
#include "UrbanModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include <QtCore/QSignalMapper>

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager constructor
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
UrbanMenuManager::UrbanMenuManager( kernel::Controllers& controllers, UrbanModel& model, const kernel::StaticModel& staticModel )
    : controllers_( controllers )
    , model_      ( model )
    , staticModel_( staticModel )
    , element_    ( 0 )
    , pMapper_    ( new QSignalMapper( this ) )
{
    connect( pMapper_, SIGNAL( mapped( const QString & ) ), this, SLOT( OnCreateInfrastructure( const QString& ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager destructor
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
UrbanMenuManager::~UrbanMenuManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::Register
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::Register()
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::Unregister
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::Unregister()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::NotifySelectionChanged
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::NotifySelectionChanged( const T_Elements& elements )
{
    selected_ = elements;
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::NotifyContextMenu
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::NotifyContextMenu( const kernel::UrbanObject_ABC& object, kernel::ContextMenu& menu )
{
    element_ = selected_.empty() ? const_cast< kernel::UrbanObject_ABC* >( &object ) : 0;
    if( GetCurrentMode() != ePreparationMode_Terrain)
        return;
    const UrbanHierarchies& hierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
    if( hierarchies.GetLevel() == eUrbanLevelCity )
        menu.InsertItem( "Urban", tr( "Create district" ), this, SLOT( OnCreateDistrict() ) );
    CreateInfrastructuresMenu( menu );
    menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDelete() ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::CreateInfrastructuresMenu
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanMenuManager::CreateInfrastructuresMenu( kernel::ContextMenu& menu ) const
{
    kernel::ContextMenu* inframenu = menu.SubMenu( "Urban", tr( "Infrastructures" ) );
    bool hasInfra = element_ ? ( element_->Get< kernel::Infrastructure_ABC >().GetType() != 0 ) : 0;
    for( CIT_Elements it = selected_.begin(); it != selected_.end() && hasInfra == false; ++it )
        hasInfra = ( *it )->Get< kernel::Infrastructure_ABC >().GetType() != 0;
    bool oneElement = element_ != 0 || selected_.size() == 1;
    QAction* action = inframenu->InsertItem( "DeleteInfrastructures", oneElement ? tr( "Delete" ) : tr( "Delete on selection" ), this, SLOT( OnDeleteInfrastructures() ) );
    if( action )
        action->setEnabled( hasInfra );
    tools::Iterator< const kernel::InfrastructureType& > it = staticModel_.objectTypes_.tools::StringResolver< kernel::InfrastructureType >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::InfrastructureType& type = it.NextElement();
        QString name( type.GetName().c_str() );
        QAction* action = inframenu->InsertItem( "Infrastructures", name, pMapper_, SLOT( map() ) );
        pMapper_->setMapping( action, name );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnCreateDistrict
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnCreateDistrict()
{
    for( CIT_Elements it = selected_.begin(); it != selected_.end(); ++it )
    {
        const kernel::UrbanObject_ABC& object = **it;
        const UrbanHierarchies& hierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
        if( hierarchies.GetLevel() == eUrbanLevelCity )
            model_.CreateCityOrDistrict( const_cast< kernel::UrbanObject_ABC* >( &object ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnDelete
// Created: JSR 2012-06-08
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnDelete()
{
    model_.DeleteBlocks( selected_ );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnDeleteInfrastructures
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnDeleteInfrastructures()
{
    if( element_ )
        element_->Get< kernel::Infrastructure_ABC >().SetType( 0 );
    else
        for( IT_Elements it = selected_.begin(); it != selected_.end(); ++it )
        {
            kernel::UrbanObject_ABC& object = const_cast< kernel::UrbanObject_ABC& >( **it );
            object.Get< kernel::Infrastructure_ABC >().SetType( 0 );
        }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnCreateInfrastructure
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnCreateInfrastructure( const QString& name )
{
    kernel::InfrastructureType* type = staticModel_.objectTypes_.tools::StringResolver< kernel::InfrastructureType >::Find( name.ascii() );
    if( !type )
        return;
    if( element_ )
        element_->Get< kernel::Infrastructure_ABC >().SetType( type );
    else
        for( IT_Elements it = selected_.begin(); it != selected_.end(); ++it )
        {
            kernel::UrbanObject_ABC& object = const_cast< kernel::UrbanObject_ABC& >( **it );
            object.Get< kernel::Infrastructure_ABC >().SetType( type );
        }
}
