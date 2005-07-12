//*****************************************************************************
//
// $Created: FBD 02-11-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Scroller.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_Scroller.cpp $
//
//*****************************************************************************
#include "MOS_Light_Pch.h"

#include "MOS_Scroller.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Scroller.inl"
#endif
#include "resources.h"

#include <QPushButton.h>
#include <QToolButton.h>
#include <QLayout.h>

//-----------------------------------------------------------------------------
// Name: MOS_Scroller constructor
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
MOS_Scroller::MOS_Scroller( QWidget* pParent, QGLWidget* pGLWidget )
:   QButtonGroup( pParent )
{

    QToolButton* pLocToolButton = new QToolButton( this );
    pLocToolButton->setAccel( Key_Space );
    pLocToolButton->setIconSet( MAKE_ICON( l_black ) );


    QToolButton* pLeftToolButton = new QToolButton( this );
    pLeftToolButton->setAccel( Key_Left );
    pLeftToolButton->setAutoRepeat( true );
    pLeftToolButton->setIconSet( MAKE_ICON( a_left ) );

    QToolButton* pRightToolButton = new QToolButton( this );
    pRightToolButton->setAccel( Key_Right );
    pRightToolButton->setAutoRepeat( true );
    pRightToolButton->setIconSet( MAKE_ICON( a_right ) );

    QToolButton* pUpToolButton = new QToolButton( this );
    pUpToolButton->setAccel( Key_Up );
    pUpToolButton->setAutoRepeat( true );
    pUpToolButton->setIconSet( MAKE_ICON( a_up ) );

    QToolButton* pDownToolButton = new QToolButton( this );
    pDownToolButton->setAccel( Key_Down );
    pDownToolButton->setAutoRepeat( true );
    pDownToolButton->setIconSet( MAKE_ICON( a_down ) );

    QGridLayout* pGrid = new QGridLayout( this, 3, 3 );
    pGrid->addWidget( pLocToolButton    , 1, 1 );
    pGrid->addWidget( pLeftToolButton   , 1, 0 );
    pGrid->addWidget( pRightToolButton  , 1, 2 );
    pGrid->addWidget( pUpToolButton     , 0, 1 );
    pGrid->addWidget( pDownToolButton   , 2, 1 );

    connect( pLocToolButton, SIGNAL( clicked() ), pGLWidget, SLOT( SlotSetCenter()) );
    connect( pLeftToolButton, SIGNAL( clicked() ), pGLWidget, SLOT( SlotSetXTranslationLeft()) );
    connect( pRightToolButton, SIGNAL( clicked() ), pGLWidget, SLOT( SlotSetXTranslationRight()) );
    connect( pUpToolButton, SIGNAL( clicked() ), pGLWidget, SLOT( SlotSetYTranslationUp()) );
    connect( pDownToolButton, SIGNAL( clicked() ), pGLWidget, SLOT( SlotSetYTranslationDown()) );
}


//-----------------------------------------------------------------------------
// Name: MOS_Scroller destructor
// Created: FBD 02-11-29
//-----------------------------------------------------------------------------
MOS_Scroller::~MOS_Scroller()
{
    
}
