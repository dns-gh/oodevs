// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NBCReport_h_
#define __NBCReport_h_

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
  class NBCObject;
// =============================================================================
/** @class  NBCReport
    @brief  NBCReport
*/
// Created: MGD 2009-06-12
// =============================================================================
  class NBCReport : public Report_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    NBCReport( ReportManager& manager, xml::xistream& xis );
    NBCReport( ReportManager& manager, dispatcher::Automat& author );
    virtual ~NBCReport();
    //@}

    ///! @name Operations
    //@{
    virtual void SerializeContent( xml::xostream& xos ) const;
    virtual void InsertOrUpdateNBC( dispatcher::Object& agent );
    virtual void UpdateSimulation();
    //@}

private:
    std::map< unsigned, NBCObject* > objects_;


};

}// xmlia

}//plugin

#endif // __NBCReport_h_
