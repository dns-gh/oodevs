// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectFactory_ABC_h_
#define __ObjectFactory_ABC_h_

namespace kernel
{
    class Object_ABC;
    class Team_ABC;
    class ObjectType;
    class Location_ABC;
}

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
    virtual kernel::Object_ABC* CreateObject( kernel::ObjectType& type, kernel::Team_ABC& team, const kernel::Location_ABC& location ) = 0;
    //@}
};

#endif // __ObjectFactory_ABC_h_
