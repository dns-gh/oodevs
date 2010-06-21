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

class PHY_DotationCategory;

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  StructuralCapacity
    @brief  StructuralCapacity
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
    virtual ~StructuralCapacity();
    //@}

    //! @name Operations
    //@{
    virtual void Register( MIL_Object_ABC& /*object*/ ){};
    virtual void Instanciate( MIL_Object_ABC& /*object*/ ) const{};
    virtual void Finalize( MIL_Object_ABC& /*object*/ ){};
    virtual void Update( MIL_Object_ABC& /*object*/, unsigned int /*time*/ ){};
    virtual void CanInteractWith         ( const MIL_Object_ABC& /*object*/, const MIL_Agent_ABC& /*agent*/, bool& /*canInteract*/ ){};
    virtual void ProcessAgentEntering    ( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    virtual void ProcessAgentExiting     ( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    virtual void ProcessAgentMovingInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    virtual void ProcessAgentInside      ( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    void ApplyIndirectFire( const urban::TerrainObject_ABC& object, const MT_Ellipse& attritionSurface, const PHY_DotationCategory& dotation );
    void ApplyDirectFire( const urban::TerrainObject_ABC& object, const PHY_DotationCategory& dotation );
    //@}

    //! @name Operations on population
    //@{
    virtual void ProcessPopulationInside ( MIL_Object_ABC& /*object*/, MIL_PopulationElement_ABC& /*population*/ ) {};
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StructuralCapacity( const StructuralCapacity& );            //!< Copy constructor
    StructuralCapacity& operator=( const StructuralCapacity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    float structuralState_;
    //@}
};

#endif // __StructuralCapacity_h_
