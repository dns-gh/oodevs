// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __StructuralCapacity_h_
#define __StructuralCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class PHY_ComposanteState;
class PHY_DotationCategory;
class PHY_UrbanAttritionData;
class PHY_Protection;
class MIL_Army_ABC;
class TER_Localisation;

namespace xml
{
    class xistream;
}

namespace sword
{
    class UrbanAttributes;
    class MissionParameter_Value;
}

// =============================================================================
/** @class  StructuralCapacity
    @brief  Structural capacity
*/
// Created: SLG 2010-06-17
// =============================================================================
class StructuralCapacity : public ObjectCapacity_ABC
                         , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             StructuralCapacity();
    explicit StructuralCapacity( xml::xistream& xis );
    virtual ~StructuralCapacity();
    //@}

    //! @name Operations
    //@{
    void Update( xml::xistream& xis, const MIL_Object_ABC& object );
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;

    void ApplyDestruction( MIL_Object_ABC& object, const TER_Localisation& attritionSurface,
                            const PHY_UrbanAttritionData& attrition );
    void ApplyIndirectFire( MIL_Object_ABC& object, const TER_Localisation& attritionSurface,
                            const PHY_DotationCategory& dotation, MIL_Army_ABC* army );
    void ApplyDirectFire( const MIL_Object_ABC& object, const PHY_DotationCategory& dotation );
    const PHY_ComposanteState& ComputeComposanteState( const MIL_Object_ABC& object, const PHY_Protection& targetProtection );
    void Build( double rDeltaPercentage );
    float GetStructuralState() const;
    void SetStructuralState( float state );

    void SendState( sword::UrbanAttributes& message ) const;
    void SendFullState( sword::UrbanAttributes& message ) const;
    void OnUpdate( const sword::MissionParameter_Value& attribute );

    //! @name From MIL_InteractiveContainer_ABC
    //@{
    virtual void ProcessAgentEntering( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive >
    void serialize( Archive& file, const unsigned int );
    void WriteUrban( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StructuralCapacity( const StructuralCapacity& );            //!< Copy constructor
    StructuralCapacity& operator=( const StructuralCapacity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    bool ApplyDestruction( MIL_Object_ABC& object, const TER_Localisation& attritionSurface, double factor );
    void CreateCrumbling( MIL_Object_ABC& object, const TER_Localisation& surface, MIL_Army_ABC& army ) const;
    //@}

private:
    //! @name Member data
    //@{
    float structuralState_;
    mutable float lastStructuralState_;
    std::vector< MIL_Agent_ABC* > agents_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( StructuralCapacity )

#endif // __StructuralCapacity_h_
