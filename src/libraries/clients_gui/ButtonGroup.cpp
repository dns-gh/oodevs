// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ButtonGroup.h"
#include "moc_ButtonGroup.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ButtonGroup constructor
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
ButtonGroup::ButtonGroup ( const QString& objectName, QWidget* parent /* = 0*/ )
    : RichWidget< QGroupBox >( objectName, parent )
    , buttonGroup_( new QButtonGroup( this ) )
    , layout_( new QHBoxLayout( this ) )
{
    buttonGroup_->setObjectName( objectName + "-buttongroup" );
    connect( buttonGroup_, SIGNAL( buttonClicked( int ) ), this, SIGNAL( ButtonClicked( int ) ) );
    connect( buttonGroup_, SIGNAL( buttonClicked( QAbstractButton* ) ), this, SIGNAL( ButtonClicked( QAbstractButton* ) ) );
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup destructor
// Created: RPD 2008-08-21
// -----------------------------------------------------------------------------
ButtonGroup::~ButtonGroup()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup::AddButton
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
void ButtonGroup::AddButton( int id, QAbstractButton* button )
{
    buttonGroup_->addButton( button, id );
    layout_->addWidget( button );
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup::SelectId
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
void ButtonGroup::SetChecked( int id )
{
    if( auto* button = buttonGroup_->button( id ) )
        if( !button->isChecked())
            button->click();
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup::SelectedId
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
int ButtonGroup::CheckedId() const
{
    return buttonGroup_->checkedId();
}

// -----------------------------------------------------------------------------
// Name: ButtonGroup::SelectedButton
// Created: ABR 2014-10-03
// -----------------------------------------------------------------------------
QAbstractButton* ButtonGroup::CheckedButton() const
{
    return buttonGroup_->checkedButton();
}
