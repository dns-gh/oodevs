// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfile_h_
#define __UserProfile_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "RightsResolver.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

namespace sword
{
    class ProfileCreation;
    class ProfileUpdate;
}

class Publisher_ABC;

// =============================================================================
/** @class  UserProfile
    @brief  UserProfile
*/
// Created: SBO 2007-01-19
// =============================================================================
class UserProfile : public kernel::Extension_ABC
                  , public kernel::Updatable_ABC< sword::ProfileUpdate >
                  , public RightsResolver
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfile( const sword::ProfileCreation& message, kernel::Controller& controller, Publisher_ABC& publisher );
             UserProfile( const QString& login, kernel::Controller& controller, Publisher_ABC& publisher );
             UserProfile( const UserProfile& );
    virtual ~UserProfile();
    //@}

    //! @name Operations
    //@{
    void RequestCreation();
    void RequestDeletion();
    void RequestUpdate( const QString& newLogin );
    virtual void DoUpdate( const sword::ProfileUpdate& message );
    //@}

    //! @name Accessors
    //@{
    virtual QString GetLogin() const;
    virtual bool IsSupervision() const;
    virtual bool HasTimeControl() const;

    QString GetPassword() const;
    bool IsPasswordProtected() const;
    bool IsReadable( const kernel::Entity_ABC& entity ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    void SetPassword( const QString& password );
    void SetSupervisor( bool supervisor );
    void SetTimeControl( bool timeControl );
    void SetReadable( const kernel::Entity_ABC& entity, bool readable );
    void SetWriteable( const kernel::Entity_ABC& entity, bool writeable );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfile& operator=( const UserProfile& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetProfile( const sword::Profile& profile );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    bool registered_;
    QString login_;
    QString password_;
    bool supervision_;
    bool timeControl_;
    //@}
};

#endif // __UserProfile_h_
