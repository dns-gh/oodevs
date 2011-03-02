// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ProfilesModel_h_
#define __ProfilesModel_h_

namespace xml
{
    class xistream;
}

namespace kernel
{
    class Entity_ABC;
}

namespace tools
{
    class Loader_ABC;
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
class ProfilesModel
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ProfilesModel( const ProfileFactory_ABC& factory );
    virtual ~ProfilesModel();
    //@}

    //! @name Operations
    //@{
    void Load( const tools::Loader_ABC& fileLoader, const std::string& file );
    void Serialize( const std::string& file ) const;
    void Purge();
    bool CheckValidity( const Model& model, ModelChecker_ABC& checker ) const;

    void CreateProfile();
    void CreateProfile( const QString& name, const kernel::Entity_ABC& entity, bool readonly );
    void DeleteProfile( const UserProfile& profile );

    bool Exists( const QString& login ) const;
    const UserProfile* Find( const QString& name ) const;
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
    bool IsReadable( const kernel::Entity_ABC& entity ) const;
    bool IsWriteable( const kernel::Entity_ABC& entity ) const;
    //@}

    //! @name Types
    //@{
    typedef std::vector< UserProfile* >      T_UserProfiles;
    typedef T_UserProfiles::const_iterator CIT_UserProfiles;
    //@}

private:
    //! @name Member data
    //@{
    const ProfileFactory_ABC& factory_;
    T_UserProfiles userProfiles_;
    //@}
};

#endif // __ProfilesModel_h_
