// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RC_h_
#define __RC_h_

#include "clients_kernel/Types.h"
#include "Attr_Def.h"
#include "ASN_Types.h"
#include "Report_ABC.h"

class Entity_ABC;
enum E_OrderConduiteID;
class Simulation;

// =============================================================================
// Created: AGN 2003-12-22
// =============================================================================
class RC : public Report_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             RC( const Entity_ABC& agent, const Simulation& simulation, const ASN1T_MsgCR& asnMsg );
    virtual ~RC();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    RC( const RC& );
    RC& operator=( const RC& );
    //@}

    //! @name Helpers
    //@{
    std::string ObjectKnowledgeLink( ASN1T_OID nId );
    std::string AgentKnowledgeLink ( ASN1T_OID nId );
    std::string PopulationKnowledgeLink ( ASN1T_OID nId );
    std::string AgentLink          ( ASN1T_OID nId );

    void Initialize( const ASN1T_MsgCR& asnMsg );
    //@}
};

#endif // __RC_h_
