// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PopulationStates_h_
#define __PopulationStates_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  PopulationStates
    @brief  PopulationStates
*/
// Created: FPO 2011-05-05
// =============================================================================
class PopulationStates : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 PopulationStates();
    /*implicit*/ PopulationStates( xml::xistream& xis );
    virtual     ~PopulationStates();
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
    //! @name Types
    //@{
    struct State
    {
        State( bool isAsked = false ) : isAsked_( isAsked ), quantity_( 0 ) { }
        bool isAsked_;
        int quantity_;
    };
    //@}

private:
    //! @name Member data
    //@{
    State healthy_;
    State wounded_;
    State dead_;
    //@}
};

}

#endif // __PopulationStates_h_
