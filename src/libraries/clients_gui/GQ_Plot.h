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
// $Archive: /Tools/Gui/build/Libraries/GQ_Widgets/src/GQ_Plot.h $
// $Author: Cbx $
// $Modtime: 10/01/05 15:52 $
// $Revision: 10 $
// $Workfile: GQ_Plot.h $
//
// *****************************************************************************

#ifndef __GQ_Plot_h_
#define __GQ_Plot_h_

#include "GQ_PlotAxis.h"
#include "GQ_PlotData.h"
#include "GQ_PlotDataBBox.h"
#include "GQ_ValueConvertor.h"

#include <map>
#include <vector>

class QLabel;
class QStringList;

namespace gui
{
// =============================================================================
/** @class  GQ_Plot
    @brief  2D Plot widget

*/
// Created: CBX 2003-08-07
// =============================================================================
class GQ_Plot : public QWidget
{
    Q_OBJECT

public:
    typedef std::pair< double, double >    T_Point;
    typedef std::vector< GQ_PlotData* >    T_PlotLayer;
    typedef std::map< int, T_PlotLayer >   T_LayerMap; //!< map< layer depth, layer >
    typedef T_LayerMap::iterator          IT_LayerMap;
    typedef T_LayerMap::const_iterator   CIT_LayerMap;
    typedef std::pair< int, int >          T_DataIndex;

    typedef GQ_ValueConvertor< T_Point, QString > T_CoordToString; //!< string convertor for coordinates display

    struct iterator
    {
        iterator( const GQ_Plot& plot                        ) : pPlot_( &plot ), dataIndex_( 0, -1 )     {}
        iterator( const GQ_Plot& plot, T_DataIndex dataIndex ) : pPlot_( &plot ), dataIndex_( dataIndex ) {}

        iterator& operator++()
        {
            dataIndex_ = pPlot_->GetNextDataIndex( dataIndex_ );
            return *this;
        }

        bool operator !=( const iterator& it ) const
        {
            return ( &it != this && ( it.pPlot_ != pPlot_ || it.dataIndex_ != dataIndex_ ) );
        }

        GQ_PlotData& operator*()
        {
            return pPlot_->GetPlotData( dataIndex_ );
        }

        const GQ_Plot* pPlot_;
        T_DataIndex    dataIndex_;
    };

public:
    //! @name Constructors/Destructor
    //@{
    explicit GQ_Plot( QWidget* pParent = 0, const char* name = 0 );
    virtual ~GQ_Plot();
    //@}

    //! @name Settings
    //@{
    void SetPlotMargin( unsigned int nMargin );

    void ShowPlotFrame( bool );

    void SetBackgroundColor( const QColor& );
    void SetAxisColor      ( const QColor& );
    void SetFrameColor     ( const QColor& );
    void SetGridColor      ( const QColor& );
    void SetCaptionColor   ( const QColor& );

    void SetColor( QColorGroup::ColorRole role, const QColor& );

    void SetCoordToStringConvertor( const T_CoordToString* );
    //@}

    //! @name Accessors
    //@{
    GQ_PlotAxis& XAxis();
    GQ_PlotAxis& YAxis();

    virtual QString GetStrCoordinates( const T_Point& ) const;

    virtual bool GetToolTips( const QPoint&, QRect& tipRect, QStringList& ) const;
    //@}

    //! @name Plot data Accessors
    //@{
    GQ_PlotData* GetPlotData ( unsigned int nPlotIndex, int nDepth = 0 ) const;

    GQ_PlotData* FindPlotData ( unsigned int           nUserID ) const;
    GQ_PlotData* FindPlotData ( const QString& name    ) const;

    const T_LayerMap& LayerMap   () const;
    int               GetMinDepth() const;
    int               GetMaxDepth() const;

    iterator begin() const;
    iterator end  () const;

    T_DataIndex  GetNextDataIndex( T_DataIndex ) const;
    GQ_PlotData& GetPlotData     ( T_DataIndex ) const;
    //@}

    //! @name Coordinate system accessors
    //@{
    QPoint  MapToViewport  ( const T_Point&       ) const;
    QPoint  MapToViewport  ( double rX, double rY ) const;
    T_Point MapFromViewport( const QPoint&        ) const;
    //@}

    //! @name Modifiers
    //@{
    void RegisterPlotData  ( GQ_PlotData&, int nDepth = 0 );
    bool UnregisterPlotData( GQ_PlotData&, bool bDestroy );

    void SetLayerVisible( bool bVisible, int nDepth = 0 );
    void SetAllVisible  ( bool bVisible );

    void ClearLayerData( int nDepth );
    void ClearAllData  ();
    //@}

public slots:
    //! @name Slots
    //@{
    void TouchFrame();
    void TouchData ();
    //@}

signals:
    //! @name Signals
    //@{
    void FrameTouched();
    void DataTouched ();
    //@}

protected:
    //! @name Qt overloaded methods
    //@{
    virtual void paintEvent( QPaintEvent* );
//    virtual void resizeEvent( QResizeEvent* pResizeEvent );

    virtual void Draw( QPainter&, const QColorGroup& );

    virtual void UpdateDataRange();
    virtual void UpdateBackground( const QColorGroup& );
    virtual void UpdateDataPlot  ( const QColorGroup& );

    void UpdateLayerBBox( const T_PlotLayer&, GQ_PlotDataBBox& );

    void CenterOnPlotRect( QPainter& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GQ_Plot( const GQ_Plot& rhs );            //!< Copy constructor
    GQ_Plot& operator=( const GQ_Plot& rhs ); //!< Assignment operator
    //@}

private:
    //! @name Settings parameters
    //@{
    bool bShowPlotFrame_;
    //@}

    //! @name Member data
    //@{
    GQ_PlotAxis x_;
    GQ_PlotAxis y_;

    const T_CoordToString* pCoordToString_;

    T_LayerMap layerMap_;

    QPixmap* pBackground_;
    QPixmap* pPlot_;

    unsigned int  nPlotMargin_;
    QRect plotRect_;
    //@}

    //! @name Runtime parameters
    //@{
    GQ_PlotDataBBox bbox_;

    bool bBgUpdateNeeded_;
    bool bPlotUpdateNeeded_;
    //@}
};

// =============================================================================
// Inline methods implementation
// =============================================================================

// -----------------------------------------------------------------------------
// Name: GQ_Plot::XAxis
/** @return
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
inline
GQ_PlotAxis& GQ_Plot::XAxis()
{
    return x_;
}

// -----------------------------------------------------------------------------
// Name: GQ_Plot::YAxis
/** @return
*/
// Created: CBX 2003-08-07
// -----------------------------------------------------------------------------
inline
GQ_PlotAxis& GQ_Plot::YAxis()
{
    return y_;
}

}

#endif // __GQ_Plot_h_
