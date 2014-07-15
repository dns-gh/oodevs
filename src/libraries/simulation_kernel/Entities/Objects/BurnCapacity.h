// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BurnCapacity_h_
#define __BurnCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_PopulationElement_ABC;
class MIL_Agent_ABC;

// =============================================================================
/** @class  BurnCapacity
    @brief  @see "Incendie" in 4311340-Modèles de feu.doc
*/
// Created: RFT 2008-06-02
// =============================================================================

class BurnCapacity : public ObjectCapacity_ABC
                   , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             BurnCapacity();
    virtual ~BurnCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Register( MIL_Object_ABC& object );
    //@}

    //! @name Capacity creation
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Finalize( MIL_Object_ABC& object );
    //@}

    //! @name Update
    //@{
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

    //! @name MIL_InteractiveContainer_ABC
    //@{
    //Agent
    virtual void ProcessAgentMovingInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //Population
    virtual void ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& agent );
    //@}

private:
    //! @name Copy constructor
    //@{
    explicit BurnCapacity( const BurnCapacity& );
    //@}
};

BOOST_CLASS_EXPORT_KEY( BurnCapacity )

#endif // __BurnCapacity_h_
