//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Abattis.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:07 $
// $Revision: 4 $
// $Workfile: MIL_Abattis.h $
//
//*****************************************************************************

#ifndef __MIL_Abattis_h_
#define __MIL_Abattis_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_Abattis : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_Abattis );

public:
             MIL_Abattis( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_Abattis();
    virtual ~MIL_Abattis();
    
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );

    virtual bool Initialize( DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    //@}
};

#include "MIL_Abattis.inl"

#endif // __MIL_Abattis_h_
