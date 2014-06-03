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

#include "preparation/UserProfile.h"
#include <boost/noncopyable.hpp>

class ProfilesModel;

// =============================================================================
/** @class  ProfileEditor
    @brief  Profile editor
*/
// Created: SBO 2007-11-07
// =============================================================================
class ProfileEditor : public UserProfile
                    , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileEditor( const QString& login, kernel::Controller& controller, const Model& model );
    explicit ProfileEditor( UserProfile& profile );
    virtual ~ProfileEditor();
    //@}

    //! @name Operations
    //@{
    void Commit( ProfilesModel& model, kernel::Controller& controller );
    void Delete();
    bool IsDeleted() const;
    //@}

private:
    //! @name Member data
    //@{
    UserProfile* originalProfile_;
    bool deleted_;
    //@}
};

#endif // __ProfileEditor_h_
