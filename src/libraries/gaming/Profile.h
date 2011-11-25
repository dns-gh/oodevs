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

#include "RightsResolver.h"

namespace kernel
{
    class Automat_ABC;
    class Controller;
    class Controllers;
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
class Profile : public RightsResolver
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
    typedef std::map< std::string, unsigned int > T_Profiles;
    typedef T_Profiles::iterator                 IT_Profiles;
    typedef T_Profiles::const_iterator          CIT_Profiles;
    //@}

    //! @name Helpers
    //@{
    void Clean();
    void Update( const sword::Profile& profile );
    template< typename T >
    void ReadList( const T& idList, T_Ids& ids );
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void NotifyUpdated( const Services& services );
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
    T_Profiles profiles_;
    //@}
};

#endif // __Profile_h_
