// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AvailableProfile_h_
#define __AvailableProfile_h_

namespace sword
{
    class ProfileDescription;
}

// =============================================================================
/** @class  AvailableProfile
    @brief  AvailableProfile
*/
// Created: SBO 2009-12-18
// =============================================================================
class AvailableProfile
{
public:
    //! @name Constructors/Destructor
    //@{
             AvailableProfile();
    explicit AvailableProfile( const sword::ProfileDescription& profile );
    virtual ~AvailableProfile();
    //@}

    //! @name Operations
    //@{
    const QString& GetLogin() const;
    bool IsPasswordProtected() const;
    bool IsSupervisor() const;
    AvailableProfile& operator=( const AvailableProfile& );
    //@}

private:
    //! @name Member data
    //@{
    QString login_;
    bool password_;
    bool supervisor_;
    //@}
};

#endif // __AvailableProfile_h_
