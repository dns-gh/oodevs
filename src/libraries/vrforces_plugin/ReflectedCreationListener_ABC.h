// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ReflectedCreationListener_ABC_h_
#define __ReflectedCreationListener_ABC_h_

class DtReflectedAggregate;
class DtReflectedEntity;

namespace plugins
{
namespace vrforces
{

// =============================================================================
/** @class  ReflectedCreationListener_ABC
    @brief  Reflected creation listener base class
*/
// Created: SBO 2011-04-01
// =============================================================================
class ReflectedCreationListener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ReflectedCreationListener_ABC() {}
    virtual ~ReflectedCreationListener_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool NotifyCreated( DtReflectedAggregate& /*aggregate*/ ) { return false; };
    virtual bool NotifyCreated( DtReflectedEntity& /*entity*/ ) { return false; };
    //@}
};
}
}

#endif // __ReflectedCreationListener_ABC_h_
