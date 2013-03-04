// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __SIMControlToolbar_h_
#define __SIMControlToolbar_h_

#include "clients_gui/Types.h"
#include "gaming/Simulation.h"
#include "tools/ElementObserver_ABC.h"
#include "clients_gui/RichToolBar.h"
#include "clients_gui/RichAction.h"

#pragma warning( push, 0 )
#include <QtGui/qicon.h>
#pragma warning( pop )

class QToolButton;
class QSpinBox;

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Logger_ABC;
}

class ConnectDialog;
class DisconnectDialog;
class Services;
class Network;
class Publisher_ABC;

// =============================================================================
/** @class  SIMControlToolbar
    @brief  SIMControlToolbar
*/
// Created: AGE 2007-04-11
// =============================================================================
class SIMControlToolbar : public gui::RichToolBar
                        , public tools::ElementObserver_ABC< Simulation >
                        , public tools::ElementObserver_ABC< Services >
                        , public tools::ElementObserver_ABC< kernel::Profile_ABC >
                        , public tools::ElementObserver_ABC< Simulation::sStartTick >
                        , public tools::ElementObserver_ABC< Simulation::sEndTick >
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             SIMControlToolbar( QMainWindow* pParent, kernel::Controllers& controllers, Network& network, Publisher_ABC& publisher, kernel::Logger_ABC& logger );
    virtual ~SIMControlToolbar();
    //@}

public slots:
    //! @name
    //@{
    void SlotSpeedChange();
    void SlotNamedCheckPoint();
    //@}

private slots:
    //! @name Slots
    //@{
    void SlotConnectDisconnect();
    void SlotPlayPause();
    void SlotStep();
    void SlotOnSpinBoxChange( int );
    void SlotCheckpoint();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SIMControlToolbar( const SIMControlToolbar& );
    SIMControlToolbar& operator=( const SIMControlToolbar& );
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Services& services );
    virtual void NotifyUpdated( const kernel::Profile_ABC& profile );
    virtual void NotifyUpdated( const Simulation::sStartTick& startTick );
    virtual void NotifyUpdated( const Simulation::sEndTick& endTick );
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    void RequestCheckpoint( const std::string& name );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers&        controllers_;
    Publisher_ABC&      publisher_;
    QToolButton*        pConnectButton_;
    QToolButton*        pPlayButton_;
    QToolButton*        pStepButton_;
    QToolButton*        pSpeedButton_;
    QSpinBox*           pSpeedSpinBox_;
    QToolButton*        pCheckpointButton_;
    QAction*            pCheckpointAction_ ;
    ConnectDialog*      pConnectDlg_;
    DisconnectDialog*   pDisconnectDlg_;
    int speed_;
    unsigned int tickCount_;
    bool connected_, paused_;
    bool hasReplay_, hasSimulation_;
    bool gamingPaused_;
    bool replayStepMode_;

    QIcon connectPix_, disconnectPix_;
    QIcon playPix_, stopPix_;
    //@}
};

#endif // __SIMControlToolbar_h_