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

namespace dispatcher
{
  class Model;
  class Agent;
  class SimulationPublisher_ABC;
  class ClientPublisher_ABC;
  class Profile_ABC;
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
  class Sitrep;
  class Publisher_ABC;
  class LdapClient;

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
    ReportManager( dispatcher::Model& model, dispatcher::SimulationPublisher_ABC& simulationPublisher, LdapClient& Ldap );
    virtual ~ReportManager();
    //@}

    ///! @name Operations
    //@{
    dispatcher::Model& GetModel() const;
    dispatcher::SimulationPublisher_ABC& GetSimulationPublisher() const;
    dispatcher::Profile_ABC* GetClientProfile() const;
    void SetClientProfile( dispatcher::Profile_ABC& profile );
    dispatcher::ClientPublisher_ABC* GetClientPublisher() const;
    void SetClientPublisher( dispatcher::ClientPublisher_ABC& clientPublisher );
    bool IsTakenIntoAccount( const std::string messageType );
    //@}

    ///! @name Export Operations
    //@{
    void Send( Publisher_ABC& publisher )const;
    void Receive( Publisher_ABC& publisher);
    void DoUpdate( dispatcher::Agent& agent );
    void DoUpdate( dispatcher::Agent& agent,  dispatcher::Agent& detected );
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
    std::map< unsigned, Sitrep* > reports_;//@refactor when new report type will be added
    std::vector< Sitrep* > receivedRapports_;
    dispatcher::Model& model_;
	dispatcher::SimulationPublisher_ABC& simulationPublisher_;
    dispatcher::Profile_ABC* clientProfile_;
    dispatcher::ClientPublisher_ABC* clientPublisher_;
    LdapClient& ldap_;
    //@}
};

}// xmlia

}//plugin

#endif // __ReportManager_h_
