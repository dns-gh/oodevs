//*****************************************************************************
//
// $Created: NLD 2003-04-09 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Population/MIL_PopulationMission_ABC.h $
// $Author: Nld $
// $Modtime: 21/03/05 12:14 $
// $Revision: 6 $
// $Workfile: MIL_PopulationMission_ABC.h $
//
//*****************************************************************************

#ifndef __MIL_PopulationMission_ABC_h_
#define __MIL_PopulationMission_ABC_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class MIL_Population;
class MIL_PopulationMissionType;

//=============================================================================
// Created: NLD 2003-04-09
//=============================================================================
class MIL_PopulationMission_ABC : public DIA_Thing
{
    MT_COPYNOTALLOWED( MIL_PopulationMission_ABC )

public:
    MIL_PopulationMission_ABC( MIL_Population& population, const MIL_PopulationMissionType& type );
    virtual ~MIL_PopulationMission_ABC();

    //! @name Init
    //@{
    virtual ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MsgPopulationOrder& asnMsg );
    virtual void                     Terminate ();
    //@}

    //! @name DIA
    //@{
    static void InitializeDIA();
    const char* GetName() const;
    //@}

    //! @name Control
    //@{
    void Start  ();
    void Stop   ();
    //@}

    //! @name Accessors
    //@{
          uint                       GetOrderID   () const;
    const MIL_PopulationMissionType& GetType      () const;
          MIL_Population&            GetPopulation() const;  
    //@}

protected:
    MIL_Population& population_;

private:
    const MIL_PopulationMissionType& type_;
          uint                       nOrderID_;

    bool bDIABehaviorActivated_;

public:
    static int nDIAMissionType_;
};

#include "MIL_PopulationMission_ABC.inl"

#endif // __MIL_PopulationMission_ABC_h_
