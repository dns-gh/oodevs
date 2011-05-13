// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PopulationSatisfaction_h_
#define __PopulationSatisfaction_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  PopulationSatisfaction
    @brief  PopulationSatisfaction
*/
// Created: FPO 2011-05-10
// =============================================================================
class PopulationSatisfaction : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             PopulationSatisfaction();
    explicit PopulationSatisfaction( xml::xistream& xis );
    virtual ~PopulationSatisfaction();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return ( wrapper.message().has_population_update() && wrapper.message().population_update().has_satisfaction() );
    }
    float Extract( const sword::SimToClient& wrapper );
    //@}

private:
    //! @name Types
    //@{
    struct State
    {
        State( bool isAsked = false ) : isAsked_( isAsked ), value_( 0 ) { }
        bool isAsked_;
        float value_;
    };
    //@}

private:
    //! @name Member data
    //@{q
    State lodging_;
    State health_;
    State safety_;
    //@}
};

}

#endif // __PopulationSatisfaction_h_
