// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __ExerciseIdentifierFactory_ABC_h_
#define __ExerciseIdentifierFactory_ABC_h_

#include <string>

namespace frontend
{

// =============================================================================
/** @class  ExerciseIdentifierFactory_ABC
    @brief  ExerciseIdentifierFactory_ABC
*/
// Created: SBO 2010-10-28
// =============================================================================
class ExerciseIdentifierFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ExerciseIdentifierFactory_ABC() {}
    virtual ~ExerciseIdentifierFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::string CreateIdentifier( const tools::Path& exercise ) const = 0;
    //@}
};

}

#endif // __ExerciseIdentifierFactory_ABC_h_
