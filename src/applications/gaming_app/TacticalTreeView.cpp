// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TacticalTreeView.h"
#include "moc_TacticalTreeView.cpp"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "actions/ActionsModel.h"
#include "actions/Automat.h"
#include "actions/Army.h"
#include "actions/Formation.h"
#include "actions/UnitMagicAction.h"
#include "clients_gui/ChangeSuperiorDialog.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Profile_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "gaming/Attributes.h"
#include "gaming/StaticModel.h"
#include "clients_gui/Decisions_ABC.h"
#include "tools/GeneralConfig.h"

// -----------------------------------------------------------------------------
// Name: TacticalTreeView constructor
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
TacticalTreeView::TacticalTreeView( const QString& objectName,
                                    kernel::Controllers& controllers,
                                    const kernel::Profile_ABC& profile,
                                    gui::ModelObserver_ABC& modelObserver,
                                    const gui::EntitySymbols& symbols,
                                    const StaticModel& staticModel,
                                    const kernel::Time_ABC& simulation,
                                    actions::ActionsModel& actionsModel,
                                    gui::ChangeSuperiorDialog& changeSuperiorDialog,
                                    QWidget* parent /*= 0*/ )
    : gui::TacticalTreeView( objectName, controllers, profile, modelObserver, symbols, changeSuperiorDialog, parent )
    , static_              ( staticModel )
    , simulation_          ( simulation )
    , actionsModel_        ( actionsModel )
    , displayMode_         ( eObservableUnits )
    , changeSuperiorDialog_( changeSuperiorDialog )
    , currentEntity_       ( 0 )
    , icon_user_           ( tools::GeneralConfig::BuildResourceChildFile( "images/gaming/icon_user.png" ).ToUTF8().c_str() )
{
    controllers_.Update( *this );
    //setEditTriggers( 0 );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView destructor
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
TacticalTreeView::~TacticalTreeView()
{
    controllers_.Unregister( *this );
}

namespace
{
    void CreateSubMenu( TacticalTreeView* view, kernel::ContextMenu& menu, const QString& text, const char* slot )
    {
        kernel::ContextMenu* subMenu = menu.SubMenu( "Formation", text );
        for( int levelIt = static_cast< int >( eNatureLevel_xxxxx ); levelIt > 0; --levelIt )
            subMenu->insertItem( ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( levelIt ), ENT_Tr::eToTr ).c_str(), view, slot, 0, levelIt );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyContextMenu
// Created: SLI 2013-07-16
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyContextMenu( const kernel::Team_ABC& team, kernel::ContextMenu& menu )
{
    if( !isVisible() || !profile_.IsVisible( team ) )
        return;
    if( team.GetId() == 0 || !AddCommonMenu( team, menu ) ) // id = 0 -> no side team
        return;
    if( profile_.CanDoMagic( team ) )
    {
        currentEntity_ = &team;
        CreateSubMenu( this, menu, tr( "Create formation" ), SLOT( OnCreateFormation( int ) ) );
        CreateSubMenu( this, menu, tr( "Create logistic base" ), SLOT( OnCreateLogisticBase( int ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::NotifyUpdated
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::NotifyUpdated( const gui::Decisions_ABC& decisions )
{
    if( decisions.GetAgent().GetTypeName() == kernel::Automat_ABC::typeName_ )
        proxyModel_->invalidate();
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::drawRow
// Created: JSR 2012-10-10
// -----------------------------------------------------------------------------
void TacticalTreeView::drawRow( QPainter* painter, const QStyleOptionViewItem& options, const QModelIndex &index ) const
{
    if( !index.isValid() )
        return;
    QStandardItem* item = dataModel_.GetItemFromIndex( dataModel_.GetMainModelIndex( index ) );
    if( item && item->data( gui::Roles::MimeTypeRole).isValid() &&
        item->data( gui::Roles::MimeTypeRole).toString() == typeid( kernel::Agent_ABC ).name() )
    {
        if( const kernel::Agent_ABC* agent = dataModel_.GetDataFromItem< const kernel::Agent_ABC >( *item ) )
        {
            if( const Attributes* attributes = agent->Retrieve< Attributes >() )
            {
                if( attributes->bDead_ || attributes->nOpState_ == eOperationalStatus_DetruitTotalement )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/TotallyDestroyed", QString( "" ) ).To< QString >() ) );
                else if( attributes->nOpState_ == eOperationalStatus_DetruitTactiquement )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/TacticallyDestroyed", QString( "" ) ).To< QString >() ) );
                if( attributes->bNeutralized_ )
                    painter->fillRect( options.rect, QColor( controllers_.options_.GetOption( "Color/Neutralized", QString( "" ) ).To< QString >() ) );
            }
        }
    }
    gui::TacticalTreeView::drawRow( painter, options, index );
}

namespace
{
    bool CanBeOrdered( const kernel::Entity_ABC& entity, const kernel::Profile_ABC& profile )
    {
        if( ( dynamic_cast< const kernel::Agent_ABC* >( &entity ) || dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
         && profile.CanBeOrdered( entity ) )
            return true;
        if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            auto it = hierarchies->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& sub = it.NextElement();
                return CanBeOrdered( sub, profile );
            }
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::GetEntityPixmap
// Created: JSR 2013-01-17
// -----------------------------------------------------------------------------
std::vector< const QPixmap* > TacticalTreeView::GetEntityPixmap( const kernel::Entity_ABC& entity )
{
    std::vector< const QPixmap* > ret = gui::TacticalTreeView::GetEntityPixmap( entity );
    if( dynamic_cast< const kernel::Agent_ABC* >( &entity ) || dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        if( profile_.CanBeOrdered( entity ) )
            ret.push_back( &icon_user_ );
    if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) || dynamic_cast< const kernel::Team_ABC* >( &entity ) )
    {
        if( QStandardItem* item = dataModel_.FindDataItem( entity ) )
        {
            if( !isExpanded( proxyModel_->mapFromSource( dataModel_.indexFromItem( item ) ) ) && CanBeOrdered( entity, profile_ ) )
                ret.push_back( &icon_user_ );
        }
    }
    return ret;
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::ChangeDisplay
// Created: JSR 2013-01-18
// -----------------------------------------------------------------------------
void TacticalTreeView::ChangeDisplay( int mode )
{
    displayMode_ = static_cast< EDisplayMode >( mode );
    EntityTreeView_ABC::ApplyProfileFilter();
}

namespace
{
    bool IsVisible( const kernel::Entity_ABC& entity, const kernel::Profile_ABC& profile )
    {
        if( profile.IsVisible( entity ) )
            return true;

        if( const kernel::TacticalHierarchies* hierarchies = entity.Retrieve< kernel::TacticalHierarchies >() )
        {
            auto it = hierarchies->CreateSubordinateIterator();
            while( it.HasMoreElements() )
            {
                const kernel::Entity_ABC& sub = it.NextElement();
                return IsVisible( sub, profile );
            }
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::ApplyProfileFilter
// Created: JSR 2013-01-18
// -----------------------------------------------------------------------------
bool TacticalTreeView::ApplyProfileFilter( QStandardItem& item ) const
{
    QFont font = item.font();
    if( font.italic() )
    {
        font.setItalic( false );
        item.setFont( font );
        item.setForeground( QBrush( Qt::black ) );
    }
    if( displayMode_ == eObservableUnits )
        return gui::TacticalTreeView::ApplyProfileFilter( item );
    else if( displayMode_ == eControlledUnits )
    {
        if( !gui::TacticalTreeView::ApplyProfileFilter( item ) )
            return false;
        const kernel::Entity_ABC* entity = dataModel_.GetDataFromItem< kernel::Entity_ABC >( item );
        return entity && CanBeOrdered( *entity, profile_ );
    }
    else // eSides
    {
        const kernel::Entity_ABC* entity = dataModel_.GetDataFromItem< kernel::Entity_ABC >( item );
        if( entity )
        {
            if( profile_.IsVisible( *entity ) )
                return true;
            if( const kernel::TacticalHierarchies* hierarchies = entity->Retrieve< kernel::TacticalHierarchies >() )
                if( IsVisible( hierarchies->GetTop(), profile_ ) )
                {
                    font.setItalic( true );
                    item.setFont( font );
                    item.setForeground( QBrush( Qt::lightGray ) );
                    return true;
                }
        }
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target )
{
    if( const kernel::Automat_ABC* automat = dynamic_cast< const kernel::Automat_ABC* >( &target ) )
        if( &item.Get< kernel::TacticalHierarchies >().GetUp() != automat )
            actionsModel_.PublishUnitChangeSuperior( item, *automat );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: JSR 2012-09-27
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target)
{
    if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target ) )
        if( &item.Get< kernel::TacticalHierarchies >().GetUp() != formation )
            actionsModel_.PublishChangeFormationSuperior( item, *formation );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::Drop
// Created: AHC 2012-01-11
// -----------------------------------------------------------------------------
void TacticalTreeView::Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target)
{
    if( const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &target ) )
    {
        if( &item.Get< kernel::TacticalHierarchies >().GetUp() != formation )
            actionsModel_.PublishChangeFormationSuperior( item, *formation );
    }
    else if( const kernel::Team_ABC* team = dynamic_cast< const kernel::Team_ABC* >( &target ) )
    {
        if( &item.Get< kernel::TacticalHierarchies >().GetUp() != team &&
            &item.Get< kernel::TacticalHierarchies >().GetTop() == team )
            actionsModel_.PublishChangeFormationSuperior( item, *team );
    }
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnActivate
// Created: JSR 2013-01-21
// -----------------------------------------------------------------------------
void TacticalTreeView::OnActivate( const QModelIndex& index )
{
    const kernel::Entity_ABC* entity = dataModel_.GetDataFromIndex< kernel::Entity_ABC >( index );
    if( entity && profile_.IsVisible( *entity ) )
        entity->Activate( controllers_.actions_ );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnCreateFormation
// Created: SLI 2013-07-16
// -----------------------------------------------------------------------------
void TacticalTreeView::OnCreateFormation( int level )
{
    CreateFormation( level, false );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::OnCreateLogisticBase
// Created: JSR 2013-12-17
// -----------------------------------------------------------------------------
void TacticalTreeView::OnCreateLogisticBase( int level )
{
    CreateFormation( level, true );
}

// -----------------------------------------------------------------------------
// Name: TacticalTreeView::CreateFormation
// Created: JSR 2013-12-17
// -----------------------------------------------------------------------------
void TacticalTreeView::CreateFormation( int level, bool isLogisticBase )
{
    actionsModel_.PublishFormationCreationAction( level, *currentEntity_, isLogisticBase );
}
