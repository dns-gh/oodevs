// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Limit_h_
#define __Limit_h_

#include "TacticalLine_ABC.h"
#include "Entity_ABC.h"

namespace dispatcher
{
    class Publisher_ABC;

// =============================================================================
/** @class  Limit
    @brief  Limit
*/
// Created: SBO 2006-11-15
// =============================================================================
class Limit : public TacticalLine_ABC, public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Limit( const Model& model, const ASN1T_MsgLimitCreation& message );
    virtual ~Limit();
    //@}

    //! @name Operations
    //@{
    void Update      ( const ASN1T_MsgLimitCreation& message );
    void Update      ( const ASN1T_MsgLimitUpdate& message );
    void SendCreation( Publisher_ABC& publisher ) const;
    void SendFullUpdate( Publisher_ABC& publisher ) const;
    void CommitDestruction();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Limit( const Limit& );            //!< Copy constructor
    Limit& operator=( const Limit& ); //!< Assignement operator
    //@}
};

}

#endif // __Limit_h_

