// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RapportManager_h_
#define __RapportManager_h_

namespace dispatcher
{
  class Model;
  class Agent;
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

// =============================================================================
/** @class  RapportManager
    @brief  RapportManager
*/
// Created: MGD 2009-06-12
// =============================================================================
class RapportManager
{

public:
    //! @name Constructors/Destructor
    //@{
    RapportManager( dispatcher::Model& model );
    virtual ~RapportManager();
    //@}

    ///! @name Operations
    //@{
    dispatcher::Model& GetModel() const;
    //@}

    ///! @name Export Operations
    //@{
    void Send()const;
    void DoUpdate( dispatcher::Agent& agent );
    void DoUpdate( dispatcher::Agent& agent,  dispatcher::Agent& detected );
    //@}

    ///! @name Import Operations
    //@{
    void CleanReceivedRapport();
    void Read( xml::xistream& xis );
    void UpdateSimulation() const ;
    //@}

private:
    //! @name Member data
    //@{
    std::map< unsigned, Sitrep* > rapports_;//@refactor when new rapport type will be added
    std::vector< Sitrep* > receivedRapports_;
    dispatcher::Model& model_;
    //@}
};

}// xmlia

}//plugin

#endif // __RapportManager_h_
