// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileEditor_h_
#define __ProfileEditor_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class UserProfile_ABC;
}

class ProfilesModel;

// =============================================================================
/** @class  ProfileEditor
    @brief  Profile editor
*/
// Created: SBO 2007-11-07
// =============================================================================
class ProfileEditor : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileEditor( kernel::UserProfile_ABC* profile, kernel::UserProfile_ABC* originalProfile );
    virtual ~ProfileEditor();
    //@}

    //! @name Operations
    //@{
    void Delete();
    bool IsDeleted() const;
    // todo ne pas faire de getter
    kernel::UserProfile_ABC& GetProfile();
    kernel::UserProfile_ABC* GetOriginalProfile() const;
    void NotifyOriginalProfileDeleted();
    void NotifyOriginalProfileCreated( kernel::UserProfile_ABC* profile );
    //@}

private:
    //! @name Member data
    //@{
    std::unique_ptr< kernel::UserProfile_ABC > profile_;
    kernel::UserProfile_ABC* originalProfile_;
    bool deleted_;
    //@}
};

#endif // __ProfileEditor_h_
