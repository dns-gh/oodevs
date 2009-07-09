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
    dispatcher::ClientPublisher_ABC& GetClientPublisher() const;
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
    void DoUpdateLog( dispatcher::Agent& agent );
    bool ReadLine( const ASN1T_Line& asn, std::vector< Point >& points );
    //@}

    ///! @name Import Operations
    //@{
    void CleanReceivedRapport();
    void Read( xml::xistream& xis );
    void ReadUrl( xml::xistream& xis, Publisher_ABC& publisher );
    void UpdateSimulation() const ;
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned, std::map< std::string, Report_ABC* > > reports_;
    std::vector< Report_ABC* > receivedRapports_;
    dispatcher::Model& model_;
	dispatcher::SimulationPublisher_ABC& simulationPublisher_;
    LdapClient& ldap_;
    ClientManager& clientManager_;
    //@}
};

}// xmlia

}//plugin

#endif // __ReportManager_h_
