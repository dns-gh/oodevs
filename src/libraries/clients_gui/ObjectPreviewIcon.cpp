// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::ObjectPreviewIcon */

#include "clients_gui_pch.h"
#include "ObjectPreviewIcon.h"
//#include "moc_ObjectPreviewIcon.cpp"
#include "SymbolIcons.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ObjectType.h"
#include "tools/App6Symbol.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: ObjectPreviewIcon constructor
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
ObjectPreviewIcon::ObjectPreviewIcon( QWidget* parent, kernel::Controllers& controllers, SymbolIcons& icons )
    : QHBox        ( parent )    
    , icons_       ( icons )
    , controllers_ ( controllers )    
{
    setFrameStyle( QFrame::Plain | QFrame::Box );
    setPaletteBackgroundColor( Qt::white );
    layout()->setAlignment( Qt::AlignCenter | Qt::AlignHCenter );
    icon_ = new QLabel( this );
    icon_->setMargin( 10 );
    // QToolTip::add( icon_, tr( "Drag and drop symbol to map to create a new unit." ) );
    UpdateSymbol();    
}

// -----------------------------------------------------------------------------
// Name: ObjectPreviewIcon destructor
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
ObjectPreviewIcon::~ObjectPreviewIcon()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectPreviewIcon::NotifySelected
// Created: JCR 2008-08-27
// -----------------------------------------------------------------------------
void ObjectPreviewIcon::NotifySelected( const kernel::ObjectType& type )
{
    symbol_ = type.GetSymbol();    
    UpdateSymbol();
}

// -----------------------------------------------------------------------------
// Name: ObjectPreviewIcon::UpdateSymbol
// Created: SBO 2007-10-16
// -----------------------------------------------------------------------------
void ObjectPreviewIcon::UpdateSymbol()
{
    if( symbol_ != "" )
    {        
        std::string symbol = tools::app6::MergeSymbol( symbol_, symbol_ );
        QImage img;
        SymbolIcon icon( symbol );
        icon.SetColor( Qt::black );
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

