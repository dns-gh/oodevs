// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichLineEdit.h"
#include "ObjectNameManager.h"
#include "moc_RichLineEdit.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// SideWidget
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: SideWidget constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
SideWidget::SideWidget( QWidget *parent /*= 0*/ )
    : QWidget( parent )
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}

// -----------------------------------------------------------------------------
// Name: SideWidget::event
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
bool SideWidget::event( QEvent *event )
{
    if( event->type() == QEvent::LayoutRequest )
        emit sizeHintChanged();
    return QWidget::event( event );
}

// -----------------------------------------------------------------------------
// RichLineEdit
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: RichLineEdit constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
RichLineEdit::RichLineEdit( const QString& objectName, QWidget* parent /*= 0*/ )
    : QLineEdit( parent )
{
    Init( objectName );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::BaseEditLine_ABC
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
RichLineEdit::RichLineEdit( const QString& objectName, const QString& contents, QWidget* parent /*= 0*/ )
    : QLineEdit( contents, parent )
{
    if( parent && parent->layout() )
        parent->layout()->addWidget( this );
    Init( objectName );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit destructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
RichLineEdit::~RichLineEdit()
{
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::Init
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::Init( const QString& objectName )
{
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
    leftWidget_ = new SideWidget( this );
    leftWidget_->resize( 0, 0 );
    leftLayout_ = new QHBoxLayout( leftWidget_ );
    leftLayout_->setContentsMargins( 0, 0, 0, 0 );
    if( isRightToLeft() )
        leftLayout_->setDirection( QBoxLayout::RightToLeft );
    else
        leftLayout_->setDirection( QBoxLayout::LeftToRight );
    leftLayout_->setSizeConstraint( QLayout::SetFixedSize );

    rightWidget_ = new SideWidget( this );
    rightWidget_->resize( 0, 0 );
    rightLayout_ = new QHBoxLayout( rightWidget_ );
    if( isRightToLeft() )
        rightLayout_->setDirection( QBoxLayout::RightToLeft );
    else
        rightLayout_->setDirection( QBoxLayout::LeftToRight );
    rightLayout_->setContentsMargins( 0, 0, 0, 0 );

    QSpacerItem* horizontalSpacer = new QSpacerItem( 0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum );
    rightLayout_->addItem( horizontalSpacer );

    SetWidgetSpacing( 3 );
    connect( leftWidget_, SIGNAL( sizeHintChanged() ), this, SLOT( UpdateTextMargins() ) );
    connect( rightWidget_, SIGNAL( sizeHintChanged() ), this, SLOT( UpdateTextMargins() ) );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::AddWidget
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::AddWidget( QWidget *widget, WidgetPosition position )
{
    if( widget == 0 )
        return;

    if( isRightToLeft() )
        position = ( position == LeftSide ) ? RightSide : LeftSide;
    if( position == LeftSide )
        leftLayout_->addWidget( widget );
    else
        rightLayout_->insertWidget( 1, widget );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::RemoveWidget
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::RemoveWidget( QWidget *widget )
{
    if( widget == 0 )
        return;

    leftLayout_->removeWidget( widget );
    rightLayout_->removeWidget( widget );
    widget->hide();
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::SetWidgetSpacing
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::SetWidgetSpacing( int spacing )
{
    leftLayout_->setSpacing( spacing );
    rightLayout_->setSpacing( spacing );
    UpdateTextMargins();
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::WidgetSpacing
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
int RichLineEdit::WidgetSpacing() const
{
    return leftLayout_->spacing();
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::TextMargin
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
int RichLineEdit::TextMargin( WidgetPosition position ) const
{
    int spacing = rightLayout_->spacing();
    int width = ( position == LeftSide ) ? leftWidget_->sizeHint().width() : rightWidget_->sizeHint().width();
    if( width == 0 )
        return 0;
    return width + spacing * 2;
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::InactiveText
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
QString RichLineEdit::InactiveText() const
{
    return inactiveText_;
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::SetInactiveText
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::SetInactiveText( const QString &text )
{
    inactiveText_ = text;
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::paintEvent
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::paintEvent( QPaintEvent *event )
{
    QLineEdit::paintEvent( event );
    if( text().isEmpty() && !inactiveText_.isEmpty() && !hasFocus() )
    {
        QStyleOptionFrameV2 panel;
        initStyleOption( &panel );
        QRect textRect = style()->subElementRect( QStyle::SE_LineEditContents, &panel, this );
        int horizontalMargin = 2;
        textRect.adjust( horizontalMargin, 0, 0, 0 );
        int left = TextMargin( RichLineEdit::LeftSide );
        int right = TextMargin( RichLineEdit::RightSide );
        textRect.adjust( left, 0, -right, 0 );
        QPainter painter( this );
        painter.setPen( palette().brush( QPalette::Disabled, QPalette::Text ).color() );
        painter.drawText( textRect, Qt::AlignLeft | Qt::AlignVCenter, inactiveText_ );
    }
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::keyPressEvent
// Created: ABR 2012-03-28
// -----------------------------------------------------------------------------
void RichLineEdit::keyPressEvent( QKeyEvent* event )
{
    if( event && event->key() == Qt::Key_Escape )
        clear();
    QLineEdit::keyPressEvent( event );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::resizeEvent
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::resizeEvent( QResizeEvent *event )
{
    UpdateSideWidgetLocations();
    QLineEdit::resizeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::event
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
bool RichLineEdit::event( QEvent *event )
{
    if( event->type() == QEvent::LayoutDirectionChange )
    {
        if( isRightToLeft() )
        {
            leftLayout_->setDirection( QBoxLayout::RightToLeft );
            rightLayout_->setDirection( QBoxLayout::RightToLeft );
        }
        else
        {
            leftLayout_->setDirection( QBoxLayout::LeftToRight );
            rightLayout_->setDirection( QBoxLayout::LeftToRight );
        }
    }
    return QLineEdit::event( event );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::UpdateTextMargins
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::UpdateTextMargins()
{
    int left = TextMargin( RichLineEdit::LeftSide );
    int right = TextMargin( RichLineEdit::RightSide );
    int top = 0;
    int bottom = 0;
    setTextMargins(left, top, right, bottom);
    UpdateSideWidgetLocations();
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::UpdateSideWidgetLocations
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void RichLineEdit::UpdateSideWidgetLocations()
{
    QStyleOptionFrameV2 opt;
    initStyleOption( &opt );
    QRect textRect = style()->subElementRect( QStyle::SE_LineEditContents, &opt, this );
    int spacing = rightLayout_->spacing();
    textRect.adjust( spacing, 0, -spacing, 0 );

    int left = TextMargin( RichLineEdit::LeftSide );
    int midHeight = textRect.center().y() + 1;

    if( leftLayout_->count() > 0 )
    {
        int leftHeight = midHeight - leftWidget_->height() / 2;
        int leftWidth = leftWidget_->width();
        if (leftWidth == 0)
            leftHeight = midHeight - leftWidget_->sizeHint().height() / 2;
        leftWidget_->move( textRect.x(), leftHeight );
    }
    textRect.setX( left );
    textRect.setY( midHeight - rightWidget_->sizeHint().height() / 2 );
    textRect.setHeight( rightWidget_->sizeHint().height() );
    rightWidget_->setGeometry( textRect );
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::clear
// Created: ABR 2012-01-26
// -----------------------------------------------------------------------------
void RichLineEdit::clear()
{
    QLineEdit::clear();
    if( isReadOnly() )
    {
        setReadOnly( false );
        setPaletteForegroundColor( Qt::black );
    }
}

// -----------------------------------------------------------------------------
// Name: RichLineEdit::SetText
// Created: ABR 2014-02-14
// -----------------------------------------------------------------------------
void RichLineEdit::SetText( const QString& text )
{
    const int pos = cursorPosition();
    setText( text );
    setCursorPosition( pos );
}
