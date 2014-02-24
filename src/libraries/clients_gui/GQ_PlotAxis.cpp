// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: CBX 2003-08-07 $
// $Archive: /Tools/Gui/build/Libraries/GQ_Widgets/src/GQ_PlotAxis.cpp $
// $Author: Cbx $
// $Modtime: 10/01/05 16:02 $
// $Revision: 13 $
// $Workfile: GQ_PlotAxis.cpp $
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GQ_PlotAxis.h"
#include "moc_GQ_PlotAxis.cpp"

#include <vector>
#include <stdexcept>
#include <cassert>
#include <cmath>

using namespace gui;

const unsigned int _defaultTickMultiples   [ 4 ] = {  1, 2, 5, 25  };
const unsigned int _defaultMinutesMultiples[ 6 ] = {  1, 5, 10, 20, 15, 30 };

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis constructor
/** @param  GQ_Plot parent plot widget
    @param  Orientation axis orientation (Qt::Horizontal or Qt::Vertical)
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
GQ_PlotAxis::GQ_PlotAxis( QWidget* pParent, Qt::Orientation nOrientation )
: QObject          ( pParent )
, nOrientation_    ( nOrientation )
, nCaptionMargin_  ( 2 )
, bShowAxis_       ( true )
, bShowGrid_       ( true )
, nMinTicksSpacing_( 20 )
, nTicksSpacing_   ( 0 )
, nTicksLength_    ( 4 )
, nSubTicksLength_ ( 2 )
, nNbrSubTicks_    ( 0 )
, rMinTickPeriod_  (    0.0 )
, rMultiplesBase_  (   10.0 )
, nMinBasePower_   ( -100   )
, tickMultiples_   ( _defaultTickMultiples, _defaultTickMultiples + 4 )
, bShowTickValues_ ( false )
, pValueToString_  ( 0 )
, captionFont_     ( "helvetica", 8, QFont::DemiBold )
, tickValueFont_   ( "helvetica", 6 )
, rMinDataValue_   ( 0.0 )
, rMaxDataValue_   ( 1.0 )
, rMinVirtualValue_( 0.0 )
, rMaxVirtualValue_( 0.0 )
, bFixedRange_     ( false )
, bVirtualRange_   ( false )
, nAxisLength_     ( 100 )
, bUpdateNeeded_   ( true )
, rMinAxisValue_   ( 0.0 )
, rMaxAxisValue_   ( 0.0 )
, rScale_          ( 0.0 )
, rFirstTick_      ( 0.0 )
, rTickPeriod_     ( 0.0 )
, rSubTickPeriod_  ( 0.0 )
, nAxisWidth_      ( 0 )
{
    Update();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis destructor
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
GQ_PlotAxis::~GQ_PlotAxis()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetAxisCaption
/** @param  QString&
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetAxisCaption( const QString& caption )
{
    if( caption == caption_ )
        return;

    caption_ = caption;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetCaptionMargin
/** @param
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetCaptionMargin( unsigned int nMargin )
{
    if( nMargin == nCaptionMargin_ )
        return;

    nCaptionMargin_ = nMargin;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ShowAxis
/** @param  bShow
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotAxis::ShowAxis( bool bShow )
{
    if( bShow == bShowAxis_ )
        return;

    bShowAxis_ = bShow;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ShowGrid
/** @param  bShow
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotAxis::ShowGrid( bool bShow )
{
    if( bShow == bShowGrid_ )
        return;

    bShowGrid_ = bShow;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ShowTicks
/** Toggles display of tick marks with preferred space in pixels between them.

  If @a nTicksSpacing is -1, tick marks are disabled.

  The real space between tick marks will be adjusted according to min spacing, preferred spacing and
  selection of a user readable scale.
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::ShowTicks( unsigned int nTicksSpacing )
{
    if( nTicksSpacing == nTicksSpacing_ )
        return;

    if( nTicksSpacing > 0 && nTicksSpacing < nMinTicksSpacing_ )
        nTicksSpacing = nMinTicksSpacing_;

    nTicksSpacing_ = nTicksSpacing;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetMinTicksSpacing
/** Sets minimum space in pixels allowed between to main tick marks.
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetMinTicksSpacing( unsigned int nMinTicksSpacing )
{
    if( nMinTicksSpacing == nMinTicksSpacing_ )
        return;

    nMinTicksSpacing_ = nMinTicksSpacing;

    if( nTicksSpacing_ > 0 && nTicksSpacing_ < nMinTicksSpacing )
        ShowTicks( nMinTicksSpacing_ );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetTicksLength
/** Sets the length in pixels of main tick marks.
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetTicksLength( unsigned int nLength )
{
    if( nLength == nTicksLength_ )
        return;

    nTicksLength_ = nLength;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetSubTicksLength
/** Sets the length in pixels of sub tick marks.
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetSubTicksLength( unsigned int nLength )
{
    if( nLength == nSubTicksLength_ )
        return;

    nSubTicksLength_ = nLength;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ShowSubTicks
/** @param  nNbrSubTicks
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::ShowSubTicks( unsigned int nNbrSubTicks )
{
    if( nNbrSubTicks == nNbrSubTicks_ )
        return;

    nNbrSubTicks_ = nNbrSubTicks;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ShowTicksValue
/** @param  bShow
    @param  valueFormat
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::ShowTicksValue( bool bShow )
{
    if( bShow == bShowTickValues_ )
        return;

    bShowTickValues_ = bShow;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::GQ_PlotAxis::SetTickMultiples
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetTickMultiples( double rMultiplesBase, const std::vector< unsigned int >& multiples, double rMinTickPeriod, int nMinBasePower )
{
    if( rMultiplesBase == rMultiplesBase_
        && multiples      == tickMultiples_
        && rMinTickPeriod == rMinTickPeriod_
        && nMinBasePower  == nMinBasePower_  )
        return;

    if( rMultiplesBase <= 1.0 )
        throw MASA_EXCEPTION( "GQ_PlotAxis : invalid tick multiples base (should be > 1)." );

    if( multiples.empty() )
        throw MASA_EXCEPTION( "GQ_PlotAxis : invalid tick multiples set (empty)." );

    rMultiplesBase_ = rMultiplesBase;
    nMinBasePower_  = nMinBasePower;
    tickMultiples_  = multiples;
    rMinTickPeriod_ = rMinTickPeriod;

    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::InitTickMultiplesForIntegers
// Created: OT 2004-03-08
// -----------------------------------------------------------------------------
void GQ_PlotAxis::InitTickMultiplesForIntegers()
{
    T_TickMultipleList multiples( _defaultTickMultiples, _defaultTickMultiples + 4 );
    SetTickMultiples( 10.0, multiples, 1.0, 0 );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::InitTickMultiplesForMinutes
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_PlotAxis::InitTickMultiplesForMinutes()
{
    T_TickMultipleList multiples( _defaultMinutesMultiples, _defaultMinutesMultiples + 6 );
    SetTickMultiples( 60.0, multiples, 1.0, 0 );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetValueToStringConvertor
/** @param  T_ValueToString*
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetValueToStringConvertor( const T_ValueToString* pValueToString )
{
    if( pValueToString == pValueToString_ )
        return;

    pValueToString_ = pValueToString;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetCaptionFont
/** @param  QFont&
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetCaptionFont( const QFont& font )
{
    if( font == captionFont_ )
        return;

    captionFont_ = font;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetTickValueFont
/** @param  QFont&
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetTickValueFont( const QFont& font )
{
    if( font == tickValueFont_ )
        return;

    tickValueFont_ = font;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetDataRange
/** @param  rMinDataValue
    @param  rMaxDataValue
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetDataRange( double rMinDataValue, double rMaxDataValue )
{
    if( rMinDataValue == rMinDataValue_ && rMaxDataValue == rMaxDataValue_ )
        return;

    if( rMinDataValue <= rMaxDataValue )
    {
        rMinDataValue_ = rMinDataValue;
        rMaxDataValue_ = rMaxDataValue;
    }
    else
    {
        rMinDataValue_ = rMaxDataValue;
        rMaxDataValue_ = rMinDataValue;
    }

    if( bVirtualRange_ )
    {
        if( rMinVirtualValue_ < rMinDataValue_ )
            rMinDataValue_ = rMinVirtualValue_;

        if( rMaxVirtualValue_ > rMaxDataValue_ )
            rMaxDataValue_ = rMaxVirtualValue_;
    }

    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetAxisRange
/** @param  rMinAxisValue
    @param  rMaxAxisValue
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetAxisRange( double rMinAxisValue, double rMaxAxisValue, bool bFixedRange )
{
    if( rMinAxisValue == rMinAxisValue_ && rMaxAxisValue == rMaxAxisValue_ && bFixedRange == bFixedRange_ )
        return;

    if( rMinAxisValue <= rMaxAxisValue )
    {
        rMinAxisValue_ = rMinAxisValue;
        rMaxAxisValue_ = rMaxAxisValue;

        if( rMaxAxisValue_ == rMinAxisValue_ )
            rMaxAxisValue_ += 1.0;
    }
    else
    {
        rMinAxisValue_ = rMaxAxisValue;
        rMaxAxisValue_ = rMinAxisValue;
    }

    bFixedRange_ = bFixedRange;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ClearVirtualRange
/** Disables virtual range

  @sa AddVirtualValue()
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotAxis::ClearVirtualRange()
{
    bVirtualRange_ = false;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::AddVirtualValue
/** Forces provided value to be included in the axis range.

    Toggles virtual range usage on. Should be used in combination with automatic axis range.
    Won't have any effect if a fixed range has been provided.

    Several virtual values might be added.

    Using example:
    @code

    // Forces Y axis to include 0
    plot.YAxis().AddVirtualValue( 0.0 );

    // Forces X axis to include [-10, 10] interval
    plot.XAxis().AddVirtualValue( -10.0 );
    plot.XAxis().AddVirtualValue(  10.0 );

    @encode

  @sa ClearVirtualRange()
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotAxis::AddVirtualValue( double rValue )
{
    if( !bVirtualRange_ )
    {
        bVirtualRange_ = true;
        rMinVirtualValue_ = rValue;
        rMaxVirtualValue_ = rValue;

        Touch();
        return;
    }

    if( rValue < rMinVirtualValue_ )
    {
        rMinVirtualValue_ = rValue;
        Touch();
    }

    if( rValue > rMaxVirtualValue_ )
    {
        rMaxVirtualValue_ = rValue;
        Touch();
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::SetAxisLength
/** @internal
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::SetAxisLength( unsigned int nLength )
{
    if( nLength == nAxisLength_ || nLength <= 1 )
        return;

    nAxisLength_ = nLength;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::Caption
/** @return
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
const QString& GQ_PlotAxis::Caption() const
{
    return caption_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::UpdateNeeded
/** @return
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
bool GQ_PlotAxis::UpdateNeeded() const
{
    return bUpdateNeeded_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::GetCaptionSize
/** @return
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
QSize GQ_PlotAxis::GetCaptionSize() const
{
    if( caption_.isEmpty() )
        return QSize( 0, 0 );

    QFontMetrics metrics( captionFont_ );

    QSize captionSize = metrics.boundingRect( caption_ ).size();

    if( nOrientation_ == Qt::Horizontal )
        return QSize( captionSize.width(), captionSize.height() + 2 * nCaptionMargin_ );

    return QSize( captionSize.height() + 2 * nCaptionMargin_, captionSize.width() );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ComputeSize
/** @return
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
QSize GQ_PlotAxis::GetAxisSize() const
{
    if( !bShowAxis_ )
        return QSize();

    if( nOrientation_ == Qt::Horizontal )
        return QSize( nAxisLength_, nAxisWidth_ );

    return QSize( nAxisWidth_, nAxisLength_ );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::GetRefTickPos
/** @return
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
int GQ_PlotAxis::GetRefTickPos() const
{
    double rRefPos = rFirstTick_;

    if( rFirstTick_ <= 0.0 && 0.0 <= rMaxAxisValue_ - rTickPeriod_ * 0.5 )
        rRefPos = 0.0;

    int nPos = MapToViewport( rRefPos );
    return nPos;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ClipValue
/** @param  rValue
    @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
double GQ_PlotAxis::ClipValue( double rValue ) const
{
    if( rValue < rMinAxisValue_ )
        return rMinAxisValue_;

    if( rValue > rMaxAxisValue_ )
        return rMaxAxisValue_;

    return rValue;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::MapToViewport
/** @param  rValue
    @return
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
int GQ_PlotAxis::MapToViewport( double rValue ) const
{
    assert( !bUpdateNeeded_ );
    if( bUpdateNeeded_ )
        return -1;

    double rPos = ( rValue - rMinAxisValue_ ) * rScale_;

    if( rPos < 0.0 )
        rPos -= 0.5;
    else
        rPos += 0.5;

    int nPos = ( int )rPos;

    if( nOrientation_ == Qt::Horizontal )
        return nPos;

    return -nPos; // Y axis is upward by default
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::ClipToViewport
/** @param  rValue
    @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
int GQ_PlotAxis::ClipToViewport( double rValue ) const
{
    rValue = ClipValue( rValue );
    return MapToViewport( rValue );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::MapFromViewport
/** @param  nPos
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
double GQ_PlotAxis::MapFromViewport( int nPos ) const
{
    assert( !bUpdateNeeded_ );
    if( bUpdateNeeded_ || rScale_ <= 0.0 )
        return 0.0;

    if( nOrientation_ == Qt::Vertical )
        nPos = -nPos;

    double rValue = rMinAxisValue_ + nPos / rScale_;
    return rValue;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::AxisLength
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
int GQ_PlotAxis::AxisLength() const
{
    return nAxisLength_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::Scale
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
double GQ_PlotAxis::Scale() const
{
    assert( !bUpdateNeeded_ );
    if( bUpdateNeeded_ )
        return 0.0;

    return rScale_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::GetStrValue
/** @param  rValue
    @return
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
QString GQ_PlotAxis::GetStrValue( double rValue ) const
{
    if( pValueToString_ )
        return pValueToString_->GetConversion( rValue );
    return QLocale().toString( rValue );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::Update
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::Update()
{
    if( !bUpdateNeeded_ )
        return;

    // Reset parameters
    bUpdateNeeded_  = false;
    rTickPeriod_    = 0.0;
    rSubTickPeriod_ = 0.0;

    nAxisWidth_ = 0;

    if( !bFixedRange_ )
    {
        rMinAxisValue_ = rMinDataValue_;
        rMaxAxisValue_ = rMaxDataValue_;
        if( rMaxAxisValue_ <= rMinAxisValue_ )
            rMaxAxisValue_ = rMinAxisValue_ + 1.0;
    }

    rFirstTick_ = rMinAxisValue_;

    double rSize = rMaxAxisValue_ - rMinAxisValue_;
    rScale_ = ( double )( nAxisLength_ - 1 ) / rSize;

    if( !bShowAxis_ )
        return;

    nAxisWidth_ += 1; // Axis line width

    if( nTicksSpacing_ == 0 )
        return;

    nAxisWidth_ += nTicksLength_;

//    GQ_LOG( "GQ_PlotAxis::Update()" );

    // Get ideal ticks period
    UpdateTickPeriod();

    // Update axis range and scale to better match tick period
    if( !bFixedRange_ )
    {
        rMinAxisValue_ = rTickPeriod_ * floor( rMinAxisValue_ / rTickPeriod_ );
        rMaxAxisValue_ = rTickPeriod_ * ceil ( rMaxAxisValue_ / rTickPeriod_ );

        rFirstTick_ = rMinAxisValue_;
    }
    else
    {
        rFirstTick_ = rTickPeriod_ * ceil( rMinAxisValue_ / rTickPeriod_ );
    }

    rScale_ = ( double )( nAxisLength_ - 1 )/ ( rMaxAxisValue_ - rMinAxisValue_ );

    rSubTickPeriod_ = rTickPeriod_ / ( nNbrSubTicks_ + 1 );

    // Estimate space needed to display tick values
    if( bShowTickValues_ )
    {
        QFontMetrics metrics( tickValueFont_ );

        if( nOrientation_ == Qt::Horizontal )
        {
            nAxisWidth_ += metrics.height();
        }
        else
        {
            unsigned int nMinValueWidth = metrics.width( GetStrValue( rMinAxisValue_ ) );
            unsigned int nMaxValueWidth = metrics.width( GetStrValue( rMaxAxisValue_ ) );

            if( nMaxValueWidth >= nMinValueWidth )
                nAxisWidth_ += nMaxValueWidth;
            else
                nAxisWidth_ += nMinValueWidth;
        }
    }

    UpdateTicks();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::UpdateTickPeriod
// Created: CBX 2003-08-20
// -----------------------------------------------------------------------------
void GQ_PlotAxis::UpdateTickPeriod()
{
    double rRawTickPeriod  = nTicksSpacing_ / rScale_;
    double rMinDelta       = -1.0;
    double rBaseFactor     = 1.0 / log( rMultiplesBase_ );

    if( rRawTickPeriod < rMinTickPeriod_ )
        rRawTickPeriod = rMinTickPeriod_;

    for( std::size_t i = 0; i < tickMultiples_.size(); ++i )
    {
        unsigned int nMultiple = tickMultiples_[i];

        double rPi  = log( rRawTickPeriod / nMultiple ) * rBaseFactor;
        double rPi0 = floor( rPi + 0.5 );

        if( rPi0 < nMinBasePower_ )
            rPi0 = ( double ) nMinBasePower_;

        double rTickPeriod = pow( rMultiplesBase_, rPi0 ) * nMultiple;

        while( rTickPeriod < rMinTickPeriod_ )
            rTickPeriod *= rMultiplesBase_;

        double rDelta = fabs( ( rTickPeriod / rRawTickPeriod ) - 1.0 );

//        GQ_LOG( "    candidate tick period: " << nMultiple << ", " << rTickPeriod << " / " << rRawTickPeriod << ", r = " << rDelta );
        if( rMinDelta < 0.0 || rDelta < rMinDelta )
        {
            rMinDelta    = rDelta;
            rTickPeriod_ = rTickPeriod;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::Touch
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::Touch()
{
    bUpdateNeeded_ = true;
    emit Touched();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::DrawCaption
/** @param  painter
    @param  colors
    @param  rect
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::DrawCaption( QPainter& painter, const QColorGroup& colors, const QRect& rect )
{
    if( caption_.isEmpty() )
        return;

    painter.setPen ( colors.text() );
    painter.setFont( captionFont_ );

    if( nOrientation_ == Qt::Horizontal )
    {
        painter.drawText( rect, Qt::AlignCenter, caption_ );
        return;
    }

    // Draw vertical caption
    QPoint center = rect.center();
    painter.translate( center.x(), center.y() );
    painter.rotate( -90.0 );

    int nrWidth  = rect.height();
    int nrHeight = rect.width();

    QRect rrect( -nrWidth / 2, -nrHeight / 2, nrWidth, nrHeight );
    painter.drawText( rrect, Qt::AlignCenter, caption_ );

    painter.resetXForm();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::DrawAxis
/** @param  painter
    @param  refPoint
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::DrawAxis( QPainter& painter, const QColorGroup& colors, const QPoint& refPoint )
{
    if( !bShowAxis_ )
        return;

    if( nOrientation_ == Qt::Horizontal )
        DrawX( painter, colors, refPoint );
    else
        DrawY( painter, colors, refPoint );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::DrawX
/** @param  painter
    @param  refPoint
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::DrawX( QPainter& painter, const QColorGroup& colors, const QPoint& refPoint )
{
    int nX0 = refPoint.x();
    int nY0 = refPoint.y();

    int nX1 = nX0 + nAxisLength_ - 1;

    // Draw axis line
    painter.setPen( colors.foreground() );
    painter.drawLine( nX0, nY0, nX1, nY0 );

    // Draw ticks
    if( nTicksSpacing_ == 0 )
        return;

    ++nY0;

    std::size_t nNbrTicks = ticks_.size();

    if( nNbrTicks == 0 )
        return;

    QFontMetrics metrics( tickValueFont_ );
    if( bShowTickValues_ )
        painter.setFont( tickValueFont_ );

    int nLastXstr = -10;
    int nYstr     = nY0 + nTicksLength_ + metrics.height();

    for( std::size_t i = 0; i < nNbrTicks; ++i )
    {
        const T_Tick& tick = ticks_[i];
        double rTick = tick.first;
        int    nX    = tick.second;

        painter.drawLine( nX, nY0, nX, nY0 + nTicksLength_ - 1 );

        for( unsigned int j = 0; j < nNbrSubTicks_; ++j )
        {
            double rSubTick = rTick + rSubTickPeriod_ * ( j + 1 );
            int nXst = nX0 + MapToViewport( rSubTick );
            painter.drawLine( nXst, nY0, nXst, nY0 + nSubTicksLength_ - 1 );
        }

        if( bShowTickValues_ )
        {
            QString strValue( GetStrValue( rTick ) );

            QRect rect = metrics.boundingRect( strValue );
            int nXstart = nX - rect.width() / 2;
            if( nXstart > nLastXstr )
            {
                rect.moveBy( nX - rect.width() / 2, nYstr );
                painter.drawText( rect, Qt::AlignCenter, strValue );

                nLastXstr = rect.right() + 2;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::DrawY
/** @param  painter
    @param  refPoint
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_PlotAxis::DrawY( QPainter& painter, const QColorGroup& colors, const QPoint& refPoint )
{
    int nX0 = refPoint.x();
    int nY0 = refPoint.y();

    int nY1 = nY0 - nAxisLength_ + 1;

    // Draw axis line
    painter.setPen( colors.foreground() );
    painter.drawLine( nX0, nY0, nX0, nY1 );

    // Draw ticks
    if( nTicksSpacing_ == 0 )
        return;

    --nX0;

    std::size_t nNbrTicks = ticks_.size();

    if( nNbrTicks == 0 )
        return;

    QFontMetrics metrics( tickValueFont_ );
    if( bShowTickValues_ )
        painter.setFont( tickValueFont_ );

    int nLastYstr  = nY0 + 10;
    int nXstrRight = nX0 - nTicksLength_;

    for( std::size_t i = 0; i < nNbrTicks; ++i )
    {
        const T_Tick& tick = ticks_[i];
        double rTick = tick.first;
        int    nY    = tick.second;

        painter.drawLine( nX0 - nTicksLength_ + 1, nY, nX0, nY );

        for( unsigned int j = 0; j < nNbrSubTicks_; ++j )
        {
            double rSubTick = rTick + rSubTickPeriod_ * ( j + 1 );

            if( rSubTick > rMaxAxisValue_ )
                break;

            int nYst = nY0 + MapToViewport( rSubTick );
            painter.drawLine( nX0 - nSubTicksLength_ + 1, nYst, nX0, nYst );
        }

        if( bShowTickValues_ )
        {
            QString strValue( GetStrValue( rTick ) );
            strValue = " " + strValue;

            QRect rect = metrics.boundingRect( strValue );
            int nYstart = nY + rect.height() / 2 - 1;
            if( nYstart < nLastYstr )
            {
                rect.moveBy( nXstrRight - rect.right() - 2, nYstart );
                painter.drawText( rect, Qt::AlignVCenter + Qt::AlignRight, strValue );

                nLastYstr = rect.top() + 2;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::UpdateTicks
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotAxis::UpdateTicks()
{
    ticks_.clear();

    if( nTicksSpacing_ == 0 )
        return;

    for( unsigned int i = 0; ; ++i )
    {
        double rTick = rFirstTick_ + rTickPeriod_ * i;
        if( rTick > rMaxAxisValue_ )
            break;

        int nPos = MapToViewport( rTick );

        ticks_.push_back( T_Tick( rTick, nPos ) );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::DrawGrid
/** @param  painter
    @param  colors
    @param  nGridLength
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotAxis::DrawGrid( QPainter& painter, const QColorGroup& colors, const QRect& plotRect )
{
    if( ticks_.empty() )
        return;

    painter.setPen( QPen( colors.midlight(), 0, Qt::DotLine ) );

    if( nOrientation_ == Qt::Horizontal )
        DrawVGrid( painter, 0, - plotRect.height() + 1 );
    else
        DrawHGrid( painter, 0, plotRect.width () - 1 );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::DrawVGrid
/** @param  painter
    @param  colors
    @param  nY0
    @param  nY1
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotAxis::DrawVGrid( QPainter& painter, int nY0, int nY1 )
{
    for( std::size_t i = 0; i < ticks_.size(); ++i )
    {
        int nX = ticks_[i].second;

        painter.drawLine( nX, nY0, nX, nY1 );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotAxis::DrawHGrid
/** @param  painter
    @param  colors
    @param  refPoint
    @param  nLength
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_PlotAxis::DrawHGrid( QPainter& painter, int nX0, int nX1 )
{
    for( std::size_t i = 0; i < ticks_.size(); ++i )
    {
        int nY = ticks_[i].second;

        painter.drawLine( nX0, nY, nX1, nY );
    }
}
