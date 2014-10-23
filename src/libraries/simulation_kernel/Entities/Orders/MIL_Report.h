// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Report_h_
#define __MIL_Report_h_

#include <string>
#include <vector>
#include <map>
#include "MIL_MissionParameterFactory.h"
#include "MIL_DecisionalReport.h"
#include "Entities/Populations/DEC_PopulationKnowledge.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "Decision/DEC_Tools.h"
#include "Entities/Agents/MIL_Agent_ABC.h"

namespace xml
{
    class xistream;
}

namespace client
{
    class Report;
}

class MIL_ParameterType_ABC;
class PHY_ComposanteTypePion;
class PHY_DotationCategory;
class MIL_Effect_IndirectFire;
class MIL_MissionParameter_ABC;
class MIL_LimaFunction;

// =============================================================================
/** @class  MIL_Report
    @brief  MIL_Report
*/
// Created: NLD 2006-11-14
// =============================================================================
class MIL_Report
{
public:
    //! @name Types
    //@{
    enum E_Type
    {
        eRcTypeMessage     = 0,
        eRcTypeOperational = 1,
        eRcTypeEvent       = 2,
        eRcTypeWarning     = 3
    };
    //@}

public:
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static const MIL_Report* Find( const std::string& key );
    static const MIL_Report* Find( unsigned int nID );
    //@}

    //! @name Operations
    //@{
    template< typename T > void Send( const T& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const;
    void Send( const sword::Tasker& tasker, const sword::MissionParameters& parameters ) const;
    //@}

    //! @name $$ BOF
    //@{
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_ComposanteTypePion& parameter );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const MIL_AgentPion& pion );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_DotationCategory& parameter );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_DotationCategory& parameter, int nParam );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_DotationCategory& parameter, const MIL_AgentPion& pion );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, double nParam1, double nParam2 );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, int nParam );
    template< typename T > static void PostEventInteger( const T& receiver, const MIL_DecisionalReport& nReport, int nParam );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const std::string& nParam );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, int nParam1, int nParam2, int nParam3 );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const MIL_Effect_IndirectFire& flyingShell );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const MIL_LimaFunction& limaFunction, const std::string& nParam );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Population >& populationKnowledge, int nParam2 );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Population >& populationKnowledge );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const boost::shared_ptr< DEC_Knowledge_Agent >& agentKnowledge, const std::string& nParam2, int nParam3, int nParam4, int nParam5 );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const boost::shared_ptr< DEC_Knowledge_Agent >& agentKnowledge, int nParam3, int nParam4, int nParam5 );
    //@}

