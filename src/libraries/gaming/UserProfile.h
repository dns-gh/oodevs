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
#include "clients_kernel/UserProfile_ABC.h"
#include "RightsResolver.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class EntityResolver_ABC;
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
             UserProfile( const sword::ProfileCreation& message, kernel::Controller& controller, Publisher_ABC& publisher, const kernel::EntityResolver_ABC& resolver );
             UserProfile( kernel::Controller& controller, Publisher_ABC& publisher, const kernel::EntityResolver_ABC& resolver );
             UserProfile( const UserProfile& );
    virtual ~UserProfile();
    //@}

    //! @name From UserProfile_ABC
    //@{
    virtual const QString& GetPassword() const;
    virtual const kernel::UserRights& GetRights() const;
    virtual void SetLogin( const QString& value );
    virtual void SetPassword( const QString& password );
    virtual void SetSupervisor( bool supervisor );
    virtual void SetTimeControl( bool timeControl );
    virtual void VisitAllAutomats( std::set< unsigned long >& elements ) const;
    virtual void Visit( std::vector< unsigned long >& elements ) const;
    virtual bool IsReadable( const kernel::Entity_ABC& entity ) const;
    virtual bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    virtual void SetReadable( const kernel::Entity_ABC& entity, bool readable );
    virtual void SetWriteable( const kernel::Entity_ABC& entity, bool writeable );

    virtual kernel::UserProfile_ABC& operator=( const kernel::UserProfile_ABC& );
    virtual bool operator==( const kernel::UserProfile_ABC& ) const;
    virtual bool operator!=( const kernel::UserProfile_ABC& ) const;
    //@}

    //! @name Operations
    //@{
    void RequestCreation() const;
    void RequestDeletion() const;
    void RequestUpdate( const QString& oldLogin ) const;
    virtual void DoUpdate( const sword::ProfileUpdate& message );
    //@}

    //! @name Accessors
    //@{
    virtual const QString& GetLogin() const;
    virtual bool IsSupervision() const;
    virtual bool HasTimeControl() const;

    virtual bool IsPasswordProtected() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UserProfile& operator=( const UserProfile& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SetProfile( const sword::Profile& profile );
    const kernel::UserProfile_ABC& Base() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    const kernel::EntityResolver_ABC& resolver_;
    bool registered_;
    QString login_;
    QString password_;
    bool supervisor_;
    bool timeControl_;
    //@}
};

#endif // __UserProfile_h_
