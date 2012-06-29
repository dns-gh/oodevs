// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Decision/DEC_Tools.h"

// -----------------------------------------------------------------------------
// Name: MIL_Report::Find
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
inline
const MIL_Report* MIL_Report::Find( unsigned int nID )
{
    CIT_ReportMap it = reports_.find( nID );
    if( it == reports_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& parameters )
{
    if( diaEvents_.size() <= static_cast< unsigned int >( nReport ) )
        return;
    const MIL_Report* pReport = MIL_Report::Find( diaEvents_[ nReport ] );
    if( pReport )
        pReport->Send< T >( receiver, pReport->category_, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, const PHY_ComposanteTypePion& parameter )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::CreateEquipmentType( &parameter ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, const PHY_DotationCategory& parameter )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::CreateDotationType( &parameter ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, double nParam1, double nParam2 )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter1( MIL_MissionParameterFactory::Create( static_cast< float >( nParam1 ) ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter2( MIL_MissionParameterFactory::Create( static_cast< float >( nParam2 ) ) );
    parameters.push_back( pParameter1 );
    parameters.push_back( pParameter2 );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, int nParam )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::Create( nParam ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, int nParam1, int nParam2, int nParam3 )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter1( MIL_MissionParameterFactory::CreateInteger( nParam1 ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter2( MIL_MissionParameterFactory::CreateInteger( nParam2 ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter3( MIL_MissionParameterFactory::CreateInteger( nParam3 ) );
    parameters.push_back( pParameter1 );
    parameters.push_back( pParameter2 );
    parameters.push_back( pParameter3 );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, const MIL_Effect_IndirectFire& flyingShell )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::CreateTir( static_cast< int >( flyingShell.GetFireID() ) ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: JSR 2010-08-05
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > parameters;
    boost::shared_ptr<MIL_MissionParameter_ABC> pParameter( MIL_MissionParameterFactory::CreateAgentKnowledge( agentKnowledge ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, const std::string& nParam )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > parameters;
    boost::shared_ptr<MIL_MissionParameter_ABC> pParameter( MIL_MissionParameterFactory::Create( nParam ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: MMC 2012-04-16
// -----------------------------------------------------------------------------
template< typename T > inline
void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, DEC_Knowledge_Population& populationKnowledge )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > parameters;
    boost::shared_ptr<MIL_MissionParameter_ABC> pParameter( MIL_MissionParameterFactory::CreatePopulationKnowledge( &populationKnowledge ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: template< typename T > static void MIL_Report::PostEvent
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
template< typename T > inline
 void MIL_Report::PostEvent( const T& receiver, E_EngineReport nReport, int nParam1, const std::string& nParam2, int nParam3, int nParam4, int nParam5 )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter1( MIL_MissionParameterFactory::CreateInteger( nParam1 ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter2( MIL_MissionParameterFactory::Create( nParam2 ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter3( MIL_MissionParameterFactory::CreateInteger( nParam3 ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter4( MIL_MissionParameterFactory::CreateInteger( nParam4 ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter5( MIL_MissionParameterFactory::CreateInteger( nParam5 ) );
    parameters.push_back( pParameter1 );
    parameters.push_back( pParameter2 );
    parameters.push_back( pParameter3 );
    parameters.push_back( pParameter4 );
    parameters.push_back( pParameter5 );
    PostEvent( receiver, nReport, parameters );
}
