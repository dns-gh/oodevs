// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSuperior_h_
#define __LogisticSuperior_h_

#include "clients_kernel/SubTypes.h"

namespace kernel
{
    class Entity_ABC;
    class Automat_ABC;
    class Formation_ABC;
}

// =============================================================================
/** @class  LogisticSuperior
    @brief  LogisticSuperior
*/
// Created: SBO 2006-10-25
// =============================================================================
class LogisticSuperior
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LogisticSuperior( const kernel::Entity_ABC& entity );
    virtual ~LogisticSuperior();
    //@}

    //! @name Operations
    //@{
    bool IsValidSuperior( const kernel::LogisticBaseSuperior& superior ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticSuperior( const LogisticSuperior& );            //!< Copy constructor
    LogisticSuperior& operator=( const LogisticSuperior& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool IsSameTeam( const kernel::Automat_ABC& automat ) const;
    bool IsSameTeam( const kernel::Formation_ABC& automat ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    //@}
};

#endif // __LogisticSuperior_h_
