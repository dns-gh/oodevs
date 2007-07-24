// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelDataAcceptor_ABC_h_
#define __ModelDataAcceptor_ABC_h_

#include "game_asn/Asn.h"

namespace dispatcher {

// =============================================================================
/** @class  ModelDataAcceptor_ABC
    @brief  ModelDataAcceptor_ABC
*/
// Created: JCR 2007-05-15
// =============================================================================

class ModelDataAcceptor_ABC
{
public:
    //! @name Constructor / Destructor
    //@{
             ModelDataAcceptor_ABC() {}
    virtual ~ModelDataAcceptor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual bool Accept( const ASN1T_MsgUnitCreation& /*asn*/ )     { return false; }
    virtual bool Accept( const ASN1T_MsgAutomatCreation& /*asn*/ ) { return false; }
    virtual bool Accept( const ASN1T_MsgObjectCreation& /*asn*/ )   { return false; }
    virtual bool Accept( const ASN1T_MsgTeamCreation& /*asn*/ )     { return false; }
    virtual bool Accept( const ASN1T_MsgLimitCreation& /*asn*/ )    { return false; }
    virtual bool Accept( const ASN1T_MsgLimaCreation& /*asn*/ )     { return false; }
    virtual bool Accept( const ASN1T_TacticalLine& /*asn*/ )        { return false; }
    virtual bool Accept( const ASN1T_MsgFormationCreation& /*asn*/ ){ return false; }
    virtual bool Accept( const ASN1T_MsgKnowledgeGroupCreation& /*asn*/ ) { return false; }
    virtual bool Accept( const ASN1T_MsgUnitKnowledgeCreation& /*asn*/ ) { return false; }
    virtual bool Accept( const ASN1T_MsgUnitKnowledgeUpdate& /*asn*/ ) { return false; }
    //@}
};

}

#endif // __ModelDataAcceptor_ABC_h_
