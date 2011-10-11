// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AutomatTypeResolver_h
#define plugins_hla_AutomatTypeResolver_h

#include "AutomatTypeResolver_ABC.h"

namespace kernel
{
    class AutomatType;
}

namespace tools
{
    template< typename T, typename Identifier > class Resolver_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AutomatTypeResolver
    @brief  Automat type resolver
*/
// Created: SLI 2011-10-11
// =============================================================================
class AutomatTypeResolver : public AutomatTypeResolver_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AutomatTypeResolver( const tools::Resolver_ABC< kernel::AutomatType, std::string >& automatTypes );
    virtual ~AutomatTypeResolver();
    //@}

    //! @name Operations
    //@{
    virtual unsigned long Resolve( const std::string& automatTypeName ) const;
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::AutomatType, std::string >& automatTypes_;
    //@}
};

}
}

#endif // AutomatTypeResolver_h
