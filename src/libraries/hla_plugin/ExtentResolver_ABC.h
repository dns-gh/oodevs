// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_ExtentResolver_ABC_h
#define plugins_hla_ExtentResolver_ABC_h

#include <boost/noncopyable.hpp>
#include <geometry/Types.h>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  ExtentResolver_ABC
    @brief  Extent resolver definition
*/
// Created: SLI 2011-11-29
// =============================================================================
class ExtentResolver_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ExtentResolver_ABC() {}
    virtual ~ExtentResolver_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsInBoundaries( const geometry::Point2d& geoPoint ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_ExtentResolver_ABC_h
