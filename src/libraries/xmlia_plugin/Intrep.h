// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Intrep_h_
#define __Intrep_h_

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
/** @class  Intrep
    @brief  Intrep
*/
// Created: MGD 2009-06-12
// =============================================================================
  class Intrep : public Report_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    Intrep( ReportManager& manager, xml::xistream& xis );
    Intrep( ReportManager& manager, dispatcher::Automat& author );
    virtual ~Intrep();
    //@}

    ///! @name Operations
    //@{
    virtual void SerializeContent( xml::xostream& xos ) const;
    virtual void InsertOrUpdateEnemy( dispatcher::Agent& agent );
    virtual void UpdateSimulation();
    //@}

private:
    UnitAgent* fired;
};

}// xmlia

}//plugin

#endif // __Intrep_h_
