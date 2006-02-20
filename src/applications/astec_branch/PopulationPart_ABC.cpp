// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationPart_ABC.h"


// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC constructor
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
PopulationPart_ABC::PopulationPart_ABC()
    : attitude_( E_PopulationAttitude( 0 ) )
{
    InterfaceContainer< Extension_ABC >::Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC destructor
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
PopulationPart_ABC::~PopulationPart_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::GetAttitude
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
std::string PopulationPart_ABC::GetAttitude() const
{
    return ENT_Tr::ConvertFromPopulationAttitude( attitude_ ); // $$$$ AGE 2006-02-20: 
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::DoUpdate
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void PopulationPart_ABC::DoUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
    if ( asnMsg.m.attitudePresent )
		attitude_ = (E_PopulationAttitude)asnMsg.attitude;
}

// -----------------------------------------------------------------------------
// Name: PopulationPart_ABC::DoUpdate
// Created: AGE 2006-02-20
// -----------------------------------------------------------------------------
void PopulationPart_ABC::DoUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
    if ( asnMsg.m.attitudePresent )
		attitude_ = (E_PopulationAttitude)asnMsg.attitude;
}
    