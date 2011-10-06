// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Carriers_h_
#define __Carriers_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  Carriers
    @brief  Carriers
*/
// Created: JSR 2011-10-05
// =============================================================================
class Carriers : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Carriers();
    explicit Carriers( xml::xistream& xis );
    virtual ~Carriers();
    //@}

    //! @name Operations
    //@{
    bool IsCreation( const sword::SimToClient& wrapper ) const;
    NumericValue Extract( const sword::SimToClient& wrapper ) const;
    bool IsDestruction( const sword::SimToClient& wrapper ) const;
    //@}

private:
    //! @name Member data
    //@{
    FilterHelper< int > dotationFilter_;
    //@}
};

}

#endif // __Carriers_h_
