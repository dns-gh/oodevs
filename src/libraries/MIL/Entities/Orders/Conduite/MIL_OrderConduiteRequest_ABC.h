//*****************************************************************************
//
// $Created: NLD 2003-01-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Conduite/MIL_OrderConduiteRequest_ABC.h $
// $Author: Nld $
// $Modtime: 3/09/04 18:15 $
// $Revision: 1 $
// $Workfile: MIL_OrderConduiteRequest_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_OrderConduiteRequest_ABC_h_
#define __MIL_OrderConduiteRequest_ABC_h_

#include "MIL.h"

#include "Network/NET_ASN_Tools.h"
#include "DIA/DIA_Kernel_lib.h"

class MIL_OrderConduiteType;

//=============================================================================
// Created: NLD 2003-01-08
//=============================================================================
class MIL_OrderConduiteRequest_ABC : public DIA_Thing
{
    MT_COPYNOTALLOWED( MIL_OrderConduiteRequest_ABC );

    friend class MT_ObjectRecycler< MIL_OrderConduiteRequest_ABC >; 

public:
    MIL_OrderConduiteRequest_ABC( const MIL_OrderConduiteType& type );
    virtual ~MIL_OrderConduiteRequest_ABC();

    //! @name Initialization
    //@{
              void Initialize( DIA_Parameters& diaParams, uint& nCurrentDIAParameter );
    /*final*/ void Terminate ();
    static    void InitializeDIA();
    //@}

    //! @name Operations
    //@{
    virtual void Serialize( ASN1T_MsgAttenteOrdreConduite_ordres_conduite_element& asnMsg ) const;
    //@}

    //! @name Accessors
    //@{
    const MIL_OrderConduiteType& GetType() const;
    //@}

private:
    //! @name DIA
    //@{
    const char* GetName() const;
    //@}
  
private:
    const MIL_OrderConduiteType& type_;

private:
    static int nDIATypeIdx_;
};

#include "MIL_OrderConduiteRequest_ABC.inl"

#endif // __MIL_OrderConduiteRequest_ABC_h_
