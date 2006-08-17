//*****************************************************************************
//
// $Created: NLD 2003-01-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Conduite/MIL_OrderConduite_ABC.h $
// $Author: Nld $
// $Modtime: 9/11/04 10:31 $
// $Revision: 2 $
// $Workfile: MIL_OrderConduite_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_OrderConduite_h_
#define __MIL_OrderConduite_h_

#include "MIL.h"

#include "Network/NET_ASN_Tools.h"
#include "DIA/DIA_Kernel_lib.h"

class MIL_KnowledgeGroup;
class MIL_OrderConduiteType;
class MIL_OrderConduiteRequest_ABC;

//=============================================================================
// Created: NLD 2003-01-08
//=============================================================================
class MIL_OrderConduite_ABC : public DIA_Representation
{
    MT_COPYNOTALLOWED( MIL_OrderConduite_ABC );

public:
             MIL_OrderConduite_ABC( const MIL_OrderConduiteType& type, const MIL_KnowledgeGroup* pKnowledgeGroup );
    virtual ~MIL_OrderConduite_ABC();

    //! @name Initialization
    //@{
    virtual ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgOrderConduite_order_conduite& asnMsg );
    virtual void                     Initialize( DIA_Parameters& diaParams, uint nCurDIAParamIdx );

    static  void InitializeDIA();
    //@}

    //! @name Main
    //@{
    void Launch( DIA_Engine& engine, const std::string& strHint );
    //@}

    //! @name Accessors
    //@{
    const MIL_OrderConduiteType& GetType() const;
    //@}

protected:
    const MIL_KnowledgeGroup* pKnowledgeGroup_;

private:
    const MIL_OrderConduiteType& type_;
          DIA_Parameters         eventParameters_;

private:
    static int nDIATypeIdx_;
    static int nDIAHintIdx_;
};

#include "MIL_OrderConduite_ABC.inl"

#endif // __MIL_OrderConduite_h_
