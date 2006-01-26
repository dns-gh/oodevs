//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Light2_Pch.h $
// $Author: Age $
// $Modtime: 31/03/05 16:18 $
// $Revision: 5 $
// $Workfile: MOS_Light2_Pch.h $
//
//*****************************************************************************

#ifndef __MOS_Pch_h_
#define __MOS_Pch_h_

#pragma warning( disable : 4290 )
#pragma warning( disable : 4702 )

#include "MT/MT_Tools/MT_Tools_lib.h"
#include "MT/MT_IO/MT_IO_lib.h"
#include "MT/MT_Archive/MT_Archive_lib.h"
#include "MT/MT_Logger/MT_Logger_lib.h"
#include "MT/MT_Time/MT_Time_lib.h"
#include "MT/MT_XmlTools/MT_XmlTools_lib.h"
#include "MT/MT_Qt/MT_Qt_lib.h"

#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceServerUserCbk.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceClientUserCbk.h"

#include "MT_Tools/MT_Assert.h"
#include "MT_Tools/MT_Vector2D.h"
#include "MT_Tools/MT_Vector3D.h"
#include "MT_Tools/MT_Line.h"
#include "MT_Tools/MT_Rect.h"
#include "MT_Tools/MT_InputArchive_Logger.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qgroupbox.h>
#include <qframe.h>
#include <qspinbox.h>
#include <qaction.h>
#include <qgl.h>
#include <qradiobutton.h>
#include <qmainwindow.h>
#include <qdialog.h>
#include <qtoolbar.h>
#include <qsignalmapper.h> 
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtextedit.h>
#include <qtimer.h>
#include <qstatusbar.h>
#include <qsettings.h>
#include <qheader.h>
#include <qfile.h>
#include <qobject.h>
#include <qlistview.h>
#include <qfont.h>
#include <qfontmetrics.h>
#include <qsplashscreen.h>
#include <qtabwidget.h>
#include <qlineedit.h>
#include <qcheckbox.h>
#include <qwidgetstack.h>
#include <qvbox.h>
#include <qdialog.h>
#include <qtextedit.h>
#include <qheader.h>
#include <qdial.h>
#include <qhbox.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qtooltip.h>
#include <qdragobject.h> 
#include <qcheckbox.h>
#include <qsimplerichtext.h>
#include <qhbox.h>
#include <qdial.h>
#include <qgroupbox.h>
#include <qslider.h>
#include <qdatetimeedit.h>
#include <qbuttongroup.h>
#include <qstring.h>
#include <qiconset.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qmainwindow.h>
#include <qdockwindow.h>
#include <qsound.h>
#include <stdexcept>

#include "GFX/GFX_Tools.h"
#include "GFX/GFX_Text.h"
#include "GFX/GFX_Color.h"

#ifdef min
#   undef min
#endif

#ifdef max
#   undef max
#endif

#include "resources.h"

#include "ENT/ENT_Tr.h"

#define RUNTIME_ERROR { std::stringstream str; str << "Runtime error in function " __FUNCTION__ ", file : " __FILE__ ", line : " << __LINE__ << "."; throw std::runtime_error( str.str().c_str() ); }

#define APP_NAME    "ASTEC"
#define APP_VERSION "beta-1"


#endif // __MOS_Pch_h_
