// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Attributes_h_
#define __Attributes_h_

#include "Equipments.h"
#include "Extractors.h"
#include "Humans.h"
#include "Resources.h"
#include "UnitDetection.h"
#include "Values.h"

// =============================================================================
/** @namespace  Attributes
    @brief      Attributes
*/
// Created: AGE 2007-08-28
// =============================================================================

namespace attributes
{

// =============================================================================
/** @class  UnitAttribute
    @brief  UnitAttribute function
*/
// Created: AGE 2007-08-28
// =============================================================================
template< typename Extractor >
struct UnitAttribute : public ContinuousValue< typename Extractor::Type >
{
    enum { has_parameter = Extractor::has_parameter };

    explicit UnitAttribute( const Extractor& extractor = Extractor() )
        : extractor_( extractor ) {}

    void Receive( const MsgsSimToClient::MsgSimToClient& wrapper )
    {
        if( wrapper.message().has_unit_attributes() )
        {
            const MsgsSimToClient::MsgUnitAttributes& attributes = wrapper.message().unit_attributes();
            if( extractor_.HasFlag( attributes ) )
                Set( extractor_.Extract( attributes ) );
        }
    }

    Extractor extractor_;
};

typedef UnitAttribute< extractors::OperationalState > OperationalState;
typedef UnitAttribute< extractors::Position >         Position;
typedef UnitAttribute< extractors::Resources >        Resources;
typedef UnitAttribute< extractors::Equipments >       Equipments;
typedef UnitAttribute< extractors::Humans >           Humans;
typedef UnitAttribute< extractors::Mounted >          Mounted;

struct Detections : public ContinuousValue< extractors::UnitDetection::Type >
{
    enum { has_parameter = extractors::UnitDetection::has_parameter };
    explicit Detections( const extractors::UnitDetection& extractor = extractors::UnitDetection() )
        : extractor_( extractor ) {}
    void Receive( const MsgsSimToClient::MsgSimToClient& wrapper )
    {
        if( extractor_.HasValue( wrapper ) )
            Set( extractor_.Extract( wrapper ) );
    }

    extractors::UnitDetection extractor_;
};

} // namespace attributes


#endif // __Attributes_h_
