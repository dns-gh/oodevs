// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ContextFactory_h
#define plugins_hla_ContextFactory_h

#include "ContextFactory_ABC.h"

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ContextFactory
    @brief  Context factory
*/
// Created: SLI 2011-09-09
// =============================================================================
class ContextFactory : public ContextFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ContextFactory();
    virtual ~ContextFactory();
    //@}

    //! @name Operations
    //@{
    virtual int Create() const;
    //@}

private:
    //! @name Member data
    //@{
    mutable int context_;
    //@}
};

}
}

#endif // plugins_hla_ContextFactory_h
