// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SideSymbols_h_
#define __SideSymbols_h_

#include "EntitySymbols_ABC.h"

namespace dispatcher
{
    class Side;

// =============================================================================
/** @class  SideSymbols
    @brief  SideSymbols
*/
// Created: SBO 2008-02-13
// =============================================================================
class SideSymbols : public EntitySymbols_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit SideSymbols( const Side& holder );
    virtual ~SideSymbols();
    //@}

    //! @name Operations
    //@{
    virtual std::string BuildSymbol( bool up = true ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SideSymbols( const SideSymbols& );            //!< Copy constructor
    SideSymbols& operator=( const SideSymbols& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Side& holder_;
    //@}
};

}

#endif // __SideSymbols_h_
