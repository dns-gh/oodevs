// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __AutomateFactory_ABC_h_
#define __AutomateFactory_ABC_h_

#include "tools/Resolver.h"
#include "Entities/Automates/MIL_Automate.h"

class MIL_AutomateType;
class MIL_Formation;

namespace xml
{
    class xistream;
}
// =============================================================================
/** @class  AutomateFactory_ABC
    @brief  AutomateFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class AutomateFactory_ABC : public tools::Resolver< MIL_Automate >
                          , private boost::noncopyable
{
public:
    //! @name Constructor/Destructor
    //@{
             AutomateFactory_ABC() {}
    virtual ~AutomateFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual MIL_Automate& Create( xml::xistream& xis, MIL_Automate& parent ) = 0;
    virtual MIL_Automate& Create( xml::xistream& xis, MIL_Formation& parent ) = 0;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const uint );
    //@}
};

namespace boost {
namespace serialization {
    template<>
    struct is_abstract<const AutomateFactory_ABC> : boost::true_type { } ;
} // namespace serialization
}

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void AutomateFactory_ABC::serialize
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void AutomateFactory_ABC::serialize( Archive& file, const uint )
{
    file & elements_;
}

#endif // __AutomateFactory_ABC_h_
