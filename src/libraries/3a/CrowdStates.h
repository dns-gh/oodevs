// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __CrowdStates_h_
#define __CrowdStates_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  CrowdStates
    @brief  CrowdStates
*/
// Created: FPO 2011-05-13
// =============================================================================
class CrowdStates : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             CrowdStates();
    explicit CrowdStates( xml::xistream& xis );
    virtual ~CrowdStates();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return ( wrapper.message().has_crowd_update() );
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
    State contaminated_;
    //@}
};

}

#endif // __CrowdStates_h_
