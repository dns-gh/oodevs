// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Frago_h_
#define __Frago_h_

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
  class Point;
  class ReportManager;
// =============================================================================
/** @class  Frago
    @brief  Frago
*/
// Created: MGD 2009-06-12
// =============================================================================
  class Frago : public Report_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    Frago( ReportManager& manager, xml::xistream& xis );
    Frago( ReportManager& manager, dispatcher::Automat& author );
    virtual ~Frago();
    //@}

    ///! @name Operations
    //@{
    virtual void SerializeContent( xml::xostream& xos ) const;
    virtual void UpdateMission( dispatcher::Agent agent, kernel::MissionType& mission );
    virtual void UpdateSimulation();
    //@}

private:
    void ReadUnite( xml::xistream& xis, std::map< unsigned, UnitAgent* >& map );
    std::map< unsigned, UnitAgent* > unitesAMI_;
};

}// xmlia

}//plugin

#endif // __Frago_h_
