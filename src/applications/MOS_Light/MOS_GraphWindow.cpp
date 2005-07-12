//*****************************************************************************
//
// $Created: NLD 2002-10-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_GraphWindow.cpp $
// $Author: Nld $
// $Modtime: 27/10/03 17:57 $
// $Revision: 8 $
// $Workfile: MOS_GraphWindow.cpp $
//
//*****************************************************************************


#include "MOS_Light_Pch.h"
#include "MOS_GraphWindow.h"
#include "moc_MOS_GraphWindow.cpp"
#include "MOS_MainWindow.h"
#include "AIStrip.h"

#include <qcheckbox.h>

#define MOS_GRAPH_X_RANGE   200

//-----------------------------------------------------------------------------
// Name: MOS_GraphWindow constructor
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
MOS_GraphWindow::MOS_GraphWindow( MOS_MainWindow& mainWindow )
    : QFrame        ( &mainWindow, "Graphics", Qt::WType_TopLevel | Qt::WStyle_Tool )
    , nCurrentFrame_( 0 )
    , rYMaxValue_   ( 0. )
{
    return;
    
    /*QHBoxLayout* pMainLayout = new QHBoxLayout( this ); 
    pMainLayout->setMargin( 20 );
    pMainLayout->setSpacing( 10 );
    
    pGraphic_ = new AIPlot( this, "Data", MOS_GRAPH_X_RANGE * 10, 4 );
    pGraphic_->setRanges( 0, MOS_GRAPH_X_RANGE, 0, 100 );   
    pGraphic_->setBackgroundColor( 127, 127, 127 );
    pGraphic_->setDoubleBuffer(TRUE);

    pGraphic_->setPlotColor( 0, 255, 0  , 0 );
    pGraphic_->setPlotColor( 1, 0  , 255, 0 );
    pGraphic_->setPlotColor( 2, 0  , 0  , 255 );
    pGraphic_->setPlotColor( 3, 0  , 255, 255 );


    pGraphic_->setXName( "Frame" );
    pGraphic_->setYName( "Time" );
    pGraphic_->setYUnit( "ms" );
    pGraphic_->setMinimumSize( 500, 500 );

    pMainLayout->addWidget( pGraphic_ );

    // Buttons
    QGroupBox* pGroupBox = new QGroupBox( this );
    pMainLayout->addWidget( pGroupBox );   

    QGridLayout* pRightLayout = new QGridLayout( pGroupBox );
    pRightLayout->setMargin( 20 );
    int nCurRow = 0;

    QCheckBox* pCheckBoxTmp = new QCheckBox( "Main loop time trace", pGroupBox );
    pRightLayout->addWidget( pCheckBoxTmp, nCurRow, 0 );
    pCheckBoxTmp->setChecked( true );
    connect( pCheckBoxTmp, SIGNAL( toggled( bool ) ), SLOT( SlotMainLoopTraceToggled( bool ) ) );
    traceCheckBoxVector_.push_back( pCheckBoxTmp );

    nCurRow++;
    pCheckBoxTmp = new QCheckBox( "Perception time trace", pGroupBox );
    pRightLayout->addWidget( pCheckBoxTmp, nCurRow, 0 );
    pCheckBoxTmp->setChecked( true );
    connect( pCheckBoxTmp, SIGNAL( toggled( bool ) ), SLOT( SlotPerceptionTraceToggled( bool ) ) );
    traceCheckBoxVector_.push_back( pCheckBoxTmp );

    nCurRow++;
    pCheckBoxTmp = new QCheckBox( "Decision time trace", pGroupBox );
    pRightLayout->addWidget( pCheckBoxTmp, nCurRow, 0 );
    pCheckBoxTmp->setChecked( true );
    connect( pCheckBoxTmp, SIGNAL( toggled( bool ) ), SLOT( SlotDecisionTraceToggled( bool ) ) );
    traceCheckBoxVector_.push_back( pCheckBoxTmp );

    nCurRow++;    
    pCheckBoxTmp = new QCheckBox( "Action time trace", pGroupBox );
    pRightLayout->addWidget( pCheckBoxTmp, nCurRow, 0 );
    pCheckBoxTmp->setChecked( true );
    connect( pCheckBoxTmp, SIGNAL( toggled( bool ) ), SLOT( SlotActionTraceToggled( bool ) ) );
    traceCheckBoxVector_.push_back( pCheckBoxTmp );

//    resize( 600, 520 );
    setFixedSize( 600, 520 );*/

}

