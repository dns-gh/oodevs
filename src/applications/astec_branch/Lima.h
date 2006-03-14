//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/Lima.h $
// $Author: Ape $
// $Modtime: 26/01/05 10:22 $
// $Revision: 6 $
// $Workfile: Lima.h $
//
//*****************************************************************************

#ifndef __Lima_h_
#define __Lima_h_

#include "TacticalLine_ABC.h"
#include "IDManager.h"
#include "ASN_Types.h"

class CoordinateConverter;

// =============================================================================
/** @class  Lima
    @brief  Represents a lima.
*/
// Created: APE 2004-03-12
// =============================================================================
class Lima : public TacticalLine_ABC
{
    MT_COPYNOTALLOWED( Lima );

public:
    static IDManager idManager_;

public:
     Lima();
     Lima( T_PointVector pointList, E_FuncLimaType nFuncType );
     Lima( const ASN1T_MsgLimaCreation& asnMsg, const CoordinateConverter& converter );
    ~Lima();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    TacticalLine_ABC::E_LineType GetLineType() const;

    bool UpdateToSim();
    //@}

    //-------------------------------------------------------------------------
    /** @name Func*/
    //-------------------------------------------------------------------------
    //@{
    E_FuncLimaType  GetTypeFunc() const;
    void            SetTypeFunc( E_FuncLimaType nTypeFunc );
    //@}
    
    //! @name In/Out
    //@{
    void Read( MT_InputArchive_ABC& archive );
    void Write( MT_OutputArchive_ABC& archive ) const;
    //@}

private:
    E_FuncLimaType        nFuncType_;
};

#   include "Lima.inl"

#endif // __Lima_h_
