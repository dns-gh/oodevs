// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "DiplomacyCell.h"
#include "moc_DiplomacyCell.cpp"
#include "preparation/Diplomacy.h"

#include <qpainter.h>

// -----------------------------------------------------------------------------
// Name: DiplomacyCell constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
DiplomacyCell::DiplomacyCell( QTable* table, const QStringList& list )
    : QComboTableItem( table, list )
    , valueSet_( false )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: DiplomacyCell destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
DiplomacyCell::~DiplomacyCell()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiplomacyCell::SetColor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void DiplomacyCell::SetColor( const Diplomacy& diplomacy, const QColor& color )
{
    const QString name = diplomacy.GetName();
    colors_[name] = color;
    diplomacies_[name] = diplomacy;
}

// -----------------------------------------------------------------------------
// Name: DiplomacyCell::paint
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void DiplomacyCell::paint( QPainter* p, const QColorGroup& cg, const QRect& cr, bool selected )
{
    QColorGroup newCg( cg );
    newCg.setColor( QColorGroup::Base, colors_[QTableItem::text()] );

    p->setBackgroundColor( colors_[QTableItem::text()] );
    QTableItem::paint( p, newCg, cr, selected );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyCell::GetValue
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
Diplomacy DiplomacyCell::GetValue() const
{
    std::map< const QString, Diplomacy >::const_iterator it = diplomacies_.find( QTableItem::text() );
    if( it != diplomacies_.end() )
        return it->second;
    return Diplomacy::Neutral();
}

// -----------------------------------------------------------------------------
// Name: DiplomacyCell::createEditor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
QWidget* DiplomacyCell::createEditor() const
{
    QWidget* widget = QComboTableItem::createEditor();
    connect( (QComboBox*)widget, SIGNAL( activated( const QString& ) ), this, SLOT( OnTextChanged( const QString& ) ) );
    return widget;
}

// -----------------------------------------------------------------------------
// Name: DiplomacyCell::OnTextChanged
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void DiplomacyCell::OnTextChanged( const QString& str )
{
    valueSet_ = true;
    DiplomacyCell* symetric = static_cast< DiplomacyCell* >( table()->item( col(), row() ) );
    if( symetric && !symetric->valueSet_ )
        table()->setText( col(), row(), str );
}
