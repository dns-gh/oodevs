//*****************************************************************************
//
// $Created: FBD 03-01-14 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/SIMControlToolbar.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:28 $
// $Revision: 2 $
// $Workfile: SIMControlToolbar.h $
//
//*****************************************************************************

#ifndef __SIMControlToolbar_h_
#define __SIMControlToolbar_h_

#include "Types.h"

class ConnectDialog;
class DisconnectDialog;
class QMainWindow;
class MT_SpinBox;

//*****************************************************************************
// Created: FBD 03-01-14
//*****************************************************************************
class SIMControlToolbar : public QToolBar
{
    Q_OBJECT
    MT_COPYNOTALLOWED( SIMControlToolbar );

public:
    //! @name Constructor/Destructor
    //@{
    explicit SIMControlToolbar( QMainWindow* pParent );
    virtual ~SIMControlToolbar();
    //@}

protected slots:
    //! @name Slots
    //@{
    void SlotConnectDisconnect();
    void SlotPlayPause();
    void SlotSpeedChange();
    void SlotOnSpinBoxChange();
    void SlotOnSpinBoxEnterPressed();
    void SlotOnConnexionStatusChanged( bool bConnected );
    void SlotOnPauseStatusChanged( bool bPaused );
    void SlotOnSpeedChanged( int nSpeed );
    //@}

private:
    QToolButton*            pConnectButton_;
    QToolButton*            pPlayButton_;
    QToolButton*            pSpeedButton_;
    MT_SpinBox*             pSpeedSpinBox_;
    ConnectDialog*      pConnectDlg_;
    DisconnectDialog*   pDisconnectDlg_;
};

#endif // __SIMControlToolbar_h_