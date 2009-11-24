// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Profile_h_
#define __Profile_h_

#include "game_asn/Simulation.h"
#include "clients_kernel/Profile_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Controller;
    class Controllers;
    class Automat_ABC;
    class Team_ABC;
    class Population_ABC;
    class Hierarchies;
    class Formation_ABC;
}

class Services;
class Simulation;
class Publisher_ABC;
class Model;

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: AGE 2006-10-11
// =============================================================================
class Profile : public kernel::Profile_ABC
              , public tools::Observer_ABC
              , public tools::ElementObserver_ABC< kernel::Automat_ABC >
              , public tools::ElementObserver_ABC< kernel::Population_ABC >
              , public tools::ElementObserver_ABC< kernel::Team_ABC >
              , public tools::ElementObserver_ABC< kernel::Formation_ABC >
              , public tools::ElementObserver_ABC< Simulation >
              , public tools::ElementObserver_ABC< Services >
{

public:
    //! @name Constructors/Destructor
    //@{
             Profile( kernel::Controllers& controllers, Publisher_ABC& publisher, const std::string& profile = "", bool needLogin = true );
    virtual ~Profile();
    //@}

    //! @name Operations
    //@{
    void Login() const;
    void Login( const std::string& login, const std::string& password ) const;
    void Update( const ASN1T_MsgAuthenticationResponse& asnMsg );
    void Update( const Model& model, const ASN1T_MsgProfileUpdate& asnMsg );
    //@}
    
    //! @name Accessors
    //@{
    QString GetLogin() const;
    bool IsLoggedIn() const;
    virtual bool IsSupervision() const;
    //@}

    //! @name Operations
    //@{
    virtual bool IsVisible   ( const kernel::Entity_ABC& entity ) const;
    virtual bool CanBeOrdered( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic  ( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned long >   T_Ids;
    typedef T_Ids::const_iterator        CIT_Ids;

    typedef std::set< const kernel::Entity_ABC* > T_Entities;
    typedef T_Entities::const_iterator          CIT_Entities;
    //@}

    //! @name Helpers
    //@{
    void Update( const ASN1T_Profile& profile );
    void Clean();
    void ResolveEntities( const Model& model );
    template< typename Entity >
    void ResolveEntities( const tools::Resolver_ABC< Entity >& resolver, const T_Ids& readIds, const T_Ids& readWriteIds );

    template< typename T >
    void ReadList( const T& idList, T_Ids& ids );
    virtual void NotifyCreated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyCreated( const kernel::Population_ABC& popu );
    virtual void NotifyDeleted( const kernel::Population_ABC& popu );
    virtual void NotifyCreated( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyCreated( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Services& services );
    
    void Add   ( const kernel::Entity_ABC& entity, const T_Ids& readIds, const T_Ids& readWriteIds );
    void Remove( const kernel::Entity_ABC& entity );
    static bool IsInHierarchy( const kernel::Entity_ABC& entity, const T_Entities& entities, bool childOnly );
    static bool IsInHierarchy( const kernel::Entity_ABC& entity, const kernel::Hierarchies& hierarchy, const kernel::Entity_ABC& other, bool childOnly );
    static const kernel::Hierarchies* FindHierarchies( const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    Publisher_ABC& publisher_;
    mutable std::string login_;
    mutable std::string password_;
    bool loggedIn_;
    bool supervision_;
    bool simulation_;
    mutable bool needLogin_;

    T_Entities readEntities_;
    T_Entities readWriteEntities_;

    T_Ids readTeams_;
    T_Ids writeTeams_;
    T_Ids readAutomats_;
    T_Ids writeAutomats_;
    T_Ids readPopulations_;
    T_Ids writePopulations_;
    T_Ids readFormations_;
    T_Ids writeFormations_;
    //@}
};

#endif // __Profile_h_
