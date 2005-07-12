//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneMineeParDispersion.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:30 $
// $Revision: 6 $
// $Workfile: MIL_ZoneMineeParDispersion.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneMineeParDispersion_h_
#define __MIL_ZoneMineeParDispersion_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneMineeParDispersion : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_ZoneMineeParDispersion );

public:
             MIL_ZoneMineeParDispersion();
    virtual ~MIL_ZoneMineeParDispersion();

    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();

    virtual bool                      Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( uint nID, MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( uint nID, const ASN1T_MagicActionCreateObject& asn );
    virtual void                      Initialize( MIL_Army& army, const TER_Localisation& localisation, uint nNbrMines );
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    //@}
};

#include "MIL_ZoneMineeParDispersion.inl"

#endif // __MIL_ZoneMineeParDispersion_h_
