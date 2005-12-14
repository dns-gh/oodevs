//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_RC.h $
// $Author: Ape $
// $Modtime: 22/10/04 17:48 $
// $Revision: 7 $
// $Workfile: MOS_RC.h $
//
//*****************************************************************************

#ifndef __MOS_RC_h_
#define __MOS_RC_h_

#include "MOS_Types.h"
#include "MOS_Attr_Def.h"
#include "MOS_ASN_Types.h"
#include "MOS_Report_ABC.h"

class MOS_Agent_ABC;
enum E_OrderConduiteID;

// =============================================================================
// Created: AGN 2003-12-22
// =============================================================================
class MOS_RC : public MOS_Report_ABC
{
    MT_COPYNOTALLOWED( MOS_RC );

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
     MOS_RC( MOS_Agent_ABC& agent );
    ~MOS_RC();
    //@}

    //! @name
    //@{
    void Initialize( const ASN1T_MsgCR& asnMsg );
    void Initialize( const ASN1T_MsgAttenteOrdreConduite& asnMsg );
    //@}
    
    //! @name Accessors
    //@{
    MOS_Agent_ABC&           GetAgent     ();
    const T_FragOrderVector& GetFragOrders() const;
    //@}

private:
    std::string ObjectKnowledgeLink( ASN1T_OID nId );
    std::string AgentKnowledgeLink ( ASN1T_OID nId );
    std::string PopulationKnowledgeLink ( ASN1T_OID nId );
    std::string AgentLink          ( ASN1T_OID nId );

private:
    MOS_Agent_ABC&      agent_;
    T_FragOrderVector   fragOrderVector_;
};

#   include "MOS_RC.inl"

#endif // __MOS_RC_h_
