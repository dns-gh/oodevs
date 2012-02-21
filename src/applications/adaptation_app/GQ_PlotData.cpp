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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_PlotData.cpp $
// $Author: Ape $
// $Modtime: 12/04/05 16:39 $
// $Revision: 2 $
// $Workfile: GQ_PlotData.cpp $
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "GQ_PlotData.h"

#include "moc_GQ_PlotData.cpp"

#include "GQ_Plot.h"

#include <cassert>

// -----------------------------------------------------------------------------
// Name: GQ_PlotData constructor
/** @param  GQ_Plot&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
GQ_PlotData::GQ_PlotData( uint nUserID, GQ_Plot& plot )
: QObject         ( &plot )
, plot_           ( plot  )
, bOwnData_       ( true  )
, nUserID_        ( nUserID )
, name_           ()
, bVisible_       ( true )
, pointPen_       ( Qt::red   )
, linePen_        ( Qt::blue  )
, barPen_         ( Qt::black )
, barBrush_       ( Qt::green )
, nPointShapeType_( eDot )
, pointShape_     ()
, bPolylineShape_ ( true )
, bDrawBars_      ( false )
, rBarWidth_      ( -1 )
, nBarAlignment_  ( eAlign_Left  )
, pData_          ( new T_Data() )
, nFirstPoint_    ( 0 )
, nNbrPoints_     ( -1 )
, bbox_           ()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData constructor
/** @param  GQ_Plot&
    @param  T_Data&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
GQ_PlotData::GQ_PlotData( uint nUserID, GQ_Plot& plot, T_Data& data, uint nFirstPoint, int nNbrPoints )
: QObject         ( &plot )
, plot_           ( plot  )
, bOwnData_       ( false )
, nUserID_        ( nUserID )
, name_           ()
, bVisible_       ( true )
, pointPen_       ( Qt::red   )
, linePen_        ( Qt::blue  )
, barPen_         ( Qt::black )
, barBrush_       ( Qt::green )
, nPointShapeType_( eDot )
, pointShape_     ()
, bPolylineShape_ ( true )
, bDrawBars_      ( false )
, rBarWidth_      ( -1 )
, pData_          ( &data )
, nFirstPoint_    ( nFirstPoint )
, nNbrPoints_     ( nNbrPoints  )
, bbox_           ()
{
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData destructor
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
GQ_PlotData::~GQ_PlotData()
{
    if( bOwnData_ )
        delete pData_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetUserID
/** @param
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetUserID( uint nID )
{
    nUserID_ = nID;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetName
/** @param  QString&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetName( const QString& name )
{
    if( name == name_ )
        return;

    name_ = name;
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetVisible
/** @param  bShow
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetVisible( bool bShow )
{
    if( bShow == bVisible_ )
        return;

    bVisible_ = bShow;

    TouchRange();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetPointPen
/** @param  QPen&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetPointPen( const QPen& pen )
{
    if( pen == pointPen_ )
        return;

    pointPen_ = pen;
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetLinePen
/** @param  QPen&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetLinePen( const QPen& pen )
{
    if( pen == linePen_ )
        return;

    linePen_ = pen;
    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetBarStyle
/** @param  pen
    @param  brush
    @param  bBarsEnabled
    @param  rWidth
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetBarStyle( const QPen& pen, const QBrush& brush, bool bDrawBars, double rWidth, E_Alignment nBarAlignment )
{
    if( pen == barPen_ && brush == barBrush_ && bDrawBars == bDrawBars_ && rWidth == rBarWidth_ && nBarAlignment == nBarAlignment_ )
        return;

    barPen_        = pen;
    barBrush_      = brush;
    bDrawBars_     = bDrawBars;
    rBarWidth_     = rWidth;
    nBarAlignment_ = nBarAlignment;

    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetPointShape
/** @param  nPredefinedShape
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetPointShape( E_PointShapeType nShapeType, uint nShapeSize )
{
    if( nShapeType == nPointShapeType_ )
        return;

    assert( nShapeType < eUserShape );

    nPointShapeType_ = nShapeType;
    InitPointShape( nShapeSize );

    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetPointShape
/** @param  nUserShapeType
    @param  shape
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetPointShape( E_PointShapeType nShapeType, const Q3PointArray& shape )
{
    if( nShapeType == nPointShapeType_ && shape == pointShape_ )
        return;

    assert( nShapeType >= eUserShape );

    nPointShapeType_ = nShapeType;
    pointShape_      = shape;
    pointShape_.detach(); // Perform deep copy to remove link to original shape

    bPolylineShape_  = ( nPointShapeType_ == eUserShape );

    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::InitPointShape
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::InitPointShape( uint nShapeSize )
{
    pointShape_.resize( 0 );

    int w = nShapeSize;                          // Shape size (along axis)
    int d = ( int )( 1.414 * w + 0.5 ); // Shape size (along diagonal)
    int W = 2 * w + 1;

    bPolylineShape_ = true;

    switch( nPointShapeType_ )
    {
      case eCircle:
          pointShape_.makeEllipse( -w, -w, W, W );
          break;
      case eSquare:
          pointShape_.resize( 5 );
          pointShape_.setPoint( 0, -w, -w );
          pointShape_.setPoint( 1,  w, -w );
          pointShape_.setPoint( 2,  w,  w );
          pointShape_.setPoint( 3, -w,  w );
          pointShape_.setPoint( 4, -w,  -w );
          break;
      case eDiagSquare:
          pointShape_.resize( 5 );
          pointShape_.setPoint( 0,  0, -d );
          pointShape_.setPoint( 1,  d,  0 );
          pointShape_.setPoint( 2,  0,  d );
          pointShape_.setPoint( 3, -d,  0 );
          pointShape_.setPoint( 4,  0, -d );
          break;
      case eUTriangle:
          pointShape_.resize( 4 );
          pointShape_.setPoint( 0, -w, -d );
          pointShape_.setPoint( 1,  w, -d );
          pointShape_.setPoint( 2,  0,  w );
          pointShape_.setPoint( 3, -w, -d );
          break;
      case eDTriangle:
          pointShape_.resize( 4 );
          pointShape_.setPoint( 0, -w,  d );
          pointShape_.setPoint( 1,  w,  d );
          pointShape_.setPoint( 2,  0, -w );
          pointShape_.setPoint( 3, -w,  d );
          break;
      case eCross:
          pointShape_.resize( 4 );
          pointShape_.setPoint( 0, -w, 0 );
          pointShape_.setPoint( 1,  w, 0 );
          pointShape_.setPoint( 2,  0, -w );
          pointShape_.setPoint( 3,  0,  w );
          bPolylineShape_ = false;
          break;
      case eDiagCross:
          pointShape_.resize( 4 );
          pointShape_.setPoint( 0, -w, -w );
          pointShape_.setPoint( 1,  w,  w );
          pointShape_.setPoint( 2,  w, -w );
          pointShape_.setPoint( 3, -w,  w );
          bPolylineShape_ = false;
          break;
      case eVLine:
          pointShape_.resize( 2 );
          pointShape_.setPoint( 1,  0, -w );
          pointShape_.setPoint( 2,  0,  w );
          bPolylineShape_ = false;
          break;
      case eBDiag:
          pointShape_.resize( 2 );
          pointShape_.setPoint( 1,  -w, -w );
          pointShape_.setPoint( 2,   w,  w );
          bPolylineShape_ = false;
          break;
      case eFDiag:
          pointShape_.resize( 2 );
          pointShape_.setPoint( 1,  w, -w );
          pointShape_.setPoint( 2, -w,  w );
          bPolylineShape_ = false;
          break;

      default:
          break;
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::ClearData
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::ClearData()
{
    if( pData_->empty() )
        return;

    // Clear the selection.
    bSelected_ = false;
    selectedIndexes_.clear();

    pData_->clear();
    barLimits_.clear();

    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetSharedData
/** @param  data
    @param  nFirstPoint
    @param  nNbrPoints
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetSharedData( T_Data& data, uint nFirstPoint, int nNbrPoints )
{
    if( &data != pData_ )
    {
        if( bOwnData_ )
            delete pData_;

        bOwnData_ = false;
        pData_ = &data;

        // Clear the selection.
        bSelected_ = false;
        selectedIndexes_.clear();

        TouchData();
    }

    SetDataRange( nFirstPoint, nNbrPoints );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetData
/** @param  data
    @param  nFirstPoint
    @param  nNbrPoints
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetData( const T_Data& data, uint nFirstPoint, int nNbrPoints )
{
    *pData_ = data;

    // Clear the selection.
    bSelected_ = false;
    selectedIndexes_.clear();

    TouchData();

    SetDataRange( nFirstPoint, nNbrPoints );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetDataRange
/** @param  nFirstPoint
    @param  nNbrPoints
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::SetDataRange( uint nFirstPoint, int nNbrPoints )
{
    if( nFirstPoint == nFirstPoint_ && nNbrPoints == nNbrPoints_ )
        return;

    nFirstPoint_  = nFirstPoint;
    nNbrPoints_ = nNbrPoints;

    TouchData();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::AddPoint
/** @param  T_Point&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::AddPoint( const T_Point& point )
{
    pData_->push_back( point );

    if( nNbrPoints_ == -1 || pData_->size() <= nFirstPoint_ + nNbrPoints_ )
        UpdateBBox();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::AddPoint
/** @param  rX
    @param  rY
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::AddPoint( double rX, double rY )
{
    AddPoint( T_Point( rX, rY ) );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::ChangePoint
/** @param  nIndex
    @param  point
    */
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
void GQ_PlotData::ChangePoint( uint nIndex, const T_Point& point )
{
    assert( pData_ !=0 );
    assert( nIndex < pData_->size() );
    pData_->at( nIndex ) = point;

    UpdateBBox();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::DeletePoint
/** @param  nIndex
*/
// Created: APE 2004-12-21
// -----------------------------------------------------------------------------
void GQ_PlotData::DeletePoint( uint nIndex )
{
    assert( pData_ !=0 );
    assert( nIndex < pData_->size() );

    pData_->erase( pData_->begin() + nIndex );

    // Update the selection.
    for( IT_IndexVector it = selectedIndexes_.begin(); it != selectedIndexes_.end(); )
    {
        // Remove the removed item from the selection.
        if( (*it) == (int)nIndex )
        {
            it = selectedIndexes_.erase( it );
            continue;
        }

        // Update the selected indexes to reflect the fact that an item was removed and
        // indexes have changed.
        if( (*it) > (int)nIndex )
            (*it) = (*it) - 1;

        ++it;
    }

    // Update the bounding box.
    UpdateBBox();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::TouchRange
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
void GQ_PlotData::TouchRange()
{
    emit RangeTouched();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::TouchData
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::TouchData()
{
    emit DataTouched();

    UpdateBBox();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::Draw
/** @param
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::Draw( QPainter& painter )
{
    if( !bVisible_ )
        return;

    Q3PointArray points;

    PreparePoints( points );

    if( points.isEmpty() )
        return;

    DrawPoints  ( painter, points );
    DrawPolyline( painter, points );
    DrawBars    ( painter, points );

//    DrawSelection(); // / EraseSelection
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::DrawCaption
// Created: CBX 2003-11-25
// -----------------------------------------------------------------------------
void GQ_PlotData::DrawCaption( QPixmap& caption, int nSize )
{
    if( nSize > 0 )
    {
        caption.resize( nSize, nSize );
        caption.fill  ( plot_.backgroundColor() );
    }

    int dx  = caption.width ();
    int dy  = caption.height();

    int x0  = dx / 2;
    int y0  = dy / 2;

    QPainter painter( &caption );

    if( bDrawBars_ )
    {
        painter.setPen  ( barPen_   );
        painter.setBrush( barBrush_ );

        QRect rect( 0, dy / 4, dx, y0 );
        painter.drawRect( rect );
    }

    Q3PointArray line( 2 );
    line.setPoint( 0, 0,      y0 );
    line.setPoint( 1, dx - 1, y0 );
    DrawPolyline( painter, line );

    if( pointPen_.style() != Qt::NoPen )
    {
        painter.setPen( pointPen_ );
        DrawPoint( painter, QPoint( x0, y0 ) );
    }
}

typedef std::vector<QPoint> T_QPointVector;

struct QPointVector_Comp
{
    bool operator ()( const QPoint& lhs, const QPoint rhs ) { return  lhs.x() < rhs.x(); }
};

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::PreparePoints
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::PreparePoints( Q3PointArray& points )
{
    uint nLastPoint = static_cast< uint>( pData_->size() );

    if( nNbrPoints_ >= 0 && nFirstPoint_ + nNbrPoints_ < nLastPoint )
        nLastPoint = nFirstPoint_ + nNbrPoints_;

    if( nLastPoint <= nFirstPoint_ )
        return;

    const GQ_PlotAxis& xAxis = plot_.XAxis();
    const GQ_PlotAxis& yAxis = plot_.YAxis();

    QPoint ppoint;
    points.resize( nLastPoint - nFirstPoint_ );

    for( uint i = nFirstPoint_; i < nLastPoint; ++i )
    {
        const T_Point& point = ( *pData_ )[i];

        ppoint.setX( xAxis.MapToViewport( point.first  ) );
        ppoint.setY( yAxis.MapToViewport( point.second ) );

        points.setPoint( i - nFirstPoint_, ppoint );
    }

    T_QPointVector vPoints;
    for( int n = 0; n < points.count(); ++n )
        vPoints.push_back( points.point( n ) );
    std::sort( vPoints.begin(), vPoints.end(), QPointVector_Comp() );
    for( int n = 0; n < points.count(); ++n )
        points.setPoint( n, vPoints[ n ] );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::DrawPoints
/** @param  painter
    @param  points
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::DrawPoints( QPainter& painter, const Q3PointArray& points )
{
    if( pointPen_.style() == Qt::NoPen )
        return;

    painter.setPen( pointPen_ );
    uint nNbrPoints = points.size();
    for( uint i = 0; i < nNbrPoints; ++i )
    {
        QPoint point = points.point( i );

        DrawPoint( painter, point );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::DrawPoint
/** @param  painter
    @param  QPoint&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::DrawPoint( QPainter& painter, const QPoint& point )
{
    if( nPointShapeType_ == eDot )
    {
        painter.drawPoint( point );
        return;
    }

    painter.translate( point.x(), point.y() );

    Q3PointArray shape( pointShape_ );
    shape.detach();

    if( bPolylineShape_ )
        painter.drawPolyline( shape );
    else
        painter.drawLineSegments( shape );

    painter.translate( -point.x(), -point.y() );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::DrawPolyline
/** @param  painter
    @param  QPointArray&
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::DrawPolyline( QPainter& painter, const Q3PointArray& polyline )
{
    if( linePen_ == Qt::NoPen )
        return;

    painter.setPen( linePen_ );
    painter.drawPolyline( polyline );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::DrawBars
/** @param  painter
    @param  points
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
void GQ_PlotData::DrawBars( QPainter& painter, const Q3PointArray& points )
{
    barLimits_.clear();

    if( !bDrawBars_ )
        return;

    painter.setPen  ( barPen_   );
    painter.setBrush( barBrush_ );

    const GQ_PlotAxis& xAxis = plot_.XAxis();

    bool bAutoWidth = ( rBarWidth_ < 0.0 );
    int nWidth = -1;
    if( !bAutoWidth )
    {
        double rScale = xAxis.Scale();

        nWidth = ( int )( rBarWidth_ * rScale + 0.5 );
    }

    int nPrevX = 0;
    int nNextX = 0;

    int nLeftWidth  = 0;
    int nRightWidth = 0;

    if( nBarAlignment_ == eAlign_Center )
    {
        nRightWidth = nWidth / 2;
        nLeftWidth  = nWidth - nRightWidth - 1;
    }
    else if( nBarAlignment_ == eAlign_Right )
    {
        nLeftWidth  = nWidth - 1;
        nRightWidth = 0;
    }
    else // if( nBarAlignment_ == eAlign_Left )
    {
        nLeftWidth  = 0;
        nRightWidth = nWidth - 1;
    }

    uint nNbrPoints = points.size();
    barLimits_.reserve( nNbrPoints );

    for( uint i = 0; i < nNbrPoints; ++i )
    {
        const QPoint& point = points.point( i );

        int nX0 = point.x();

        if( bAutoWidth )
        {
            int nX1 = 0;
            if( i < nNbrPoints - 1 )
                nX1 = points.point( i + 1 ).x();

            if( i == 0 )
            {
                if( nBarAlignment_ != eAlign_Left )
                {
                    if( nNbrPoints >= 2 )
                        nLeftWidth = nX1 - nX0;
                    else
                        nLeftWidth = 20;

                    if( nBarAlignment_ == eAlign_Center )
                        nLeftWidth /= 2;
                }

                nPrevX = nX0 - nLeftWidth;
            }

            if( i == nNbrPoints - 1 )
            {
                if( nBarAlignment_ == eAlign_Center )
                    nRightWidth = nX0 - nPrevX;
//                else if( nBarAlignment_ == eAlign_Left )
//                    nRightWidth = 10;
            }
            else
            {
                if( nBarAlignment_ != eAlign_Right )
                {
                    nRightWidth = nX1 - nX0;

                    if( nBarAlignment_ == eAlign_Center )
                        nRightWidth /= 2;
                }
            }
        }
        else
        {
            nPrevX = nX0 - nLeftWidth;
        }

        nNextX = nX0 + nRightWidth;

        if( nNextX <= nPrevX )
            nNextX = nPrevX + 1;

        int nY0 = GetBaseline( i );
        int nY1 = point.y();
        QRect rect( nPrevX, nY0, nNextX - nPrevX, nY1 - nY0 );

        T_Range barLimits( xAxis.MapFromViewport( nPrevX ), xAxis.MapFromViewport( nNextX ) );
        barLimits_.push_back( barLimits );

//$$$$ TEST
        if( std::find( selectedIndexes_.begin(), selectedIndexes_.end(), i) != selectedIndexes_.end() )
            painter.setBrush( QBrush( QColor( Qt::red ) ) );
        else
            painter.setBrush( QBrush( QColor( Qt::blue ) ) );

        painter.drawRect( rect );

        nPrevX = nNextX;
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::UpdateBBox
// Created: APE 2004-12-22
// -----------------------------------------------------------------------------
void GQ_PlotData::UpdateBBox()
{
    GQ_PlotDataBBox boundingBox( *pData_, nFirstPoint_, nNbrPoints_ );

    // If we are drawing bars, we must take into account the bars' width.
    if( bDrawBars_ && rBarWidth_ > 0.0 && ! boundingBox.IsEmpty() )
    {
        switch( nBarAlignment_ )
        {
            case eAlign_Left:
                boundingBox.UpdateWithPoint( T_Point( boundingBox.XMax() + rBarWidth_, boundingBox.YMax() ) );
                break;
            case eAlign_Right:
                boundingBox.UpdateWithPoint( T_Point( boundingBox.XMin() - rBarWidth_, boundingBox.YMax() ) );
                break;
            case eAlign_Center:
                boundingBox.UpdateWithPoint( T_Point( boundingBox.XMax() + rBarWidth_ / 2.0, boundingBox.YMax() ) );
                boundingBox.UpdateWithPoint( T_Point( boundingBox.XMin() - rBarWidth_ / 2.0, boundingBox.YMax() ) );
                break;
        }
    }

    if( bbox_.SetBBox( boundingBox ) )
        TouchRange();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::GetBaseline
// Created: OT 2004-06-14
// -----------------------------------------------------------------------------
int GQ_PlotData::GetBaseline( uint )
{
    const GQ_PlotAxis& yAxis = plot_.YAxis();
    return yAxis.ClipToViewport( 0.0 );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::GetToolTips
/** @param  bbox
    @param  toolTipList
    @return
*/
// Created: CBX 2003-08-18
// -----------------------------------------------------------------------------
bool GQ_PlotData::GetToolTips( const GQ_PlotDataBBox& bbox, QStringList& tipList ) const
{
    std::size_t nLastPoint = pData_->size();

    if( nNbrPoints_ >= 0 && nFirstPoint_ + nNbrPoints_ < nLastPoint )
        nLastPoint = nFirstPoint_ + nNbrPoints_;

    if( nLastPoint <= nFirstPoint_ )
        return false;

    uint nNbrNewTips = 0;

    bool bCheckBars = !barLimits_.empty();
    assert( !bCheckBars || barLimits_.size() == nLastPoint - nFirstPoint_ );
    T_Point bboxCenter = bbox.Center();

    for( uint i = nFirstPoint_; i < nLastPoint; ++i )
    {
        const T_Point& point = ( *pData_ )[i];

        if( bbox.Includes( point )
           || ( bCheckBars && IsOnBar( bboxCenter, point, barLimits_[ i - nFirstPoint_ ] ) ) )
        {
            QString tipStr( name_ );
            if( !tipStr.isEmpty() )
                tipStr += " ";

            tipStr += plot_.GetStrCoordinates( point );

            tipList.append( tipStr );
            ++nNbrNewTips;
        }
    }

    return( nNbrNewTips > 0 );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::IsOnBar
/** @param  barTopPoint
    @param  barLimits
    @return
*/
// Created: CBX 2004-03-08
// -----------------------------------------------------------------------------
bool GQ_PlotData::IsOnBar( const T_Point& bboxCenter, const T_Point& barTopPoint, const T_Range& barLimits ) const
{
    double rX0 = bboxCenter.first;
    if( rX0 < barLimits.first || rX0 > barLimits.second )
        return false;

    double rY0 = bboxCenter.second;

    if( rY0 >= 0 )
    {
        if( rY0 <= barTopPoint.second )
            return true;
    }
    else if( rY0 >= barTopPoint.second )
            return true;

    return false;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetSelected
// Created: APE 2004-12-17
// -----------------------------------------------------------------------------
void GQ_PlotData::SetSelected( bool bSelected )
{
    bSelected_ = bSelected;
    selectedIndexes_.clear();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetSelected
/** @param  nIndex
*/
// Created: APE 2004-12-17
// -----------------------------------------------------------------------------
void GQ_PlotData::SetSelected( uint nIndex, bool bSelected )
{
    assert( pData_ != 0 );
    assert( nIndex < pData_->size() );

    bSelected_ = false;

    if( bSelected )
    {
        if( std::find( selectedIndexes_.begin(), selectedIndexes_.end(), nIndex ) == selectedIndexes_.end() )
        {
            selectedIndexes_.push_back( nIndex );
        }
    }
    else
    {
        IT_IndexVector it = std::find( selectedIndexes_.begin(), selectedIndexes_.end(), nIndex );
        if( it != selectedIndexes_.end() )
        {
            selectedIndexes_.erase( it );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::SetSelection
/** @param  selection
*/
// Created: APE 2004-12-22
// -----------------------------------------------------------------------------
void GQ_PlotData::SetSelection( T_DataItemVector& selection )
{
    for( IT_DataItemVector it = selection.begin(); it != selection.end(); ++it )
    {
        if( (*it).first != this )
            continue;

        if( (*it).second == -1 )
            this->SetSelected( true );
        else
            this->SetSelected( (*it).second, true );
    }
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::GetSelection
/** @param  selection
*/
// Created: APE 2004-12-17
// -----------------------------------------------------------------------------
void GQ_PlotData::GetSelection( T_DataItemVector& selection )
{
    if( bSelected_ )
    {
        selection.push_back( T_DataItem( this, -1 ) );
        return;
    }

    for( IT_IndexVector it = selectedIndexes_.begin(); it != selectedIndexes_.end(); ++it )
        selection.push_back( T_DataItem( this, *it ) );
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::DataAt
/** @return
*/
// Created: APE 2004-12-17
// -----------------------------------------------------------------------------
int GQ_PlotData::DataAt( const T_Point& point )
{
    if( bDrawBars_ )
    {
        for( IT_RangeVector it = barLimits_.begin(); it != barLimits_.end(); ++it )
        {
            if( point.first >= (*it).first && point.first <= (*it).second )
                return static_cast< int >( std::distance( barLimits_.begin(), it ) );
        }
    }

    return -1;
}
