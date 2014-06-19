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
// $Archive: /Tools/Gui/build/Libraries/GQ_Widgets/src/GQ_PlotAxis.h $
// $Author: Cbx $
// $Modtime: 10/01/05 16:02 $
// $Revision: 8 $
// $Workfile: GQ_PlotAxis.h $
//
// *****************************************************************************

#ifndef __GQ_PlotAxis_h_
#define __GQ_PlotAxis_h_

#include "GQ_ValueConvertor.h"
#include <vector>

class QColorGroup;
class QPainter;
class QPoint;

namespace gui
{
// =============================================================================
/** @class  GQ_PlotAxis
    @brief  GQ_PlotAxis
    @par    Using example
    @code
    GQ_PlotAxis;
    @endcode
*/
// Created: CBX 2003-08-07
// =============================================================================
class GQ_PlotAxis : public QObject
{
    Q_OBJECT

public:
    typedef std::pair< double, int > T_Tick;     //!< pair< tick value, tick position >
    typedef std::vector< T_Tick >    T_TickList;

    typedef GQ_ValueConvertor< double, QString > T_ValueToString; //!< double to string convertor for value display

    typedef std::vector< unsigned int > T_TickMultipleList;

public:
    //! @name Constructors/Destructor
    //@{
    GQ_PlotAxis( QWidget* pParent, Qt::Orientation );

    virtual ~GQ_PlotAxis();
    //@}

    //! @name Display settings
    //@{
    void SetAxisCaption( const QString& );

    void SetCaptionMargin( unsigned int );

    void ShowAxis      ( bool bShow        );
    void ShowGrid      ( bool bShow        );
    void ShowTicks     ( unsigned int nTickSpacing );
    void ShowSubTicks  ( unsigned int nNbrSubTicks );

    void SetMinTicksSpacing( unsigned int nMinTicksSpacing );
    void SetTicksLength    ( unsigned int );
    void SetSubTicksLength ( unsigned int );
    void ShowTicksValue( bool bShow );

    void SetTickMultiples( double rMultiplesBase, const std::vector< unsigned int >& multiples, double rMinTickPeriod = 0.0, int nMinBasePower = -100 );
    void InitTickMultiplesForIntegers();
    void InitTickMultiplesForMinutes ();

    void SetValueToStringConvertor( const T_ValueToString* );

    void SetCaptionFont  ( const QFont& );
    void SetTickValueFont( const QFont& );
    //@}

    //! @name Accessors
    //@{
    const QString& Caption() const;

    bool UpdateNeeded() const;

    QSize GetCaptionSize() const;
    QSize GetAxisSize   () const;
    int   GetRefTickPos () const;

    double GetMaxAxisValue() const;
    double ClipValue( double rValue ) const;

    int MapToViewport ( double rValue ) const;
    int ClipToViewport( double rValue ) const;

    double MapFromViewport ( int nPos ) const;

    int    AxisLength() const;
    double Scale     () const;

    QString GetStrValue( double rValue ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetDataRange  ( double rMinDataValue, double rMaxDataValue );

    void SetAxisRange  ( double rMinAxisValue, double rMaxAxisValue, bool bFixedRange );

    void ClearVirtualRange();
    void AddVirtualValue  ( double rValue );

    void SetAxisLength ( unsigned int nLength );

    void Update();
    //@}

    //! @name I/O methods
    //@{
    virtual void DrawCaption( QPainter&, const QColorGroup&, const QRect& );
    virtual void DrawAxis   ( QPainter&, const QColorGroup&, const QPoint& refPoint /* left bottom corner of axis */ );
    virtual void DrawGrid   ( QPainter&, const QColorGroup&, const QRect& plotRect );
    //@}

signals:
    void Touched();

protected:
    //! @name Internal helpers
    //@{
    void Touch();

    virtual void UpdateTickPeriod();
    virtual void UpdateTicks();

    virtual void DrawX   ( QPainter&, const QColorGroup&, const QPoint& refPoint );
    virtual void DrawY   ( QPainter&, const QColorGroup&, const QPoint& refPoint );

    virtual void DrawVGrid( QPainter&, int nY0, int nY1 );
    virtual void DrawHGrid( QPainter&, int nX0, int nX1 );
    //@}

private:
    //! @name Display settings parameters
    //@{
    Qt::Orientation nOrientation_;

    QString caption_;
    unsigned int    nCaptionMargin_;

    bool   bShowAxis_;
    bool   bShowGrid_;

    unsigned int   nMinTicksSpacing_;           //!< min space (pixels) allowed between ticks
    unsigned int   nTicksSpacing_;              //!< approximate space (pixels) between ticks
    unsigned int   nTicksLength_;               //!< length (pixels) of the ticks line
    unsigned int   nSubTicksLength_;            //!< length (pixels) of the subticks line
    unsigned int   nNbrSubTicks_;               //!< number of sub ticks to display between two main ticks

    double             rMinTickPeriod_; //!< min range between two tick values
    double             rMultiplesBase_; //!< base used to generate possible tick values
    int                nMinBasePower_;  //!< min power of the base value acceptable for tick values
    T_TickMultipleList tickMultiples_;  //!< set of values (up to a base power) acceptable for tick values

    bool   bShowTickValues_;
    const T_ValueToString* pValueToString_; //!< External convertor used to display values as string

    QFont captionFont_;
    QFont tickValueFont_;
    //@}

    //! @name Range settings parameters
    //@{
    double rMinDataValue_;
    double rMaxDataValue_;

    double rMinVirtualValue_;
    double rMaxVirtualValue_;

    bool   bFixedRange_;
    bool   bVirtualRange_;

    unsigned int   nAxisLength_;
    //@}

    //! @name Runtime parameters
    //@{
    bool   bUpdateNeeded_;

    double rMinAxisValue_;
    double rMaxAxisValue_;
    double rScale_;

    double rFirstTick_;
    double rTickPeriod_;
    double rSubTickPeriod_;

    unsigned int nAxisWidth_; //!< Estimated perpendicular size needed to display the axis

    T_TickList ticks_;
    //@}
};

}

#endif // __GQ_PlotAxis_h_
