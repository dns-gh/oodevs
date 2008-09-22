// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DrawingCategoryItem.h"
#include "moc_DrawingCategoryItem.cpp"
#include "DrawingTemplate.h"
#include "DrawingCategory.h"
#include "clients_kernel/Iterator.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: DrawingCategoryItem constructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingCategoryItem::DrawingCategoryItem( QWidget* parent, const DrawingCategory& category )
    : QVButtonGroup( parent )
{
    setExclusive( true );
    setMargin( 0 );
    setInsideMargin( 0 );
    setInsideSpacing( 0 );
    setBackgroundColor( Qt::white );
    FillStyles( category );
    connect( this, SIGNAL( clicked( int ) ), this, SLOT( OnClicked( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: DrawingCategoryItem destructor
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
DrawingCategoryItem::~DrawingCategoryItem()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DrawingCategoryItem::FillStyles
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingCategoryItem::FillStyles( const DrawingCategory& category )
{
    kernel::Iterator< const DrawingTemplate& > it( category.CreateIterator() );
    while( it.HasMoreElements() )
        AddStyle( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: DrawingCategoryItem::AddStyle
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingCategoryItem::AddStyle( const DrawingTemplate& style )
{
    QPushButton* button = new QPushButton( style.GetPixmap(), style.GetName(), this, style.GetName() );
    button->setToggleButton( true );
    button->setBackgroundColor( Qt::white );
    button->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Preferred );
    button->setFlat( true );
    QToolTip::add( button, style.GetDescription() );

    const int id = insert( button );
    styles_[id] = &style;
}

// -----------------------------------------------------------------------------
// Name: DrawingCategoryItem::OnClicked
// Created: SBO 2008-06-04
// -----------------------------------------------------------------------------
void DrawingCategoryItem::OnClicked( int id )
{
    T_Styles::const_iterator it = styles_.find( id );
    if( it != styles_.end() )
        emit Selected( *it->second );
}
