// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertiesTable.h"
#include "moc_PropertiesTable.cpp"
#include "TableItemDisplayer.h"
#include "PropertyTableItem.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PropertiesTable constructor
// Created: SBO 2006-10-17
// -----------------------------------------------------------------------------
PropertiesTable::PropertiesTable( QWidget* parent, kernel::EditorFactory_ABC& factory, TableItemDisplayer& displayer )
    : Q3Table( parent )
    , factory_( factory )
    , itemDisplayer_( displayer )
    , row_( 0 )
{
    setSelectionMode( Q3Table::SingleRow );
    setNumCols( 2 );
    setFixedHeight( 0 );
    setColumnReadOnly( 0, true );
    verticalHeader()->hide();
    setLeftMargin( 0 );
    horizontalHeader()->hide();
    setTopMargin( 0 );
    setColumnStretchable( 0, true );
    setColumnStretchable( 1, true );
    setHScrollBarMode( Q3ScrollView::AlwaysOff );
    setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
    setFrameStyle( QFrame::StyledPanel );
    setResizePolicy( Q3ScrollView::AutoOne );

    connect( this, SIGNAL( valueChanged( int, int ) ), this, SLOT( OnValueChanged( int, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable destructor
// Created: SBO 2006-10-17
// -----------------------------------------------------------------------------
PropertiesTable::~PropertiesTable()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::sizeHint
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
QSize PropertiesTable::sizeHint() const
{
    QSize original = Q3Table::sizeHint();
    if( numRows() > 0 )
        return QSize( original.width(), contentsHeight () + 4 );
    return QSize( original.width(), 0 );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::Clear
// Created: AGE 2006-10-26
// -----------------------------------------------------------------------------
void PropertiesTable::Clear()
{
    rows_.clear();
    setNumRows( 0 );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::Hide
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::Hide()
{
    hide(); // $$$$ SBO 2006-10-18:
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::Show
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
void PropertiesTable::Show( bool b )
{
    setShown( b && numRows() > 0 );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::OnValueChanged
// Created: SBO 2006-10-30
// -----------------------------------------------------------------------------
void PropertiesTable::OnValueChanged( int row, int col )
{
    setCellContentFromEditor( row, col );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::SubItem
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
Displayer_ABC& PropertiesTable::SubItem( const QString& name )
{
    show();
    CIT_PropertyRows it = rows_.find( name );
    row_ = 0;
    if( it == rows_.end() )
    {
        row_ = numRows();
        insertRows( row_ );
        rows_[name] = row_;
        setText( row_, 0, name );
        setFixedHeight( contentsHeight () + 4 );
        parentWidget()->adjustSize();
    }
    else
        row_ = it->second;
    return *this;
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::StartDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::StartDisplay()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::DisplayFormatted
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::DisplayFormatted( const QString& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::EndDisplay
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::EndDisplay()
{
    clearSelection();
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::Call
// Created: SBO 2006-10-18
// -----------------------------------------------------------------------------
void PropertiesTable::Call( kernel::Property_ABC* const& property )
{
    clearSelection();
    setItem( row_, 1, new PropertyTableItem( this, *property, itemDisplayer_, factory_ ) );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::focusOutEvent
// Created: SBO 2006-10-20
// -----------------------------------------------------------------------------
void PropertiesTable::focusOutEvent( QFocusEvent* event )
{
    clearSelection();
    Q3Table::focusOutEvent( event );
}

// -----------------------------------------------------------------------------
// Name: PropertiesTable::Update
// Created: AME 2010-03-10
// -----------------------------------------------------------------------------
void PropertiesTable::Update( const QString& name )
{
    CIT_PropertyRows it = rows_.find( name );
    if( it != rows_.end() )
        updateCell( it->second, 1 );
}