//-----------------------------------------------------------------------------
// Name: MOS_GraphWindow destructor
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
MOS_GraphWindow::~MOS_GraphWindow()
{
    
}

//-----------------------------------------------------------------------------
// Name: MOS_GraphWindow::AddValue
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MOS_GraphWindow::AddValues( double /*rMainLoopTime*/, double /*rPerceptionTime*/, double /*rDecisionTime*/, double /*rActionTime*/ )
{
    return;
    /*mainLoopTimeVector_  .push_back( std::make_pair( nCurrentFrame_, rMainLoopTime   ) );
    perceptionTimeVector_.push_back( std::make_pair( nCurrentFrame_, rPerceptionTime ) );
    decisionTimeVector_  .push_back( std::make_pair( nCurrentFrame_, rDecisionTime   ) );
    actionTimeVector_    .push_back( std::make_pair( nCurrentFrame_, rActionTime     ) );

    double rTmp = max( rMainLoopTime, max( rPerceptionTime, max( rDecisionTime, rActionTime ) ) );

    if( rTmp > rYMaxValue_ )
    {
        rYMaxValue_ = rTmp;
        pGraphic_->setYRange( 0, rYMaxValue_ );
    }

    if( nCurrentFrame_ > MOS_GRAPH_X_RANGE )
        pGraphic_->setXRange( nCurrentFrame_ - MOS_GRAPH_X_RANGE, nCurrentFrame_ );

    pGraphic_->setData( 0, mainLoopTimeVector_   );
    pGraphic_->setData( 1, perceptionTimeVector_ );
    pGraphic_->setData( 2, decisionTimeVector_   );
    pGraphic_->setData( 3, actionTimeVector_     );

    ++nCurrentFrame_;

    if( mainLoopTimeVector_.size() == (10 * MOS_GRAPH_X_RANGE) )
    {
        mainLoopTimeVector_  .clear();
        perceptionTimeVector_.clear();
        decisionTimeVector_  .clear();
        actionTimeVector_    .clear();
        rYMaxValue_    = 0;
        pGraphic_->setXRange( MOS_GRAPH_X_RANGE * 10, MOS_GRAPH_X_RANGE * 10 + MOS_GRAPH_X_RANGE );
    }*/
}

//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_GraphWindow::SlotMainLoopTraceToggled
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MOS_GraphWindow::SlotMainLoopTraceToggled( bool bOn )
{
//    pGraphic_->setEnabled( 0, bOn );
//    pGraphic_->repaint( false );
}


//-----------------------------------------------------------------------------
// Name: MOS_GraphWindow::SlotPerceptionTraceToggled
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MOS_GraphWindow::SlotPerceptionTraceToggled( bool bOn )
{
//    pGraphic_->setEnabled( 1, bOn );    
//    pGraphic_->repaint( false );
}

//-----------------------------------------------------------------------------
// Name: MOS_GraphWindow::SlotDecisionTraceToggled
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MOS_GraphWindow::SlotDecisionTraceToggled( bool bOn )
{
//    pGraphic_->setEnabled( 2, bOn );
//    pGraphic_->repaint( false );
}

//-----------------------------------------------------------------------------
// Name: MOS_GraphWindow::SlotActionTraceToggled
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MOS_GraphWindow::SlotActionTraceToggled( bool bOn )
{
//    pGraphic_->setEnabled( 3, bOn );
//    pGraphic_->repaint( false );
}

