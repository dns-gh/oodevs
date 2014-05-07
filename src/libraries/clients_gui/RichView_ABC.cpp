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
    , filtersLayout_( 0 )
    , filterLine_( 0 )
    , signalMapper_( new QSignalMapper( this ) )
{
    connect( signalMapper_, SIGNAL( mapped( QWidget* ) ), this, SLOT( OnFiltersChanged( QWidget* ) ) );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC destructor
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
RichView_ABC::~RichView_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::CreateGUI
// Created: ABR 2012-03-27
// -----------------------------------------------------------------------------
void RichView_ABC::CreateGUI( QAbstractItemView* view )
{
    SubObjectName subObject( view->objectName() );
    QWidget* searchLineWidget = 0;
    SearchLineEdit* filterLine = 0;
    ExpandableGroupBox* filterBox = 0;
    if( HasOption( eOptions_SearchLineEdit ) )
    {
        auto searchLine = new SearchLineEdit( "searchLine", this );

        RichPushButton* next = new RichPushButton( "next", QIcon( MAKE_ICON( search ) ), "", this );
        next->setAccel( Qt::Key_F3 );
        next->setMaximumWidth( searchLine->height() );
        next->setMaximumHeight( searchLine->height() );

        connect( searchLine, SIGNAL( textChanged( const QString& ) ), view, SLOT( SearchAndSelect( const QString& ) ) );
        connect( searchLine, SIGNAL( returnPressed() ),               view, SLOT( SearchAndSelectNext() ) );
        connect( next,       SIGNAL( pressed() ),                     view, SLOT( SearchAndSelectNext() ) );

        searchLineWidget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout( searchLineWidget );
        layout->setMargin( 0 );
        layout->setSpacing( 2 );
        layout->addWidget( searchLine, 1 );
        layout->addWidget( next );
    }
    if( HasOption( options_ & eOptions_FilterLineEdit ) )
    {
        filterLine = new SearchLineEdit( "filterLine", this );
        connect( filterLine, SIGNAL( textChanged( const QString& ) ), signalMapper_, SLOT( map() ) );
        signalMapper_->setMapping( filterLine, static_cast< QWidget* >( filterLine ) );
        filterLine_ = filterLine;
    }
    if( HasOption( options_ & eOptions_FilterBox ) )
    {
        if( options_ & eOptions_ClearButton )
        {
            clearButton_ = new RichPushButton( "clear", tr( "Clear" ) );
            connect( clearButton_, SIGNAL( clicked() ), this, SLOT( OnClearFilters() ) );
        }
        filtersContainer_ = new QWidget();
        filtersLayout_ = new QGridLayout( filtersContainer_, 0, 2 );
        filtersLayout_->setSpacing( 5 );
        filtersLayout_->setContentsMargins( 5, 2, 5, 2 );
        filtersLayout_->addWidget( clearButton_, 0, 0, 1, 2, Qt::AlignLeft );

        filterBox = new ExpandableGroupBox( "filtersWidget", 0, tr( "Filters" ) );
        filterBox->AddComponent( filtersContainer_ );      
    }
    if( HasOption( options_ & eOptions_FilterMenu ) )
    {
        view_->GetHeader()->setContextMenuPolicy( Qt::CustomContextMenu );
        connect( view_->GetHeader(), SIGNAL( customContextMenuRequested( const QPoint& ) ), SLOT( OnHeaderContextMenuRequested( const QPoint& ) ) );
    }
    // Layout
    QVBoxLayout* layout = new QVBoxLayout( this );
    auto margin = searchLineWidget || filterLine || filterBox ? 2 : 0;
    layout->setMargin( margin );
    layout->setSpacing( margin );
    if( searchLineWidget )
        layout->addWidget( searchLineWidget );
    if( filterLine )
        layout->addWidget( filterLine );
    if( filterBox )
        layout->addWidget( filterBox );
    layout->addWidget( view, 1 );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::Load
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void RichView_ABC::Load()
{
    view_->CreateFilters( *this );
    if( filterLine_ )
        filters_[ -1 ].push_back( filterLine_ );
    for( auto it = menus_.begin(); it != menus_.end(); ++it )
    {
        QWidget* widget = new QWidget();
        QHBoxLayout* layout = new QHBoxLayout( widget );
        layout->setMargin( 0 );
        layout->setSpacing( 0 );
        QPushButton* button = new RichPushButton( "close", tr( "Close" ) );
        connect( button, SIGNAL( clicked() ), it->second, SLOT( hide() ) );
        layout->addWidget( button, 0, Qt::AlignRight );
        it->second->AddWidget( widget );
    }
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::Purge
// Created: ABR 2012-06-19
// -----------------------------------------------------------------------------
void RichView_ABC::Purge()
{
    view_->Purge();
    if( HasOption( eOptions_FilterBox ) )
    {
        delete filtersLayout_;
        filtersLayout_ = new QGridLayout( filtersContainer_, 0, 2 );
        filtersLayout_->addWidget( clearButton_, 0, 0, 1, 2, Qt::AlignLeft );
    }
    for( auto it = filters_.begin(); it != filters_.end(); ++it )
        for( auto filter = it->second.begin(); filter != it->second.end(); ++filter )
            if( !filterLine_ || *filter != filterLine_ )
                delete *filter;
    filters_.clear();
    for( auto it = menus_.begin(); it != menus_.end(); ++it )
        delete it->second;
    menus_.clear();
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
    view_->ApplyFilters( filters_ );
    FiltersStatusChanged( IsFiltered() );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::OnHeaderContextMenuRequested
// Created: ABR 2014-04-25
// -----------------------------------------------------------------------------
void RichView_ABC::OnHeaderContextMenuRequested( const QPoint& pos )
{
    auto header = view_->GetHeader();
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
        menus_[ col ] = new WidgetMenu( this );
        it = menus_.find( col );
    }
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::AddWidgetToFilterBox
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
void RichView_ABC::AddWidgetToFilterBox( int col, const QString& text, Filter_ABC* filter, QWidget* widget )
{
    filtersLayout_->addWidget( new QLabel( text ) );
    filtersLayout_->addWidget( widget );
    filters_[ col ].push_back( filter );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::AddWidgetToMenu
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
void RichView_ABC::AddWidgetToMenu( int col, const QString& text, Filter_ABC* filter, QWidget* widget )
{
    auto menu = GetMenu( col );
    if( !text.isEmpty() )
        menu->AddWidget( new QLabel( text ) );
    menu->AddWidget( widget );
    filters_[ col ].push_back( filter );
    model_->setHeaderData( col, Qt::Horizontal, QPixmap( "resources/images/gui/filter_empty.png" ), Qt::DecorationRole );
}

// -----------------------------------------------------------------------------
// Name: RichView_ABC::function< void
// Created: ABR 2014-04-29
// -----------------------------------------------------------------------------
CheckComboBox* RichView_ABC::CreateCheckCombo( QWidget* parent,
                                               bool useNone,
                                               bool useCheckAll,
                                               const CheckComboBox::T_Extractor& extractor,
                                               const CheckComboBox::T_Filler& filler )
{
    CheckComboBox* combo = new CheckComboBox( "combo", parent, useNone, useCheckAll, extractor, filler );
    connect( combo, SIGNAL( CheckedItemsChanged( const QStringList& ) ), signalMapper_, SLOT( map() ) );
    signalMapper_->setMapping( combo, static_cast< QWidget* >( combo ) );
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
        AddWidgetToFilterBox( col, text, combo, combo );
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
