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
#include "clients_kernel/Resolver_ABC.h"
#include "Attr_Def.h"
#include "ASN_Types.h"
#include "Report_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class DotationType;
    class EquipmentType;
}

enum E_OrderConduiteID;
class Simulation;
class RcEntityResolver_ABC;

// =============================================================================
// Created: AGN 2003-12-22
// =============================================================================
class RC : public Report_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             RC( const kernel::Entity_ABC& agent, const Simulation& simulation, const ASN1T_MsgCR& asnMsg, 
                 const RcEntityResolver_ABC& rcResolver,
                 const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver,
                 const kernel::Resolver_ABC< kernel::EquipmentType >& equimentResolver );
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
    std::string ObjectKnowledgeLink     ( ASN1T_OID nId, const RcEntityResolver_ABC& rcResolver );
    std::string AgentKnowledgeLink      ( ASN1T_OID nId, const RcEntityResolver_ABC& rcResolver );
    std::string PopulationKnowledgeLink ( ASN1T_OID nId, const RcEntityResolver_ABC& rcResolver );
    std::string AgentLink               ( ASN1T_OID nId, const RcEntityResolver_ABC& rcResolver );
    std::string DotationLink            ( ASN1T_OID nId, const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver);
    std::string EquipmentLink           ( ASN1T_OID nId, const kernel::Resolver_ABC< kernel::EquipmentType >& equimentResolver );

    void Initialize( const ASN1T_MsgCR& asnMsg, const RcEntityResolver_ABC& rcResolver,
                     const kernel::Resolver_ABC< kernel::DotationType >& dotationResolver,
                     const kernel::Resolver_ABC< kernel::EquipmentType >& equipmentResolver );
    //@}
};

#endif // __RC_h_
