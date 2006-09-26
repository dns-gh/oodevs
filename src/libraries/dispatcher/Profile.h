// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#if 0

#ifndef __Profile_h_
#define __Profile_h_

namespace dispatcher
{

// =============================================================================
/** @class  Profile
    @brief  Profile
*/
// Created: NLD 2006-09-19
// =============================================================================
class Profile
{
public:
    //! @name Constructors/Destructor
    //@{
     Profile();
    ~Profile();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Profile( const Profile& );            //!< Copy constructor
    Profile& operator=( const Profile& ); //!< Assignement operator
    //@}

private:
    liste des entites RO et RW
        liste des 'formations' RO et RW 
        liste des automates RO et RW

    => liste des knowledges groups RO / RW
};

}

#endif // __Profile_h_

#endif