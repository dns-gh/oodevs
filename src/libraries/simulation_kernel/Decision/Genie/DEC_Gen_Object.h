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

#include "Network/NET_ASN_Types.h"

class DEC_AutomateDecision;
class MIL_RealObjectType;

//*****************************************************************************
// Created: AGN 03-08-27
//*****************************************************************************
class DEC_Gen_Object
{
public:
             DEC_Gen_Object();
             DEC_Gen_Object( const DEC_Gen_Object& rhs );
    virtual ~DEC_Gen_Object();

    //! @name Accessors
    //@{
    const MIL_RealObjectType& GetType        () const;
    const TER_Localisation&   GetLocalisation() const;
          uint                GetPreliminaire() const;
          MT_Float            GetDensity     () const;
    const MIL_Automate*       GetTC2         () const;
    //@}

    //! @name Init
    //@{
    ASN1T_EnumOrderErrorCode Initialize( const ASN1T_MissionGenObject& asn );
    //@}

private:
    const MIL_RealObjectType* pType_;
          TER_Localisation    localisation_;
          uint                nPreliminaire_;
          MT_Float            rDensity_;
    const MIL_Automate*       pTC2_;
};

#include "DEC_Gen_Object.inl"

#endif // __DEC_Gen_Object_h_