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
// $Archive: /Tools/Gui/build/Libraries/GQ_Widgets/src/GQ_PlotData.h $
// $Author: Cbx $
// $Modtime: 10/01/05 16:02 $
// $Revision: 8 $
// $Workfile: GQ_PlotData.h $
//
// *****************************************************************************

#ifndef __GQ_PlotData_h_
#define __GQ_PlotData_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "GQ_PlotDataBBox.h"

#include <qbrush.h>
#include <qcolor.h>
#include <qobject.h>
#include <qpen.h>
#include <qpointarray.h>

class QPainter;
class QPixmap;
class QStringList;

namespace gui
{
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
    typedef std::pair< double, double > T_Range;
    typedef std::vector< T_Point >      T_Data;

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
    GQ_PlotData( unsigned int nUserID, GQ_Plot& );
    GQ_PlotData( unsigned int nUserID, GQ_Plot&, T_Data&, unsigned int nFirstPoint = 0, int nNbrPoints = -1 );

    virtual ~GQ_PlotData();
    //@}

    //! @name Settings
    //@{
    void SetUserID( unsigned int );
    void SetName  ( const QString& );

    void SetVisible( bool bShow );

    void SetPointPen  ( const QPen& );
    void SetPointShape( E_PointShapeType nPredefinedType, unsigned int nShapeSize = 3 );
    void SetPointShape( E_PointShapeType nUserType, const QPointArray& shape );

    void SetLinePen   ( const QPen& );

    void SetBarStyle  ( const QPen&, const QBrush&, bool bDrawBars, double rWidth = -1 /* auto */, E_Alignment = eAlign_Left );

    void AddIgnoreValue( double value );
    //@}

    //! @name Accessors
    //@{
    bool IsEmpty     () const;

    unsigned int           UserID() const;
    const QString& Name  () const;

    const T_Data&          Data() const;
    const GQ_PlotDataBBox& BBox() const;

    virtual bool GetToolTips( const GQ_PlotDataBBox&, QStringList& ) const;
    //@}

    //! @name Modifiers
    //@{
    virtual void ClearData();

    void SetSharedData(       T_Data&, unsigned int nFirstPoint = 0, int nNbrPoints = -1 );
    void SetData      ( const T_Data&, unsigned int nFirstPoint = 0, int nNbrPoints = -1 );
    void SetDataRange ( unsigned int nFirstPoint, int nNbrPoints = -1 );

    void AddPoint( const T_Point& );
    void AddPoint( double rX, double rY );
    //@}

    //! @name Operators
    //@{
    virtual void Draw       ( QPainter& );
    virtual void DrawCaption( QPixmap&, int nSize = 16 );
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
    virtual int  GetBaseline( unsigned int ); //!< returns bottom position for histograms (default = yaxis)

private:
    //! @name Copy/Assignement
    //@{
    GQ_PlotData( const GQ_PlotData& rhs );            //!< Copy constructor
    GQ_PlotData& operator=( const GQ_PlotData& rhs ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void InitPointShape( unsigned int nShapeSize );
    virtual void PreparePoints  ( QPointArray& );
    virtual void DrawPoints     ( QPainter&, const QPointArray& );
    virtual void DrawPoint      ( QPainter&, const QPoint&      );
    virtual void DrawPolyline   ( QPainter&, const QPointArray& );
    virtual void DrawBars       ( QPainter&, const QPointArray& );
    //@}

protected:
    //! @name Member data
    //@{
    GQ_Plot& plot_;
    bool bOwnData_;

    unsigned int    nUserID_;
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
    QPointArray      pointShape_;
    bool             bPolylineShape_;

    bool        bDrawBars_;
    double      rBarWidth_;
    E_Alignment nBarAlignment_;
    //@}

    //! @name Runtime data
    //@{
    T_Data* pData_;
    unsigned int nFirstPoint_;
    int  nNbrPoints_;

    std::vector< T_Range > barLimits_;

    GQ_PlotDataBBox bbox_;

    std::set< double > ignoredValues_;
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
// Name: GQ_PlotData::UserID
/** @return
*/
// Created: CBX 2003-08-08
// -----------------------------------------------------------------------------
inline
unsigned int GQ_PlotData::UserID() const
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

}

#endif // __GQ_PlotData_h_
