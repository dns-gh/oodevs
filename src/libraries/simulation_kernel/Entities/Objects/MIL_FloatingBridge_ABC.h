//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_FloatingBridge_ABC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 6 $
// $Workfile: MIL_FloatingBridge_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_FloatingBridge_ABC_h_
#define __MIL_FloatingBridge_ABC_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"
class TER_DynamicData;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_FloatingBridge_ABC : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_FloatingBridge_ABC );

public:
             MIL_FloatingBridge_ABC( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_FloatingBridge_ABC();
    virtual ~MIL_FloatingBridge_ABC();

    //! @name Init
    //@{
    virtual bool                      Initialize( DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject );
    //@}

    //! @name HLA
    //@{
    virtual bool Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    //@}

protected:
    //! @name Tools
    //@{
    void LinkToPathFinder    ();
    void UnlinkFromPathFinder();
    //@}

private:
    // Path finder enhancement
    TER_DynamicData* pPathfindData_;
};

#include "MIL_FloatingBridge_ABC.inl"

#endif // __MIL_FloatingBridge_ABC_h_
