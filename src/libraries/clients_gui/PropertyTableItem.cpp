// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
    : QTableItem( table, QTableItem::WhenCurrent, "" )
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
    if( editor )
    {
        table()->setRowHeight( row(), std::max( table()->rowHeight( row() ), editor->height() ) );
        table()->updateGeometry();
    }
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
    table()->updateGeometry();
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
