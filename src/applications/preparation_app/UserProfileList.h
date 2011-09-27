// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfileList_h_
#define __UserProfileList_h_

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class UserProfile;
class UserProfileWidget;
class ProfilesModel;
class ControlsChecker_ABC;

// =============================================================================
/** @class  UserProfileList
    @brief  UserProfileList
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfileList : public Q3VBox
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< UserProfile >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UserProfileList( QWidget* parent, UserProfileWidget& pages, kernel::Controllers& controllers,
                              ProfilesModel& model, ControlsChecker_ABC& checker );
    virtual ~UserProfileList();
    //@}

    //! @name Operations
    //@{
    void Save();
    void Cancel();
    //@}

signals:
    void DoConsistencyCheck( unsigned int );

private slots:
    //! @name Slots
    //@{
    void OnSelectionChanged();
    void OnCreate();
    void OnDelete();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfileList( const UserProfileList& );            //!< Copy constructor
    UserProfileList& operator=( const UserProfileList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const UserProfile& profile );
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );
    virtual void showEvent( QShowEvent* event );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const UserProfile* >            T_Profiles;
    typedef std::map< const UserProfile*, UserProfile* > T_ProfileEditors;
    typedef T_ProfileEditors::const_iterator           CIT_ProfileEditors;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    ProfilesModel&       model_;
    ControlsChecker_ABC& checker_;
    T_Profiles           profiles_;
    T_ProfileEditors     editors_;
    UserProfileWidget&   pages_;
    Q3ListBox*           list_;
    //@}
};

#endif // __UserProfileList_h_
