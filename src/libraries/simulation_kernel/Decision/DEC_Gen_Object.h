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

#include "simulation_terrain/TER_Localisation.h"

class DEC_AutomateDecision;
class MIL_RealObjectType;
class MIL_ObstacleType;

//*****************************************************************************
// Created: AGN 03-08-27
//*****************************************************************************
class DEC_Gen_Object
{
public:
     DEC_Gen_Object( const ASN1T_PlannedWork& asn );
     DEC_Gen_Object( const DEC_Gen_Object& rhs );
    ~DEC_Gen_Object();

    //! @name Accessors
    //@{
    const MIL_RealObjectType&    GetType             () const;
    const TER_Localisation&      GetLocalisation     () const;
    const MIL_ObstacleType&      GetObstacleType     () const;
          MT_Float               GetDensity          () const;
          uint                   GetMinesActivityTime() const;
    const MIL_Automate*          GetTC2              () const;
    //@}

    //! @name Operations
    //@{
    void operator=( const DEC_Gen_Object& rhs );
    void Serialize( ASN1T_PlannedWork& asn ) const;
    //@}

private:
    const MIL_RealObjectType*    pType_;
          TER_Localisation       localisation_;
    const MIL_ObstacleType*      pObstacleType_;
          MT_Float               rDensity_;
          uint                   nMinesActivityTime_;
    const MIL_Automate*          pTC2_;
};

#include "DEC_Gen_Object.inl"

#endif // __DEC_Gen_Object_h_