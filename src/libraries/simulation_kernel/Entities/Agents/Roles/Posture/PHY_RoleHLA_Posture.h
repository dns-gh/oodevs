// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PHY_RoleHLA_Posture_h_
#define __PHY_RoleHLA_Posture_h_

#include "PHY_RoleInterface_Posture.h"

#include "MT_Tools/AlgorithmModifier_ABC.h"
#include "hla/Deserializer.h"

namespace hla 
{
    class AttributeIdentifier;
}

namespace detection
{
    class DetectionComputer_ABC;
}

class MIL_Agent_ABC;

// =============================================================================
/** @class  PHY_RoleHLA_Posture
    @brief  HLA role posture
*/
// Created: AGE 2004-11-09
// =============================================================================
class PHY_RoleHLA_Posture : public PHY_RoleInterface_Posture
                          , public tools::AlgorithmModifier_ABC< detection::DetectionComputer_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             PHY_RoleHLA_Posture( const MIL_Agent_ABC& pion );
    virtual ~PHY_RoleHLA_Posture();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_Posture& GetLastPosture                () const;
    virtual const PHY_Posture& GetCurrentPosture             () const;
    virtual       MT_Float     GetPostureCompletionPercentage() const;    
    virtual       void         SetPosturePostePrepareGenie   ();
    virtual       void         UnsetPosturePostePrepareGenie ();
    virtual       void         SetTimingFactor               ( MT_Float rFactor );
    
    void Deserialize( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );

    virtual void Execute( detection::DetectionComputer_ABC& algorithm ) const;
    //@}

    //! @name Modifiers
    //@{
    void ChangeStatus( const std::vector< std::string >& statuses );
    //@}



    //! @name Operations
    //@{
    virtual void Update    ( bool bIsDead ) {};
    virtual void Clean     () {};
    virtual bool HasChanged() const { return false; };

    // Override automatic postures
    virtual void SetPostureMovement           () {};
    virtual void UnsetPostureMovement         () {};

    virtual void EnableDiscreteMode () {};
    virtual void DisableDiscreteMode() {};
    //@}

    //! @name Installation
    //@{
    virtual bool IsInstalled  () const { return false; };
    virtual bool IsUninstalled() const { return false;};
    virtual void Install      () {};
    //@}

    //! @name Perception
    //@{
    virtual void SetStealthFactor( MT_Float rValue ) {};
    //@}

    //! @name Elongation
    //@{
    virtual void     SetElongationFactor( MT_Float ) {};
    virtual MT_Float GetElongationFactor() const { return 0; };
    //@}

    //! @name Accessors
    //@{
    virtual bool IsStealth() const { return false; };
    //@}

    //! @name HLA
    //@{
    virtual void Serialize( HLA_UpdateFunctor& functor ) const {};
    virtual bool HLAStatusHasChanged() const { return false; };

private:
    //! @name Member data
    //@{
    const PHY_Posture* pPosture_;
    const MIL_Agent_ABC& pion_;
    bool bIsStealth_;
    bool bTransported_;
    //@}
};

#endif // __PHY_RoleHLA_Posture_h_
