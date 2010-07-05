// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TeamKarmas_h_
#define __TeamKarmas_h_

#include "tools/Resolver.h"

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
class TeamKarmas : public tools::Resolver< kernel::Karma, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             TeamKarmas();
    virtual ~TeamKarmas();
    //@}

    //! @name Operations
    //@{
    kernel::Karma& GetDefault() const;
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
    kernel::Karma& default_;
    //@}
};

#endif // __TeamKarmas_h_
