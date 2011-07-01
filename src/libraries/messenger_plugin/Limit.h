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

namespace sword
{
    class LimitCreationRequest;
    class LimitUpdateRequest;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace xml
{
    class xistream;
    class xostream;
}

namespace plugins
{
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
              Limit( unsigned int id, const sword::LimitCreationRequest& message );
              Limit( unsigned int id, xml::xistream&, const sword::Diffusion& diffusion, const kernel::CoordinateConverter_ABC& converter);
     virtual ~Limit();
    //@}

    //! @name Network
    //@{
    void Update         ( const sword::LimitUpdateRequest& message );
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
    //! @name Copy/Assignment
    //@{
    Limit( const Limit& );            //!< Copy constructor
    Limit& operator=( const Limit& ); //!< Assignment operator
    //@}
};

}
}

#endif // __Limit_h_
