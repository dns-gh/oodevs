// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TeamKarmas_h_
#define __TeamKarmas_h_

#include "clients_kernel/Resolver.h"

class TeamKarma;

// =============================================================================
/** @class  TeamKarmas
    @brief  TeamKarmas
*/
// Created: SBO 2006-10-27
// =============================================================================
class TeamKarmas : public kernel::Resolver< TeamKarma, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamKarmas();
    virtual ~TeamKarmas();
    //@}

    //! @name Operations
    //@{
    TeamKarma& GetDefault() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TeamKarmas( const TeamKarmas& );            //!< Copy constructor
    TeamKarmas& operator=( const TeamKarmas& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    TeamKarma* default_;
    //@}
};

#endif // __TeamKarmas_h_
