// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamHierarchy_h_
#define __TeamHierarchy_h_

#include "clients_kernel/Extension_ABC.h"

namespace kernel
{
    class Team_ABC;
}

// =============================================================================
/** @class  TeamHierarchy
    @brief  TeamHierarchy
*/
// Created: SBO 2006-09-22
// =============================================================================
class TeamHierarchy : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TeamHierarchy( const kernel::Team_ABC& team );
    virtual ~TeamHierarchy();
    //@}

    //! @name Operations
    //@{
    const kernel::Team_ABC& GetTeam() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamHierarchy( const TeamHierarchy& );            //!< Copy constructor
    TeamHierarchy& operator=( const TeamHierarchy& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Team_ABC& team_;
    //@}
};

#endif // __TeamHierarchy_h_
