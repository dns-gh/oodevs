// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ColorEditor.h"
#include "moc_ColorEditor.cpp"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/tools.h"
#include "clients_kernel/Color_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_gui/ColorStrategy_ABC.h"
#include "clients_gui/ColorModifier_ABC.h"
#include "clients_gui/ColorEditor_ABC.h"

// -----------------------------------------------------------------------------
// Name: ColorEditor constructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorEditor::ColorEditor( QWidget* parent, kernel::Controllers& controllers,
                          gui::ColorStrategy_ABC& colorStrategy, gui::ColorEditor_ABC& colorEditor )
    : QObject( parent )
    , controllers_  ( controllers )
    , colorStrategy_( colorStrategy )
    , colorEditor_  ( colorEditor )
    , selected_     ( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor destructor
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
ColorEditor::~ColorEditor()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::Show
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorEditor::Show()
{
    QColor current = colorStrategy_.FindColor( *selected_ );
    QColor color = QColorDialog::getColor( current, 0, tools::translate( "ColorEditor", "Select color" ) );
    if( color.isValid() && color != current )
        colorEditor_.Add( *selected_, color );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::Reset
// Created: LGY 2011-06-24
// -----------------------------------------------------------------------------
void ColorEditor::Reset()
{
    colorEditor_.Remove( *selected_ );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2011-06-28
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Team_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2011-06-28
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2011-06-28
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: LGY 2011-06-28
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::NotifyContextMenu
// Created: ABR 2011-10-19
// -----------------------------------------------------------------------------
void ColorEditor::NotifyContextMenu( const kernel::Ghost_ABC& entity, kernel::ContextMenu& menu )
{
    Update( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: ColorEditor::Update
// Created: LGY 2011-06-23
// -----------------------------------------------------------------------------
void ColorEditor::Update( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = const_cast< kernel::Entity_ABC* >( &entity );
    menu.InsertItem( "Color", tools::translate( "ColorEditor", "Change color" ), this, SLOT( Show() ) );
    if( const kernel::Color_ABC* pColor = entity.Retrieve< kernel::Color_ABC>() )
        if( pColor->IsOverride() )
            menu.InsertItem( "Color", tools::translate( "ColorEditor", "Reset color" ), this, SLOT( Reset() ) );
}
