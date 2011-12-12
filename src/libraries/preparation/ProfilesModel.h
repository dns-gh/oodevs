// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfilesModel_h_
#define __ProfilesModel_h_

#include "tools/ElementObserver_ABC.h"

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Population_ABC;
    class Team_ABC;
}

namespace tools
{
    class Loader_ABC;
    class SchemaWriter_ABC;
}

class UserProfile;
class ProfileFactory_ABC;
class Model;
class ModelChecker_ABC;

// =============================================================================
/** @class  ProfilesModel
    @brief  ProfilesModel
*/
// Created: SBO 2007-01-16
// =============================================================================
class ProfilesModel : public tools::Observer_ABC
                    , public tools::ElementObserver_ABC< kernel::Team_ABC >
                    , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                    , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                    , public tools::ElementObserver_ABC< kernel::Population_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ProfilesModel( kernel::Controllers& controllers, const ProfileFactory_ABC& factory );
    virtual ~ProfilesModel();
    //@}

    //! @name Types
    //@{
    typedef std::set< std::string >               T_Profiles;
    typedef std::map< unsigned long, T_Profiles > T_Units;
    typedef T_Units::const_iterator             CIT_Units;
    //@}

    //! @name Operations
    //@{
    void Load( const tools::Loader_ABC& fileLoader, const std::string& file );
    void Serialize( const std::string& file, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Purge();

    void CreateProfile( const QString& name );
    void CreateProfile( const QString& name, const std::string& userRole );
    void CreateProfile( const QString& name, const std::string& userRole, const kernel::Entity_ABC& entity, bool readonly );
    void CreateProfile( const QString& name, const std::string& userRole, const std::vector< const kernel::Entity_ABC* >& entities, bool readonly );
    void DeleteProfile( const UserProfile& profile );
    void Visit( T_Units& units ) const;
    void Visit( T_Profiles& profiles ) const;

    bool Exists( const QString& login ) const;
    const UserProfile* Find( const QString& name ) const;
    bool IsReadable( const kernel::Entity_ABC& entity ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    bool IsReadable( const kernel::Entity_ABC& entity, const std::string& profile ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity, const std::string& profile ) const;

    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Population_ABC& population );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfilesModel( const ProfilesModel& );            //!< Copy constructor
    ProfilesModel& operator=( const ProfilesModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    void LoadProfile( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< UserProfile* >      T_UserProfiles;
    typedef T_UserProfiles::const_iterator CIT_UserProfiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const ProfileFactory_ABC& factory_;
    T_UserProfiles userProfiles_;
    //@}
};

#endif // __ProfilesModel_h_
