//*****************************************************************************
//
// $Created: NLD 2002-08-08 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_Lima.h $
// $Author: Ape $
// $Modtime: 26/01/05 10:22 $
// $Revision: 6 $
// $Workfile: MOS_Lima.h $
//
//*****************************************************************************

#ifndef __MOS_Lima_h_
#define __MOS_Lima_h_

#include "MOS_TacticalLine_ABC.h"
#include "MOS_IDManager.h"
#include "MOS_ASN_Types.h"


// =============================================================================
/** @class  MOS_Lima
    @brief  Represents a lima.
*/
// Created: APE 2004-03-12
// =============================================================================
class MOS_Lima : public MOS_TacticalLine_ABC
{
    MT_COPYNOTALLOWED( MOS_Lima );

public:
    static MOS_IDManager idManager_;

public:
     MOS_Lima();
     MOS_Lima( T_PointVector pointList, E_FuncLimaType nFuncType );
     MOS_Lima( const ASN1T_MsgLimaCreation& asnMsg );
    ~MOS_Lima();

    //-------------------------------------------------------------------------
    /** @name Accessors */
    //-------------------------------------------------------------------------
    //@{
    MOS_TacticalLine_ABC::E_LineType GetLineType() const;

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

#   include "MOS_Lima.inl"

#endif // __MOS_Lima_h_
