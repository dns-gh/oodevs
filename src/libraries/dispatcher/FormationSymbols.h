// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationSymbols_h_
#define __FormationSymbols_h_

#include "EntitySymbols_ABC.h"

namespace dispatcher
{
    class Formation;

// =============================================================================
/** @class  FormationSymbols
    @brief  FormationSymbols
*/
// Created: SBO 2008-02-13
// =============================================================================
class FormationSymbols : public EntitySymbols_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit FormationSymbols( const Formation& holder );
    virtual ~FormationSymbols();
    //@}

    //! @name Operations
    //@{
    virtual std::string BuildSymbol( bool up = true ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationSymbols( const FormationSymbols& );            //!< Copy constructor
    FormationSymbols& operator=( const FormationSymbols& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Formation& holder_;
    //@}
};

}

#endif // __FormationSymbols_h_
