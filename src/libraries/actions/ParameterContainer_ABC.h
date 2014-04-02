// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionParameterContainer_ABC_h_
#define __ActionParameterContainer_ABC_h_

#include <tools/Resolver.h>

namespace actions
{

class Parameter_ABC;

// =============================================================================
/** @class  ParameterContainer_ABC
    @brief  Action parameter container definition
*/
// Created: AGE 2007-07-11
// =============================================================================
class ParameterContainer_ABC : public tools::Resolver< Parameter_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ParameterContainer_ABC() {}
    virtual ~ParameterContainer_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void AddParameter( Parameter_ABC& parameter ) = 0;
    //@}
};

}

#endif // __ActionParameterContainer_ABC_h_
