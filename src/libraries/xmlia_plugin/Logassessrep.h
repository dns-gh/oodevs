// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Logassessrep_h_
#define __Logassessrep_h_

#include "Report_ABC.h"

namespace dispatcher
{
    class Automat;
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
  class ReportManager;
// =============================================================================
/** @class  Logassessrep
    @brief  Logassessrep
*/
// Created: MGD 2009-06-12
// =============================================================================
  class Logassessrep : public Report_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    Logassessrep( ReportManager& manager, xml::xistream& xis );
    Logassessrep( ReportManager& manager, dispatcher::Automat& author );
    virtual ~Logassessrep();
    //@}

    ///! @name Operations
    //@{
    virtual void InsertOrUpdateFriendly( dispatcher::Agent& agent );
    virtual void SerializeContent( xml::xostream& xos ) const;
    virtual void UpdateSimulation();
    void ReadUnite( xml::xistream& xis, std::map< unsigned, UnitAgent* >& map );
    //@}

private:
    std::map< unsigned, UnitAgent* > unitesAMI_;

};

}// xmlia

}//plugin

#endif // __Logassessrep_h_
