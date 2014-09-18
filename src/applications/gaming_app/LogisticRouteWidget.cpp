// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticRouteWidget.h"
#include "moc_LogisticRouteWidget.cpp"
#include "actions/PullFlowParameters.h"
#include "actions/PushFlowParameters.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/TaskerWidget.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Pathfind_ABC.h"
#include "clients_kernel/Tools.h"
#include <boost/range/algorithm_ext/erase.hpp>

LogisticRouteWidget::LogisticRouteWidget( kernel::Controllers& controllers, const gui::EntitySymbols& symbols )
    : layout_                    ( new QGridLayout() )
    , controllers_               ( controllers )
    , selectedPathfind_          ( controllers )
    , symbols_                   ( symbols )
    , upIcon_                    ( "resources/images/gui/up.png" )
    , downIcon_                  ( "resources/images/gui/down.png" )
    , upSignalMapper_            ( new QSignalMapper( this ) )
    , downSignalMapper_          ( new QSignalMapper( this ) )
    , routeSignalMapper_         ( new QSignalMapper( this ) )
    , routeActivatedSignalMapper_( new QSignalMapper( this ) )
{
    QVBoxLayout* main = new QVBoxLayout();
    main->setSizeConstraint( QLayout::SetMinimumSize );
    main->addLayout( layout_ );
    main->addStretch( 1 );
    setLayout( main );
    connect( upSignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnUp( int ) ) );
    connect( downSignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( OnDown( int ) ) );
    connect( routeSignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( SelectRoute( int ) ) );
    connect( routeActivatedSignalMapper_, SIGNAL( mapped( int ) ), this, SLOT( ActivateRoute( int ) ) );
    controllers_.Register( *this );
}

LogisticRouteWidget::~LogisticRouteWidget()
{
    controllers_.Unregister( *this );
}

void LogisticRouteWidget::AddRecipient( const kernel::Entity_ABC& recipient )
{
    if( recipients_.empty() )
        return;
    // Last position is reserved for the requester
    recipients_.insert( recipients_.end() - 1, &recipient );
    Build();
}

void LogisticRouteWidget::AddRequester( const kernel::Entity_ABC* requester )
{
    Clear();
    if( requester )
        recipients_.push_back( requester );
    Build();
}

void LogisticRouteWidget::Clear()
{
    recipients_.clear();
    routes_.clear();
}

void LogisticRouteWidget::Remove( const kernel::Entity_ABC& recipient )
{
    routes_.erase( recipient.GetId() );
    boost::remove_erase( recipients_, &recipient );
}

void LogisticRouteWidget::RemoveRecipient( const kernel::Entity_ABC& recipient )
{
    Remove( recipient );
    Build();
}

namespace
{
    QPushButton* CreateButton( const QIcon& icon, bool enabled, int index, QSignalMapper* mapper )
    {
        QPushButton* button = new QPushButton( icon, "" );
        button->setIconSize( QSize( 20, 20 ) );
        button->setEnabled( enabled );
        QObject::connect( button, SIGNAL( clicked() ), mapper, SLOT( map() ) );
        mapper->setMapping( button, index );
        return button;
    }
    QLayoutItem* ClearLayoutItem( QLayoutItem* item )
    {
        if( auto layout = item->layout() )
            for( int i = 0; i < layout->count(); ++i )
                ClearLayoutItem( layout->itemAt( i ) );
        delete item->widget();
        return item;
    }
    void ClearLayout( QLayout* layout )
    {
        QLayoutItem* child;
        while( ( child = layout->takeAt( 0 ) ) != 0 )
            delete ClearLayoutItem( child );
    }
    gui::RichGroupBox* CreateItineraryWidget( QSignalMapper* mapper, int id, bool checked, const std::string& label )
    {
        gui::RichGroupBox* groupBox = new gui::RichGroupBox( "route-box", tools::translate( "LogisticRouteWidget", "Itinerary" ) );
        groupBox->setCheckable( true );
        groupBox->setChecked( checked );
        QObject::connect( groupBox, SIGNAL( clicked() ), mapper, SLOT( map() ) );
        mapper->setMapping( groupBox, id );

        QVBoxLayout* bLayout = new QVBoxLayout( groupBox );
        QLabel* entityLabel = new QLabel( label.c_str() );
        entityLabel->setMinimumWidth( 100 );
        entityLabel->setAlignment( Qt::AlignCenter );
        bLayout->addWidget( entityLabel );
        return groupBox;
    }
}

void LogisticRouteWidget::OnUp( int index )
{
    if( index > 0 && recipients_.size() > index )
    {
        std::swap( recipients_[ index ], recipients_[ index - 1 ] );
        Build();
    }
}

void LogisticRouteWidget::OnDown( int index )
{
    if( index >= 0 && index < recipients_.size() - 1 )
    {
        std::swap( recipients_[ index ], recipients_[ index + 1 ] );
        Build();
    }
}

