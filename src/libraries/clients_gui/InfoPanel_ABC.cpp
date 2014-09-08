// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "InfoPanel_ABC.h"
#include "PanelStack_ABC.h"
#include "DisplayBuilder.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC constructor
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
InfoPanel_ABC::InfoPanel_ABC( QWidget* parent, PanelStack_ABC& panel, const QString& name, const char* qtName )
    : QScrollArea( parent )
    , stack_     ( panel )
    , name_      ( name )
{
    setName( qtName );
    setWidgetResizable( true );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC destructor
// Created: AGE 2005-04-05
// -----------------------------------------------------------------------------
InfoPanel_ABC::~InfoPanel_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::Show
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanel_ABC::Show()
{
    stack_.Add( this );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::Hide
// Created: AGE 2006-02-17
// -----------------------------------------------------------------------------
void InfoPanel_ABC::Hide()
{
    stack_.Remove( this );
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::IsVisible
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
bool InfoPanel_ABC::IsVisible() const
{
    return isVisible();
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::ResizeModelOnNewContent
// Created: LGY 2012-12-18
// -----------------------------------------------------------------------------
void InfoPanel_ABC::ResizeModelOnNewContent( QStandardItemModel* model, QItemSelectionModel* selectionModel, int wantedSize,
                                             DisplayBuilder& display )
{
    ResizeModelOnNewContent( model, selectionModel, wantedSize );
    // reset display information
    if( wantedSize == 0 )
        display.Clear();
}

// -----------------------------------------------------------------------------
// Name: InfoPanel_ABC::ResizeModelOnNewContent
// Created: NPT 2012-10-23
// -----------------------------------------------------------------------------
void InfoPanel_ABC::ResizeModelOnNewContent( QStandardItemModel* model, QItemSelectionModel* selectionModel, int wantedSize )
{
    bool wasBlocked = selectionModel->blockSignals( true );
    int modelSize = model->rowCount();
    if( modelSize > wantedSize )
        model->removeRows( wantedSize, modelSize - wantedSize );
    else if( modelSize < wantedSize )
        for( int i = 0; i < wantedSize - modelSize; ++i )
        {
            QList< QStandardItem *> list;
            for( int j = 0; j < model->columnCount(); ++j )
                list.append( new QStandardItem() );
            model->appendRow( list );
        }
    selectionModel->blockSignals( wasBlocked );
}
