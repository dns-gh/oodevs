// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogMedicalWaitingAttention_h_
#define __LogMedicalWaitingAttention_h_

#include "Extractors.h"
#include "FilterHelper.h"

namespace extractors
{

// =============================================================================
/** @class  LogMedicalWaitingAttention
    @brief  LogMedicalWaitingAttention
*/
// Created: FPO 2011-05-09
// =============================================================================
class LogMedicalWaitingAttention : public Extractor< NumericValue >
{

public:
    //! @name Types
    //@{
    enum { has_parameter = true };
    //@}

    //! @name Constructors/Destructor
    //@{
             LogMedicalWaitingAttention();
    explicit LogMedicalWaitingAttention( xml::xistream& xis );
    virtual ~LogMedicalWaitingAttention();
    //@}

    //! @name Operations
    //@{
    bool HasValue( const sword::SimToClient& wrapper ) const
    {
        return ( wrapper.message().has_log_medical_handling_update() && wrapper.message().log_medical_handling_update().has_state() );
    }
    int Extract( const sword::SimToClient& wrapper );
    //@}

private:
    //! @name Member data
    //@{
    std::map< int, bool > requestWaitingForAttention_;
    //@}
};

}

#endif // __LogMedicalWaitingAttention_h_
