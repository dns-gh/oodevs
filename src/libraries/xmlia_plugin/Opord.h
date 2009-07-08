// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Opord_h_
#define __Opord_h_

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
/** @class  Opord
    @brief  Opord
*/
// Created: MGD 2009-06-12
// =============================================================================
  class Opord : public Report_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    Opord( ReportManager& manager, xml::xistream& xis );
    Opord( ReportManager& manager, dispatcher::Automat& author );
    virtual ~Opord();
    //@}

    ///! @name Operations
    //@{
    virtual void SerializeContent( xml::xostream& xos ) const;
    virtual void UpdateMission( kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2  );
    virtual void UpdateSimulation();
    //@}

private:
    void ReadUnite( xml::xistream& xis, std::map< unsigned, UnitAgent* >& map );
    std::map< unsigned, UnitAgent* > unitesAMI_;
};

}// xmlia

}//plugin

#endif // __Opord_h_
