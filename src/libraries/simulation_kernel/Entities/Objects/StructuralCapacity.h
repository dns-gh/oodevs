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

namespace MsgsSimToClient
{
    class MsgUrbanAttributes;
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
    void Load( xml::xistream& xis );
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void CanInteractWith( const MIL_Object_ABC& /*object*/, const MIL_Agent_ABC& /*agent*/, bool& /*canInteract*/ ){};
    virtual void ProcessAgentEntering( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    virtual void ProcessAgentExiting( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    virtual void ProcessAgentMovingInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    virtual void ProcessAgentInside( MIL_Object_ABC& /*object*/, MIL_Agent_ABC& /*agent*/ ){};
    void ApplyIndirectFire( const MIL_Object_ABC& object, const MT_Ellipse& attritionSurface, const PHY_DotationCategory& dotation );
    void ApplyDirectFire( const MIL_Object_ABC& object, const PHY_DotationCategory& dotation );
    void SendState( MsgsSimToClient::MsgUrbanAttributes& message ) const;
    float GetStructuralState() const;
    //@}

    //! @name Operations on population
    //@{
    virtual void ProcessPopulationInside( MIL_Object_ABC& /*object*/, MIL_PopulationElement_ABC& /*population*/ ) {};
    //@}

private:
    //! @name Copy/Assignment
    //@{
    StructuralCapacity( const StructuralCapacity& );            //!< Copy constructor
    StructuralCapacity& operator=( const StructuralCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    float structuralState_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( StructuralCapacity )

#endif // __StructuralCapacity_h_
