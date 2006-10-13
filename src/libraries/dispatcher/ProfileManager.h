// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileManager_h_
#define __ProfileManager_h_

namespace xml{ class xistream; };

namespace dispatcher
{
class Dispatcher;
class Profile;

// =============================================================================
/** @class  ProfileManager
    @brief  Profile manager
*/
// Created: NLD 2006-09-19
// =============================================================================
class ProfileManager
{
public:
    //! @name Constructors/Destructor
    //@{
             ProfileManager( Dispatcher& dispatcher, const std::string& strFile );
    virtual ~ProfileManager();
    //@}

    //! @name Main
    //@{
    void     Reset       ();
    Profile* Authenticate( const std::string& strName, const std::string& strPassword );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ProfileManager( const ProfileManager& );            //!< Copy constructor
    ProfileManager& operator=( const ProfileManager& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void ReadProfile( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, Profile* > T_ProfileMap;
    typedef T_ProfileMap::const_iterator      CIT_ProfileMap;
    //@}

private:
    Dispatcher& dispatcher_;
    const std::string strFile_;

    T_ProfileMap profiles_;
};

}

#endif // __ProfileManager_h_
