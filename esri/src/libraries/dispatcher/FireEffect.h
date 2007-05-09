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

#include "Entity_ABC.h"
#include "game_asn/Asn.h"
#include "Localisation.h"

namespace dispatcher
{

// =============================================================================
/** @class  FireEffect
    @brief  FireEffect
*/
// Created: AGE 2007-04-18
// =============================================================================
class FireEffect : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             FireEffect( Model& model, const ASN1T_MsgStartFireEffect& message );
    virtual ~FireEffect();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgStartFireEffect& message );
    virtual void SendFullUpdate ( Publisher_ABC& publisher ) const;
    virtual void SendCreation   ( Publisher_ABC& publisher ) const;
    virtual void SendDestruction( Publisher_ABC& publisher ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireEffect( const FireEffect& );            //!< Copy constructor
    FireEffect& operator=( const FireEffect& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    unsigned id_;
    Localisation localisation_;
    ASN1T_EnumFireEffectType  type_;
    //@}
};

}

#endif // __FireEffect_h_
