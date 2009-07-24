// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ReportManager_h_
#define __ReportManager_h_

#include "game_asn/Simulation.h"
#include "game_asn/plugin.h"
#include <set>

namespace dispatcher
{
  class Model;
  class Agent;
  class Automat;
  class Object;
  class SimulationPublisher_ABC;
  class ClientPublisher_ABC;
  class Profile_ABC;
}

namespace kernel
{
  class MissionType;
}

namespace xml
{
  class xistream;
  class xostream;
}

namespace plugins
{
namespace xmlia
{
  class Point;
  class UnitAgent;
  class UnitAutomate;
  class Report_ABC;
  class Publisher_ABC;
  class LdapClient;
  class XmliaPlugin;
  class ClientManager;

// =============================================================================
/** @class  ReportManager
    @brief  ReportManager
*/
// Created: MGD 2009-06-12
// =============================================================================
class ReportManager
{
//@GLOBAL REFACTOR replace pointer by autoptr
public:
    //! @name Constructors/Destructor
    //@{
    ReportManager( dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulationPublisher, LdapClient& Ldap, ClientManager& clientManager );
    virtual ~ReportManager();
    //@}

    ///! @name Operations
    //@{
    dispatcher::Model& GetModel() const;
    dispatcher::SimulationPublisher_ABC& GetSimulationPublisher() const;
    dispatcher::ClientPublisher_ABC& GetClientPublisher( const std::string& name ) const;
    bool IsTakenIntoAccount( const std::string messageType );
    //@}

    ///! @name Export Operations
    //@{
    void Send( Publisher_ABC& publisher )const;
    void Receive( Publisher_ABC& publisher);
    void DoUpdate( dispatcher::Agent& agent );
    void DoUpdate( dispatcher::Agent& agent, dispatcher::Agent& detected );
    void DoUpdate( dispatcher::Agent& agent, dispatcher::Object& detected );
    void DoUpdate( dispatcher::Agent& agent, const ASN1T_MsgUnitOrder& message );
    void DoUpdate( dispatcher::Automat& agent, const ASN1T_MsgAutomatOrder& message );
    void DoUpdateIntrep( dispatcher::Agent& agent,  dispatcher::Agent& fired );
    bool ReadLine( const ASN1T_Line& asn, std::vector< Point >& points );
    void AddIntelligence( const unsigned intelligenceID, const std::string );
    bool FindIntelligence( unsigned intelligenceID );
    //@}

    ///! @name Import Operations
    //@{
    void CleanReceivedRapport();
    void Read( std::string& message );
    void ReadUrl( xml::xistream& xis, Publisher_ABC& publisher );
    void UpdateSimulation() const ;
    //@}

    ///! @name MagicAction Operations
    //@{
    void SendReport( std::string report );
    void MagicMove( UnitAgent& reportedUnit );
    void UpdateOperationnalState( UnitAgent& reportedUnit );
    void InsertOrUpdateIntelligence( UnitAgent& reportedUnit );
    void NewOrderUpdate( UnitAutomate& reportedUnit );
    //@}

private:

    ///! @name MagicAction Helpers
    //@{
    void CreateLimit( const unsigned int idLimit, const dispatcher::Automat& automat, const std::vector< Point >& limit);
    void DeleteLimit( unsigned int id);
    void SentToAllClients( ASN1T_MsgsPluginToClient& asnMsg );
    //@}

    //! @name Member data
    //@{
    std::map< unsigned, std::map< std::string, Report_ABC* > > reports_;
    std::vector< Report_ABC* > receivedReports_;
    dispatcher::Model& model_;
	  dispatcher::SimulationPublisher_ABC& simulationPublisher_;
    LdapClient& ldap_;
    ClientManager& clientManager_;
    std::map< unsigned, std::string > intelligences_;
    std::set< unsigned int > oldLimits;

    std::map< std::string, std::string > managedReports_;
    //@}
};


}// xmlia

}//plugin

#endif // __ReportManager_h_
