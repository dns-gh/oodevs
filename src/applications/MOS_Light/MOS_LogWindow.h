
//****************************************************************************
//
// $Created:  NLD 2002-01-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LogWindow.h $
// $Author: Nld $
// $Modtime: 21/02/03 10:10 $
// $Revision: 2 $
// $Workfile: MOS_LogWindow.h $
//
//*****************************************************************************

#ifndef __MOS_LogWindow_h_
#define __MOS_LogWindow_h_

#ifdef __GNUG__
#	pragma interface
#endif

#include "MOS_Types.h"

#include "MT/MT_Logger/MT_Logger_ABC.h"

#include <qlistview.h>

class QPopupMenu;
class QListViewItem;

//=============================================================================
// Created:  NLD 2002-01-07 
//=============================================================================
class MOS_LogWindow : public QListView, public MT_Logger_ABC
{
    MT_COPYNOTALLOWED( MOS_LogWindow )
    Q_OBJECT

public:
    MOS_LogWindow( QWidget* pParent = 0, uint nLogLevels = eLogLevel_All, uint nLayers = eLogLayer_All );
    virtual ~MOS_LogWindow();

protected:
    //-------------------------------------------------------------------------
    /** @name Log methods */
    //-------------------------------------------------------------------------
    //@{
    virtual void LogString( const char* strLayerName, E_LogLevel nLevel, const char* szMsg, const char* strContext, int nCode );
    //@}

    //-------------------------------------------------------------------------
    /** @name QT events */
    //-------------------------------------------------------------------------
    //@{
    void hideEvent( QHideEvent* pEvent );
    //@}
    
protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotRightButtonPressed( QListViewItem *, const QPoint &, int );
    void SlotSetAutoScroll     ();
    //@}

private:
    //-------------------------------------------------------------------------
    /** @name Properties */
    //-------------------------------------------------------------------------
    //@{
    QPopupMenu* pMenu_;
    bool        bAutoScroll_;
    int			nNbrLinesMax_;
    //@}
};

#endif // __MOS_LogWindow_h_
