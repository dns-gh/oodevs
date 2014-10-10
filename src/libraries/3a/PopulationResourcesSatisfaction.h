// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __PopulationResourcesSatisfaction_h_
#define __PopulationResourcesSatisfaction_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  PopulationResourcesSatisfaction
    @brief  PopulationResourcesSatisfaction
*/
// Created: JSR 2014-10-08
// =============================================================================
class PopulationResourcesSatisfaction : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PopulationResourcesSatisfaction();
    explicit PopulationResourcesSatisfaction( xml::xistream& xis );
    virtual ~PopulationResourcesSatisfaction();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const;
    float Extract( const sword::SimToClient& wrapper );
    //@}

private:
    //! @name Member data
    //@{
    FilterHelper< int > filter_;
    //@}
};

}

#endif // __PopulationResourcesSatisfaction_h_
