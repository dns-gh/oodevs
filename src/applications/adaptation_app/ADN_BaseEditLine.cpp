// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_BaseEditLine.h"
#include "moc_ADN_BaseEditLine.cpp"

// -----------------------------------------------------------------------------
// ADN_SideWidget
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_SideWidget constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_SideWidget::ADN_SideWidget( QWidget *parent /*= 0*/ )
    : QWidget( parent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::event
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
bool ADN_SideWidget::event( QEvent *event )
{
    if( event->type() == QEvent::LayoutRequest )
        emit sizeHintChanged();
    return QWidget::event( event );
}


// -----------------------------------------------------------------------------
// ADN_BaseEditLine
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine constructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_BaseEditLine::ADN_BaseEditLine( QWidget* parent /*= 0*/, const char* name /*= 0*/ )
    : QLineEdit( parent, name )
{
    Init();
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::ADN_BaseEditLine_ABC
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_BaseEditLine::ADN_BaseEditLine( const QString& contents, QWidget* parent /*= 0*/, const char* name /*= 0*/ )
    : QLineEdit( contents, parent, name )
{
    Init();
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine destructor
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
ADN_BaseEditLine::~ADN_BaseEditLine()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::Init
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::Init()
{
    leftWidget_ = new ADN_SideWidget( this );
    leftWidget_->resize( 0, 0 );
    leftLayout_ = new QHBoxLayout( leftWidget_ );
    leftLayout_->setContentsMargins( 0, 0, 0, 0 );
    if( isRightToLeft() )
        leftLayout_->setDirection( QBoxLayout::RightToLeft );
    else
        leftLayout_->setDirection( QBoxLayout::LeftToRight );
    leftLayout_->setSizeConstraint( QLayout::SetFixedSize );

    rightWidget_ = new ADN_SideWidget( this );
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
// Name: ADN_BaseEditLine::AddWidget
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::AddWidget( QWidget *widget, WidgetPosition position )
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
// Name: ADN_BaseEditLine::RemoveWidget
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::RemoveWidget( QWidget *widget )
{
    if( widget == 0 )
        return;

    leftLayout_->removeWidget( widget );
    rightLayout_->removeWidget( widget );
    widget->hide();
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::SetWidgetSpacing
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::SetWidgetSpacing( int spacing )
{
    leftLayout_->setSpacing( spacing );
    rightLayout_->setSpacing( spacing );
    UpdateTextMargins();
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::WidgetSpacing
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
int ADN_BaseEditLine::WidgetSpacing() const
{
    return leftLayout_->spacing();
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::TextMargin
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
int ADN_BaseEditLine::TextMargin( WidgetPosition position ) const
{
    int spacing = rightLayout_->spacing();
    int width = ( position == LeftSide ) ? leftWidget_->sizeHint().width() : rightWidget_->sizeHint().width();
    if( width == 0 )
        return 0;
    return width + spacing * 2;
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::InactiveText
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
QString ADN_BaseEditLine::InactiveText() const
{
    return inactiveText_;
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::SetInactiveText
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::SetInactiveText( const QString &text )
{
    inactiveText_ = text;
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::paintEvent
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::paintEvent( QPaintEvent *event )
{
    QLineEdit::paintEvent( event );
    if( text().isEmpty() && !inactiveText_.isEmpty() && !hasFocus() )
    {
        QStyleOptionFrameV2 panel;
        initStyleOption( &panel );
        QRect textRect = style()->subElementRect( QStyle::SE_LineEditContents, &panel, this );
        int horizontalMargin = 2;
        textRect.adjust( horizontalMargin, 0, 0, 0 );
        int left = TextMargin( ADN_BaseEditLine::LeftSide );
        int right = TextMargin( ADN_BaseEditLine::RightSide );
        textRect.adjust( left, 0, -right, 0 );
        QPainter painter( this );
        painter.setPen( palette().brush( QPalette::Disabled, QPalette::Text ).color() );
        painter.drawText( textRect, Qt::AlignLeft | Qt::AlignVCenter, inactiveText_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::resizeEvent
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::resizeEvent( QResizeEvent *event )
{
    UpdateSideWidgetLocations();
    QLineEdit::resizeEvent( event );
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::event
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
bool ADN_BaseEditLine::event( QEvent *event )
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
// Name: ADN_BaseEditLine::UpdateTextMargins
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::UpdateTextMargins()
{
    int left = TextMargin( ADN_BaseEditLine::LeftSide );
    int right = TextMargin( ADN_BaseEditLine::RightSide );
    int top = 0;
    int bottom = 0;
    setTextMargins(left, top, right, bottom);
    UpdateSideWidgetLocations();
}

// -----------------------------------------------------------------------------
// Name: ADN_BaseEditLine::UpdateSideWidgetLocations
// Created: ABR 2012-01-19
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::UpdateSideWidgetLocations()
{
    QStyleOptionFrameV2 opt;
    initStyleOption( &opt );
    QRect textRect = style()->subElementRect( QStyle::SE_LineEditContents, &opt, this );
    int spacing = rightLayout_->spacing();
    textRect.adjust( spacing, 0, -spacing, 0 );

    int left = TextMargin( ADN_BaseEditLine::LeftSide );
    int midHeight = textRect.center().y() + 1;

    if ( leftLayout_->count() > 0 )
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
// Name: ADN_BaseEditLine::clear
// Created: ABR 2012-01-26
// -----------------------------------------------------------------------------
void ADN_BaseEditLine::clear()
{
    QLineEdit::clear();
    if( isReadOnly() )
    {
        setReadOnly( false );
        setPaletteForegroundColor( Qt::black );
    }
}
