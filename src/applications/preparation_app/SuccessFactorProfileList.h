// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __SuccessFactorProfileList_h_
#define __SuccessFactorProfileList_h_

#include "clients_gui/RichWidget.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class UserProfile_ABC;
}

class ProfileSelection;

// =============================================================================
/** @class  SuccessFactorProfileList
    @brief  SuccessFactorProfileList
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorProfileList : public gui::RichWidget< QListWidget >
                               , public tools::Observer_ABC
                               , public tools::ElementObserver_ABC< kernel::UserProfile_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    SuccessFactorProfileList( const QString& objectName, kernel::Controllers& controllers );
    virtual ~SuccessFactorProfileList();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const ProfileSelection& profiles );
    void CommitTo( ProfileSelection& profiles ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Select( const kernel::UserProfile_ABC& profile );
    virtual void NotifyCreated( const kernel::UserProfile_ABC& profile );
    virtual void NotifyUpdated( const kernel::UserProfile_ABC& profile );
    virtual void NotifyDeleted( const kernel::UserProfile_ABC& profile );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::UserProfile_ABC* > T_Profiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Profiles profiles_;
    //@}
};

#endif // __SuccessFactorProfileList_h_
