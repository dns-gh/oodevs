// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_HlaObjectNameFactory_ABC_h
#define plugins_hla_HlaObjectNameFactory_ABC_h

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  HlaObjectNameFactory_ABC
    @brief  HlaObjectNameFactory_ABC
*/
// Created: AHC 2012-03-22
// =============================================================================
class HlaObjectNameFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    virtual ~HlaObjectNameFactory_ABC() {}

    //! @name Operations
    //@{
    virtual std::string CreateName( const std::string& hint ) const = 0;
    //@}
};
}
}

#endif // plugins_hla_HlaObjectNameFactory_ABC_h
