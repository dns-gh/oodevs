// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireEffect_h_
#define __FireEffect_h_

#include "SimpleEntity.h"
#include "Localisation.h"

namespace kernel
{
    class ModelVisitor_ABC;
}

namespace sword
{
    enum EnumFireEffectType;
    class StartFireEffect;
}

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  FireEffect
    @brief  FireEffect
*/
// Created: AGE 2007-04-18
// =============================================================================
class FireEffect : public SimpleEntity< >
{
public:
    //! @name Constructors/Destructor
    //@{
             FireEffect( Model& model, const sword::StartFireEffect& message );
    virtual ~FireEffect();
    //@}

    //! @name Operations
    //@{
    void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    void SendDestruction( ClientPublisher_ABC& publisher ) const;

    void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireEffect( const FireEffect& );            //!< Copy constructor
    FireEffect& operator=( const FireEffect& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    unsigned id_;
    Localisation localisation_;
    sword::EnumFireEffectType  type_;
    //@}
};

}

#endif // __FireEffect_h_
