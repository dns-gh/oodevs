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

    void Receive( const MsgSimToClient& wrapper )
    {
        if( wrapper.message().has_unit_attributes() )
        {
            const MsgUnitAttributes& attributes = wrapper.message().unit_attributes();
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

} // namespace attributes


#endif // __Attributes_h_
