// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichView_ABC.h"
#include "moc_RichView_ABC.cpp"

#include "ExpandableGroupBox.h"
#include "Resources.h"
#include "RichPushButton.h"
#include "RichTreeView.h"
#include "SearchLineEdit.h"
#include "SubObjectName.h"
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichView_ABC constructor
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
RichView_ABC::RichView_ABC( int options, QWidget* parent )
    : QWidget( parent )
    , options_( options )
    , signalMapper_( new QSignalMapper( this ) )
{
    connect( signalMapper_.get(), SIGNAL( mapped( QWidget* ) ), this, SLOT( OnFiltersChanged( QWidget* ) ) );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC destructor
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
RichView_ABC::~RichView_ABC()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::CreateGUI
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
void RichView_ABC::CreateGUI()
{
    assert( view_ != 0 );
    assert( filterable_ != 0 );
    SubObjectName subObject( view_->objectName() );
    mainLayout_.reset( new QVBoxLayout( this ) );
    auto margin = HasOption( eOptions_SearchLineEdit ) ||
                  HasOption( eOptions_FilterLineEdit ) ||
                  HasOption( eOptions_FilterBox ) ? 2 : 0;
    mainLayout_->setMargin( margin );
    mainLayout_->setSpacing( margin );
    if( HasOption( eOptions_SearchLineEdit ) )
    {
        auto searchLine = new SearchLineEdit( "searchLine", 0 );

        RichPushButton* next = new RichPushButton( "next", QIcon( MAKE_ICON( search ) ), "", 0 );
        next->setAccel( Qt::Key_F3 );
        next->setMaximumWidth( searchLine->height() );
        next->setMaximumHeight( searchLine->height() );

        connect( searchLine, SIGNAL( textChanged( const QString& ) ), view_, SLOT( SearchAndSelect( const QString& ) ) );
        connect( searchLine, SIGNAL( returnPressed() ),               view_, SLOT( SearchAndSelectNext() ) );
        connect( next,       SIGNAL( pressed() ),                     view_, SLOT( SearchAndSelectNext() ) );

        searchLineWidget_.reset( new QWidget( this ) );
        QHBoxLayout* layout = new QHBoxLayout( searchLineWidget_.get() );
        layout->setMargin( 0 );
        layout->setSpacing( 2 );
        layout->addWidget( searchLine, 1 );
        layout->addWidget( next );
        mainLayout_->addWidget( searchLineWidget_.get() );
    }
    if( HasOption( eOptions_FilterLineEdit ) )
    {
        std::shared_ptr< SearchLineEdit > filterLine_( new SearchLineEdit( "filterLine", this ) );
        connect( filterLine_.get(), SIGNAL( textChanged( const QString& ) ), signalMapper_.get(), SLOT( map() ) );
        signalMapper_->setMapping( filterLine_.get(), static_cast< QWidget* >( filterLine_.get() ) );
        filters_[ -1 ].push_back( filterLine_ );
        mainLayout_->addWidget( filterLine_.get() );
    }
    if( HasOption( eOptions_FilterBox ) )
    {
        QWidget* clearButton = 0;
        if( options_ & eOptions_ClearButton )
        {
            clearButton = new RichPushButton( "clear", tr( "Clear" ) );
            connect( clearButton, SIGNAL( clicked() ), this, SLOT( OnClearFilters() ) );
        }
        auto filtersContainer = new QWidget();
        filtersLayout_.reset( new QGridLayout( filtersContainer, 0, 2 ) );
        filtersLayout_->setSpacing( 5 );
        filtersLayout_->setContentsMargins( 5, 2, 5, 2 );
        if( clearButton )
            filtersLayout_->addWidget( clearButton, 0, 0, 1, 2, Qt::AlignLeft );

        filterBox_.reset( new ExpandableGroupBox( "filtersWidget", 0, tr( "Filters" ) ) );
        static_cast< ExpandableGroupBox& >( *filterBox_ ).AddComponent( filtersContainer );      
        mainLayout_->addWidget( filterBox_.get() );
    }
    if( HasOption( eOptions_FilterMenu ) )
    {
        filterable_->GetHeader()->setContextMenuPolicy( Qt::CustomContextMenu );
        connect( filterable_->GetHeader(), SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnHeaderContextMenuRequested( const QPoint& ) ) );
    }
    mainLayout_->addWidget( view_, 1 );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::Load
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void RichView_ABC::Load()
{
    assert( filterable_ != 0 );
    CreateGUI();
    filterable_->CreateFilters( *this );
    for( auto it = menus_.begin(); it != menus_.end(); ++it )
    {
        QWidget* widget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout( widget );
        layout->setMargin( 0 );
        layout->setSpacing( 0 );
        QPushButton* button = new RichPushButton( "close", tr( "Close" ) );
        connect( button, SIGNAL( clicked() ), it->second.get(), SLOT( hide() ) );
        layout->addWidget( button, 0, Qt::AlignRight );
        it->second->AddWidget( widget );
    }
    view_->setVisible( true );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::Purge
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void RichView_ABC::Purge()
{
    assert( filterable_ != 0 );
    if( HasOption( eOptions_FilterMenu ))
        disconnect( filterable_->GetHeader(), SIGNAL( customContextMenuRequested( const QPoint& ) ),
                    this,                     SLOT( OnHeaderContextMenuRequested( const QPoint& ) ) );
    filterable_->Purge();
    filters_.clear();
    menus_.clear();
    filtersLayout_.reset();
    filterBox_.reset();
    searchLineWidget_.reset();
    mainLayout_.reset();
    view_->setVisible( false );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::OnClearFilters
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void RichView_ABC::OnClearFilters()
{
    for( auto it = filters_.begin(); it != filters_.end(); ++it )
        for( auto filter = it->second.begin(); filter != it->second.end(); ++filter )
            ( *filter )->Clear();
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::IsFiltered
// Created: ABR 2014-04-30
// -----------------------------------------------------------------------------
bool RichView_ABC::IsFiltered() const
{
    return !model_->match( model_->index( 0, 0 ),
                           Roles::FilterRole,
                           QVariant( gui::StandardModel::hideValue_ ),
                           1,
                           Qt::MatchExactly ).isEmpty();
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::OnFiltersChanged
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void RichView_ABC::OnFiltersChanged( QWidget* )
{
    assert( filterable_ != 0 );
    filterable_->ApplyFilters( filters_ );
    emit FiltersStatusChanged( IsFiltered() );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::OnHeaderContextMenuRequested
// Created: ABR 2014-04-25
// -----------------------------------------------------------------------------
void RichView_ABC::OnHeaderContextMenuRequested( const QPoint& pos )
{
    assert( filterable_ != 0 );
    auto header = filterable_->GetHeader();
    auto section = header->logicalIndexAt( pos );
    auto it = menus_.find( section );
    if( it != menus_.end() && it->second )
    {
        QPoint point( header->pos().x() + header->sectionPosition( section ),
                      header->pos().y() + header->height() );
        it->second->exec( header->mapToGlobal( point ) );
    }
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::IsValidArea
// Created: ABR 2014-04-28
// -----------------------------------------------------------------------------
bool RichView_ABC::IsValidArea( int areas ) const
{
    return areas & eFilterAreas_Box && HasOption( eOptions_FilterBox ) ||
           areas & eFilterAreas_Menu && HasOption( eOptions_FilterMenu );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::GetMenu
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
WidgetMenu* RichView_ABC::GetMenu( int col )
{
    auto it = menus_.find( col );
    if( it == menus_.end() )
    {
        menus_[ col ].reset( new WidgetMenu( this ) );
        it = menus_.find( col );
    }
    return it->second.get();
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::AddWidgetToFilterBox
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
void RichView_ABC::AddWidgetToFilterBox( int col, const QString& text, const std::shared_ptr< Filter_ABC >& filter, QWidget* widget )
{
    filtersLayout_->addWidget( new QLabel( text ) );
    filtersLayout_->addWidget( widget );
    filters_[ col ].push_back( filter );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::AddWidgetToMenu
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
void RichView_ABC::AddWidgetToMenu( int col, const QString& text, const std::shared_ptr< Filter_ABC >& filter, QWidget* widget )
{
    auto menu = GetMenu( col );
    if( !text.isEmpty() )
        menu->AddWidget( new QLabel( text ) );
    menu->AddWidget( widget );
    model_->setHeaderData( col, Qt::Horizontal, QPixmap( "resources/images/gui/filter_empty.png" ), Qt::DecorationRole );
    filters_[ col ].push_back( filter );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::function< void
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
std::shared_ptr< CheckComboBox > RichView_ABC::CreateCheckCombo( QWidget* parent,
                                                                 bool useNone,
                                                                 bool useCheckAll,
                                                                 const CheckComboBox::T_Extractor& extractor,
                                                                 const CheckComboBox::T_Filler& filler )
{
    auto combo = std::make_shared< CheckComboBox >( "combo", parent, useNone, useCheckAll, extractor, filler );
    connect( combo.get(), SIGNAL( CheckedItemsChanged( const QStringList& ) ), signalMapper_.get(), SLOT( map() ) );
    signalMapper_->setMapping( combo.get(), static_cast< QWidget* >( combo.get() ) );
    return combo;
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::AddEnumFilter
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void RichView_ABC::AddCheckComboBoxFilter( int col,
                                           int areas,
                                           const QString& text,
                                           bool useNone,
                                           bool useCheckAll,
                                           const CheckComboBox::T_Extractor& extractor,
                                           const CheckComboBox::T_Filler& filler )
{
    if( !IsValidArea( areas ) )
    {
        MT_LOG_ERROR_MSG( "Incompatible filter area and rich view options" );
        return;
    }
    SubObjectName subObject( "ResolverFilter" + text );
    if( areas & eFilterAreas_Box )
    {
        auto combo = CreateCheckCombo( this, useNone, useCheckAll, extractor, filler );
        AddWidgetToFilterBox( col, text, combo, combo.get() );
    }
    if( areas & eFilterAreas_Menu )
    {
        auto menu = GetMenu( col );
        auto combo = CreateCheckCombo( menu, useNone, useCheckAll, extractor, filler );
        combo->setVisible( false );
        AddWidgetToMenu( col, text, combo, combo->CreateListView( menu ) );
    }
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::HasOption
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
bool RichView_ABC::HasOption( int option ) const
{
    return ( options_ & option ) != 0;
}
