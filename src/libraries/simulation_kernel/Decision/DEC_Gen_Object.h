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

#include "Entities/Objects/ObstacleAttribute.h"
#include "simulation_terrain/TER_Localisation.h"

class DEC_AutomateDecision;
class MIL_ObjectType_ABC;
class MIL_EntityManager_ABC;

//*****************************************************************************
// Created: AGN 03-08-27
//*****************************************************************************
class DEC_Gen_Object
{
public:
    //! @name Type
    //@{
    typedef EnumDemolitionTargetType::Root E_DemolitionTargetType;
    //@}

public:
             DEC_Gen_Object( const ASN1T_PlannedWork& asn, const MIL_EntityManager_ABC& entityManager );
             DEC_Gen_Object( const DEC_Gen_Object& rhs );
    virtual ~DEC_Gen_Object();

    //! @name Accessors
    //@{
    const MIL_ObjectType_ABC& GetType             () const;
    const TER_Localisation&   GetLocalisation     () const;
    E_DemolitionTargetType    GetObstacleType     () const;
          MT_Float            GetDensity          () const;
          unsigned int        GetMinesActivityTime() const;
    const MIL_Automate*       GetTC2              () const;
    //@}

    //! @name Operations
    //@{
    void operator=( const DEC_Gen_Object& rhs );
    void Serialize( ASN1T_PlannedWork& asn ) const;
    //@}

private:
    //! @name Data members
    //@{
    const MIL_ObjectType_ABC*       pType_;
          TER_Localisation          localisation_;
          E_DemolitionTargetType    pObstacleType_;
          MT_Float                  rDensity_;
          uint                      nMinesActivityTime_;
    const MIL_Automate*             pTC2_;
    //@}
};

#include "DEC_Gen_Object.inl"

#endif // __DEC_Gen_Object_h_
