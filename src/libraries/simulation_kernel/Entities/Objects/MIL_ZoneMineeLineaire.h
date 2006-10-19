//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneMineeLineaire.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:29 $
// $Revision: 6 $
// $Workfile: MIL_ZoneMineeLineaire.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneMineeLineaire_h_
#define __MIL_ZoneMineeLineaire_h_

#include "MIL.h"

#include "MIL_Obstacle.h"

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneMineeLineaire : public MIL_Obstacle
{
    MT_COPYNOTALLOWED( MIL_ZoneMineeLineaire );

public:
             MIL_ZoneMineeLineaire();
    virtual ~MIL_ZoneMineeLineaire();
    
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();

    virtual bool                      Initialize( const MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( uint nID, MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( uint nID, const ASN1T_MagicActionCreateObject& asn );
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentMovingInside( MIL_Agent_ABC& agent );
    //@}
};

#include "MIL_ZoneMineeLineaire.inl"

#endif // __MIL_ZoneMineeLineaire_h_
