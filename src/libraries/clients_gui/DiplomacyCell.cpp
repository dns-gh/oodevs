// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DiplomacyCell.h"
#include "moc_DiplomacyCell.cpp"
#include "clients_kernel/Karma.h"

using namespace gui;

namespace
{
    QStringList MakeValueList()
    {
        QStringList list;
        list.append( kernel::Karma::friend_ .GetName() );
        list.append( kernel::Karma::enemy_  .GetName() );
        list.append( kernel::Karma::neutral_.GetName() );
        return list;
    }
}

// -----------------------------------------------------------------------------
// Name: DiplomacyCell constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
DiplomacyCell::DiplomacyCell( Q3Table* table )
    : Q3ComboTableItem( table, MakeValueList() )
    , valueSet_( false )
{
    SetColor( kernel::Karma::friend_ , QColor( 200, 200, 255 ) );
    SetColor( kernel::Karma::enemy_  , QColor( 255, 200, 200 ) );
    SetColor( kernel::Karma::neutral_, QColor( 200, 255, 200 ) );
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
void DiplomacyCell::SetColor( const kernel::Karma& diplomacy, const QColor& color )
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
    newCg.setColor( QColorGroup::Base, colors_[Q3TableItem::text()] );

    p->setBackgroundColor( colors_[Q3TableItem::text()] );
    Q3TableItem::paint( p, newCg, cr, selected );
}

// -----------------------------------------------------------------------------
// Name: DiplomacyCell::GetValue
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
kernel::Karma DiplomacyCell::GetValue() const
{
    std::map< const QString, kernel::Karma >::const_iterator it = diplomacies_.find( Q3TableItem::text() );
    if( it != diplomacies_.end() )
        return it->second;
    return kernel::Karma::neutral_;
}

// -----------------------------------------------------------------------------
// Name: DiplomacyCell::createEditor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
QWidget* DiplomacyCell::createEditor() const
{
    QWidget* widget = Q3ComboTableItem::createEditor();
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
