// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ScoreProfilesPage_h_
#define __ScoreProfilesPage_h_

#include "tools/ElementObserver_ABC.h"
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
}

class ProfileSelection;
class UserProfile;
class QCheckListItem;

// =============================================================================
/** @class  ScoreProfilesPage
    @brief  ScoreProfilesPage
*/
// Created: SBO 2011-05-16
// =============================================================================
class ScoreProfilesPage : public Q3VBox
                        , private boost::noncopyable
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< UserProfile >
{

public:
    //! @name Constructors/Destructor
    //@{
             ScoreProfilesPage( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ScoreProfilesPage();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const ProfileSelection& profiles );
    std::auto_ptr< ProfileSelection > CreateResult() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const UserProfile& profile );
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );
    //@}

    //! @name Types
    //@{
    typedef std::map< const UserProfile*, Q3CheckListItem* > T_Profiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Profiles profiles_;
    Q3ListView* list_;
    //@}
};

#endif // __ScoreProfilesPage_h_
