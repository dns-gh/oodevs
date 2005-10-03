// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Logistic/MIL_PopulationType.cpp $
// $Author: Jvt $
// $Modtime: 26/04/05 15:52 $
// $Revision: 7 $
// $Workfile: MIL_PopulationType.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "MIL_PopulationType.h"
#include "MIL_Population.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Functions/DEC_PopulationFunctions.h"
#include "Decision/Functions/DEC_ActionFunctions.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMove.h"
#include "MIL_AgentServer.h"

MIL_PopulationType::T_PopulationMap MIL_PopulationType::populations_;

// =============================================================================
// STATIC INITIALIZATION (MANAGER)
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Initialize
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_PopulationType::Initialize( MIL_InputArchive& archive )
{
    MT_LOG_INFO_MSG( "Initializing population types" );

    archive.BeginList( "Populations" );
    while( archive.NextListElement() )
    {
        archive.Section( "Population" );

        std::string strName;
        archive.ReadAttribute( "nom", strName );

        const MIL_PopulationType*& pPopulation = populations_[ strName ];
        if( pPopulation )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Population type already exists", archive.GetContext() );

        pPopulation = new MIL_PopulationType( strName, archive );       
            
        archive.EndSection(); // Population
    }
    archive.EndList(); // Populations
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::Terminate
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
void MIL_PopulationType::Terminate()
{
    for( CIT_PopulationMap it = populations_.begin(); it != populations_.end(); ++it )
        delete it->second;
    populations_.clear();
}

// =============================================================================
// INITIALIZATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType constructor
// Created: NLD 2004-12-20
// -----------------------------------------------------------------------------
MIL_PopulationType::MIL_PopulationType( const std::string& strName, MIL_InputArchive& archive )
    : strName_              ( strName )
    , rConcentrationDensity_( 0. )
    , rDefaultFluxDensity_  ( 0. )
    , rMaxSpeed_            ( 0. )
    , pModel_               ( 0 )
    , pDIAFunctionTable_    ( new DIA_FunctionTable< MIL_Population >() )
{
    archive.ReadField( "MosID", nID_ );
    archive.ReadField( "DensiteConcentration"      , rConcentrationDensity_ );
    archive.ReadField( "DensiteNominaleDeplacement", rDefaultFluxDensity_   );
    archive.ReadField( "VitesseDeplacement"        , rMaxSpeed_             );
    
    std::string strModel;
    archive.ReadField( "ModeleDecisionnel", strModel );
    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPopulation( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown population model", archive.GetContext() );

    InitializeDiaFunctions();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
MIL_PopulationType::~MIL_PopulationType()
{
    delete pDIAFunctionTable_;
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InitializeDiaFunctions
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void MIL_PopulationType::InitializeDiaFunctions()
{
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StopAction   < MIL_Population            >, "DEC_StopAction"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::SuspendAction< MIL_Population            >, "DEC_PauseAction"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::ResumeAction < MIL_Population            >, "DEC_ReprendAction"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_Population_ActionMove >, "DEC_StartDeplacement" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PopulationFunctions::Debug                                 , "DEC_Debug"            );
} 

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationType::InstanciatePopulation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
MIL_Population& MIL_PopulationType::InstanciatePopulation( uint nID, MIL_InputArchive& archive ) const
{
    return *new MIL_Population( *this, nID, archive );
}

