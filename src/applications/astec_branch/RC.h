//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/RC.h $
// $Author: Ape $
// $Modtime: 22/10/04 17:48 $
// $Revision: 7 $
// $Workfile: RC.h $
//
//*****************************************************************************

#ifndef __RC_h_
#define __RC_h_

#include "Types.h"
#include "Attr_Def.h"
#include "ASN_Types.h"
#include "Report_ABC.h"

class Agent_ABC;
enum E_OrderConduiteID;

// =============================================================================
// Created: AGN 2003-12-22
// =============================================================================
class RC : public Report_ABC
{
    MT_COPYNOTALLOWED( RC );

public:
    //! @name Types
    //@{
    typedef std::vector< uint >               T_FragOrderVector;
    typedef T_FragOrderVector::iterator       IT_FragOrderVector;
    typedef T_FragOrderVector::const_iterator CIT_FragOrderVector;
    //@}

public:
    //! @name Constructor/Destructor
    //@{
     RC( Agent_ABC& agent );
    ~RC();
    //@}

    //! @name
    //@{
    void Initialize( const ASN1T_MsgCR& asnMsg );
    void Initialize( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
    //@}
    
    //! @name Accessors
    //@{
    Agent_ABC&           GetAgent     ();
    const T_FragOrderVector& GetFragOrders() const;
    //@}

private:
    std::string ObjectKnowledgeLink( ASN1T_OID nId );
    std::string AgentKnowledgeLink ( ASN1T_OID nId );
    std::string PopulationKnowledgeLink ( ASN1T_OID nId );
    std::string AgentLink          ( ASN1T_OID nId );

private:
    Agent_ABC&      agent_;
    T_FragOrderVector   fragOrderVector_;
};

#   include "RC.inl"

#endif // __RC_h_
