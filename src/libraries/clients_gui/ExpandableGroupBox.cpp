// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ExpandableGroupBox.h"
#include "moc_ExpandableGroupBox.cpp"
#include "ImageWrapper.h"
#include "RichPushButton.h"
#include "SubObjectName.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox constructor
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
ExpandableGroupBox::ExpandableGroupBox( const QString& objectName, QWidget* parent /* = 0 */, const QString& title /* = "" */, Qt::Orientation orientation /* = Qt::Vertical */ )
    : QFrame( parent )
    , title_       ( title )
    , orientation_ ( orientation )
    , expandIcon_  ( gui::Icon( "resources/images/gui/toggle-expand.png" ) )
    , collapseIcon_( gui::Icon( "resources/images/gui/toggle-collapse.png" ) )
{
    SubObjectName subObject( objectName );
    titleWidget_ = new QFrame();
    titleWidget_->setFrameStyle( QFrame::StyledPanel );

    titleLabel_ = new QLabel( "<b>" + title_ + "</b>" );
    expandButton_ = new RichPushButton( "expandButton", "" );
    expandButton_->setFixedSize( 20, 20 );
    connect( expandButton_, SIGNAL( clicked() ), this, SLOT( OnCollapseClicked() ) );

    QHBoxLayout* titleLayout = new QHBoxLayout( titleWidget_ );
    titleLayout->setContentsMargins( 5, 2, 5, 2 );
    titleLayout->addWidget( titleLabel_, 1, Qt::AlignLeft );
    titleLayout->addWidget( expandButton_, 0, Qt::AlignRight );

    componentsWidget_ = new QFrame();
    componentsWidget_->setFrameStyle( QFrame::Panel | QFrame::Sunken );
    new QBoxLayout( ( orientation_ == Qt::Vertical ) ? QBoxLayout::TopToBottom : QBoxLayout::LeftToRight, componentsWidget_ );

    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->setMargin( 0 );
    layout->setSpacing( 0 );
    layout->addWidget( titleWidget_ );
    QVBoxLayout* tmpLayout = new QVBoxLayout();
    tmpLayout->setContentsMargins( 2, 0, 2, 0 );
    tmpLayout->setSpacing( 0 );
    tmpLayout->addWidget( componentsWidget_ );
    layout->addLayout( tmpLayout );
    Collapse();
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox destructor
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
ExpandableGroupBox::~ExpandableGroupBox()
{
    components_.clear();
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::SetTitle
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
void ExpandableGroupBox::SetTitle( const QString& title )
{
    title_ = title;
    titleLabel_->setText( title_ );
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::Title
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
const QString& ExpandableGroupBox::Title() const
{
    return title_;
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::AddComponent
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
void ExpandableGroupBox::AddComponent( QWidget* component )
{
    components_.push_back( component );
    componentsWidget_->layout()->addWidget( component );
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::Components
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
QList< QWidget* > ExpandableGroupBox::Components() const
{
    return components_;
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::SetOrientation
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
void ExpandableGroupBox::SetOrientation( Qt::Orientation orientation )
{
    // $$$$ ABR 2012-06-21: Not tested
    if( orientation_ != orientation )
    {
        orientation_ = orientation;
        static_cast< QBoxLayout* >( componentsWidget_->layout() )->setDirection( ( orientation_ == Qt::Vertical ) ? QBoxLayout::TopToBottom : QBoxLayout::LeftToRight );
    }
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::Orientation
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
Qt::Orientation ExpandableGroupBox::Orientation() const
{
    return orientation_;
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::Collapse
// Created: ABR 2012-06-22
// -----------------------------------------------------------------------------
void ExpandableGroupBox::Collapse()
{
    SetExpanded( false );
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::Expand
// Created: ABR 2012-06-22
// -----------------------------------------------------------------------------
void ExpandableGroupBox::Expand()
{
    SetExpanded( true );
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::SetExpanded
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
void ExpandableGroupBox::SetExpanded( bool expanded )
{
    componentsWidget_->setVisible( expanded );
    expandButton_->setIcon( ( !expanded ) ? expandIcon_ : collapseIcon_ );
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::IsExpanded
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
bool ExpandableGroupBox::IsExpanded() const
{
    return componentsWidget_->isVisible();
}

// -----------------------------------------------------------------------------
// Name: ExpandableGroupBox::OnCollapseClicked
// Created: ABR 2012-06-21
// -----------------------------------------------------------------------------
void ExpandableGroupBox::OnCollapseClicked()
{
    SetExpanded( !IsExpanded() );
}

