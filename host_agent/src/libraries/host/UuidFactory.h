// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef UUID_FACTORY_H
#define UUID_FACTORY_H

#include "UuidFactory_ABC.h"

namespace host
{
// =============================================================================
/** @class  UuidFactory
    @brief  UuidFactory interface
*/
// Created: BAX 2012-03-19
// =============================================================================
class UuidFactory : public UuidFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             UuidFactory();
    virtual ~UuidFactory();
    //@}

    //! @name Methods
    //@{
    virtual Uuid Create() const;
    //@}
};
}

#endif // SESSION_H
