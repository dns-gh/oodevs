// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_Handler_ABC_h
#define plugins_hla_Handler_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Handler_ABC
    @brief  Handler definition
*/
// Created: SLI 2011-06-24
// =============================================================================
class Handler_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Handler_ABC() {}
    virtual ~Handler_ABC() {}
    //@}
};

}
}

#endif // plugins_hla_Handler_ABC_h
