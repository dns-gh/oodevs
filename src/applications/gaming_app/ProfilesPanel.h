// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ProfilesPanel_h_
#define __ProfilesPanel_h_

#include "clients_gui/RichDockWidget.h"
#include "gaming/AvailableProfile.h"
#include "gaming/Simulation.h"
#include <tools/ElementObserver_ABC.h>
#include <vector>

namespace kernel
{
    class Controllers;
    class UserProfile_ABC;
}

class Profile;
class ProfileFilter;
class Network;
class TeamsModel;

// =============================================================================
/** @class  ProfilesPanel
    @brief  Profiles panel
*/
// Created: LGY 2011-11-15
// =============================================================================
class ProfilesPanel : public gui::RichDockWidget
                    , public tools::ElementObserver_ABC< kernel::UserProfile_ABC >
                    , public tools::ElementObserver_ABC< Profile >
                    , public tools::ElementObserver_ABC< AvailableProfile >
                    , public tools::ElementObserver_ABC< Simulation >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfilesPanel( QMainWindow* mainWindow, kernel::Controllers& controllers, Network& network, ProfileFilter& filter, TeamsModel& teams );
    virtual ~ProfilesPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::UserProfile_ABC& profile );
    virtual void NotifyUpdated( const kernel::UserProfile_ABC& profile );
    virtual void NotifyDeleted( const kernel::UserProfile_ABC& profile );

    virtual void NotifyUpdated( const Profile& profile );
    virtual void NotifyUpdated( const AvailableProfile& profile );
    virtual void NotifyUpdated( const Simulation& simulation );
    //@}

private slots:
    //! @name Slots
    //@{
    void Reconnect();
    void Filter();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const kernel::UserProfile_ABC* > T_UserProfiles;
    //@}

private:
    //! @name Helpers
    //@{
    void Clean();
    void AddProfile( unsigned int index );
    template< typename T >
    void UpdateProfile( unsigned int index, const T& profile );
    //@}

private:
    //! @name Member data
    //@{
    Network& network_;
    ProfileFilter& filter_;
    TeamsModel& teams_;
    QTableView* tableView_;
    QSortFilterProxyModel* proxyModel_;
    QStandardItemModel* dataModel_;
    T_UserProfiles profiles_;
    QIcon star_;
    QIcon lock_;
    QIcon profile_;
    QIcon red_;
    QIcon green_;
    QString current_;
    //@}
};

#endif // __ProfilesPanel_h_
