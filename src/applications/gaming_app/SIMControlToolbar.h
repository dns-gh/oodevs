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
    class Profile_ABC;
}

class ConnectDialog;
class DisconnectDialog;
class QMainWindow;
class Simulation;
class Network;
class Publisher_ABC;

// =============================================================================
/** @class  SIMControlToolbar
    @brief  SIMControlToolbar
*/
// Created: AGE 2007-04-11
// =============================================================================
class SIMControlToolbar : public QToolBar
                        , public kernel::Observer_ABC
                        , public kernel::ElementObserver_ABC< Simulation >
                        , public kernel::ElementObserver_ABC< kernel::Profile_ABC >
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             SIMControlToolbar( QMainWindow* pParent, kernel::Controllers& controllers, Network& network, Publisher_ABC& publisher );
    virtual ~SIMControlToolbar();
    //@}

public slots:
    //! @name 
    //@{
    void SlotSpeedChange();
    //@}

private slots:
    //! @name Slots
    //@{
    void SlotConnectDisconnect();
    void SlotPlayPause();
    void SlotOnSpinBoxChange( int );
    void SlotCheckpoint();
    void SlotNamedCheckPoint();
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
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    void RequestCheckpoint( const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&        controllers_;
    Publisher_ABC&      publisher_;
    QToolButton*        pConnectButton_;
    QToolButton*        pPlayButton_;
    QToolButton*        pSpeedButton_;
    QSpinBox*           pSpeedSpinBox_;
    QToolButton*        pCheckpointButton_;
    ConnectDialog*      pConnectDlg_;
    DisconnectDialog*   pDisconnectDlg_;
    int speed_;
    bool connected_, paused_;

    QIconSet connectedPix_, disconnectedPix_;
    QIconSet playPix_, stopPix_;
    //@}
};

#endif // __SIMControlToolbar_h_