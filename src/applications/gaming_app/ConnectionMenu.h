// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ConnectionMenu_h_
#define __ConnectionMenu_h_

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Logger_ABC;
    class Controllers;
}

class Network;
class ConnectDialog;
class DisconnectDialog;
class Simulation;

// =============================================================================
/** @class  ConnectionMenu
    @brief  ConnectionMenu
*/
// Created: SBO 2008-09-18
// =============================================================================
class ConnectionMenu : public QObject
                     , public tools::Observer_ABC
                     , public tools::ElementObserver_ABC< Simulation >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ConnectionMenu( Q3PopupMenu* parent, kernel::Controllers& controllers, Network& network, kernel::Logger_ABC& logger );
    virtual ~ConnectionMenu();
    //@}

private slots:
    //! @name Slots
    //@{
    void Connect();
    void Disconnect();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ConnectionMenu( const ConnectionMenu& );            //!< Copy constructor
    ConnectionMenu& operator=( const ConnectionMenu& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Q3PopupMenu* menu_;
    Q3PopupMenu* hosts_;
    ConnectDialog* connectDialog_;
    DisconnectDialog* disconnectDialog_;
    int connectItem_, disconnectItem_, recentItem_;
    //@}
};

#endif // __ConnectionMenu_h_
