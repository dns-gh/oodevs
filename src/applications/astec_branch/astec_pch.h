//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Light2_Pch.h $
// $Author: Age $
// $Modtime: 31/03/05 16:18 $
// $Revision: 5 $
// $Workfile: Light2_Pch.h $
//
//*****************************************************************************

#ifndef __Pch_h_
#define __Pch_h_

#pragma warning( disable : 4290 )
#pragma warning( disable : 4702 )

#define WIN32_LEAN_AND_MEAN
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
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <set>

#ifdef min
#   undef min
#endif

#ifdef max
#   undef max
#endif

#include "geometry/Types.h"
typedef std::vector< geometry::Point2f > T_PointVector; // $$$$ AGE 2006-03-16: 
typedef T_PointVector::iterator         IT_PointVector;
typedef T_PointVector::const_iterator  CIT_PointVector;

#include "resources.h"

#include "ENT/ENT_Tr.h"

#define RUNTIME_ERROR { std::stringstream str; str << "Runtime error in function " __FUNCTION__ ", file : " __FILE__ ", line : " << __LINE__ << "."; throw std::runtime_error( str.str().c_str() ); }

#define APP_NAME    "ASTEC - EM"
#define APP_VERSION "beta-1"


#endif // __Pch_h_
