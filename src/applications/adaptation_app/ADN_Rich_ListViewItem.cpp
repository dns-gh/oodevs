//*****************************************************************************
//
// $Created: JDY 03-07-02 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_ListViewItem.cpp $
// $Author: Ape $
// $Modtime: 28/02/05 11:13 $
// $Revision: 5 $
// $Workfile: ADN_ListViewItem.cpp $
//
//*****************************************************************************
#include "adaptation_app_pch.h"
#include "ADN_Rich_ListViewItem.h"

#include "ADN_ListView.h"

#include <QtGui/qpainter.h>

QColor ADN_Rich_ListViewItem::warningColor_( 255, 185, 125 );
QColor ADN_Rich_ListViewItem::errorColor_  ( 255, 125, 125 );

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem constructor
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
ADN_Rich_ListViewItem::ADN_Rich_ListViewItem( ADN_ListView* parent, bool bGrid )
: Q3ListViewItem       ( parent )
, backgroundColors_   ()
, bGrid_              ( bGrid )
, eSortingConstraint_ ( eSortingConstraint_Default )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem constructor
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
ADN_Rich_ListViewItem::ADN_Rich_ListViewItem( Q3ListViewItem* parent, bool bGrid )
: Q3ListViewItem     ( parent )
, backgroundColors_ ()
, bGrid_            ( bGrid )
, eSortingConstraint_ ( eSortingConstraint_Default )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem destructor
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
ADN_Rich_ListViewItem::~ADN_Rich_ListViewItem()
{
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::SetBackgroundColor
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::SetBackgroundColor( uint nColumn, const QColor& color )
{
    if( nColumn > backgroundColors_.size() )
        backgroundColors_.resize( nColumn + 1 );
    backgroundColors_[ nColumn ] = &color;
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::SetWarning
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::SetWarning( uint nColumn )
{
    SetBackgroundColor( nColumn, warningColor_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::SetError
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::SetError( uint nColumn )
{
    SetBackgroundColor( nColumn, errorColor_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::paintCell
// Created: SBO 2006-01-04
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::paintCell( QPainter* pPainter, const QColorGroup& cg, int nColumn, int nWidth, int nAlign )
{
    if( isSelected() || nColumn >= (int)backgroundColors_.size() || !backgroundColors_[ nColumn ] )
    {
        Q3ListViewItem::paintCell( pPainter, cg, nColumn, nWidth, nAlign );
    }
    else
    {
        QColorGroup cg2( cg );
        cg2.setColor( QColorGroup::Base, *backgroundColors_[ nColumn ] );
        Q3ListViewItem::paintCell( pPainter, cg2, nColumn, nWidth, nAlign );
    }
    if( bGrid_ )
    {
        QPen pen;
        pen.setWidth( 1 );
        pPainter->setPen( pen );
        pPainter->drawLine( 0, 0, nWidth - 1, 0 );
        pPainter->drawLine( 0, 0, nWidth - 1, height() - 1 );
        //pPainter->lineTo( 0, height() - 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::OverrideSorting
// Created: SBO 2006-01-13
// -----------------------------------------------------------------------------
void ADN_Rich_ListViewItem::OverrideSorting( E_SortingConstraint eConstraint )
{
    eSortingConstraint_ = eConstraint;
}

// -----------------------------------------------------------------------------
// Name: ADN_Rich_ListViewItem::key
// Created: SBO 2006-01-13
// -----------------------------------------------------------------------------
QString ADN_Rich_ListViewItem::key( int column, bool /*ascending*/ ) const
{
    if( eSortingConstraint_ == eSortingConstraint_Default )
        return text( column );
    else if( eSortingConstraint_ == eSortingConstraint_First )
        return "  " + text( column );
    else if( eSortingConstraint_ == eSortingConstraint_Last )
        return "zz" + text( column );
    return text( column );
}
