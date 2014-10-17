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
#include "Entities/Specialisations/LOG/MIL_AutomateTypeBaseLOG.h"
#include "Entities/Specialisations/Refugie/MIL_AutomateTypeREFUGIE.h"
#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"

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
    automateTypeAllocators_[ "Automate LOG TC2"                ] = &MIL_AutomateTypeBaseLOG::Create;
    automateTypeAllocators_[ "Automate JOINT"                  ] = &MIL_AutomateType       ::Create;
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
    automateTypeAllocators_[ "Automate Organization"           ] = &MIL_AutomateTypeBaseLOG::Create;
    automateTypeAllocators_[ "Automate Notable"                ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate Journalist"             ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate Civilian"               ] = &MIL_AutomateType       ::Create;

    xis >> xml::start( "automats" )
            >> xml::list( "automat", &MIL_AutomateType::ReadAutomat )
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

    auto itAutomateAllocator = automateTypeAllocators_.find( strType );
    if( itAutomateAllocator == automateTypeAllocators_.end() )
        throw MASA_EXCEPTION( xis.context() + "Unknown automate type" );

    const MIL_AutomateType*& pType = automateTypes_[ strName ];
    if( pType )
        throw MASA_EXCEPTION( xis.context() + "Automate type already defined" );

    try
    {
        pType = (*itAutomateAllocator->second)( strName, xis );
    }
    catch( const std::exception& e )
    {
        throw MASA_EXCEPTION( xis.context() + tools::GetExceptionMsg( e ) );
    }

    if( pType )
    {
        if( !ids_.insert( pType->GetID() ).second )
            throw MASA_EXCEPTION( xis.context() + "Automate type ID already used" );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AutomateType::Terminate()
{
    for( auto it = automateTypes_.begin(); it != automateTypes_.end(); ++it )
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
    : nID_    ( 0 )
    , strName_( strName )
    , pModel_ ( 0 )
    , pTypePC_( 0 )
{
    xis >> xml::attribute( "id", nID_ )
        >> xml::list( "unit", *this, &MIL_AutomateType::ReadUnit );
    if( !pTypePC_ )
        throw MASA_EXCEPTION( xis.context() + "No command-post defined for automat type: " + strName_ );
    InitializeModel( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType constructor
// Created: LDC 2009-04-24
// -----------------------------------------------------------------------------
MIL_AutomateType::MIL_AutomateType( const DEC_Model_ABC& model )
    : nID_( 0 )
    , pModel_( &model )
    , pTypePC_( 0 )
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
        throw MASA_EXCEPTION( xis.context() + "Unknown pawn type" );

    if( FindUnitType( *pType ) )
        throw MASA_EXCEPTION( xis.context() + "Pawn type already defined in composition" );

    sCompositionBounds bounds;
    bounds.nMax_ = std::numeric_limits< unsigned int >::max();
    bounds.nMin_ = 0u;

    xis >> xml::optional >> xml::attribute( "min-occurs", bounds.nMin_ )
        >> xml::optional >> xml::attribute( "max-occurs", bounds.nMax_ );

    bool isCommandPost = false;
    xis >> xml::optional >> xml::attribute( "command-post", isCommandPost );
    if( isCommandPost )
    {
        if( !pTypePC_ )
            pTypePC_ = pType;
        else
            throw MASA_EXCEPTION( xis.context() + "Multiple command-post defined in automat type: " + strName_ );
        composition_.insert( composition_.begin(), std::make_pair( pType, bounds ) );
    }
    else
        composition_.push_back( std::make_pair( pType, bounds ) );
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
        throw MASA_EXCEPTION( xis.context() + "Unknown automata model" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::CheckComposition
// Created: NLD 2004-09-10
// Modified: JVT 2005-02-01
// -----------------------------------------------------------------------------
bool MIL_AutomateType::CheckComposition( const MIL_Automate& automate ) const
{
    std::map< const MIL_AgentType_ABC*, unsigned int > currentComposition;
    const MIL_Automate::T_PionVector& pions = automate.GetPions();
    for( auto it = pions.begin(); it != pions.end(); ++it )
    {
        const MIL_AgentPion& pion = **it;
        const MIL_AgentType_ABC& pionType = pion.GetType();
        ++currentComposition[ &pionType ];
        if( !FindUnitType( pionType ) )
            return false;
    }

    for( auto it = composition_.begin(); it != composition_.end(); ++it )
    {
        const sCompositionBounds& bounds = it->second;
        const unsigned int& nRealNbr = currentComposition[ it->first ];
        if( bounds.nMin_ > nRealNbr || bounds.nMax_ < nRealNbr )
            return false;
    }
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InstanciateAutomate
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateType::InstanciateAutomate( unsigned int nID,
                                                     MIL_Entity_ABC& parent,
                                                     MissionController_ABC& controller,
                                                     xml::xistream& xis,
                                                     unsigned int gcPause,
                                                     unsigned int gcMult,
                                                     sword::DEC_Logger* logger ) const
{
    return *new MIL_Automate( *this, nID, controller, parent, xis, gcPause, gcMult, logger );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InstanciateAutomate
// Created: LDC 2010-10-05
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateType::InstanciateAutomate( unsigned int nID,
                                                     MIL_Entity_ABC& parent,
                                                     MissionController_ABC& controller,
                                                     unsigned int knowledgeGroup,
                                                     const std::string& name,
                                                     unsigned int gcPause,
                                                     unsigned int gcMult,
                                                     sword::DEC_Logger* logger,
                                                     unsigned int context,
                                                     const MIL_DictionaryExtensions& extensions ) const
{
    return *new MIL_Automate( *this, nID, controller, parent, knowledgeGroup, name, gcPause, gcMult, logger, context, extensions );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AutomateType::RegisterFunctions( sword::Brain& /*brain*/, MIL_Automate& /*automat*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::FindAutomateType
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateType::FindAutomateType( const std::string& strName )
{
    auto it = automateTypes_.find( strName );
    return it == automateTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::FindAutomateType
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateType::FindAutomateType( unsigned int nID )
{
    for( auto it = automateTypes_.begin(); it != automateTypes_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
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

std::string MIL_AutomateType::GetArchetypeName() const
{
    return "MIL_AutomateType";
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetID
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
// Name: MIL_AutomateType::IsLogisticBase
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
bool MIL_AutomateType::IsLogisticBase() const
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

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetComposition
// Created: LGY 2014-01-20
// -----------------------------------------------------------------------------
const MIL_AutomateType::T_CompositionMap& MIL_AutomateType::GetComposition() const
{
    return composition_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::GetComposition
// Created: LGY 2014-01-20
// -----------------------------------------------------------------------------
bool MIL_AutomateType::FindUnitType( const MIL_AgentType_ABC& type ) const
{
    return std::find_if( composition_.begin(), composition_.end(),
        [&type]( std::pair< const MIL_AgentType_ABC*, sCompositionBounds > value )->bool {
            return type.GetID() == value.first->GetID(); } ) != composition_.end();
}
