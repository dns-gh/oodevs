//*****************************************************************************
//
// $Created: FBD 03-01-14 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_SIMControlToolbar.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:28 $
// $Revision: 2 $
// $Workfile: MOS_SIMControlToolbar.h $
//
//*****************************************************************************

#ifndef __MOS_SIMControlToolbar_h_
#define __MOS_SIMControlToolbar_h_

#include "MOS_Types.h"

class MOS_ConnectDialog;
class MOS_DisconnectDialog;
class QMainWindow;

//*****************************************************************************
// Created: FBD 03-01-14
//*****************************************************************************
class MOS_SIMControlToolbar : public QToolBar
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MOS_SIMControlToolbar );

public:
    explicit MOS_SIMControlToolbar( QMainWindow* pParent );
    virtual ~MOS_SIMControlToolbar();

protected slots:
    //-------------------------------------------------------------------------
    /** @name Slots */
    //-------------------------------------------------------------------------
    //@{
    void SlotConnectDisconnect();
    void SlotPlayPause();
    void SlotSpeedChange();
    void SlotOnSpinBoxChange();
    void SlotOnConnexionStatusChanged( bool bConnected );
    void SlotOnPauseStatusChanged( bool bPaused );
    void SlotOnSpeedChanged( int nSpeed );
    //@}

private:
    QToolButton*            pConnectButton_;
    QToolButton*            pPlayButton_;
    QToolButton*            pSpeedButton_;
    QSpinBox*               pSpeedSpinBox_;
    MOS_ConnectDialog*      pConnectDlg_;
    MOS_DisconnectDialog*   pDisconnectDlg_;
};

#ifdef MOS_USE_INLINE
#	include "MOS_SIMControlToolbar.inl"
#endif


#endif // __MOS_SIMControlToolbar_h_