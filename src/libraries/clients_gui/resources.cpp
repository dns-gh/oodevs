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

#include "clients_gui_pch.h"
#include "resources.h"

// Hack to encapsulate the xpm resources in a resources.cpp
// instead of resources.h
#define static

#include "res/csword.xpm"
#include "res/connected.xpm"
#include "res/connecting.xpm"
#include "res/notconnected.xpm"
#include "res/play.xpm"
#include "res/step.xpm"
#include "res/stop.xpm"
#include "res/go_to_start.xpm"
#include "res/go_to_end.xpm"
#include "res/textsmall.xpm"
#include "res/textbig.xpm"
#include "res/crossings.xpm"
#include "res/weather.xpm"
#include "res/embraye.xpm"
#include "res/debraye.xpm"
#include "res/tickon.xpm"
#include "res/tickoff.xpm"
#include "res/tickred.xpm"
#include "res/msg.xpm"
#include "res/sim.xpm"
#include "res/simdbg.xpm"
#include "res/open.xpm"
#include "res/save.xpm"
#include "res/saveas.xpm"
#include "res/new.xpm"
#include "res/ammo.xpm"
#include "res/construction.xpm"
#include "res/observe.xpm"
#include "res/tacticallines.xpm"
#include "res/flare.xpm"
#include "res/smoke.xpm"
#include "res/agentmessage.xpm"
#include "res/messageoptions.xpm"
#include "res/conflict.xpm"
#include "res/threed.xpm"
#include "res/twodnoterrain.xpm"
#include "res/visible.xpm"
#include "res/select.xpm"
#include "res/point.xpm"
#include "res/line.xpm"
#include "res/circle.xpm"
#include "res/rectangle.xpm"
#include "res/polygon.xpm"
#include "res/text.xpm"
#include "res/image.xpm"
#include "res/empty.xpm"
#include "res/cross.xpm"
#include "res/saveall.xpm"
#include "res/autolog.xpm"
#include "res/realtime.xpm"
#include "res/cadenas_ouvert.xpm"
#include "res/talkie.xpm"
#include "res/aggregate.xpm"
#include "res/desaggregate.xpm"
#include "res/recrec.xpm"
#include "res/recplay.xpm"
#include "res/recstop.xpm"
#include "res/curve.xpm"
#include "res/search.xpm"
#include "res/plus.xpm"
#include "res/minus.xpm"
#include "res/lock.xpm"
#include "res/scisors.xpm" // LTO
#include "res/commandpost.xpm"
#include "res/cancel.xpm"
#include "res/option_general.xpm"
#include "res/check.xpm"
#include "res/check_grey.xpm"
#include "res/profiles.xpm"
#include "res/profile.xpm"
#include "res/left_arrow.xpm"
#include "res/right_arrow.xpm"
#include "res/filter.xpm"
#include "res/pencil.xpm"
#include "res/pencil_checkbox.xpm"
#include "res/goto.xpm"
#include "res/add_point.xpm"
#include "res/special_point.xpm"
#include "res/drawings.xpm"
#include "res/arrow_up.xpm"
#include "res/arrow_down.xpm"
#include "res/checkbox_on.xpm"
#include "res/checkbox_off.xpm"
#include "res/trash.xpm"
#include "res/copy.xpm"
#include "res/refresh.xpm"
#include "res/dnd_lock.xpm"
#include "res/dnd_unlock.xpm"

#include "cursors/circle_cursor.xpm"
#include "cursors/line_cursor.xpm"
#include "cursors/path_cursor.xpm"
#include "cursors/pen_cursor.xpm"
#include "cursors/point_cursor.xpm"
#include "cursors/polygon_cursor.xpm"
#include "cursors/rectangle_cursor.xpm"
#include "cursors/text_cursor.xpm"

QIcon MakeExtendedIcon( const char* pEnabledXpm[], const char* pDisabledXpm[] )
{
    QPixmap enabledPixmap ( pEnabledXpm );
    QPixmap disabledPixmap( pDisabledXpm );

    QIcon iconset  ( enabledPixmap );
    QIcon iconset_x( disabledPixmap );

    iconset.setPixmap( iconset_x.pixmap(), QIcon::Automatic, QIcon::Disabled );

    return iconset;
}
