// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitAgent_h_
#define __UnitAgent_h_

#include "Unit_ABC.h"

namespace dispatcher
{
  class Agent;
}

namespace kernel
{
  class MissionType;
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
  class Mission;
  class Point;
  class XmliaOperationalState;
// =============================================================================
/** @class  UniteAgent
    @brief  UniteAgent
*/
// Created: MGD 2009-06-12
// =============================================================================
class UnitAgent : public Unit_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    UnitAgent( xml::xistream& xis );
    UnitAgent( dispatcher::Agent& );
    virtual ~UnitAgent();
    //@}

    ///! @name Operations
    //@{
    virtual void SerializePosition( xml::xostream& xos ) const;
    virtual void SerializeEtatOps( xml::xostream& xos ) const;
    virtual void SerializeMission( xml::xostream& xos ) const;
    void Update( dispatcher::Agent& agent );
    void UpdateMission( kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2 );
    bool IsSide( unsigned int idSide) const;
    Point* GetLocalization() const;
    XmliaOperationalState* GetOperationalState() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int idSide_;
    
    Point* localisation_;
    XmliaOperationalState* etatOps_;
    Mission* mission_;
    //@}
};

}// xmlia

}//plugin

#endif // __UnitAgent_h_
