// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineSymbols_h_
#define __TacticalLineSymbols_h_

#include "EntitySymbols_ABC.h"

namespace dispatcher
{
    class TacticalLine_ABC;
    class Limit;
    class Lima;

// =============================================================================
/** @class  TacticalLineSymbols
    @brief  TacticalLineSymbols
*/
// Created: SBO 2008-02-14
// =============================================================================
class TacticalLineSymbols : public EntitySymbols_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit TacticalLineSymbols( const Limit& holder );
    explicit TacticalLineSymbols( const Lima& holder );
    virtual ~TacticalLineSymbols();
    //@}

    //! @name Operations
    //@{
    virtual std::string BuildSymbol( bool up = true ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLineSymbols( const TacticalLineSymbols& );            //!< Copy constructor
    TacticalLineSymbols& operator=( const TacticalLineSymbols& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const TacticalLine_ABC& holder_;
    std::string function_;
    //@}
};

}

#endif // __TacticalLineSymbols_h_
