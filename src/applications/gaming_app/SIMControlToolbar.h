// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SIMControlToolbar_h_
#define __SIMControlToolbar_h_

#include "clients_gui/Types.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class MT_SpinBox;
}

class ConnectDialog;
class DisconnectDialog;
class QMainWindow;
class Simulation;
class Network;
class Publisher_ABC;

//*****************************************************************************
// Created: FBD 03-01-14
//*****************************************************************************
class SIMControlToolbar : public QToolBar
                        , public kernel::Observer_ABC
                        , public kernel::ElementObserver_ABC< Simulation >
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             SIMControlToolbar( QMainWindow* pParent, kernel::Controllers& controllers, Network& network, Publisher_ABC& publisher );
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
    kernel::Controllers&        controllers_;
    Publisher_ABC&      publisher_;
    QToolButton*        pConnectButton_;
    QToolButton*        pPlayButton_;
    QToolButton*        pSpeedButton_;
    gui::MT_SpinBox*         pSpeedSpinBox_; // $$$$ AGE 2006-03-24: MT_Caca
    ConnectDialog*      pConnectDlg_;
    DisconnectDialog*   pDisconnectDlg_;
    int speed_;
    bool connected_, paused_;
    //@}
};

#endif // __SIMControlToolbar_h_