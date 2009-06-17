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

#include "Rapport.h"

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
  class RapportManager;
  class Unite_ABC;
  class UniteAgent;
// =============================================================================
/** @class  Sitrep
    @brief  Sitrep
*/
// Created: MGD 2009-06-12
// =============================================================================
  class Sitrep : public Rapport
{

public:
    //! @name Constructors/Destructor
    //@{
    Sitrep( RapportManager& manager, xml::xistream& xis );
    Sitrep( RapportManager& manager, dispatcher::Automat& author );
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
    //@}
};

}// xmlia

}//plugin

#endif // __Sitrep_h_
