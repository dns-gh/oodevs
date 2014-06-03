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

#include "clients_kernel/UserRights.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

class Model;

// =============================================================================
/** @class  UserProfile
    @brief  UserProfile
*/
// Created: SBO 2007-01-16
// =============================================================================
class UserProfile
{
public:
    //! @name Constructors/Destructor
    //@{
             UserProfile( xml::xistream& xis, kernel::Controller& controller, const Model& model );
             UserProfile( const QString& login, kernel::Controller& controller, const Model& model );
             UserProfile( const UserProfile& );
    virtual ~UserProfile();
    //@}

    //! @name Accessors
    //@{
    const QString& GetLogin() const;
    const QString& GetPassword() const;
    bool IsSupervisor() const;
    bool HasTimeControl() const;
    bool IsReadable( const kernel::Entity_ABC& entity ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    void Visit( std::vector< unsigned long >& elements ) const;
    void VisitAllAutomats( std::set< unsigned long >& elements ) const;
    //@}

    //! @name Setters
    //@{
    virtual void SetLogin( const QString& value );
    void SetPassword( const QString& value );
    void SetSupervisor( bool value );
    void SetTimeControl( bool value );
    void SetReadable( const kernel::Entity_ABC& entity, bool readable );
    void SetWriteable( const kernel::Entity_ABC& entity, bool writeable );
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
    const Model& model_;
    QString login_;
    QString password_;
    bool supervisor_;
    bool timeControl_;
    bool isClone_;
    kernel::UserRights rights_;
    //@}
};

#endif // __UserProfile_h_
