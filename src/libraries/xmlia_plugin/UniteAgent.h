// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UniteAgent_h_
#define __UniteAgent_h_

#include "Unite_ABC.h"

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
  class EtatOperationnel;
// =============================================================================
/** @class  UniteAgent
    @brief  UniteAgent
*/
// Created: MGD 2009-06-12
// =============================================================================
class UniteAgent : public Unite_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    UniteAgent( xml::xistream& xis );
    UniteAgent( dispatcher::Agent& );
    virtual ~UniteAgent();
    //@}

    ///! @name Operations
    //@{
    virtual void SerializeExtension( xml::xostream& xos, const std::string& sQnameRapport  ) const;
    virtual void SerializeIncludeEntities( xml::xostream& xos, const std::string& sQnameRapport  ) const;
    void Update( dispatcher::Agent& agent );
    bool IsSide( unsigned int idSide) const;
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
    EtatOperationnel* etatOps_;
    //@}
};

}// xmlia

}//plugin

#endif // __UniteAgent_h_
