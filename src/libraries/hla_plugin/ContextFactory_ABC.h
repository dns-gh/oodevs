// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef ContextFactory_ABC_h
#define ContextFactory_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ContextFactory_ABC
    @brief  ContextFactory definition
*/
// Created: SLI 2011-09-09
// =============================================================================
class ContextFactory_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ContextFactory_ABC() {}
    virtual ~ContextFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual int Create() const = 0;
    //@}
};

}
}

#endif // ContextFactory_ABC_h
