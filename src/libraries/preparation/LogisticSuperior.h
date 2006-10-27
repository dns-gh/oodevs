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

#include "Types.h"

namespace kernel
{
    class Entity_ABC;
    class Automat_ABC;
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
    bool IsValidSuperior( const TC2& superior ) const;
    bool IsValidSuperior( const MaintenanceSuperior& superior ) const;
    bool IsValidSuperior( const MedicalSuperior& superior ) const;
    bool IsValidSuperior( const SupplySuperior& superior ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticSuperior( const LogisticSuperior& );            //!< Copy constructor
    LogisticSuperior& operator=( const LogisticSuperior& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool IsSameTeam( const kernel::Automat_ABC& automat ) const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    //@}
};

#endif // __LogisticSuperior_h_
