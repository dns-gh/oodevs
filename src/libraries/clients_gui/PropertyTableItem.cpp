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
PropertyTableItem::PropertyTableItem( Q3Table* table, Property_ABC& property, TableItemDisplayer& displayer, EditorFactory_ABC& factory )
    : Q3TableItem( table, Q3TableItem::WhenCurrent, "" )
    , factory_  ( factory )
    , displayer_( displayer )
    , property_ ( property )
    , color_    ( Qt::white )
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
// Name: PropertyTableItem::alignment
// Created: ABR 2012-04-06
// -----------------------------------------------------------------------------
int PropertyTableItem::alignment() const
{
    return Qt::AlignRight;
}

// -----------------------------------------------------------------------------
// Name: PropertyTableItem::createEditor
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
QWidget* PropertyTableItem::createEditor() const
{
    QWidget* editor = property_.CreateEditor( table(), factory_ );
    if( !editor )
        return 0;
    if( editor->inherits( "QLineEdit" ) )
        static_cast< QLineEdit* >( editor )->setAlignment( Qt::AlignRight );
    else if( editor->inherits( "QAbstractSpinBox" ) )
        static_cast< QAbstractSpinBox* >( editor )->setAlignment( Qt::AlignRight );
    else if( editor->inherits( "QComboBox" ) )
    {
        // Seems to be not possible without setting the combo editable which is laggy and crappy.
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
}

// -----------------------------------------------------------------------------
// Name: PropertyTableItem::paint
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertyTableItem::paint( QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected )
{
    displayer_.SetItem( this );
    property_.Display( displayer_ );

    QColorGroup newCg( cg );
    newCg.setColor( QColorGroup::Base, color_ );
    p->setBackgroundColor( color_ );
    Q3TableItem::paint( p, newCg, cr, selected );
}

// -----------------------------------------------------------------------------
// Name: PropertyTableItem::SetColor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
void PropertyTableItem::SetColor( QColor color )
{
    color_ = color;
}
