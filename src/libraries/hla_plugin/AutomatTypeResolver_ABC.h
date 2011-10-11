// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AutomatTypeResolver_ABC_h
#define plugins_hla_AutomatTypeResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <string>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AutomatTypeResolver_ABC
    @brief  AutomatTypeResolver definition
*/
// Created: SLI 2011-10-11
// =============================================================================
class AutomatTypeResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatTypeResolver_ABC() {}
    virtual ~AutomatTypeResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual unsigned long Resolve( const std::string& automatTypeName ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_AutomatTypeResolver_ABC_h
