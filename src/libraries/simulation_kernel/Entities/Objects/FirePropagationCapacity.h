// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FirePropagationCapacity_h_
#define __FirePropagationCapacity_h_

#include "PropagationCapacity_ABC.h"

namespace xml
{
    class xistream;
}

class MIL_PropagationManager;
class FireAttribute;
class TER_Localisation;

// =============================================================================
/** @class  FirePropagationCapacity
    @brief  FirePropagationCapacity
*/
// Created: RFT 2008-05-30
// =============================================================================
class FirePropagationCapacity : public PropagationCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FirePropagationCapacity();
    explicit FirePropagationCapacity( xml::xistream& xis, MIL_PropagationManager& manager );
    virtual ~FirePropagationCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    //@}

private:
    //! @name Copy
    //@{
    FirePropagationCapacity( const FirePropagationCapacity& from );
    //@}

    //! @name Helpers
    //@{
    void InitializeUpdate( MIL_Object_ABC& object, const FireAttribute& attr );
    int  UpdateState( MIL_Object_ABC& object, const FireAttribute& attr, unsigned int time );
    void Propagate( MIL_Object_ABC& object );
    void CheckPropagation( const MT_Vector2D& vOrigin, MIL_Object_ABC& object );
    //@}

    //! @name Helpers
    //@{
    TER_Localisation GetLocalisation( const MT_Vector2D& pt ) const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_PropagationManager* pManager_;
    //Time of creation, and time of death of the fire
    bool needUpdate_;
    unsigned int timeOfCreation_;
    unsigned int timeOfDeath_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( FirePropagationCapacity )

#endif
