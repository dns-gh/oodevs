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

#include "tools/ElementObserver_ABC.h"
#include "gaming/AvailableProfile.h"
#include "gaming/Simulation.h"
#include <boost/noncopyable.hpp>
#include <vector>

namespace kernel
{
    class Controllers;
}

class UserProfile;
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
class ProfilesPanel : public QDockWidget
                    , private boost::noncopyable
                    , public tools::Observer_ABC
                    , public tools::ElementObserver_ABC< UserProfile >
                    , public tools::ElementObserver_ABC< Profile >
                    , public tools::ElementObserver_ABC< AvailableProfile >
                    , public tools::ElementObserver_ABC< Simulation >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfilesPanel( QMainWindow* mainWindow, kernel::Controllers& controllers,
                            Network& network, ProfileFilter& filter, TeamsModel& teams );
    virtual ~ProfilesPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyCreated( const UserProfile& profile );
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );

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
    typedef std::vector< const UserProfile* > T_UserProfiles;
    typedef T_UserProfiles::const_iterator  CIT_UserProfiles;
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
    kernel::Controllers& controllers_;
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
