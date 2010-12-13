// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include "clients_kernel/EntityImplementation.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Karma.h"

namespace sword
{
    class PartyCreation;
}

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: AGN 2003-12-22
// =============================================================================
class Team : public kernel::EntityImplementation< kernel::Team_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             Team( const sword::PartyCreation& message, kernel::Controller& controller );
    virtual ~Team();
    //@}

    //! @name Accessors
    //@{
    virtual const kernel::Karma& GetKarma() const;
    //@}

private:
    //! @name Copy / Assignment
    //@{
    Team( const Team& );
    Team& operator=( const Team& );
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::Controller& controller );
    //@}
private:
    //! @name Attributes
    //@{
    kernel::Karma karma_;
    //@}

};

#endif // __Team_h_
