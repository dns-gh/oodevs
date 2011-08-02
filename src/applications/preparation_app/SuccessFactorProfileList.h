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

#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class ProfileSelection;
class UserProfile;

// =============================================================================
/** @class  SuccessFactorProfileList
    @brief  SuccessFactorProfileList
*/
// Created: SBO 2009-06-15
// =============================================================================
class SuccessFactorProfileList : public Q3ListBox
                               , public tools::Observer_ABC
                               , public tools::ElementObserver_ABC< UserProfile >
{
public:
    //! @name Constructors/Destructor
    //@{
             SuccessFactorProfileList( QWidget* parent, kernel::Controllers& controllers );
    virtual ~SuccessFactorProfileList();
    //@}

    //! @name Operations
    //@{
    void StartEdit( const ProfileSelection& profiles );
    void CommitTo( ProfileSelection& profiles ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SuccessFactorProfileList( const SuccessFactorProfileList& );            //!< Copy constructor
    SuccessFactorProfileList& operator=( const SuccessFactorProfileList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Select( const UserProfile& profile );
    virtual void NotifyCreated( const UserProfile& profile );
    virtual void NotifyUpdated( const UserProfile& profile );
    virtual void NotifyDeleted( const UserProfile& profile );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const UserProfile* > T_Profiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Profiles profiles_;
    //@}
};

#endif // __SuccessFactorProfileList_h_
