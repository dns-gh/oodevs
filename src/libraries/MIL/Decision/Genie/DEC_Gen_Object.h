//*****************************************************************************
//
// $Created: AGN 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Genie/DEC_Gen_Object.h $
// $Author: Nld $
// $Modtime: 26/04/04 11:20 $
// $Revision: 1 $
// $Workfile: DEC_Gen_Object.h $
//
//*****************************************************************************

#ifndef __DEC_Gen_Object_h_
#define __DEC_Gen_Object_h_

#include "MIL.h"

#include "TER/TER_Localisation.h"

#include "Network/NET_ASN_Types.h"

//*****************************************************************************
// Created: AGN 03-08-27
//*****************************************************************************
class DEC_Gen_Object : public DIA_Thing
{
    MT_COPYNOTALLOWED( DEC_Gen_Object )

public:
             DEC_Gen_Object();
    virtual ~DEC_Gen_Object();

    //! @name Init
    //@{
    static void InitializeDIA();

    ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MissionGenObject& asn );
    //@}

private:
    TER_Localisation localisation_;

private:
    static int nDIATypeIdx_;
    static int nDIAPosIdx_;
    static int nDIAUrgenceIdx_;
    static int nDIAPreliminaireIdx_;
    static int nDIAMosIDIdx_;
    static int nDIAPrioriteIdx_;
};

#include "DEC_Gen_Object.inl"

#endif // __DEC_Gen_Object_h_