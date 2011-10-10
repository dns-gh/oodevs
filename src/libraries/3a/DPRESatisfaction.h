// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DPRESatisfaction_h_
#define __DPRESatisfaction_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  DPRESatisfaction
    @brief  DPRESatisfaction
*/
// Created: JSR 2011-08-12
// =============================================================================
class DPRESatisfaction : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DPRESatisfaction();
    explicit DPRESatisfaction( xml::xistream& xis );
    virtual ~DPRESatisfaction();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return ( wrapper.message().has_unit_attributes() && wrapper.message().unit_attributes().has_satisfaction() );
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

#endif // __DPRESatisfaction_h_
