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
// $Archive: /MVW_v10/Build/SDK/Adn2/src/GQ_PlotData.h $
// $Author: Ape $
// $Modtime: 27/12/04 11:01 $
// $Revision: 1 $
// $Workfile: GQ_PlotData.h $
//
// *****************************************************************************

#ifndef __GQ_PlotData_h_
#define __GQ_PlotData_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "GQ_PlotDataBBox.h"

class GQ_Plot;

// =============================================================================
/** @class  GQ_PlotData
    @brief  GQ_PlotData
    @par    Using example
    @code
    GQ_PlotData;
    @endcode
*/
// Created: CBX 2003-08-07
// =============================================================================
class GQ_PlotData : public QObject
{
    Q_OBJECT

public:
    typedef std::pair< double, double > T_Point;
    typedef std::vector< T_Point >      T_Data;

    typedef std::pair< double, double >     T_Range;
    typedef std::vector< T_Range >          T_RangeVector;
    typedef T_RangeVector::iterator        IT_RangeVector;
    typedef T_RangeVector::const_iterator CIT_RangeVector;

    typedef std::vector< int >           T_IndexVector;
    typedef T_IndexVector::iterator     IT_IndexVector;

    typedef std::pair< GQ_PlotData*, int >       T_DataItem;
    typedef std::vector< T_DataItem >            T_DataItemVector;
    typedef T_DataItemVector::iterator          IT_DataItemVector;
    typedef T_DataItemVector::const_iterator   CIT_DataItemVector;

    enum E_PointShapeType
    {
        eDot        //!< '.'
      , eCircle     //!< 'o'
      , eSquare     //!< '[]'
      , eDiagSquare //!< '<>'
      , eUTriangle  //!< '^'
      , eDTriangle  //!< 'v'
      , eCross      //!< '+'
      , eDiagCross  //!< 'x'
      , eVLine      //!< '|'
      , eBDiag      //!< '/'
      , eFDiag      //!< '\'
      , eUserShape    //!< User polygonal shape
      , eUserSegments //!< User shape as set of line segments
    };

    enum E_Alignment
    {
        eAlign_Center
      , eAlign_Left
      , eAlign_Right

      , eNbrAlignments
    };

public:
    //! @name Constructors/Destructor
    //@{
    GQ_PlotData( uint nUserID, GQ_Plot& );
    GQ_PlotData( uint nUserID, GQ_Plot&, T_Data&, uint nFirstPoint = 0, int nNbrPoints = -1 );

    virtual ~GQ_PlotData();
    //@}

    //! @name Settings
    //@{
    void SetUserID( uint );
    void SetName  ( const QString& );

    void SetVisible( bool bShow );

    void SetPointPen  ( const QPen& );
    void SetPointShape( E_PointShapeType nPredefinedType, uint nShapeSize = 3 );
    void SetPointShape( E_PointShapeType nUserType, const Q3PointArray& shape );

    void SetLinePen   ( const QPen& );

    void SetBarStyle  ( const QPen&, const QBrush&, bool bDrawBars, double rWidth = -1 /* auto */, E_Alignment = eAlign_Left );
    //@}

    //! @name Accessors
    //@{
    bool IsEmpty     () const;
    bool IsVisible   () const;

    uint           UserID() const;
    const QString& Name  () const;

    const T_Data&          Data() const;
    const GQ_PlotDataBBox& BBox() const;

    virtual bool GetToolTips( const GQ_PlotDataBBox&, QStringList& ) const;
    //@}

    //! @name Modifiers
    //@{
    virtual void ClearData();

    void SetSharedData(       T_Data&, uint nFirstPoint = 0, int nNbrPoints = -1 );
    void SetData      ( const T_Data&, uint nFirstPoint = 0, int nNbrPoints = -1 );
    void SetDataRange ( uint nFirstPoint, int nNbrPoints = -1 );

    void AddPoint( const T_Point& );
    void AddPoint( double rX, double rY );
    void ChangePoint( uint nIndex, const T_Point& point );
    void DeletePoint( uint nIndex );
    //@}

    //! @name Operators
    //@{
    virtual void Draw       ( QPainter& );
    virtual void DrawCaption( QPixmap&, int nSize = 16 );
    //@}

    //! @name Selection
    //@{
    void SetSelected( bool );               //!< Selects or unselects the whole plotData. Clears the individual selection.
    void SetSelected( uint nIndex, bool );   //!< Selects or unselects an data element. Clears the global selection.
    void SetSelection( T_DataItemVector& selection );
    void GetSelection( T_DataItemVector& selection );

    int DataAt( const T_Point& point );  //!< Returns the index of the data element at the position point, -1 if there is none.
    //@}

public slots:
    //! @name Slots
    //@{
    void TouchRange();
    void TouchData ();
    //@}

signals:
    //! @name Signals
    //@{
    void RangeTouched();
    void DataTouched ();
    //@}

protected:
    virtual bool IsOnBar( const T_Point& bboxCenter, const T_Point& barTopPoint, const T_Range& barLimits ) const;
    virtual int  GetBaseline( uint ); //!< returns bottom position for histograms (default = yaxis)

private:
    //! @name Copy/Assignment
    //@{
    GQ_PlotData( const GQ_PlotData& rhs );            //!< Copy constructor
    GQ_PlotData& operator=( const GQ_PlotData& rhs ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void InitPointShape( uint nShapeSize );
    virtual void PreparePoints  ( Q3PointArray& );
    virtual void DrawPoints     ( QPainter&, const Q3PointArray& );
    virtual void DrawPoint      ( QPainter&, const QPoint&      );
    virtual void DrawPolyline   ( QPainter&, const Q3PointArray& );
    virtual void DrawBars       ( QPainter&, const Q3PointArray& );

    virtual void UpdateBBox();
    //@}

protected:
    //! @name Member data
    //@{
    GQ_Plot& plot_;
    bool bOwnData_;

    uint    nUserID_;
    QString name_;
    //@}

    //! @name Settings parameters
    //@{
    bool bVisible_;

    QPen   pointPen_;
    QPen   linePen_;
    QPen   barPen_;
    QBrush barBrush_;

    E_PointShapeType nPointShapeType_;
    Q3PointArray      pointShape_;
    bool             bPolylineShape_;

    bool        bDrawBars_;
    double      rBarWidth_;
    E_Alignment nBarAlignment_;
    //@}

    //! @name Runtime data
    //@{
    T_Data* pData_;
    uint nFirstPoint_;
    int  nNbrPoints_;

    bool bSelected_;
    T_IndexVector selectedIndexes_;

    T_RangeVector barLimits_;

    GQ_PlotDataBBox bbox_;
    //@}
};

// =============================================================================
// Inline methods implementation
// =============================================================================

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::IsEmpty
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
bool GQ_PlotData::IsEmpty() const
{
    return pData_->empty();
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::IsVisible
/** @return
*/
// Created: APE 2004-12-20
// -----------------------------------------------------------------------------
inline
bool GQ_PlotData::IsVisible() const
{
    return bVisible_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::UserID
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
uint GQ_PlotData::UserID() const
{
    return nUserID_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::Name
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
const QString& GQ_PlotData::Name() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::Data
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
const GQ_PlotData::T_Data& GQ_PlotData::Data() const
{
    return *pData_;
}

// -----------------------------------------------------------------------------
// Name: GQ_PlotData::BBox
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
const GQ_PlotDataBBox& GQ_PlotData::BBox() const
{
    return bbox_;
}

#endif // __GQ_PlotData_h_
