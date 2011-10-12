// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_AutomatChecker_ABC_h
#define plugins_hla_AutomatChecker_ABC_h

#include <boost/noncopyable.hpp>

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  AutomatChecker_ABC
    @brief  Automat checker definition
*/
// Created: SLI 2011-10-12
// =============================================================================
class AutomatChecker_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatChecker_ABC() {}
    virtual ~AutomatChecker_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool IsAutomatDisengaged( unsigned int unitIdentifier ) const = 0;
    //@}
};

}
}

#endif // AutomatChecker_ABC_h
