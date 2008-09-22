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

namespace dispatcher
{
    class ClientPublisher_ABC; 
}

namespace xml
{
    class xistream ; 
    class xostream ; 
}

namespace messenger
{

// =============================================================================
/** @class  Limit
    @brief  Limit
*/
// Created: SBO 2006-11-15
// =============================================================================
class Limit : public TacticalLine_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Limit( unsigned int id, const ASN1T_MsgLimitCreationRequest& message );
             Limit( unsigned int id, xml::xistream&, const ASN1T_TacticalLinesDiffusion& diffusion, const kernel::CoordinateConverter_ABC& converter); 
     virtual ~Limit();
    //@}

    //! @name Network
    //@{
    void Update( const ASN1T_MsgLimitUpdateRequest& message );
    void SendCreation   ( dispatcher::ClientPublisher_ABC& publisher ) const;
    void SendUpdate     ( dispatcher::ClientPublisher_ABC& publisher ) const;
    void SendFullState  ( dispatcher::ClientPublisher_ABC& publisher ) const;
    void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;
    //@}

    //! @name xml read / write
    //@{
    virtual void Write( xml::xostream& xos, const kernel::CoordinateConverter_ABC& converter ) const;
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

