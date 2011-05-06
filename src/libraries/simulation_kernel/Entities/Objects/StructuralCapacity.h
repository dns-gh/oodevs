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
class PHY_Protection;
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

namespace urban
{
    class MaterialCompositionType;
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

    void ApplyIndirectFire( MIL_Object_ABC& object, const TER_Localisation& attritionSurface, const PHY_DotationCategory& dotation );
    void ApplyDirectFire( const MIL_Object_ABC& object, const PHY_DotationCategory& dotation );
    const PHY_ComposanteState& ComputeComposanteState( const MIL_Object_ABC& object, const PHY_Protection& targetProtection );
    void Build( double rDeltaPercentage );
    float GetStructuralState() const;

    void SendState( sword::UrbanAttributes& message ) const;
    void SendFullState( sword::UrbanAttributes& message ) const;
    void OnUpdate( const sword::MissionParameter_Value& attribute );

    //! @name From MIL_InteractiveContainer_ABC
    //@{
    virtual void CanInteractWith( const MIL_Object_ABC& /*object*/, const MIL_Agent_ABC& /*agent*/, bool& /*canInteract*/ ){};
    virtual void ProcessAgentEntering( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ );
    virtual void ProcessAgentExiting( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ );
    virtual void ProcessAgentMovingInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    virtual void ProcessAgentInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    virtual void ProcessPopulationInside( MIL_Object_ABC& /*object*/, MIL_PopulationElement_ABC& /*population*/ ) {};
    //@}

    //! @name Checkpoints
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StructuralCapacity( const StructuralCapacity& );            //!< Copy constructor
    StructuralCapacity& operator=( const StructuralCapacity& ); //!< Assignment operator
    //@}

    //! @name types
    //@{
    typedef std::set< MIL_Agent_ABC* > T_Agents;
    typedef T_Agents::iterator         IT_Agents;
    //@}

private:
    //! @name Member data
    //@{
    float structuralState_;
    mutable float lastStructuralState_;
    T_Agents agents_;
    urban::MaterialCompositionType* materialType_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( StructuralCapacity )

#endif // __StructuralCapacity_h_
