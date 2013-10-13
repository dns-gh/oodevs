// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ResourcesNetworksFunctionalStates_h_
#define __ResourcesNetworksFunctionalStates_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  ResourcesNetworksFunctionalStates
    @brief  ResourcesNetworksFunctionalStates
*/
// Created: FPO 2011-05-26
// =============================================================================
class ResourcesNetworksFunctionalStates : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 ResourcesNetworksFunctionalStates();
    /*implicit*/ ResourcesNetworksFunctionalStates( xml::xistream& xis );
    virtual     ~ResourcesNetworksFunctionalStates();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const;
    float Extract( const sword::SimToClient& wrapper );
    //@}
};

}

#endif // __ResourcesNetworksFunctionalStates_h_
