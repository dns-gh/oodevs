// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Composantes/PHY_RoleHLA_Composantes.h $
// $Author: Age $
// $Modtime: 13/12/04 10:34 $
// $Revision: 8 $
// $Workfile: PHY_RoleHLA_Composantes.h $
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Composantes_h_
#define __PHY_RoleHLA_Composantes_h_

#include "PHY_RoleInterface_Composantes.h"
#include <hla/Deserializer.h>

namespace hla
{
    class AttributeIdentifier;
}

class PHY_ComposanteHLA;
class HLA_InteractionManager_ABC;

// =============================================================================
/** @class  PHY_RoleHLA_Composantes
    @brief  HLA role composantes
*/
// Created: AGE 2004-11-08
// =============================================================================
class PHY_RoleHLA_Composantes : public PHY_RoleInterface_Composantes
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_Composantes( MT_RoleContainer& role, HLA_InteractionManager_ABC& interactionManager );
    virtual ~PHY_RoleHLA_Composantes();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_Volume*          GetSignificantVolume       ( const PHY_SensorTypeAgent& sensorType ) const;
    virtual       void                 GetVisibleVolumes          ( T_ComposanteVolumeSet& volumes         ) const;
    virtual       void                 BuildKnowledgeComposantes  ( T_KnowledgeComposanteVector& knowledge ) const;
    virtual const PHY_Composante_ABC*  GetMajorComposante         () const;

    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    void Destroy();
    //@}

    //! @name Fire
    //@{
    virtual void GetComposantesAbleToBeFired( T_ComposanteVector& targets, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void GetComposantesAbleToBeFired( T_ComposanteVector& targets, uint nNbrFirer, bool bFireOnlyOnMajorComposantes = false ) const;
    virtual void Neutralize                 ();
    virtual void ApplyPopulationFire        ( PHY_Composante_ABC& compTarget, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, PHY_FireResults_ABC& result );
    virtual void ApplyDirectFire            ( PHY_Composante_ABC& compTarget, const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void ApplyIndirectFire          ( const PHY_DotationCategory& dotationCategory, PHY_FireResults_ABC& result );
    virtual void ApplyExplosion             ( const MIL_RealObjectType& objectType        , PHY_FireResults_ABC& result );
    //@}

    //! @name Accessors
    //@{
            bool     IsNeutralized           () const;
    virtual MT_Float GetOperationalState     () const;
    virtual MT_Float GetMajorOperationalState() const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< PHY_ComposanteHLA* > T_Composantes;
    typedef T_Composantes::const_iterator   CIT_Composantes;
    //@}

    //! @name Helpers
    //@{
    void UpdateComposante( const std::string& strComposanteName, uint nComposantes );
    //@}

private:
    //! @name Member data
    //@{
    T_Composantes               composantes_;
    HLA_InteractionManager_ABC& interactionManager_;
    bool                        bNeutralized_;
    //@}
};

#endif // __PHY_RoleHLA_Composantes_h_
