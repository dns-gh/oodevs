// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UnitDetection_h_
#define __UnitDetection_h_

#include "Extractors.h"
#include "FilterHelper.h"

using namespace MsgsSimToClient;

namespace extractors
{

// =============================================================================
/** @class  UnitDetection
    @brief  UnitDetection
            detected='id' id of unit to be detected
            visibility='level1,level2...' detection level
*/
// Created: SBO 2010-06-01
// =============================================================================
class UnitDetection : public Extractor< NumericValue >
{
public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
                 UnitDetection();
    /*implicit*/ UnitDetection( xml::xistream& xis );
    virtual     ~UnitDetection() {}
    //@}

    //! @name Operations
    //@{
    bool HasValue( const MsgSimToClient& wrapper ) const
    {
        return wrapper.message().has_unit_detection();
    }
    float Extract( const MsgSimToClient& wrapper ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned long detectedUnitId_;
    int visibilityMask_;
    //@}
};

}

#endif // __UnitDetection_h_
