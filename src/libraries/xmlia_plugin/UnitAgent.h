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
    virtual void SerializeExtension( xml::xostream& xos, const std::string& sQnameRapport  ) const;
    virtual void SerializeIncludeEntities( xml::xostream& xos, const std::string& sQnameRapport  ) const;
    void Update( dispatcher::Agent& agent );
    bool IsSide( unsigned int idSide) const;
    Point* GetLocalization() const;
    XmliaOperationalState* GetOperationalState() const;
    void SetPosition( Point* pt );
    void SetEtatOps( XmliaOperationalState* etatOps );
    //@}

private:
    ///! @name Operations
    //@{
    void SerializeEtatOps( xml::xostream& xos, std::string sQnameRapport   ) const;
    void SerializeAssociationLocalisation( xml::xostream& xos ) const;
    void SerializeSideAssociation( xml::xostream& xos ) const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int idSide_;
    
    Point* localisation_;
    XmliaOperationalState* etatOps_;
    //@}
};

}// xmlia

}//plugin

#endif // __UnitAgent_h_
