// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PopulationUrbanOccupation_h_
#define __PopulationUrbanOccupation_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  PopulationUrbanOccupation
    @brief  PopulationUrbanOccupation
*/
// Created: FPO 2011-05-12
// =============================================================================
class PopulationUrbanOccupation : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PopulationUrbanOccupation();
    explicit PopulationUrbanOccupation( xml::xistream& xis );
    virtual ~PopulationUrbanOccupation();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return ( wrapper.message().has_population_update() );
    }
    int Extract( const sword::SimToClient& wrapper );
    //@}

private:
    //! @name Member data
    //@{
    FilterHelper< int > filter_;
    //@}
};

}

#endif // __PopulationUrbanOccupation_h_
