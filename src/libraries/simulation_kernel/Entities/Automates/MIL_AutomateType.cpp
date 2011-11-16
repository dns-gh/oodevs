// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_AutomateType.cpp $
// $Author: Jvt $
// $Modtime: 23/05/05 16:35 $
// $Revision: 47 $
// $Workfile: MIL_AutomateType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_AutomateType.h"
#include "MIL_Automate.h"
#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Tools.h"
#include "MIL_AgentServer.h"
#include "Entities/Specialisations/ASA/MIL_AutomateTypeASA.h"
#include "Entities/Specialisations/LOG/MIL_AutomateTypeLOG.h"
#include "Entities/Specialisations/REFUGIE/MIL_AutomateTypeREFUGIE.h"
#include "Entities/Agents/Units/Humans/MIL_AutomateTypeInjuredHuman.h"
#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.hpp>

MIL_AutomateType::T_AutomateTypeAllocatorMap  MIL_AutomateType::automateTypeAllocators_;
MIL_AutomateType::T_AutomateTypeMap           MIL_AutomateType::automateTypes_;

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Create
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateType::Create( const std::string& strName, xml::xistream& xis)
{
    return new MIL_AutomateType( strName, xis );
}

struct MIL_AutomateType::LoadingWrapper
{
    void ReadAutomat( xml::xistream& xis )
    {
        MIL_AutomateType::ReadAutomat( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AutomateType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing automate types" );
    automateTypeAllocators_[ "Automate ABC"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate ALAT"                   ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate ASA"                    ] = &MIL_AutomateTypeASA    ::Create;
    automateTypeAllocators_[ "Automate ASS"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate ASY"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate CIRCULATION"            ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate GEN"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate INF"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate JOINT"                  ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate LOG TC2"                ] = &MIL_AutomateTypeLOG    ::Create;
    automateTypeAllocators_[ "Automate LOG BLD Sante"          ] = &MIL_AutomateTypeLOG    ::Create;
    automateTypeAllocators_[ "Automate LOG BLD Maintenance"    ] = &MIL_AutomateTypeLOG    ::Create;
    automateTypeAllocators_[ "Automate LOG BLD Ravitaillement" ] = &MIL_AutomateTypeLOG    ::Create;
    automateTypeAllocators_[ "Automate LOG BLT Sante"          ] = &MIL_AutomateTypeLOG    ::Create;
    automateTypeAllocators_[ "Automate LOG BLT Maintenance"    ] = &MIL_AutomateTypeLOG    ::Create;
    automateTypeAllocators_[ "Automate LOG BLT Ravitaillement" ] = &MIL_AutomateTypeLOG    ::Create;
    automateTypeAllocators_[ "Automate MILICE"                 ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate NBC"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate REFUGIE"                ] = &MIL_AutomateTypeREFUGIE::Create;
    automateTypeAllocators_[ "Automate RENS"                   ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate TRANS"                  ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate SGTIA"                  ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate Emergency"              ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate Organization"           ] = &MIL_AutomateTypeLOG    ::Create;
    automateTypeAllocators_[ "Automate Notable"                ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate Journalist"             ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate Civilian"               ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate Injured Human"          ] = &MIL_AutomateTypeInjuredHuman::Create;

    LoadingWrapper loader;

    xis >> xml::start( "automats" )
            >> xml::list( "automat", loader, &LoadingWrapper::ReadAutomat )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::ReadAutomat
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AutomateType::ReadAutomat( xml::xistream& xis )
{
    std::set< unsigned int > ids_;
    std::string strName;
    std::string strType;

    xis >> xml::attribute( "name", strName )
        >> xml::attribute( "type", strType );

    CIT_AutomateTypeAllocatorMap itAutomateAllocator = automateTypeAllocators_.find( strType );
    if( itAutomateAllocator == automateTypeAllocators_.end() )
        xis.error( "Unknown automate type" );

    const MIL_AutomateType*& pType = automateTypes_[ strName ];
    if( pType )
        xis.error( "Automate type already defined" );

    try
    {
        pType = (*itAutomateAllocator->second)( strName, xis );
    }
    catch( std::runtime_error& e )
    {
        xis.error( e.what() );
        pType = 0;
    }

    if( pType )
    {
        if( !ids_.insert( pType->GetID() ).second )
            xis.error( "Automate type ID already used" );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AutomateType::Terminate()
{
    for( CIT_AutomateTypeMap it = automateTypes_.begin(); it != automateTypes_.end(); ++it )
        delete it->second;
    automateTypes_.clear();
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_AutomateType::MIL_AutomateType( const std::string& strName, xml::xistream& xis )
    : nID_                            ( 0 )
    , strName_                        ( strName )
    , pTypePC_                        ( 0 )
    , pModel_                         ( 0 )
    , rRapForIncreasePerTimeStepValue_( DEC_Knowledge_RapFor_ABC::GetRapForIncreasePerTimeStepDefaultValue() )
{
    xis >> xml::attribute( "id", nID_ )
        >> xml::list( "unit", *this, &MIL_AutomateType::ReadUnit );
    if( !pTypePC_ )
        xis.error( "No command-post defined for automat type: " + strName_ );

    InitializeRapFor      ( xis );
    InitializeModel       ( xis );
    InitializeDiaFunctions();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType constructor
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
MIL_AutomateType::MIL_AutomateType( const DEC_Model_ABC& model )
    : pModel_( &model )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_AutomateType::~MIL_AutomateType()
{
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::ReadUnit
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AutomateType::ReadUnit( xml::xistream& xis )
{
    std::string strPionType;
    xis >> xml::attribute( "type", strPionType );

    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( strPionType );
    if( !pType )
        xis.error( "Unknown pawn type" );

    if( composition_.find( pType ) != composition_.end() )
        xis.error( "Pawn type already defined in composition" );

    composition_[ pType ].nMax_ = std::numeric_limits< unsigned int >::max();
    composition_[ pType ].nMin_ = 0;

    xis >> xml::optional >> xml::attribute( "min-occurs", composition_[ pType ].nMin_ )
        >> xml::optional >> xml::attribute( "max-occurs", composition_[ pType ].nMax_ );

    bool isCommandPost = false;
    xis >> xml::optional >> xml::attribute( "command-post", isCommandPost );
    if( isCommandPost )
        if( !pTypePC_ )
            pTypePC_ = pType;
        else
            xis.error( "Multiple command-post defined in automat type: " + strName_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeModel
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeModel( xml::xistream& xis )
{
    std::string strModel;
    xis >> xml::attribute( "decisional-model", strModel );
    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelAutomate( strModel );
    if( !pModel_ )
        xis.error( "Unknown automata model" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeRapFor
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeRapFor( xml::xistream& xis )
{
    double rTimeTmp;
    if( tools::ReadTimeAttribute( xis, "force-ratio-feedback-time", rTimeTmp ) )
    {
        rTimeTmp                         = MIL_Tools::ConvertSecondsToSim( rTimeTmp );
        rRapForIncreasePerTimeStepValue_ = DEC_Knowledge_RapFor_ABC::ComputeRapForIncreasePerTimeStepValue( rTimeTmp );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::CheckComposition
// Created: NLD 2004-09-10
// Modified: JVT 2005-02-01
// -----------------------------------------------------------------------------
bool MIL_AutomateType::CheckComposition( const MIL_Automate& automate ) const
{
    typedef std::map< const MIL_AgentType_ABC*, unsigned int > T_CounterMap;
    typedef T_CounterMap::const_iterator               CIT_CounterMap;

    T_CounterMap currentComposition;
    const MIL_Automate::T_PionVector& pions = automate.GetPions();

    for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
    {
        const MIL_AgentPion& pion = **it;

        const MIL_AgentType_ABC& pionType = pion.GetType();
        ++currentComposition[ &pionType ];
        if( composition_.find( &pionType ) == composition_.end() )
            return false;
    }

    for( CIT_CompositionMap it = composition_.begin(); it != composition_.end(); ++it )
    {
        const sCompositionBounds& bounds   = it->second;
        const unsigned int&               nRealNbr = currentComposition[ it->first ];

        if( bounds.nMin_ > nRealNbr || bounds.nMax_ < nRealNbr )
            return false;
    }

    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeDiaFunctions
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeDiaFunctions()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InstanciateAutomate
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateType::InstanciateAutomate( unsigned int nID, MIL_Entity_ABC& parent, xml::xistream& xis, unsigned int gcPause, unsigned int gcMult ) const
{
    return *new MIL_Automate( *this, nID, parent, xis, gcPause, gcMult );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InstanciateAutomate
// Created: LDC 2010-10-05
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateType::InstanciateAutomate( unsigned int nID, MIL_Entity_ABC&  parent, unsigned int knowledgeGroup, const std::string& name, unsigned int gcPause, unsigned int gcMult, unsigned int context ) const
{
    return *new MIL_Automate( *this, nID, parent, knowledgeGroup, name, gcPause, gcMult, context );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AutomateType::RegisterFunctions( directia::brain::Brain& /*brain*/, MIL_Automate& /*automat*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::FindAutomateType
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateType::FindAutomateType( const std::string& strName )
{
    CIT_AutomateTypeMap it = automateTypes_.find( strName );
    return it == automateTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::FindAutomateType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateType::FindAutomateType( unsigned int nID )
{
    CIT_AutomateTypeMap it = std::find_if( automateTypes_.begin(), automateTypes_.end(), std::compose1( std::bind2nd( std::equal_to< unsigned int >(), nID ), std::compose1( std::mem_fun( &MIL_AutomateType::GetID ), std::select2nd< T_AutomateTypeMap::value_type >() ) ) );
    return it == automateTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetRapForIncreasePerTimeStepValue
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
double MIL_AutomateType::GetRapForIncreasePerTimeStepValue() const
{
    return rRapForIncreasePerTimeStepValue_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetTypePionPC
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
const MIL_AgentType_ABC& MIL_AutomateType::GetTypePionPC() const
{
    assert( pTypePC_ );
    return *pTypePC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetModel
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
const DEC_Model_ABC& MIL_AutomateType::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetMosID
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
unsigned int MIL_AutomateType::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetName
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
const std::string& MIL_AutomateType::GetName() const
{
    return strName_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::IsLogistic
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool MIL_AutomateType::IsLogistic() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool MIL_AutomateType::IsRefugee() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::IsInjuredHuman
// Created: RPD 2010-11-10
// -----------------------------------------------------------------------------
bool MIL_AutomateType::IsInjuredHuman() const
{
    return false;
}
