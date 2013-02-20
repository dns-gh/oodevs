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
#include "Tools/MIL_IDManager.h"
#include "MIL_DecisionalReport.h"

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
    //@}

    //! @name Operations
    //@{
    template< typename T > void Send( const T& sender, E_Type nType, std::vector< boost::shared_ptr< MIL_MissionParameter_ABC > >& diaParameters ) const;
    //@}

    //! @name $$ BOF
    //@{
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_ComposanteTypePion& parameter );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_DotationCategory& parameter );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const PHY_DotationCategory& parameter, const MIL_AgentPion& pion );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, double nParam1, double nParam2 );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, int nParam );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const std::string& nParam );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, int nParam1, int nParam2, int nParam3 );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, const MIL_Effect_IndirectFire& flyingShell );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Agent > agentKnowledge );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Population >& populationKnowledge, int nParam2 );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, boost::shared_ptr< DEC_Knowledge_Population >& populationKnowledge );
    template< typename T > static void PostEvent( const T& receiver, const MIL_DecisionalReport& nReport, int nParam1, const std::string& nParam2, int nParam3, int nParam4, int nParam5 );
    //@}

private:
    //! @name Constructors/Destructor
    //@{
     MIL_Report( unsigned int nID, xml::xistream& xis );
    ~MIL_Report();
    //@}

    //! @name Tools
    //@{
    static const MIL_Report* Find( unsigned int nID );
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
    static MIL_IDManager ids_;
    //@}
};

#include "MIL_Report.inl"

#endif // __MIL_Report_h_
