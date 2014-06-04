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
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfile( xml::xistream& xis, kernel::Controller& controller, const kernel::Model_ABC& model, const kernel::EntityResolver_ABC& resolver );
             UserProfile( const QString& login, kernel::Controller& controller, const kernel::EntityResolver_ABC& resolver );
             UserProfile( const UserProfile& );
    virtual ~UserProfile();
    //@}

    //! @name Accessors
    //@{
    virtual const QString& GetLogin() const;
    virtual const QString& GetPassword() const;
    virtual bool IsSupervisor() const;
    virtual bool HasTimeControl() const;
    virtual bool IsReadable( const kernel::Entity_ABC& entity ) const;
    virtual bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    void Visit( std::vector< unsigned long >& elements ) const;
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
    void Serialize( xml::xostream& xos ) const;
    UserProfile& operator=( const UserProfile& );
    bool operator==( const UserProfile& ) const;
    bool operator!=( const UserProfile& ) const;
    void NotifyTeamDeleted( unsigned long teamId );
    void NotifyFormationDeleted( unsigned long formationId );
    void NotifyAutomatDeleted( unsigned long automatId );
    void NotifyPopulationDeleted( unsigned long populationId );
    void NotifyGhostDeleted( unsigned long ghostId );
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
