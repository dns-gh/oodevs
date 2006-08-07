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

#include "astec_gaming/Types.h"
#include "astec_kernel/ElementObserver_ABC.h"

class ConnectDialog;
class DisconnectDialog;
class QMainWindow;
class MT_SpinBox;
class Simulation;
class Controllers;
class Network;
class Publisher_ABC;

//*****************************************************************************
// Created: FBD 03-01-14
//*****************************************************************************
class SIMControlToolbar : public QToolBar
                        , public Observer_ABC
                        , public ElementObserver_ABC< Simulation >
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             SIMControlToolbar( QMainWindow* pParent, Controllers& controllers, Network& network, Publisher_ABC& publisher );
    virtual ~SIMControlToolbar();
    //@}

protected slots:
    //! @name Slots
    //@{
    void SlotConnectDisconnect();
    void SlotPlayPause();
    void SlotSpeedChange();
    void SlotOnSpinBoxChange( int );
    void SlotOnSpinBoxEnterPressed();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SIMControlToolbar( const SIMControlToolbar& );
    SIMControlToolbar& operator=( const SIMControlToolbar& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    //@}

private:
    //! @name Member data
    //@{
    Controllers&        controllers_;
    Publisher_ABC&      publisher_;
    QToolButton*        pConnectButton_;
    QToolButton*        pPlayButton_;
    QToolButton*        pSpeedButton_;
    MT_SpinBox*         pSpeedSpinBox_; // $$$$ AGE 2006-03-24: MT_Caca
    ConnectDialog*      pConnectDlg_;
    DisconnectDialog*   pDisconnectDlg_;
    int speed_;
    bool connected_, paused_;
    //@}
};

#endif // __SIMControlToolbar_h_