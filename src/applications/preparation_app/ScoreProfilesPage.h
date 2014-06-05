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

#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class UserProfile_ABC;
}

class ProfileSelection;

// =============================================================================
/** @class  ScoreProfilesPage
    @brief  ScoreProfilesPage
*/
// Created: SBO 2011-05-16
// =============================================================================
class ScoreProfilesPage : public Q3VBox
                        , public tools::Observer_ABC
                        , public tools::ElementObserver_ABC< kernel::UserProfile_ABC >
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
    std::unique_ptr< ProfileSelection > CreateResult() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::UserProfile_ABC& profile );
    virtual void NotifyUpdated( const kernel::UserProfile_ABC& profile );
    virtual void NotifyDeleted( const kernel::UserProfile_ABC& profile );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QStandardItemModel* model_;
    //@}
};

#endif // __ScoreProfilesPage_h_
