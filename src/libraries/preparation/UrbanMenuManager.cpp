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
#include "clients_gui/Infrastructure_ABC.h"
#include "clients_kernel/Architecture_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/PhysicalAttribute_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/UrbanTemplateType.h"
#include "clients_kernel/Usages_ABC.h"
#include <QtCore/QSignalMapper>

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager constructor
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
UrbanMenuManager::UrbanMenuManager( kernel::Controllers& controllers, UrbanModel& model, const kernel::StaticModel& staticModel )
    : controllers_    ( controllers )
    , model_          ( model )
    , staticModel_    ( staticModel )
    , element_        ( 0 )
    , pInfraMapper_   ( new QSignalMapper( this ) )
    , pTemplateMapper_( new QSignalMapper( this ) )
{
    connect( pInfraMapper_,    SIGNAL( mapped( const QString & ) ), this, SLOT( OnCreateInfrastructure( const QString& ) ) );
    connect( pTemplateMapper_, SIGNAL( mapped( const QString & ) ), this, SLOT( OnApplyTemplate( const QString& ) ) );
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
    element_ = 0;
    if( selected_.empty() || ( selected_.size() == 1 && &object == selected_.front() ) )
        element_ = const_cast< kernel::UrbanObject_ABC* >( &object );
    if( GetCurrentMode() != eModes_Terrain)
        return;
    const UrbanHierarchies& hierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
    if( hierarchies.GetLevel() == eUrbanLevelCity )
        menu.InsertItem( "Urban", tr( "Create district" ), this, SLOT( OnCreateDistrict() ) );
    CreateInfrastructuresMenu( menu );
    CreateTemplatesMenu( menu );
    if( element_ && hierarchies.GetLevel() != eUrbanLevelBlock )
        menu.InsertItem( "Urban", tr( "Apply properties to children" ), this, SLOT( OnApplyToChildren() ) );
    menu.InsertItem( "Command", tr( "Delete" ), this, SLOT( OnDelete() ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::CreateInfrastructuresMenu
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanMenuManager::CreateInfrastructuresMenu( kernel::ContextMenu& menu ) const
{
    kernel::ContextMenu* inframenu = menu.SubMenu( "Urban", tr( "Infrastructures" ) );
    const kernel::InfrastructureType* infraType = 0;
    bool hasInfra = false;
    if( element_ )
    {
        if( element_->Retrieve< gui::Infrastructure_ABC >() )
        {
            infraType = element_->Get< gui::Infrastructure_ABC >().GetType();
            hasInfra = infraType != 0;
        }
    }
    else
    {
        bool noInfrastructureDetected = false;
        for( auto it = selected_.begin(); it != selected_.end(); ++it )
        {
            if( ( *it )->Retrieve< gui::Infrastructure_ABC >() )
            {
                const kernel::InfrastructureType* type = ( *it )->Get< gui::Infrastructure_ABC >().GetType();
                if( type )
                {
                    hasInfra = true;
                    if( noInfrastructureDetected )
                        break;
                    if( infraType == 0 )
                        infraType = type;
                    else if( infraType != type )
                    {
                        infraType = 0;
                        break;
                    }
                }
                else
                {
                    noInfrastructureDetected = true;
                    infraType = 0;
                }
            }
        }
    }
    const kernel::UrbanObject_ABC* oneElement = element_ != 0 ? element_ : ( selected_.size() == 1 ? selected_.front() : 0 );
    QAction* action = inframenu->InsertItem( "DeleteInfrastructures", oneElement ? tr( "Delete" ) : tr( "Delete on selection" ), this, SLOT( OnDeleteInfrastructures() ) );
    if( !oneElement || ( static_cast< const UrbanHierarchies& >( oneElement->Get< kernel::Hierarchies >() ) ).GetLevel() == eUrbanLevelBlock )
    {
        if( action )
           action->setEnabled( hasInfra );
    }
    tools::Iterator< const kernel::InfrastructureType& > it = staticModel_.objectTypes_.tools::StringResolver< kernel::InfrastructureType >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::InfrastructureType& type = it.NextElement();
        QString name( type.GetName().c_str() );
        QAction* action = inframenu->InsertItem( "Infrastructures", name, pInfraMapper_, SLOT( map() ) );
        if( infraType && infraType == &type )
        {
            action->setCheckable( true );
            action->setChecked( true );
        }
        pInfraMapper_->setMapping( action, name );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::CreateTemplatesMenu
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanMenuManager::CreateTemplatesMenu( kernel::ContextMenu& menu ) const
{
    kernel::ContextMenu* templateMenu = menu.SubMenu( "Urban", tr( "Templates" ) );
    tools::Iterator< const kernel::UrbanTemplateType& > it = staticModel_.objectTypes_.tools::StringResolver< kernel::UrbanTemplateType >::CreateIterator();
    bool templateFound = false;
    while( it.HasMoreElements() )
    {
        const kernel::UrbanTemplateType& type = it.NextElement();
        QString name( type.GetName().c_str() );
        QAction* action = templateMenu->InsertItem( "Templates", name, pTemplateMapper_, SLOT( map() ) );
        if( !templateFound )
        {
            if( element_ && type.Matches( *element_ ))
            {
                templateFound = true;
                action->setCheckable( true );
                action->setChecked( true );
            }
            else
            {
                bool sameTemplate = true;
                for( auto it = selected_.begin(); it != selected_.end(); ++it )
                {
                    if( !type.Matches( **it ) )
                    {
                        sameTemplate = false;
                        break;
                    }
                }
                if( sameTemplate )
                {
                    templateFound = true;
                    action->setCheckable( true );
                    action->setChecked( true );
                }
            }
        }
        pTemplateMapper_->setMapping( action, name );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnCreateDistrict
// Created: JSR 2012-06-07
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnCreateDistrict()
{
    for( auto it = selected_.begin(); it != selected_.end(); ++it )
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
    if( selected_.empty() && element_ )
        selected_.push_back( element_ );
    if( !selected_.empty() )
    {
        model_.DeleteBlocks( selected_ );
        selected_.clear();
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnDeleteInfrastructures
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnDeleteInfrastructures()
{
    if( element_ )
        DoApplyInfrastructure( *element_, 0 );
    else
        for( IT_Elements it = selected_.begin(); it != selected_.end(); ++it )
        {
            kernel::UrbanObject_ABC& object = const_cast< kernel::UrbanObject_ABC& >( **it );
            DoApplyInfrastructure( object, 0 );
        }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnCreateInfrastructure
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnCreateInfrastructure( const QString& name )
{
    kernel::InfrastructureType* type = staticModel_.objectTypes_.tools::StringResolver< kernel::InfrastructureType >::Find( name.toStdString() );
    if( !type )
        return;
    if( element_ )
        DoApplyInfrastructure( *element_, type );
    else
        for( IT_Elements it = selected_.begin(); it != selected_.end(); ++it )
        {
            kernel::UrbanObject_ABC& object = const_cast< kernel::UrbanObject_ABC& >( **it );
            DoApplyInfrastructure( object, type );
        }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::DoApplyInfrastructure
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void UrbanMenuManager::DoApplyInfrastructure( kernel::Entity_ABC& object, kernel::InfrastructureType* type ) const
{
    auto* infra = object.Retrieve< gui::Infrastructure_ABC >();
    if( infra )
        infra->SetType( type );
    tools::Iterator< const kernel::Entity_ABC& > it = object.Get< kernel::Hierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
        DoApplyInfrastructure( const_cast< kernel::Entity_ABC& >( it.NextElement() ), type );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnApplyTemplate
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnApplyTemplate( const QString& name )
{
    const kernel::UrbanObject_ABC::UrbanTemplateTypePtr type = staticModel_.objectTypes_.tools::StringResolver< kernel::UrbanTemplateType >::Find( name.toStdString() );
    if( !type )
        return;
    if( element_ )
        element_->ApplyTemplate( type );
    else
        for( IT_Elements it = selected_.begin(); it != selected_.end(); ++it )
        {
            kernel::UrbanObject_ABC& object = const_cast< kernel::UrbanObject_ABC& >( **it );
            object.ApplyTemplate( type );
        }
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::OnApplyToChildren
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void UrbanMenuManager::OnApplyToChildren()
{
    if( !element_ )
        return;
    const UrbanHierarchies& hierarchies = static_cast< const UrbanHierarchies& >( element_->Get< kernel::Hierarchies >() );
    if( hierarchies.GetLevel() == eUrbanLevelBlock )
        return;
    tools::Iterator< const kernel::Entity_ABC& > it = hierarchies.CreateSubordinateIterator();
    while( it.HasMoreElements() )
        DoApplyToChildren( const_cast< kernel::Entity_ABC& >( it.NextElement() ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanMenuManager::DoApplyToChildren
// Created: JSR 2012-06-12
// -----------------------------------------------------------------------------
void UrbanMenuManager::DoApplyToChildren( kernel::Entity_ABC& object ) const
{
    const kernel::PhysicalAttribute_ABC& physicalSrc = element_->Get< kernel::PhysicalAttribute_ABC >();
    kernel::PhysicalAttribute_ABC& physicalDst = object.Get< kernel::PhysicalAttribute_ABC >();

    // architecture
    const kernel::Architecture_ABC& architectureSrc = physicalSrc.GetArchitecture();
    kernel::Architecture_ABC& architectureDst = physicalDst.GetArchitecture();
    architectureDst.SetMaterial( const_cast< kernel::MaterialCompositionType& >( architectureSrc.GetMaterial() ) );
    architectureDst.SetRoofShape( const_cast< kernel::RoofShapeType& >( architectureSrc.GetRoofShape() ) );
    architectureDst.SetFloorNumber( architectureSrc.GetFloorNumber() );
    architectureDst.SetParkingFloors( architectureSrc.GetParkingFloors() );
    architectureDst.SetOccupation( architectureSrc.GetOccupation() );
    architectureDst.SetHeight( architectureSrc.GetHeight() );
    architectureDst.SetTrafficability( architectureSrc.GetTrafficability() );

    // usages
    const kernel::T_Usages& usagesSrc = physicalSrc.GetUsages().GetUsages();
    kernel::Usages_ABC& usagesDst = physicalDst.GetUsages();
    usagesDst.ClearAll();
    for( auto it = usagesSrc.begin(); it != usagesSrc.end(); ++it )
        usagesDst.Add( it->first, it->second );

    // Color
    const kernel::UrbanColor_ABC& colorSrc = element_->Get< kernel::UrbanColor_ABC >();
    kernel::UrbanColor_ABC& colorDst = object.Get< kernel::UrbanColor_ABC >();
    colorDst.SetColor( colorSrc.Red(), colorSrc.Green(), colorSrc.Blue(), static_cast< int >( 255.f * colorSrc.Alpha() + 0.5f ) );

    const UrbanHierarchies& hierarchies = static_cast< const UrbanHierarchies& >( object.Get< kernel::Hierarchies >() );
    if( hierarchies.GetLevel() == eUrbanLevelBlock )
        return;
    tools::Iterator< const kernel::Entity_ABC& > it = hierarchies.CreateSubordinateIterator();
    while( it.HasMoreElements() )
        DoApplyToChildren( const_cast< kernel::Entity_ABC& >( it.NextElement() ) );
}