void LogisticRouteWidget::Build()
{
    ClearLayout( layout_ );
    // No recipients
    const auto size = recipients_.size();
    if( size <= 1 )
        return;

    int row = 0;
    // Build recipient
    for( int i = 0; i < size; ++i )
    {
        const auto* recipient = recipients_[ i ];
        const bool isRequester = i == recipients_.size() - 1;
        // Tasker widget
        if( !isRequester )
        {
            gui::TaskerWidget* tasker = new gui::TaskerWidget( "", controllers_, symbols_, "", true, false, true, Qt::AlignLeft );
            tasker->SetTasker( recipient );
            tasker->SetButtonsSize( QSize( 20, 20 ) );
            tasker->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding );
            layout_->addWidget( tasker, row, 0 );
        }
        else
            layout_->addWidget( new QLabel( tools::translate( "LogisticRouteWidget", "Return trip" ) ), row, 0, Qt::AlignCenter );

        // Itinerary widget
        const auto it = routes_.find( recipient->GetId() );
        layout_->addWidget( CreateItineraryWidget( routeActivatedSignalMapper_, recipient->GetId(),
            it != routes_.end() && it->second.first,
            it != routes_.end() && it->second.second ? it->second.second->GetName().toStdString() : "---" ),
            row, 1, Qt::AlignVCenter );

        // Buttons widget
        if( size > 2 && !isRequester )
        {
            layout_->addWidget( CreateButton( upIcon_, i != 0, i, upSignalMapper_ ), row, 2 );
            layout_->addWidget( CreateButton( downIcon_, i != recipients_.size() - 2, i, downSignalMapper_ ), row, 3 );
        }
        ++row;

        // Add separator
        if( !isRequester )
        {
            QFrame* hline = new QFrame( this );
            hline->setFrameStyle( QFrame::HLine | QFrame::Sunken );
            layout_->addWidget( hline, row, 0, 1, 4 );
            ++row;
        }
    }
}

void LogisticRouteWidget::NotifyContextMenu( const kernel::Pathfind_ABC& entity, kernel::ContextMenu& menu )
{
    // No recipients
    if( recipients_.size() <= 1 )
        return;
    selectedPathfind_ = &entity;
    for( auto it = recipients_.begin(); it != recipients_.end(); ++it )
    {
        QAction* action = menu.InsertItem( "Itinerary", it != recipients_.end() - 1 ?
            tools::translate( "LogisticRouteWidget", "Itinerary to %1" ).arg( (*it)->GetName() ) :
            tools::translate( "LogisticRouteWidget", "Return trip" ),
            routeSignalMapper_, SLOT( map() ) );
        routeSignalMapper_->setMapping( action, (*it)->GetId() );
    }
}

void LogisticRouteWidget::SelectRoute( int id )
{
    if( selectedPathfind_ )
    {
        routes_[ id ] = std::make_pair( true, selectedPathfind_ );
        Build();
    }
}

void LogisticRouteWidget::ToggleRouteActivation( int id )
{
    auto it = routes_.find( id );
    if( it == routes_.end() )
        routes_[ id ] = std::make_pair( true, nullptr );
    else
        it->second.first = !it->second.first;
}

const kernel::Pathfind_ABC* LogisticRouteWidget::GetPathfind( unsigned int id ) const
{
    const auto it = routes_.find( id );
    if( it != routes_.end() && it->second.first )
        return it->second.second;
    return 0;
}

void LogisticRouteWidget::FillPushFlowParameters( actions::parameters::PushFlowParameters& parameters )
{
    if( recipients_.size() <= 1 )
        return;

    // recipients
    for( auto it = recipients_.begin(); it != recipients_.end() - 1; ++it )
        parameters.SetPath( GetPathfind( (*it)->GetId() ), **it );

    // wayback
    const auto wayback = recipients_.back();
    if( const kernel::Pathfind_ABC* pathfind = GetPathfind( wayback->GetId() ) )
        parameters.SetWayBackPath( *pathfind );
}

void LogisticRouteWidget::FillPullFlowParameters( actions::parameters::PullFlowParameters& parameters )
{
    if( recipients_.size() != 2 )
        return;

    // wayback
    const auto* wayback = recipients_.at( 0 );
    if( const kernel::Pathfind_ABC* pathfind = GetPathfind( wayback->GetId() ) )
        parameters.SetWayOutPath( *pathfind );

    // wayOut
    const auto* wayout = recipients_.at( 1 );
    if( const kernel::Pathfind_ABC* pathfind = GetPathfind( wayout->GetId() ) )
        parameters.SetWayBackPath( *pathfind );
}

void LogisticRouteWidget::NotifyDeleted( const kernel::Pathfind_ABC& entity )
{
    bool deleted = false;
    for( auto it = routes_.begin(); it != routes_.end(); ++it )
    {
        if( const kernel::Pathfind_ABC* pathfind = it->second.second )
            if( pathfind->GetId() == entity.GetId() )
            {
                it->second = std::make_pair( false, nullptr );
                deleted = true;
            }
    }
    if( deleted )
        Build();
}

void LogisticRouteWidget::NotifyDeleted( const kernel::Automat_ABC& entity )
{
    Remove( entity );

}

void LogisticRouteWidget::NotifyDeleted( const kernel::Formation_ABC& entity )
{
    Remove( entity );
}
