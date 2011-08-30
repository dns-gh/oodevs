// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirePropagationModifierCapacity_h_
#define __FirePropagationModifierCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_FireClass;
class MIL_BurningCells;

// =============================================================================
/** @class  FirePropagationModifierCapacity
    @brief  Allows to stop fire propagation on a surface defined with a polygon.
            This polygon is roughly subdivised in fixed size cells.
    @see "Feu sauvage" in 4311340-Modèles de feu.doc
*/
// Created: BCI 2010-12-17
// =============================================================================
class FirePropagationModifierCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FirePropagationModifierCapacity( xml::xistream& xis );
             FirePropagationModifierCapacity();
    virtual ~FirePropagationModifierCapacity();
    //@}

    //! @name Operations
    //@{
    void Modify( const MIL_FireClass& fireClass, int& ignitionThreshold, int& maxCombustionEnergy ) const;
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

private:
    //! @name Copy constructor
    //@{
    explicit FirePropagationModifierCapacity( const FirePropagationModifierCapacity& );
    //@}

    //! @name Helpers
    //@{
    void ReadModifier( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    struct Modifier
    {
        int ignitionThreshold_;
        int maxCombustionEnergy_;
    };
    //@}

private:
    //! @name Member data
    //@{
    MIL_BurningCells& burningCells_;
    typedef std::map< const MIL_FireClass*, Modifier > ModifierMap;
    ModifierMap modifiers_;
    MIL_Object_ABC* pObject_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( FirePropagationModifierCapacity )

#endif // __FirePropagationModifierCapacity_h_
