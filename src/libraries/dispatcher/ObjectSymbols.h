// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectSymbols_h_
#define __ObjectSymbols_h_

#include "EntitySymbols_ABC.h"

namespace dispatcher
{
    class Object;

// =============================================================================
/** @class  ObjectSymbols
    @brief  ObjectSymbols
*/
// Created: SBO 2008-02-14
// =============================================================================
class ObjectSymbols : public EntitySymbols_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectSymbols( const Object& holder );
    virtual ~ObjectSymbols();
    //@}

    //! @name Operations
    //@{
    virtual std::string BuildSymbol( bool up = true ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectSymbols( const ObjectSymbols& );            //!< Copy constructor
    ObjectSymbols& operator=( const ObjectSymbols& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const Object& holder_;
    //@}
};

}

#endif // __ObjectSymbols_h_
