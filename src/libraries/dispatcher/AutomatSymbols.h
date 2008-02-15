// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatSymbols_h_
#define __AutomatSymbols_h_

#include "EntitySymbols_ABC.h"

namespace dispatcher
{
    class Automat;

// =============================================================================
/** @class  AutomatSymbols
    @brief  AutomatSymbols
*/
// Created: SBO 2008-02-13
// =============================================================================
class AutomatSymbols : public EntitySymbols_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatSymbols( const Automat& holder );
    virtual ~AutomatSymbols();
    //@}

    //! @name Operations
    //@{
    virtual std::string BuildSymbol( bool up = true ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatSymbols( const AutomatSymbols& );            //!< Copy constructor
    AutomatSymbols& operator=( const AutomatSymbols& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Automat& holder_;
    //@}
};

}

#endif // __AutomatSymbols_h_
