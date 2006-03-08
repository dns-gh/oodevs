// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h $
// $Author: Jvt $
// $Modtime: 31/03/05 18:21 $
// $Revision: 5 $
// $Workfile: PHY_RoleInterface_Composantes.h $
//
// *****************************************************************************

#ifndef __PHY_RoleInterface_Composantes_h_
#define __PHY_RoleInterface_Composantes_h_

#include "MIL.h"

#include "MT_Tools/MT_Role_ABC.h"

#include "Knowledge/DEC_Knowledge_Def.h"

class PHY_Composante_ABC;
class PHY_SensorTypeAgent;
class PHY_Volume;
class PHY_FireResults_ABC;
class PHY_DotationCategory;
class MIL_RealObjectType;
class MIL_PopulationType;
class MIL_PopulationAttitude;

// =============================================================================
// @class  PHY_RoleInterface_Composantes
// Created: JVT 2004-08-03
// =============================================================================
class PHY_RoleInterface_Composantes : public MT_Role_ABC
{
    MT_COPYNOTALLOWED( PHY_RoleInterface_Composantes )

public:
    //! @name Types
    //@{
    typedef PHY_RoleInterface_Composantes RoleInterface;

    typedef std::vector< PHY_Composante_ABC* >  T_ComposanteVector;
    typedef T_ComposanteVector::iterator        IT_ComposanteVector;
    typedef T_ComposanteVector::const_iterator  CIT_ComposanteVector;
    //@}

public:
             PHY_RoleInterface_Composantes( MT_RoleContainer& role );
             PHY_RoleInterface_Composantes();
    virtual ~PHY_RoleInterface_Composantes();

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Operations
    //@{
    virtual const PHY_Volume*          GetSignificantVolume       ( const PHY_SensorTypeAgent& sensorType ) const = 0;
    virtual       void                 GetVisibleVolumes          ( T_ComposanteVolumeSet& volumes         ) const = 0;
    virtual       void                 BuildKnowledgeComposantes  ( T_KnowledgeComposanteVector& knowledge ) const = 0;
    virtual const PHY_Composante_ABC*  GetMajorComposante         () const = 0;
    virtual       MT_Float             GetOperationalState        () const = 0;
    virtual       MT_Float             GetMajorOperationalState   () const = 0;
    //@}

    //! @name Fire
    //@{
    virtual void GetComposantesAbleToBeFired( T_ComposanteVector& targets, uint nNbrFirers, bool bFireOnlyOnMajorComposantes = false ) const = 0;
    virtual void GetComposantesAbleToBeFired( T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const = 0;
    virtual void Neutralize                 () = 0;
    virtual void ApplyPopulationFire        ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyDirectFire            ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyIndirectFire          ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result ) = 0;
    virtual void ApplyExplosion             ( const MIL_RealObjectType& objectType        , PHY_FireResults_ABC& result ) = 0;
    //@}
};

#include "PHY_RoleInterface_Composantes.inl"

#endif // __PHY_RoleInterface_Composantes_h_
