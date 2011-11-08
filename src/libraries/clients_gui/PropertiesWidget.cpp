// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertiesWidget.h"
#include "moc_PropertiesWidget.cpp"
#include "PropertiesTable.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DictionaryUpdated.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "resources.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertiesWidget constructor
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
PropertiesWidget::PropertiesWidget( kernel::Controllers& controllers, QWidget* parent, const QString& name, kernel::EditorFactory_ABC& factory, TableItemDisplayer& displayer )
    : QWidget( parent )
    , controllers_( controllers )
    , factory_    ( factory )
    , displayer_  ( displayer )
    , button_     ( 0 )
    , selected_   ( controllers )
{
    FillUp( name, true );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
PropertiesWidget::PropertiesWidget( kernel::Controllers& controllers, PropertiesWidget* parent, const QString& name, kernel::EditorFactory_ABC& factory, TableItemDisplayer& displayer )
    : QWidget( parent )
    , controllers_( controllers )
    , factory_    ( factory )
    , displayer_  ( displayer )
    , button_     ( 0 )
    , selected_   ( controllers )
{
    FillUp( name );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget destructor
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
PropertiesWidget::~PropertiesWidget()
{
    controllers_.Unregister( *this );
}

namespace
{
    class MyButton : public QToolButton
    {
    public:
        MyButton( QWidget* parent, const QPixmap& on, const QPixmap& off )
            : QToolButton( parent )
        {
            QIcon pIcon;
            pIcon.addPixmap( off, QIcon::Normal, QIcon::Off );
            pIcon.addPixmap( on, QIcon::Normal, QIcon::On );
            this->setIcon( pIcon );
        };
    };
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::FillUp
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void PropertiesWidget::FillUp( const QString& name, bool root /*= false*/ )
{
    static const QColor background( 115, 110, 100 );
    static const QColor foreground( 255, 255, 255 );

    if( root )
    {
        layout_ = new QGridLayout( this, 1, 1 );
        table_ = new PropertiesTable( this, factory_, displayer_ );
        layout_->addWidget( table_, 0, 0, Qt::AlignTop );
        layout_->setAlignment( Qt::AlignTop );
        table_->hide();
        return;
    }
    layout_ = new QGridLayout( this, 2, 2 );
    layout_->setColStretch( 0, 0 );
    layout_->setColStretch( 1, 1 );
    layout_->setAlignment( Qt::AlignTop );
    {
        Q3HBox* box = new Q3HBox( this );
        button_ = new MyButton( box, MAKE_PIXMAP( minus ), MAKE_PIXMAP( plus ) );
        button_->setToggleButton( true );
        button_->setFixedSize( 13, 15 );
        button_->setOn( true );
        button_->setPaletteBackgroundColor( background );
        button_->setAutoFillBackground( true );
        QLabel* headerLabel = new QLabel( name, box );
        QFont font( headerLabel->font() );
        font.setBold( true );
        headerLabel->setFont( font );
        headerLabel->setMaximumHeight( 15 );
        headerLabel->setPaletteBackgroundColor( background );
        headerLabel->setPaletteForegroundColor( foreground );
        headerLabel->setAutoFillBackground( true );
        layout_->addMultiCellWidget( box, 0, 0, 0, 1 );
    }
    QWidget* spacer = new QWidget( this );
    spacer->setFixedWidth( 5 );
    spacer->setPaletteBackgroundColor( background );
    spacer->setAutoFillBackground( true );
    layout_->addWidget( spacer, 1, 0, Qt::AlignTop );

    table_ = new PropertiesTable( this, factory_, displayer_ );
    connect( button_, SIGNAL( toggled( bool ) ), table_, SLOT( Show( bool ) ) );
    layout_->addWidget( table_, 1, 1, Qt::AlignTop );
    table_->hide();
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::Clear
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void PropertiesWidget::Clear()
{
    categories_.clear();
    for( T_SubWidgets::const_iterator it = subWidgets_.begin(); it != subWidgets_.end(); ++it )
        delete *it;
    subWidgets_.clear();
    table_->Clear();
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::Hide
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesWidget::Hide()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::SubItem
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
kernel::Displayer_ABC& PropertiesWidget::SubItem( const QString& name )
{
    QStringList path = QStringList::split( '/', name );
    if( path.size() >= 2 )
    {
        QString subItem = path.front();
        path.pop_front();
        QString subPath = path.join( "/" );
        return SubItem( subItem, subPath );
    }
    return table_->Item( name );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::SubItem
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
kernel::Displayer_ABC& PropertiesWidget::SubItem( const QString& subItem, const QString& path )
{
    CIT_SubCategories it = categories_.find( subItem );
    PropertiesWidget* subWidget = 0;
    if( it != categories_.end() )
        subWidget = subWidgets_[ it->second ];
    else
        subWidget = CreateWidget( subItem );
    return subWidget->SubItem( path );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::CreateWidget
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
PropertiesWidget* PropertiesWidget::CreateWidget( const QString& subItem )
{
    PropertiesWidget* subWidget = new PropertiesWidget( controllers_, this, subItem, factory_, displayer_ );
    if( button_ )
        connect( button_, SIGNAL( toggled( bool ) ), subWidget, SLOT( setShown( bool ) ) );
    subWidgets_.push_back( subWidget );
    categories_[ subItem ] = subWidgets_.size() - 1;
    layout_->addWidget( subWidget, static_cast< int >( subWidgets_.size() + 1 ), 1, Qt::AlignTop );
    subWidget->show();
    if( ! table_->numRows() )
        table_->hide();
    return subWidget;
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::StartDisplay
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesWidget::StartDisplay()
{
    NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::DisplayFormatted
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesWidget::DisplayFormatted( const QString& )
{
    NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::EndDisplay
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesWidget::EndDisplay()
{
    NotToBeCalled( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::NotifyUpdated
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void PropertiesWidget::NotifyUpdated( const kernel::DictionaryUpdated& message )
{
    UpdatePath( message, message.GetEntry(), *this );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::NotifyDeleted
// Created: SBO 2006-11-13
// -----------------------------------------------------------------------------
void PropertiesWidget::NotifyDeleted( const kernel::DictionaryUpdated& message )
{
    ClearPath( message.GetEntry() );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::UpdatePath
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
void PropertiesWidget::UpdatePath( const kernel::DictionaryUpdated& message, const QString& name, PropertiesWidget& parent )
{
    if( selected_ )
    {
        QStringList path = QStringList::split( '/', name );
        CIT_SubCategories it = categories_.find( path.front() );
        if( it != categories_.end() )
        {
            std::string tmp = name.ascii();
            path.pop_front();
            if( !path.empty() )
                subWidgets_[it->second]->UpdatePath( message, path.join( "/" ), parent );
            else
                message.GetEntity().Get< kernel::PropertiesDictionary >().DisplaySubPath( message.GetEntry(), parent );
        }
        else
        {
            if( path.front() == name )
            {
                CreateWidget( name );
                UpdatePath( message, name, parent );
            }
            else
                table_->Update( name );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::ClearPath
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
void PropertiesWidget::ClearPath( const QString& name )
{
    QStringList path = QStringList::split( '/', name );
    CIT_SubCategories it = categories_.find( path.front() );
    if( it != categories_.end() )
    {
        path.pop_front();
        if( !path.empty() )
            subWidgets_[it->second]->ClearPath( path.join( "/" ) );
        else
            subWidgets_[it->second]->Clear();
    }
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::NotifySelected
// Created: LGY 2011-11-07
// -----------------------------------------------------------------------------
void PropertiesWidget::NotifySelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
}
