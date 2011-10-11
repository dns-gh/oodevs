// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_SideChecker_ABC_h
#define plugins_hla_SideChecker_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  SideChecker_ABC
    @brief  SideChecker definition
*/
// Created: SLI 2011-10-11
// =============================================================================
class SideChecker_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             SideChecker_ABC() {}
    virtual ~SideChecker_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool AreSameSide( unsigned long firstIdentifier, unsigned long secondIdentifier ) const = 0;
    //@}
};

}
}

#endif // plugins_hla_SideChecker_ABC_h
