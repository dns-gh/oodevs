// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UnitPreviewIcon.h"
#include "moc_UnitPreviewIcon.cpp"
#include "SymbolIcons.h"
#include "ColorStrategy_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon constructor
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
UnitPreviewIcon::UnitPreviewIcon( QWidget* parent, Controllers& controllers, SymbolIcons& icons, ColorStrategy_ABC& colorStrategy )
    : QHBox          ( parent )
    , controllers_   ( controllers )
    , icons_         ( icons )
    , colorStrategy_ ( colorStrategy )
    , selectedParent_( controllers_ )
{
    setFrameStyle( QFrame::Plain | QFrame::Box );
    setPaletteBackgroundColor( Qt::white );
    layout()->setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
    icon_ = new QLabel( this );
    icon_->setMargin( 10 );
    QToolTip::add( icon_, tr( "Drag and drop symbol to map to create a new unit." ) );
    UpdateSymbol();
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon destructor
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
UnitPreviewIcon::~UnitPreviewIcon()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::NotifySelected
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::NotifySelected( const AgentType& type )
{
    symbol_ = type.GetSymbol();
    level_ = type.GetLevelSymbol();
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::NotifySelected
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::NotifySelected( const AutomatType& type )
{
    symbol_ = type.GetSymbol();
    level_ = type.GetTypePC()->GetLevelSymbol(); // $$$$ SBO 2007-10-16: 
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::BeforeSelection
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::BeforeSelection()
{
    selectedParent_ = 0;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::AfterSelection
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::AfterSelection()
{
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::Select
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::Select( const kernel::Formation_ABC& element )
{
    selectedParent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::Select
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::Select( const kernel::Automat_ABC& element )
{
    selectedParent_ = &element;
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::UpdateSymbol
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::UpdateSymbol()
{
    if( selectedParent_ )
    {
        QImage img;
        SymbolIcon icon( symbol_, level_ );
        icon.SetColor( colorStrategy_.FindColor( *selectedParent_ ) );
        icon.SetSize( 128 );
        img = icons_.GetSymbol( icon );
        if( !img.isNull() )
            icon_->setPixmap( img );
        else
            QTimer::singleShot( 100, this, SLOT( UpdateSymbol() ) );
    }
    else
    {
        QPixmap blank( 128, 128 );
        blank.fill( Qt::white );
        icon_->setPixmap( blank );
    }
}

// -----------------------------------------------------------------------------
// Name: UnitPreviewIcon::mouseMoveEvent
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void UnitPreviewIcon::mouseMoveEvent( QMouseEvent* event )
{
    if( selectedParent_ && ( event->button() | Qt::LeftButton ) )
        emit StartDrag();
}
