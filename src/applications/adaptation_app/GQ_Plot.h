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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_Plot.h $
// $Author: Ape $
// $Modtime: 13/01/05 11:26 $
// $Revision: 2 $
// $Workfile: GQ_Plot.h $
//
// *****************************************************************************

#ifndef __GQ_Plot_h_
#define __GQ_Plot_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "GQ_PlotAxis.h"
#include "GQ_PlotData.h"
#include "GQ_PlotDataBBox.h"
#include "GQ_ValueConvertor.h"

#include <qrect.h>
#include <qpalette.h>
#include <qpixmap.h>
#include <qwidget.h>

class GQ_PlotToolTip;

class QLabel;
class QStringList;

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

    typedef std::pair< GQ_PlotData*, int >  T_DataItem;
    typedef std::vector< T_DataItem >   T_DataItemVector;
    typedef T_DataItemVector::iterator          IT_DataItemVector;
    typedef T_DataItemVector::const_iterator   CIT_DataItemVector;

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

    enum E_SelectionMode
    {
        eNone,
        eSingle
    };
    
public:
    //! @name Constructors/Destructor
    //@{
    GQ_Plot( QWidget* pParent = 0 );

    virtual ~GQ_Plot();
    //@}

    //! @name Settings
    //@{
    void SetPlotMargin( uint nMargin );

    void ShowPlotFrame( bool );
    void ShowToolTips ( bool );

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
    GQ_PlotData* GetPlotData ( uint nPlotIndex, int nDepth = 0 ) const;

    GQ_PlotData* FindPlotData ( uint           nUserID ) const;
    GQ_PlotData* FindPlotData ( const QString& name    ) const;

    const T_LayerMap& LayerMap   () const;
    int               GetMinDepth() const;
    int               GetMaxDepth() const;

    uint GetNumberOfPlotData( int nDepth = 0 ) const;

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
    void UnregisterAllPlotData( bool bDestroy );

    void SetLayerVisible( bool bVisible, int nDepth = 0 );
    void SetAllVisible  ( bool bVisible );

    void ClearLayerData( int nDepth );
    void ClearAllData  ();
    //@}

    //! @name Selection handeling
    //@{
    void SetSelectionMode( E_SelectionMode );

    void GetSelection( T_DataItemVector& selection );
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
    void SelectionChanged( const T_DataItemVector& selection );
    //@}

protected:
    //! @name Qt overloaded methods
    //@{
    virtual void paintEvent( QPaintEvent* );
    virtual void mouseReleaseEvent( QMouseEvent* );

    virtual void Draw( QPainter&, const QColorGroup& );

    virtual void UpdateDataRange();
    virtual void UpdateBackground( const QColorGroup& );
    virtual void UpdateDataPlot  ( const QColorGroup& );

    void UpdateLayerBBox( const T_PlotLayer&, GQ_PlotDataBBox& );

    void CenterOnPlotRect( QPainter& );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    GQ_Plot( const GQ_Plot& rhs );            //!< Copy constructor
    GQ_Plot& operator=( const GQ_Plot& rhs ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{    
    //@}

private:
    //! @name Settings parameters
    //@{
    bool bShowPlotFrame_;
    E_SelectionMode nSelectionMode_;
    //@}

    //! @name Data members
    //@{
    GQ_PlotAxis x_;
    GQ_PlotAxis y_;

    const T_CoordToString* pCoordToString_;
    GQ_PlotToolTip* pToolTip_;

    T_LayerMap layerMap_;

    QPixmap* pBackground_;
    QPixmap* pPlot_;

    uint  nPlotMargin_;
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


#endif // __GQ_Plot_h_