public:
    //! @name Member data
    //@{
    static unsigned int nextMessageId_;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     MIL_Report( unsigned int nID, xml::xistream& xis );
    ~MIL_Report();
    //@}

    //! @name Tools
    //@{
    template< typename T >
    static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& parameters );
    bool DoSend( client::Report& message, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& params ) const;
    //@}

    //! @name Helpers
    //@{
    static void ReadReport( xml::xistream& xis );
    void ReadParameter( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, const MIL_Report* > T_ReportMap;
    typedef std::map< std::string, unsigned int >       T_KeyMap;
    typedef std::vector< const MIL_ParameterType_ABC* > T_ParameterVector;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int nID_;
    std::string strMessage_;
    E_Type category_;
    T_ParameterVector parameters_;
    static T_ReportMap reports_;
    static T_KeyMap keys_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >& parameters )
{
    const MIL_Report* pReport = MIL_Report::Find( static_cast< const std::string& >( nReport ) );
    if( pReport )
        pReport->Send< T >( receiver, pReport->category_, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport )
{
    if( !receiver.CanEmitReports() )
        return;
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_ComposanteTypePion& parameter )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::CreateEquipmentType( &parameter ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: MCO 2013-04-24
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const MIL_AgentPion& pion )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    parameters.push_back( MIL_MissionParameterFactory::CreateAgent( pion ) );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_DotationCategory& parameter )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::CreateDotationType( &parameter ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: JSR 2014-08-13
// -----------------------------------------------------------------------------
template< typename T > 
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_DotationCategory& parameter, int nParam )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    parameters.push_back( MIL_MissionParameterFactory::CreateDotationType( &parameter ) );
    parameters.push_back( MIL_MissionParameterFactory::CreateInteger( nParam ) );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: MCO 2012-12-03
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_DotationCategory& parameter, const MIL_AgentPion& pion )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    parameters.push_back( MIL_MissionParameterFactory::CreateDotationType( &parameter ) );
    parameters.push_back( MIL_MissionParameterFactory::CreateAgent( pion ) );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NLD 2006-12-06
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, double nParam1, double nParam2 )
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
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, int nParam )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::Create( nParam ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEventInteger
// Created: LGY 2013-08-23
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEventInteger( const T& receiver, const MIL_DecisionalReport& nReport, int nParam )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::CreateInteger( nParam ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: LDC 2011-05-09
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, int nParam1, int nParam2, int nParam3 )
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
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const MIL_Effect_IndirectFire& flyingShell )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter( MIL_MissionParameterFactory::CreateTir( static_cast< int >( flyingShell.GetFireID() ) ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: MMC 2013-10-14
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const MIL_LimaFunction& limaFunction, const std::string& nParam )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    auto pParameter1 = MIL_MissionParameterFactory::CreateLimaFunction( limaFunction.GetID() );
    auto pParameter2 = MIL_MissionParameterFactory::Create( nParam );
    parameters.push_back( pParameter1 );
    parameters.push_back( pParameter2 );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: JSR 2010-08-05
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > parameters;
    boost::shared_ptr<MIL_MissionParameter_ABC> pParameter( MIL_MissionParameterFactory::CreateAgentKnowledge( agentKnowledge ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > parameters;
    boost::shared_ptr<MIL_MissionParameter_ABC> pParameter( MIL_MissionParameterFactory::CreateObjectKnowledge( objectKnowledge ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: LGY 2011-10-14
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const std::string& nParam )
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
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Population >& populationKnowledge )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > parameters;
    boost::shared_ptr<MIL_MissionParameter_ABC> pParameter( MIL_MissionParameterFactory::CreatePopulationKnowledge( populationKnowledge ) );
    parameters.push_back( pParameter );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: NPT 2013-02-13
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Population >& populationKnowledge, int nParam2 )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > parameters;
    boost::shared_ptr<MIL_MissionParameter_ABC> pParameter( MIL_MissionParameterFactory::CreatePopulationKnowledge( populationKnowledge ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter2( MIL_MissionParameterFactory::Create( nParam2 ) );
    parameters.push_back( pParameter );
    parameters.push_back( pParameter2 );
    PostEvent( receiver, nReport, parameters );
}

// -----------------------------------------------------------------------------
// Name: MIL_Report::PostEvent
// Created: LGY 2012-06-28
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const boost::shared_ptr< DEC_Knowledge_Agent >& agentKnowledge, const std::string& nParam2, int nParam3, int nParam4, int nParam5 )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter1( MIL_MissionParameterFactory::CreateAgentKnowledge( agentKnowledge ) );
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

template< typename T >
void MIL_Report::PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const boost::shared_ptr< DEC_Knowledge_Agent >& agentKnowledge, int nParam2, int nParam3, int nParam4 )
{
    std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > > parameters;
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter1( MIL_MissionParameterFactory::CreateAgentKnowledge( agentKnowledge ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter2( MIL_MissionParameterFactory::CreateInteger( nParam2 ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter3( MIL_MissionParameterFactory::CreateInteger( nParam3 ) );
    boost::shared_ptr< MIL_MissionParameter_ABC > pParameter4( MIL_MissionParameterFactory::CreateInteger( nParam4 ) );
    parameters.push_back( pParameter1 );
    parameters.push_back( pParameter2 );
    parameters.push_back( pParameter3 );
    parameters.push_back( pParameter4 );
    PostEvent( receiver, nReport, parameters );
}

#endif // __MIL_Report_h_
