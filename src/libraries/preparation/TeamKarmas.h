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

#include <tools/Resolver.h>

namespace kernel
{
    class Karma;
}

// =============================================================================
/** @class  TeamKarmas
    @brief  TeamKarmas
*/
// Created: SBO 2006-10-27
// =============================================================================
class TeamKarmas : public tools::Resolver< const kernel::Karma, QString >
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamKarmas();
    virtual ~TeamKarmas();
    //@}

    //! @name Operations
    //@{
    const kernel::Karma& GetDefault() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TeamKarmas( const TeamKarmas& );            //!< Copy constructor
    TeamKarmas& operator=( const TeamKarmas& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Karma& default_;
    //@}
};

#endif // __TeamKarmas_h_
