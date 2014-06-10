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
             UserProfile( xml::xistream& xis, kernel::Controller& controller, const kernel::Model_ABC& model );
             UserProfile( const QString& login, kernel::Controller& controller );
             UserProfile( kernel::Controller& controller );
             UserProfile( const UserProfile& );
    virtual ~UserProfile();
    //@}

    //! @name From Profile_ABC
    //@{
    virtual const QString& GetLogin() const;
    virtual bool IsKnowledgeVisible( const kernel::Knowledge_ABC& knowledge ) const;
    virtual bool IsVisible( const kernel::Entity_ABC& entity ) const;
    virtual bool CanBeOrdered ( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic( const kernel::Entity_ABC& entity ) const;
    virtual bool IsSupervision() const;
    virtual bool HasTimeControl() const;
    virtual bool IsPerceived( const kernel::Entity_ABC& ) const;
    //@}

    //! @name Accessors
    //@{
    virtual const QString& GetPassword() const;
    virtual const kernel::UserRights& GetRights() const;
    virtual bool IsPasswordProtected() const;
    virtual bool IsReadable( const kernel::Entity_ABC& entity ) const;
    virtual bool IsWriteable( const kernel::Entity_ABC& entity ) const;
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
    virtual kernel::UserProfile_ABC& operator=( const kernel::UserProfile_ABC& );
    virtual bool operator==( const kernel::UserProfile_ABC& ) const;
    virtual bool operator!=( const kernel::UserProfile_ABC& ) const;
    void Serialize( xml::xostream& xos ) const;
    void NotifyTeamDeleted( unsigned long teamId );
    void NotifyFormationDeleted( unsigned long formationId );
    void NotifyAutomatDeleted( unsigned long automatId );
    void NotifyPopulationDeleted( unsigned long populationId );
    void NotifyGhostDeleted( unsigned long ghostId );
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
    QString login_;
    QString password_;
    bool supervisor_;
    bool timeControl_;
    bool isClone_;
    kernel::UserRights rights_;
    //@}
};

#endif // __UserProfile_h_
