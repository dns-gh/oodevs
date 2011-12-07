// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_TelepathyFunctions.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include <directia/brain/Brain.h>

// -----------------------------------------------------------------------------
// Name: DEC_TelepathyFunctions::Register
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
void DEC_TelepathyFunctions::Register( directia::brain::Brain& brain )
{
    brain[ "F_CopyPlotsRavitaillement" ] = &DEC_TelepathyFunctions::CopyPlotsRavitaillement;
    brain[ "F_GetListeEnisTirAutorise" ] = &DEC_TelepathyFunctions::GetListeEnisTirAutorise;
    brain[ "F_RemoveAgentFromListeEnisTirAutorise" ] = &DEC_TelepathyFunctions::RemoveAgentFromListeEnisTirAutorise;
}

// -----------------------------------------------------------------------------
// Name: DEC_TelepathyFunctions::CopyPlotsRavitaillement
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
void DEC_TelepathyFunctions::CopyPlotsRavitaillement( DEC_Decision_ABC* pPion, const std::vector< boost::shared_ptr< DEC_Knowledge_Object > >& value )
{
    if( !pPion )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    pPion->SetVariable( "myself.plotsRavitaillement_", value );
}

// -----------------------------------------------------------------------------
// Name: std::vector<int> DEC_TelepathyFunctions::GetListeEnisTirAutorise
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > DEC_TelepathyFunctions::GetListeEnisTirAutorise( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    return pPion->GetVariable< std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > >( "myself.listeEnisTirAutorise_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_TelepathyFunctions::RemoveAgentFromListeEnisTirAutorise
// Created: LDC 2009-07-31
// -----------------------------------------------------------------------------
void DEC_TelepathyFunctions::RemoveAgentFromListeEnisTirAutorise( DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Agent > eni )
{
    if( !pPion )
        throw std::runtime_error( __FUNCTION__ ": invalid parameter." );
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > listEnis = pPion->GetVariable< std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > >( "myself.listeEnisTirAutorise_" );
    for( std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >::iterator it = listEnis.begin(); it != listEnis.end(); ++it )
    {
        if( *it == eni )
        {
            listEnis.erase( it );
            break;
        }
    }
    pPion->SetVariable( "myself.listeEnisTirAutorise_", listEnis );
}
