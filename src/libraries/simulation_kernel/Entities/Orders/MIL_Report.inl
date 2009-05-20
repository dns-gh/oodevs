// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_Report::Find
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
inline
const MIL_Report* MIL_Report::Find( uint nID )
{
    CIT_ReportMap it = reports_.find( nID );
    if( it == reports_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: template< typename T > void MIL_Report::Send
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::Send( const T& sender, E_Type nType, const DIA_Parameters& diaParameters ) const
{
    DoSend( sender.GetID(), nType, sender.GetKnowledge(), diaParameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, DIA_Parameters& parameters )
{
    if( diaEvents_.size() <= (uint)nReport )
        return;

//    const DEC_Decision_ABC& engine = receiver.GetDecision();
//    DIA_Variable_ABC* pResult = const_cast< DEC_Decision_ABC& >( engine ).ExecuteFunction( diaEvents_[ nReport ], parameters );
//    if( pResult ) 
//        delete pResult;
    
    const MIL_Report* pReport = MIL_Report::Find( diaEvents_[ nReport ] );
    if( pReport )
        pReport->Send< T >( receiver, MIL_Report::eRcTypeOperational, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport )
{
    DIA_Parameters parameters;
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, const PHY_ComposanteTypePion& parameter )
{
    DIA_Variable_Void diaParam( (void*)&parameter, &DEC_Tools::GetTypeEquipement() );

    DIA_Parameters parameters;
    parameters.AddParam( &diaParam );

    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, const PHY_DotationCategory& parameter )
{
    DIA_Variable_Void diaParam( (void*)&parameter, &DEC_Tools::GetTypeDotation() );

    DIA_Parameters parameters;
    parameters.AddParam( &diaParam );

    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, MT_Float nParam1, MT_Float nParam2 )
{
    DIA_Variable_Float diaParam1( (float)nParam1 );
    DIA_Variable_Float diaParam2( (float)nParam2 );

    DIA_Parameters parameters;
    parameters.AddParam( &diaParam1 );
    parameters.AddParam( &diaParam2 );

    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, int nParam )
{
    DIA_Variable_Id diaParam( nParam );

    DIA_Parameters parameters;
    parameters.AddParam( &diaParam );

    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, const MIL_Effect_IndirectFire& flyingShell )
{
    DIA_Variable_Void diaParam( (void*)flyingShell.GetFireID(), &DEC_Tools::GetTypeTirIndirect() );

    DIA_Parameters parameters;
    parameters.AddParam( &diaParam );

    PostEvent( receiver, nReport, parameters );
}
