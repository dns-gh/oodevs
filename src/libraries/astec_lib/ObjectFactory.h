// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectFactory_h_
#define __ObjectFactory_h_

#include "ObjectFactory_ABC.h"

class Controllers;
class Model;

// =============================================================================
/** @class  ObjectFactory
    @brief  Object factory
*/
// Created: AGE 2006-02-13
// =============================================================================
class ObjectFactory : public ObjectFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectFactory( Controllers& controllers, Model& model );
    virtual ~ObjectFactory();
    //@}

    //! @name Operations
    //@{
    virtual Object* Create( const ASN1T_MsgObjectCreation& message );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectFactory( const ObjectFactory& );            //!< Copy constructor
    ObjectFactory& operator=( const ObjectFactory& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    Model& model_;
    //@}
};

#endif // __ObjectFactory_h_
