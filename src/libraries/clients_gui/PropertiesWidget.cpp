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

#include <qpainter.h>

using namespace gui;
using namespace kernel;


// -----------------------------------------------------------------------------
// Name: PropertiesWidget constructor
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
PropertiesWidget::PropertiesWidget( QWidget* parent, const QString& name, kernel::EditorFactory_ABC& factory )
    : QWidget( parent )
    , factory_( factory )
{
    FillUp( name );

    spacer_ = new QSpacerItem( 100, 5, QSizePolicy::Minimum, QSizePolicy::Ignored );
    layout_->addItem( spacer_, 3, 1 );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget constructor
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
PropertiesWidget::PropertiesWidget( PropertiesWidget* parent, const QString& name, kernel::EditorFactory_ABC& factory )
    : QWidget( parent )
    , factory_( factory )
    , spacer_( 0 )
{
    FillUp( name );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget destructor
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
PropertiesWidget::~PropertiesWidget()
{
    // NOTHING
}

namespace 
{
    class MyButton : public QToolButton
    {
    public:
        MyButton( QWidget* parent, const QPixmap& on, const QPixmap& off )
            : QToolButton( parent )
            , on_( on )
            , off_( off ) {};
        virtual void drawButton( QPainter* p ) {
            drawButtonLabel(p);
        }
        virtual void drawButtonLabel( QPainter* p ) {
            p->drawPixmap( QRect( 2, 2, 9, 9 ), isOn() ? on_ : off_ );
        }
    private:
        QPixmap on_;
        QPixmap off_;
    };
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::FillUp
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void PropertiesWidget::FillUp( const QString& name )
{
    layout_ = new QGridLayout( this, 2, 2 );

    // Columns
    layout_->setColStretch( 0, 0 );
    layout_->setColSpacing( 0, 10 );
    layout_->setColStretch( 1, 1 );
    layout_->setColSpacing( 1, 100 );

    // First row
    layout_->setRowStretch( 0, 0 );
    layout_->setRowSpacing( 0, 10 );

    button_ = new MyButton( this, MAKE_PIXMAP( minus ), MAKE_PIXMAP( plus ) );
    button_->setToggleButton( true );
    button_->setMinimumSize( 15, 15 );
    button_->setMaximumSize( 15, 15 );
    button_->setOn( true );
    layout_->addWidget( button_, 0, 0 );

    QLabel* headerLabel = new QLabel( name, this );
    headerLabel->setMaximumHeight( 15 );
    layout_->addWidget( headerLabel, 0, 1 );

    // second row
    layout_->setRowStretch( 1, 0 );
    layout_->setRowSpacing( 1, 1 );

    // table row
    table_ = new PropertiesTable( this, factory_ );
    connect( button_, SIGNAL( toggled( bool ) ), table_, SLOT( Show( bool ) ) );
    layout_->addWidget( table_, 2, 1 );
    table_->hide();
}


// -----------------------------------------------------------------------------
// Name: PropertiesWidget::Clear
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void PropertiesWidget::Clear()
{
    categories_.clear();
    for( CIT_SubWidgets it = subWidgets_.begin(); it != subWidgets_.end(); ++it )
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
// Name: PropertiesWidget::sizeHint
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
QSize PropertiesWidget::sizeHint() const
{
    if( button_->isOn() )
        return QWidget::sizeHint();
    return minimumSizeHint();
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::minimumSizeHint
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
QSize PropertiesWidget::minimumSizeHint() const
{
    return QSize( 100, 15 );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::sizePolicy
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
QSizePolicy PropertiesWidget::sizePolicy() const
{
    if( button_->isOn() )
        return QWidget::sizePolicy();
    return QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
}

// -----------------------------------------------------------------------------
// Name: PropertiesWidget::SubItem
// Created: AGE 2006-10-18
// -----------------------------------------------------------------------------
Displayer_ABC& PropertiesWidget::SubItem( const QString& name )
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
    PropertiesWidget* subWidget = new PropertiesWidget( this, subItem, factory_ );
    connect( button_, SIGNAL( toggled( bool ) ), subWidget, SLOT( setShown( bool ) ) );
    subWidgets_.push_back( subWidget );
    categories_[ subItem ] = subWidgets_.size() - 1;
    layout_->remove( table_ );
    if( spacer_ )
        layout_->removeItem( spacer_ );
    layout_->addWidget( subWidget, subWidgets_.size() + 1, 1 );
    subWidget->show();
    layout_->addWidget( table_, subWidgets_.size() + 2, 1 );
    if( spacer_ )
        layout_->addItem( spacer_, subWidgets_.size() + 3, 1 );
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
