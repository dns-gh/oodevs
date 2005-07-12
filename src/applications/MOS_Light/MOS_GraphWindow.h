//*****************************************************************************
//
// $Created: NLD 2002-10-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_GraphWindow.h $
// $Author: Nld $
// $Modtime: 27/10/03 17:17 $
// $Revision: 4 $
// $Workfile: MOS_GraphWindow.h $
//
//*****************************************************************************

#ifndef __MOS_GraphWindow_h_
#define __MOS_GraphWindow_h_

#include "MOS_Types.h"

#include <qframe.h>

class AIPlot;
class MOS_MainWindow;
class QCheckBox;

//=============================================================================
/**
*   @class  MOS_GraphWindow
*   @brief  PUT THE COMMENTS ON THE CLASS HERE
*   @par    Using example
*   @code
*     PUT YOUR EXAMPLE CODE HERE
*   @endcode
*/
// Created: NLD 2002-10-14
//=============================================================================
class MOS_GraphWindow : public QFrame
{
    MT_COPYNOTALLOWED( MOS_GraphWindow );

    Q_OBJECT;
public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::pair< double, double > T_DoublePair;
    typedef std::vector< T_DoublePair > T_PlotVector;
    typedef T_PlotVector::iterator      IT_PlotVector;
    //@}

public:
    MOS_GraphWindow( MOS_MainWindow& mainWindow );
    virtual ~MOS_GraphWindow();

    //-------------------------------------------------------------------------
    /** @name */
    //-------------------------------------------------------------------------
    //@{
    void AddValues( double rMainLoopTime, double rPerceptionTime, double rDecisionTime, double rActionTime );
    //@}

private slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotMainLoopTraceToggled  ( bool bOn );
    void SlotPerceptionTraceToggled( bool bOn );
    void SlotDecisionTraceToggled  ( bool bOn );
    void SlotActionTraceToggled    ( bool bOn );
    //@}
   
private:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::vector< QCheckBox* > T_CheckBoxVector;
    typedef T_CheckBoxVector          IT_CheckBoxVector;
    //@}
    
private:
    AIPlot* pGraphic_;

    T_PlotVector mainLoopTimeVector_;
    T_PlotVector perceptionTimeVector_;
    T_PlotVector decisionTimeVector_;
    T_PlotVector actionTimeVector_;

    T_CheckBoxVector traceCheckBoxVector_;

    uint   nCurrentFrame_;
    double rYMaxValue_;
};


#endif // __MOS_GraphWindow_h_
