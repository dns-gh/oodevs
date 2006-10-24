//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Objects/MIL_ControlZone.h $
// $Author: Nld $
// $Modtime: 2/11/04 11:41 $
// $Revision: 2 $
// $Workfile: MIL_ControlZone.h $
//
//*****************************************************************************

#ifndef __MIL_ControlZone_h_
#define __MIL_ControlZone_h_

#include "MIL.h"

#include "MIL_VirtualObject_ABC.h"

#include "MT_Tools/MT_Random.h"

class MIL_VirtualObjectType_ControlZone;
class MIL_Agent_ABC;
class MIL_AgentPion;
class PHY_Composante_ABC;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ControlZone : public MIL_VirtualObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ControlZone );

public:
    //! @name Types
    //@{
    typedef std::pair< MIL_Agent_ABC*, PHY_Composante_ABC* > T_TargetPair;
    typedef std::vector< T_TargetPair >                      T_TargetVector;
    typedef T_TargetVector::const_iterator                   CIT_TargetVector;
    //@}

public:
             MIL_ControlZone( MIL_Army& army, const TER_Localisation& localisation, MT_Float rRadius  );
    virtual ~MIL_ControlZone();

    //! @name Operations
    //@{
    void GetTargets( const MIL_AgentPion& firer, T_TargetVector& targets ) const;
    //@}

private:
    const MT_Float rCircleSquareRadius_;
    const bool     bMustUseCircle_;


private:
    static MT_Random randomGenerator_;
};

#include "MIL_ControlZone.inl"

#endif // __MIL_ControlZone_h_
