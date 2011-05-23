// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************
//
// $Created: CBX 2003-08-07 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_Plot.cpp $
// $Author: Ape $
// $Modtime: 13/01/05 11:27 $
// $Revision: 2 $
// $Workfile: GQ_Plot.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "GQ_Plot.h"
#include "moc_GQ_Plot.cpp"
#include "GQ_PlotToolTip.h"
#include "ADN_DataException.h"
#include <qpainter.h>
#include <qpalette.h>
#include <qstringlist.h>
#include <cassert>

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: GQ_Plot constructor
/** @param  pParent
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
GQ_Plot::GQ_Plot( QWidget* pParent )
: QWidget            ( pParent )
, bShowPlotFrame_   ( true )
, nSelectionMode_   ( eNone )
, x_                ( this, Qt::Horizontal )
, y_                ( this, Qt::Vertical   )
, pCoordToString_   ( 0 )
, pToolTip_         ( 0 )
, layerMap_         ()
, pBackground_      ( new QPixmap()  )
, pPlot_            ( new QPixmap()  )
, nPlotMargin_      ( 20 )
, bBgUpdateNeeded_  ( true )
, bPlotUpdateNeeded_( true )
{
    connect( &x_, SIGNAL( Touched() ), this, SLOT( TouchFrame() ) );
    connect( &y_, SIGNAL( Touched() ), this, SLOT( TouchFrame() ) );

    pToolTip_ = new GQ_PlotToolTip( *this );

    setEraseColor( palette().color( QPalette::Active, QColorGroup::Base ) );

    // Register default layer
    layerMap_.insert( std::make_pair( 0, T_PlotLayer() ) );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot destructor
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
GQ_Plot::~GQ_Plot()
{
    delete pBackground_;
    delete pPlot_;
    delete pToolTip_;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetPlotMargin
/** @param  nMargin
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_Plot::SetPlotMargin( uint nMargin )
{
    if( nMargin == nPlotMargin_ )
        return;

    nPlotMargin_ = nMargin;
    TouchFrame();
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::ShowPlotFrame
/** @param
*/
// Created: CBX 2003-08-20
// -----------------------------------------------------------------------------
void GQ_Plot::ShowPlotFrame( bool bShow )
{
    if( bShow == bShowPlotFrame_ )
        return;

    bShowPlotFrame_ = bShow;
    TouchFrame();
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::ShowToolTips
/** @param
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_Plot::ShowToolTips( bool bShow )
{
    pToolTip_->group()->setEnabled( bShow );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetBackgroundColor
/** @param  QColor&
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_Plot::SetBackgroundColor( const QColor& color )
{
    SetColor( QColorGroup::Background, color );
    SetColor( QColorGroup::Base,       color );
    setEraseColor( color );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetAxisColor
/** @param  QColor&
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_Plot::SetAxisColor( const QColor& color )
{
    SetColor( QColorGroup::Foreground, color );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetFrameColor
/** @param  QColor&
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_Plot::SetFrameColor( const QColor& color )
{
    SetColor( QColorGroup::Light, color );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetGridColor
/** @param  QColor&
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_Plot::SetGridColor( const QColor& color )
{
    SetColor( QColorGroup::Midlight, color );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetCaptionColor
/** @param  QColor&
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_Plot::SetCaptionColor( const QColor& color )
{
    SetColor( QColorGroup::Text, color );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetColor
/** @param  cg
    @param  QColor&
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_Plot::SetColor( QColorGroup::ColorRole role, const QColor& color )
{
    QPalette pal( palette() );
    pal.setColor( QPalette::Active,   role, color );
    pal.setColor( QPalette::Inactive, role, color );
    pal.setColor( QPalette::Disabled, role, color );

    setPalette( pal );

    TouchFrame();
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetCoordToStringConvertor
/** @param  T_CoordToString*
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
void GQ_Plot::SetCoordToStringConvertor( const T_CoordToString* pCoordToString )
{
    if( pCoordToString == pCoordToString_ )
        return;

    pCoordToString_ = pCoordToString;
    TouchFrame();
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::TouchFrame
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_Plot::TouchFrame()
{
    if( !bBgUpdateNeeded_ && !bPlotUpdateNeeded_ )
        update();

    if( !bBgUpdateNeeded_ )
        emit FrameTouched();

    bBgUpdateNeeded_ = true;

    // Force plot update too
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::TouchData
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_Plot::TouchData()
{
    if( !bBgUpdateNeeded_ && !bPlotUpdateNeeded_ )
        update();

    if( !bPlotUpdateNeeded_ )
        emit DataTouched();

    bPlotUpdateNeeded_ = true;
}


// -----------------------------------------------------------------------------
// Name: GQ_Plot::Layers
/** @return
*/
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
const GQ_Plot::T_LayerMap& GQ_Plot::LayerMap() const
{
    return layerMap_;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GetMinDepth
/** @return
*/
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
int GQ_Plot::GetMinDepth() const
{
    if( layerMap_.empty() )
        return 0;

    int nMinDepth = layerMap_.begin()->first;
    return nMinDepth;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GetMaxDepth
/** @return
*/
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
int GQ_Plot::GetMaxDepth() const
{
    if( layerMap_.empty() )
        return 0;

    int nMaxDepth = layerMap_.rbegin()->first;
    return nMaxDepth;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::begin
/** @return
*/
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
GQ_Plot::iterator GQ_Plot::begin() const
{
    for( CIT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        const T_PlotLayer& layer = it->second;
        if( !layer.empty() )
            return iterator( *this, T_DataIndex( it->first, 0 ) );
    }

    return iterator( *this );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::end
/** @return
*/
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
GQ_Plot::iterator  GQ_Plot::end() const
{
    return iterator( *this, T_DataIndex( 0, -1 ) );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GetNextDataIndex
/** @return
*/
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
GQ_Plot::T_DataIndex GQ_Plot::GetNextDataIndex( T_DataIndex dataIndex ) const
{
    if( dataIndex.second == -1 )
        return dataIndex;

    int nCurrentDepth = dataIndex.first;
    int nCurrentIndex = dataIndex.second;

    CIT_LayerMap it = layerMap_.lower_bound( nCurrentDepth );
    if( it == layerMap_.end() )
        return T_DataIndex( 0, -1 );

    if( it->first == nCurrentDepth && nCurrentIndex < ( int )it->second.size() - 1 )
        return T_DataIndex( nCurrentDepth, ++nCurrentIndex );

    ++it;
    for( ; it != layerMap_.end(); ++it )
    {
        const T_PlotLayer& layer = it->second;
        if( !layer.empty() )
            return T_DataIndex( it->first, 0 );
    }

    return T_DataIndex( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GetPlotData
/** @return
*/
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
GQ_PlotData& GQ_Plot::GetPlotData( T_DataIndex dataIndex ) const
{
    if( dataIndex.second == -1 )
        throw ADN_DataException( "GQ_Plot", "invalid iterator" );

    CIT_LayerMap it = layerMap_.find( dataIndex.first );
    if( it == layerMap_.end() || dataIndex.second >= ( int )it->second.size() )
        throw ADN_DataException( "GQ_Plot", "invalid iterator" );

    const T_PlotLayer& layer = it->second;
    GQ_PlotData* pData = layer[ dataIndex.second ];
    return *pData;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::paintEvent
/** @param
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_Plot::paintEvent( QPaintEvent* )
{
    QPainter painter( this );

    const QColorGroup* pColors = 0;
    if( !isEnabled() )
        pColors = &palette().disabled();
    else if( hasFocus() )
        pColors = &palette().active();
    else
        pColors = &palette().inactive();

    Draw( painter, *pColors );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::Draw
/** @param  pPainter
    @param  clipRect
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_Plot::Draw( QPainter& painter, const QColorGroup& colors )
{
    UpdateDataRange();
    UpdateBackground( colors );
    UpdateDataPlot  ( colors );

    painter.drawPixmap( 0, 0, *pPlot_ );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::UpdateDataRange
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_Plot::UpdateDataRange()
{
    if( !bBgUpdateNeeded_ )
        return;

    GQ_PlotDataBBox bbox;

    for( CIT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        const T_PlotLayer& layer = it->second;
        UpdateLayerBBox( layer, bbox );
    }

    if( !bbox_.SetBBox( bbox ) )
        return; // Same bbox

    if( !bbox_.IsEmpty() )
    {
        x_.SetDataRange( bbox.XMin(), bbox.XMax() );
        y_.SetDataRange( bbox.YMin(), bbox.YMax() );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::UpdateLayerBBox
/** @param
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_Plot::UpdateLayerBBox( const T_PlotLayer& layer, GQ_PlotDataBBox& bbox )
{
    std::size_t nNbrPlots = layer.size();
    for( std::size_t i = 0; i < nNbrPlots; ++i )
    {
        const GQ_PlotData* pData = layer[i];
        assert( pData );

        bbox.UpdateWithBBox( pData->BBox() );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::UpdateBackground
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_Plot::UpdateBackground( const QColorGroup& colors )
{
    QSize plotSize = size();

    bool bUpdateBackground = ( bBgUpdateNeeded_ || x_.UpdateNeeded() || y_.UpdateNeeded() );
    if( pBackground_->size() != plotSize )
    {
        pBackground_->resize( plotSize );

        bUpdateBackground = true;
    }

    if( !bUpdateBackground )
        return;

    uint nXCaptionSize = x_.GetCaptionSize().height();
    uint nYCaptionSize = y_.GetCaptionSize().width ();

    uint nLeftMargin   = std::max( nPlotMargin_, nYCaptionSize );
    uint nBottomMargin = std::max( nPlotMargin_, nXCaptionSize );

    uint nTotalXMargin = nPlotMargin_ + nLeftMargin;
    uint nTotalYMargin = nPlotMargin_ + nBottomMargin;

    uint nPlotWidth  = plotSize.width() - nTotalXMargin;
    uint nPlotHeight = plotSize.height() - nTotalYMargin;

    plotRect_ = QRect( nLeftMargin, nPlotMargin_, nPlotWidth, nPlotHeight );

    x_.SetAxisLength( nPlotWidth );
    y_.SetAxisLength( nPlotHeight );

    x_.Update();
    y_.Update();

    // Adjust margins to ensure axes visibility
    int nYAxisWidth = y_.GetAxisSize().width();
    int nYAxisPos   = x_.GetRefTickPos();

    int nYAxisMargin = nYAxisWidth - nYAxisPos;

    if( nYAxisMargin > 0 )
    {
        nLeftMargin   += nYAxisMargin;
        nTotalXMargin += nYAxisMargin;
        plotRect_.setLeft( nLeftMargin );

        nPlotWidth = plotRect_.width();

        x_.SetAxisLength( nPlotWidth );
        x_.Update();
        y_.Update();
    }

    int nXAxisWidth = x_.GetAxisSize().height();
    int nXAxisPos   = y_.GetRefTickPos();

    int nXAxisMargin = nXAxisWidth + nXAxisPos;

    if( nXAxisMargin > 0 )
    {
        nBottomMargin += nXAxisMargin;
        nTotalYMargin += nXAxisMargin;
        plotRect_.setBottom( plotRect_.bottom() - nXAxisMargin );

        nPlotHeight = plotRect_.height();

        y_.SetAxisLength( nPlotHeight );
        y_.Update();
    }

    // Reset background pixmap
    pBackground_->fill( colors.base() );

    QPainter painter( pBackground_ );

    if( bShowPlotFrame_ )
    {
        painter.setPen( colors.light() );
        painter.drawRect( plotRect_ );
    }

    // Draw captions
    if( nYCaptionSize > 0 )
    {
        QRect rect( 0, 0, nYCaptionSize, plotRect_.height() );
        y_.DrawCaption( painter, colors, rect );
    }

    if( nXCaptionSize > 0 )
    {
        QRect rect( plotRect_.x(), height() - nXCaptionSize, plotRect_.width(), nXCaptionSize );
        x_.DrawCaption( painter, colors, rect );
    }

    CenterOnPlotRect( painter );
    x_.DrawGrid( painter, colors, plotRect_ );
    y_.DrawGrid( painter, colors, plotRect_ );

    bBgUpdateNeeded_ = false;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::UpdateDataPlot
/** @param  QColorGroup&
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
void GQ_Plot::UpdateDataPlot( const QColorGroup& colors )
{
    if( !bPlotUpdateNeeded_ )
        return;

    *pPlot_ = *pBackground_;

    QPainter painter( pPlot_ );

    painter.setClipRect( plotRect_ );
    CenterOnPlotRect( painter );

    int nLastDepth = -1;

    for( IT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        int nDepth = it->first;
        T_PlotLayer& layer = it->second;

        if( nDepth >= 0 && nLastDepth < 0 )
        {
            // Draw axes before layer 0

            painter.setClipping( false );

            x_.DrawAxis( painter, colors, QPoint( 0, y_.GetRefTickPos() ) );
            y_.DrawAxis( painter, colors, QPoint( x_.GetRefTickPos(), 0 ) );

            painter.setClipping( true );
        }

        std::size_t nNbrPlots = layer.size();
        for( std::size_t i = 0; i < nNbrPlots; ++i )
        {
            GQ_PlotData* pData = layer[i];
            assert( pData );

            pData->Draw( painter );
        }

        nLastDepth = nDepth;
    }

    bPlotUpdateNeeded_ = false;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::CenterOnPlotRect
/** @param
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
void GQ_Plot::CenterOnPlotRect( QPainter& painter )
{
    painter.translate  ( plotRect_.left(), plotRect_.bottom() );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::RegisterPlotData
/** @param  nDepth
    @param  GQ_PlotData&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_Plot::RegisterPlotData( GQ_PlotData& data, int nDepth )
{
    IT_LayerMap it = layerMap_.find( nDepth );
    if( it == layerMap_.end() )
        it = layerMap_.insert( std::make_pair( nDepth, T_PlotLayer() ) ).first;

    T_PlotLayer& layer = it->second;
    layer.push_back( &data );

    connect( &data, SIGNAL( RangeTouched() ), this, SLOT( TouchFrame() ) );
    connect( &data, SIGNAL( DataTouched () ), this, SLOT( TouchData () ) );

    TouchFrame();
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::UnregisterPlotData
/** @param  data
    @param  bDestroy
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
bool GQ_Plot::UnregisterPlotData( GQ_PlotData& data, bool bDestroy )
{
    for( IT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        T_PlotLayer& layer = it->second;

        T_PlotLayer::iterator dataIt = std::find( layer.begin(), layer.end(), &data );
        if( dataIt != layer.end() )
        {
            layer.erase( dataIt );
            if( bDestroy )
                delete &data;

            TouchData();
            return true;
        }
    }

    return false;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::UnregisterAllPlotData
/** @param  bDestroy
*/
// Created: APE 2005-01-13
// -----------------------------------------------------------------------------
void GQ_Plot::UnregisterAllPlotData( bool bDestroy )
{
    for( IT_LayerMap itLayer = layerMap_.begin(); itLayer != layerMap_.end(); ++itLayer )
    {
        T_PlotLayer& layer = itLayer->second;

        while( ! layer.empty() )
        {
            GQ_PlotData* pData = layer.back();
            assert( pData );
            layer.pop_back();
            if( bDestroy )
                delete pData;
        }
    }
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetLayerVisible
/** @param  nDepth
    @param  bVisible
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_Plot::SetLayerVisible( bool bVisible, int nDepth )
{
    IT_LayerMap it = layerMap_.find( nDepth );
    if( it == layerMap_.end() )
        return;

    T_PlotLayer& layer = it->second;
    std::size_t nNbrPlots = layer.size();
    for( std::size_t i = 0; i < nNbrPlots; ++i )
    {
        GQ_PlotData* pData = layer[i];
        assert( pData );
        pData->SetVisible( bVisible );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetAllVisible
/** @param  bVisible
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_Plot::SetAllVisible( bool bVisible )
{
    for( IT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        int nDepth = it->first;
        SetLayerVisible( bVisible, nDepth );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::ClearLayerData
/** @param  nDepth
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_Plot::ClearLayerData( int nDepth )
{
    IT_LayerMap it = layerMap_.find( nDepth );
    if( it == layerMap_.end() )
        return;

    T_PlotLayer& layer = it->second;
    std::size_t nNbrPlots = layer.size();
    for( std::size_t i = 0; i < nNbrPlots; ++i )
    {
        GQ_PlotData* pData = layer[i];
        assert( pData );
        pData->ClearData();
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::ClearAllData
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_Plot::ClearAllData()
{
    for( IT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        int nDepth = it->first;
        ClearLayerData( nDepth );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GetPlotData
/** @return plot data at given index @a nPlotIndex in layer of depth @a nDepth
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
GQ_PlotData* GQ_Plot::GetPlotData( uint nPlotIndex, int nDepth ) const
{
    CIT_LayerMap it = layerMap_.find( nDepth );
    if( it == layerMap_.end() )
        return 0;

    const T_PlotLayer& layer = it->second;
    if( nPlotIndex >= layer.size() )
        return 0;

    return layer[ nPlotIndex ];
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::FindPlotData
/** @param  nUserID
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
GQ_PlotData* GQ_Plot::FindPlotData( uint nUserID ) const
{
    for( CIT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        const T_PlotLayer& layer = it->second;

        std::size_t nNbrPlots = layer.size();
        for( std::size_t i = 0; i < nNbrPlots; ++i )
        {
            GQ_PlotData* pData = layer[i];
            assert( pData );

            if( pData->UserID() == nUserID )
                return pData;
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::FindPlotData
/** @param  name
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
GQ_PlotData* GQ_Plot::FindPlotData( const QString& name ) const
{
    for( CIT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        const T_PlotLayer& layer = it->second;

        std::size_t nNbrPlots = layer.size();
        for( std::size_t i = 0; i < nNbrPlots; ++i )
        {
            GQ_PlotData* pData = layer[i];
            assert( pData );

            if( pData->Name() == name )
                return pData;
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::MapToViewport
/** @param  T_Point&
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
QPoint GQ_Plot::MapToViewport( const T_Point& point ) const
{
    return MapToViewport( point.first, point.second );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GQ_Plot::MapToViewport
/** @param  rX
    @param  rY
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
QPoint GQ_Plot::MapToViewport( double rX, double rY ) const
{
    int nX = x_.MapToViewport( rX );
    int nY = x_.MapToViewport( rY );

    return QPoint( nX, nY );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::MapFromViewport
/** @param  point
    @return
*/
// Created: APE 2004-12-17
// -----------------------------------------------------------------------------
GQ_Plot::T_Point GQ_Plot::MapFromViewport( const QPoint& point ) const
{
    double rX = x_.MapFromViewport( point.x() );
    double rY = x_.MapFromViewport( point.y() );

    return T_Point( rX, rY );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GetStrCoordinates
/** @param  T_Point&
    @return
*/
// Created: CBX 2003-08-19
// -----------------------------------------------------------------------------
QString GQ_Plot::GetStrCoordinates( const T_Point& point ) const
{
    if( pCoordToString_ )
        return pCoordToString_->GetConversion( point );

    QString strCoords = QString( "(%1, %2)" ).arg( x_.GetStrValue( point.first  ) )
                                             .arg( y_.GetStrValue( point.second ) );

    return strCoords;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GetToolTips
/** @param  QPoint&
    @param  QStringList&
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
bool GQ_Plot::GetToolTips( const QPoint& refPoint, QRect& tipRect, QStringList& tipList ) const
{
    if( bBgUpdateNeeded_ )
        return false;

    int nX = refPoint.x() - plotRect_.left  ();
    int nY = refPoint.y() - plotRect_.bottom();

    double rX = x_.MapFromViewport( nX );
    double rY = y_.MapFromViewport( nY );

    double rRadius = 2.0;

    double rdx = rRadius / x_.Scale();
    double rdy = rRadius / y_.Scale();

    GQ_PlotDataBBox bbox;
    T_Point blCorner( rX - rdx, rY - rdy );
    T_Point urCorner( rX + rdx, rY + rdy );
    bbox.UpdateWithPoint( blCorner );
    bbox.UpdateWithPoint( urCorner );

    tipRect = QRect( refPoint, QSize( 1 + ( int )( 2.0 * rRadius ), 1 + ( int )( 2.0 * rRadius ) ) );
    tipRect.moveBy( -tipRect.width() / 2, -tipRect.height() / 2 );

    bool bToolTipFound = false;

    for( CIT_LayerMap it = layerMap_.begin(); it != layerMap_.end(); ++it )
    {
        const T_PlotLayer& layer = it->second;

        std::size_t nNbrPlots = layer.size();
        for( std::size_t i = 0; i < nNbrPlots; ++i )
        {
            GQ_PlotData* pData = layer[i];
            assert( pData );

            bToolTipFound |= pData->GetToolTips( bbox, tipList );
        }
    }

    return bToolTipFound;
}


// -----------------------------------------------------------------------------
// Name: GQ_Plot::SetSelectionMode
// Created: APE 2004-12-17
// -----------------------------------------------------------------------------
void GQ_Plot::SetSelectionMode( E_SelectionMode nMode )
{
    nSelectionMode_ = nMode;

    //$$$$ Clear current selection.
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::GetSelection
/** @param  selection
*/
// Created: APE 2004-12-17
// -----------------------------------------------------------------------------
void GQ_Plot::GetSelection( T_DataItemVector& selection )
{
    iterator it(this);
    for( it = this->begin(); it != this->end(); ++it )
        (*it).GetSelection( selection );
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::mouseReleaseEvent
// Created: APE 2004-12-17
// -----------------------------------------------------------------------------
void GQ_Plot::mouseReleaseEvent( QMouseEvent* pEvent )
{
    if( nSelectionMode_ == eNone )
        return;

    QPoint qtPlotPoint = pEvent->pos() - plotRect_.bottomLeft();
    T_Point plotPoint = this->MapFromViewport( qtPlotPoint );

    // Save the old selection.
    T_DataItemVector oldSelection;
    this->GetSelection( oldSelection );

    iterator it( this );

    // Clear the current selection.
    for( it = this->begin(); it != this->end(); ++it )
        (*it).SetSelected( false );

    for( it = this->begin(); it != this->end(); ++it )
    {
        // No selection on invisible items.
        if( ! (*it).IsVisible() )
            continue;

        int nDataIndex = (*it).DataAt( plotPoint );
        if( nDataIndex != -1 )
        {
            (*it).SetSelected( nDataIndex, true );
            break;
        }
    }

    T_DataItemVector newSelection;
    this->GetSelection( newSelection );

    // Prevent empty selections.
    if( newSelection.empty() )
    {
        for( it = this->begin(); it != this->end(); ++it )
        {
            (*it).SetSelection( oldSelection );
        }
        return;
    }

    if( newSelection != oldSelection )
    {
        this->TouchData();
        emit SelectionChanged( newSelection );
    }
    //$$$$
}