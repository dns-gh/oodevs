// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectFactory_ABC_h_
#define __ObjectFactory_ABC_h_

#include "ASN_Types.h"

class Object_ABC;

// =============================================================================
/** @class  ObjectFactory_ABC
    @brief  Object factory definition
*/
// Created: AGE 2006-02-13
// =============================================================================
class ObjectFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectFactory_ABC() {};
    virtual ~ObjectFactory_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual Object_ABC* Create( const ASN1T_MsgObjectCreation& asnMsg ) = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectFactory_ABC( const ObjectFactory_ABC& );            //!< Copy constructor
    ObjectFactory_ABC& operator=( const ObjectFactory_ABC& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ObjectFactory_ABC_h_
