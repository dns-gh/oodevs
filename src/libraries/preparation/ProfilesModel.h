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

#include "clients_kernel/ProfilesModel_ABC.h"
#include <boost/noncopyable.hpp>
#include <tools/ElementObserver_ABC.h>

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
    class Ghost_ABC;
    class Population_ABC;
    class ProfileEditor;
    class ProfileFactory_ABC;
    class Team_ABC;
    class UserProfile_ABC;
}

namespace tools
{
    class Loader_ABC;
    class SchemaWriter_ABC;
}

class Model;

// =============================================================================
/** @class  ProfilesModel
    @brief  ProfilesModel
*/
// Created: SBO 2007-01-16
// =============================================================================
class ProfilesModel : public kernel::ProfilesModel_ABC
                    , public tools::Observer_ABC
                    , public tools::ElementObserver_ABC< kernel::Team_ABC >
                    , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                    , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                    , public tools::ElementObserver_ABC< kernel::Population_ABC >
                    , public tools::ElementObserver_ABC< kernel::Ghost_ABC >
                    , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfilesModel( kernel::Controllers& controllers, const kernel::ProfileFactory_ABC& factory );
    virtual ~ProfilesModel();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::Loader_ABC& fileLoader, const tools::Path& file );
    void Serialize( const tools::Path& file, const tools::SchemaWriter_ABC& schemaWriter ) const;
    void Purge();

    kernel::UserProfile_ABC* CreateProfile( const QString& name );
    void CreateProfile( const QString& name, const kernel::Entity_ABC& entity, bool readonly );
    void DeleteProfile( const kernel::UserProfile_ABC& profile );
    virtual kernel::ProfileEditor* CreateProfileEditor() const;
    virtual kernel::ProfileEditor* CreateProfileEditor( kernel::UserProfile_ABC& profile ) const;
    virtual void CommitFromEditor( kernel::ProfileEditor& editor );

    virtual void Apply( boost::function< void( kernel::UserProfile_ABC& ) > functor );

    bool Exists( const QString& login ) const;
    kernel::UserProfile_ABC* Find( const std::string& name ) const;
    const kernel::UserProfile_ABC* Find( const QString& name ) const;
    bool IsReadable( const kernel::Entity_ABC& entity ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    bool IsReadable( const kernel::Entity_ABC& entity, const std::string& profile ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity, const std::string& profile ) const;
    std::vector< std::string > GetProfilesWhoCanRead( const kernel::Entity_ABC& entity ) const;
    std::vector< std::string > GetProfilesWhoCanWrite( const kernel::Entity_ABC& entity ) const;

    virtual void NotifyDeleted( const kernel::Team_ABC& team );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Population_ABC& population );
    virtual void NotifyDeleted( const kernel::Ghost_ABC& population );

    unsigned int GetProfilesCount() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Read( xml::xistream& xis );
    void LoadProfile( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::vector< kernel::UserProfile_ABC* > T_UserProfiles;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::ProfileFactory_ABC& factory_;
    T_UserProfiles userProfiles_;
    //@}
};

#endif // __ProfilesModel_h_
