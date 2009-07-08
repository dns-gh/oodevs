// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Sitrep_h_
#define __Sitrep_h_

#include "Report_ABC.h"

namespace dispatcher
{
    class Automat;
    class Agent;
    class Side;
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
  class ReportManager;
  class Unit_ABC;
  class UnitAgent;
// =============================================================================
/** @class  Sitrep
    @brief  Sitrep
*/
// Created: MGD 2009-06-12
// =============================================================================
  class Sitrep : public Report_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    Sitrep( ReportManager& manager, xml::xistream& xis );
    Sitrep( ReportManager& manager, dispatcher::Automat& author );
    virtual ~Sitrep();
    //@}

    ///! @name Operations
    //@{
    virtual void InsertOrUpdateFriendly( dispatcher::Agent& agent );
    virtual void InsertOrUpdateEnemy( dispatcher::Agent& agent );
    virtual void UpdateSimulation();
    //@}

private:
  ///! @name Operations
  //@{
  virtual void SerializeContent( xml::xostream& xos ) const;
  void ReadUnite( xml::xistream& xis, std::map< unsigned, UnitAgent* >& map );
  //@}

  private:
    std::map< unsigned, UnitAgent* > unitesAMI_;
    std::map< unsigned, UnitAgent* > unitesENI_;
};

}// xmlia

}//plugin

#endif // __Sitrep_h_
