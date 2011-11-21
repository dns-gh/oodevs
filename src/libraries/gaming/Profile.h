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

#include "clients_kernel/Profile_ABC.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class CommunicationHierarchies;
    class Controller;
    class Controllers;
    class Hierarchies;
    class Formation_ABC;
    class Knowledge_ABC;
    class Population_ABC;
    class Team_ABC;
}

namespace sword
{
    class AuthenticationResponse;
    class Profile;
    class ProfileUpdate;
    class ConnectedProfileList;
}

class Model;
class Publisher_ABC;
class Services;
class Simulation;

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
             Profile( kernel::Controllers& controllers, Publisher_ABC& publisher, const std::string& profile = "", bool isLoginSet = false );
    virtual ~Profile();
    //@}

    //! @name Operations
    //@{
    void Login() const;
    void Login( const std::string& login, const std::string& password ) const;
    void Update( const sword::AuthenticationResponse& message );
    void Update( const Model& model, const sword::ProfileUpdate& message );
    void Update( const sword::ConnectedProfileList& message );
    //@}

    //! @name Accessors
    //@{
    QString GetLogin() const;
    bool IsLoggedIn() const;
    unsigned int GetProfileCount( const std::string& login ) const;
    virtual bool IsSupervision() const;
    //@}

    //! @name Operations
    //@{
    virtual bool IsVisible( const kernel::Entity_ABC& entity ) const;
    virtual bool IsKnowledgeVisible( const kernel::Knowledge_ABC& entity ) const;
    virtual bool CanBeOrdered( const kernel::Entity_ABC& entity ) const;
    virtual bool CanDoMagic( const kernel::Entity_ABC& entity ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< unsigned long > T_Ids;
    typedef T_Ids::const_iterator      CIT_Ids;

    typedef std::set< const kernel::Entity_ABC* > T_Entities;
    typedef T_Entities::const_iterator          CIT_Entities;

    typedef std::map< std::string, unsigned int > T_Profiles;
    typedef T_Profiles::iterator                 IT_Profiles;
    typedef T_Profiles::const_iterator          CIT_Profiles;
    //@}

    //! @name Helpers
    //@{
    void Update( const sword::Profile& profile );
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
    void Add( const kernel::Entity_ABC& entity, const T_Ids& readIds, const T_Ids& readWriteIds );
    void Remove( const kernel::Entity_ABC& entity );
    bool IsKnowledgeVisibleByEntity( const kernel::Knowledge_ABC& knowledge, const kernel::Entity_ABC& entity ) const;
    static bool IsInHierarchy( const kernel::Entity_ABC& entity, const T_Entities& entities, bool childOnly );
    static bool IsInSpecificHierarchy( const kernel::Entity_ABC& entity, const kernel::Hierarchies* hierarchy, const T_Entities& entities, bool childOnly );
    static bool IsInHierarchy( const kernel::Entity_ABC& entity, const kernel::Hierarchies& hierarchy, const kernel::Entity_ABC& other, bool childOnly );
    static bool AreInSameKnowledgeGroup( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2, bool compareTop );
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
    T_Profiles profiles_;
    //@}
};

#endif // __Profile_h_
