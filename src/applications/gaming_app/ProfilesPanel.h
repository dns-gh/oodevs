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
#include <boost/noncopyable.hpp>
#include <vector>

namespace kernel
{
    class Controllers;
}

class UserProfile;
class Profile;

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
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfilesPanel( QMainWindow* mainWindow, kernel::Controllers& controllers );
    virtual ~ProfilesPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyCreated( const UserProfile& profile );
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );

    virtual void NotifyUpdated( const Profile& profile );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const UserProfile* > T_UserProfiles;
    typedef T_UserProfiles::const_iterator  CIT_UserProfiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QTableView* tableView_;
    QSortFilterProxyModel* proxyModel_;
    QStandardItemModel* dataModel_;
    T_UserProfiles profiles_;
    QIcon star_;
    QIcon profile_;
    QString current_;
    //@}
};

#endif // __ProfilesPanel_h_
