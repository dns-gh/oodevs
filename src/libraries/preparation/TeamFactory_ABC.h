// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TeamFactory_ABC_h_
#define __TeamFactory_ABC_h_

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Team_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  TeamFactory_ABC
    @brief  TeamFactory_ABC
*/
// Created: AGE 2006-02-15
// =============================================================================
class TeamFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamFactory_ABC() {}
    virtual ~TeamFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual kernel::Team_ABC* CreateTeam() = 0;
    virtual kernel::Team_ABC* CreateTeam( xml::xistream& xis ) = 0;
    virtual kernel::Team_ABC* CreateNoSideTeam() = 0;
    virtual void ConfigureNoSideTeam( kernel::Team_ABC& team, xml::xistream& xis ) = 0;
    //@}
};

#endif // __TeamFactory_ABC_h_
