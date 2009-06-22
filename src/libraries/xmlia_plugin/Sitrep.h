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
    virtual void SerializeOtherEntities( xml::xostream& xos ) const;
    void SerializeSide( const dispatcher::Side& side, xml::xostream& xos, std::string sQnameRapport ) const;
    virtual void InsertOrUpdate( dispatcher::Agent& agent );
    virtual void UpdateSimulation();
    virtual void ReadEntities( xml::xistream& xis );
    void ReadPosition( xml::xistream& xis );
    void ReadEtatOps( xml::xistream& xis );
	virtual unsigned int GetAuthorID() const;
    //@}
};

}// xmlia

}//plugin

#endif // __Sitrep_h_
