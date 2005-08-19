//****************************************************************************
//
// This file is part of MASA DIN library.
// 
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//****************************************************************************
//
// $Created:  NLD 2002-01-02 $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "resources.h"

#include <qiconset.h>
#include <qpixmap.h>

// $$$$ CBX 01-06-26: Hack to encapsulate the xpm resources in a resources.cpp 
// $$$$ CBX 01-06-26: instead of resources.h
#define static 

//#include "res/FileOpen.xpm"
//#include "res/zoomin.xpm"
//#include "res/zoomout.xpm"
//#include "res/pause.xpm"
//#include "res/play.xpm"
//#include "res/stop.xpm"
#include "res/l_france.xpm"
#include "res/l_red.xpm"
#include "res/l_green.xpm"
#include "res/l_white.xpm"
#include "res/l_play.xpm"
#include "res/l_stop.xpm"
#include "res/a_down.xpm"
#include "res/a_left.xpm"
#include "res/a_right.xpm"
#include "res/a_up.xpm"
#include "res/l_black.xpm"
#include "res/affinity.xpm"

QIconSet MakeExtendedIcon( const char* pEnabledXpm[], const char* pDisabledXpm[] )
{
    QPixmap enabledPixmap ( pEnabledXpm );
    QPixmap disabledPixmap( pDisabledXpm );

    QIconSet iconset  ( enabledPixmap );
    QIconSet iconset_x( disabledPixmap );

    iconset.setPixmap( iconset_x.pixmap(), QIconSet::Automatic, QIconSet::Disabled );

    return iconset;
}