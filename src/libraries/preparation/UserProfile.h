// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __UserProfile_h_
#define __UserProfile_h_

#include "clients_kernel/UserProfile_ABC.h"
#include "clients_kernel/UserRights.h"

namespace kernel
{
    class Controller;
    class EntityResolver_ABC;
    class Model_ABC;
}

// =============================================================================
/** @class  UserProfile
    @brief  UserProfile
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfile : public kernel::UserProfile_ABC
                  , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfile( xml::xistream& xis, kernel::Controller& controller, const kernel::Model_ABC& model, const kernel::EntityResolver_ABC& resolver );
             UserProfile( const QString& login, kernel::Controller& controller, const kernel::EntityResolver_ABC& resolver );
             UserProfile( kernel::Controller& controller, const kernel::EntityResolver_ABC& resolver );
             UserProfile( const UserProfile& );
    virtual ~UserProfile();
    //@}

    //! @name Accessors
    //@{
    virtual const QString& GetLogin() const;
    virtual const QString& GetPassword() const;
    virtual bool IsSupervisor() const;
    virtual bool HasTimeControl() const;
    virtual const kernel::UserRights& GetRights() const;
    virtual bool IsReadable( const kernel::Entity_ABC& entity ) const;
    virtual bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    virtual void Visit( std::vector< unsigned long >& elements ) const;
    virtual void VisitAllAutomats( std::set< unsigned long >& elements ) const;
    //@}

    //! @name Setters
    //@{
    virtual void SetLogin( const QString& value );
    virtual void SetPassword( const QString& value );
    virtual void SetSupervisor( bool value );
    virtual void SetTimeControl( bool value );
    virtual void SetReadable( const kernel::Entity_ABC& entity, bool readable );
    virtual void SetWriteable( const kernel::Entity_ABC& entity, bool writeable );
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( xml::xostream& xos ) const;
    virtual kernel::UserProfile_ABC& operator=( const kernel::UserProfile_ABC& );
    virtual bool operator==( const kernel::UserProfile_ABC& ) const;
    virtual bool operator!=( const kernel::UserProfile_ABC& ) const;
    virtual void NotifyTeamDeleted( unsigned long teamId );
    virtual void NotifyFormationDeleted( unsigned long formationId );
    virtual void NotifyAutomatDeleted( unsigned long automatId );
    virtual void NotifyPopulationDeleted( unsigned long populationId );
    virtual void NotifyGhostDeleted( unsigned long ghostId );
    //@}

private:
    //! @name Helpers
    //@{
    const kernel::UserProfile_ABC& Base() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::EntityResolver_ABC& resolver_;
    QString login_;
    QString password_;
    bool supervisor_;
    bool timeControl_;
    bool isClone_;
    kernel::UserRights rights_;
    //@}
};

#endif // __UserProfile_h_
