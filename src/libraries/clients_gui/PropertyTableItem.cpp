// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertyTableItem.h"
#include "TableItemDisplayer.h"
#include "clients_kernel/Property_ABC.h"
#include "clients_kernel/EditorFactory_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyTableItem constructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
PropertyTableItem::PropertyTableItem( QTable* table, Property_ABC& property, TableItemDisplayer& displayer, EditorFactory_ABC& factory )
    : QTableItem( table, QTableItem::OnTyping, "" )
    , factory_( factory )
    , displayer_( displayer )
    , property_( property )
{
    displayer_.SetItem( this );
    property_.Display( displayer_ );
}
    
// -----------------------------------------------------------------------------
// Name: PropertyTableItem destructor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
PropertyTableItem::~PropertyTableItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyTableItem::createEditor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
QWidget* PropertyTableItem::createEditor() const
{
    QWidget* editor = property_.CreateEditor( table(), factory_ );
    table()->setRowHeight( row(), std::max( table()->rowHeight( row() ), editor->height() ) );
    table()->hide(); // $$$$ SBO 2006-10-31: allow parent table/widget to be resized accordingly
    table()->show(); // $$$$ SBO 2006-10-31: there might be a better way to do so... (or not)
    return editor;
}
    
// -----------------------------------------------------------------------------
// Name: PropertyTableItem::setContentFromEditor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertyTableItem::setContentFromEditor( QWidget* w )
{
    property_.SetValueFromEditor( w );
    displayer_.SetItem( this );
    property_.Display( displayer_ );
    table()->adjustRow( row() );
    table()->hide(); // $$$$ SBO 2006-10-31: allow parent table/widget to be resized accordingly
    table()->show(); // $$$$ SBO 2006-10-31: there might be a better way to do so... (or not)
}

// -----------------------------------------------------------------------------
// Name: PropertyTableItem::paint
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertyTableItem::paint( QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected )
{
    displayer_.SetItem( this );
    property_.Display( displayer_ );
    QTableItem::paint( p, cg, cr, selected );
}
