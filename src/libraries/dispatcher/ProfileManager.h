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

namespace dispatcher
{

class Dispatcher;

// =============================================================================
/** @class  ProfileManager
    @brief  ProfilerManager
*/
// Created: NLD 2006-09-19
// =============================================================================
class ProfileManager
{
public:
    //! @name Constructors/Destructor
    //@{
     ProfileManager( Dispatcher& dispatcher );
    ~ProfileManager();
    //@}

    //! @name Main
    //@{
    void Update();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ProfileManager( const ProfileManager& );            //!< Copy constructor
    ProfileManager& operator=( const ProfileManager& ); //!< Assignement operator
    //@}

private:
    Dispatcher& dispatcher_;
};

}

#endif // __ProfileManager_h_
