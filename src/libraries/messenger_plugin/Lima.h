// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __Lima_h_
#define __Lima_h_

#include "TacticalLine_ABC.h"


namespace MsgsClientToMessenger
{
    class MsgLimaCreationRequest;
    class MsgLimaUpdateRequest;
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
/** @class  Lima
    @brief  Lima
*/
// Created: SBO 2006-11-15
// =============================================================================
class Lima : public TacticalLine_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Lima( unsigned int id, const MsgsClientToMessenger::MsgLimaCreationRequest& message );
             Lima( unsigned int id, xml::xistream&, const MsgTacticalLine_Diffusion& diffusion, const kernel::CoordinateConverter_ABC& converter );
    virtual ~Lima();
    //@}

    //! @name Network
    //@{
    void Update         ( const MsgsClientToMessenger::MsgLimaUpdateRequest& message );
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
    Lima( const Lima& );            //!< Copy constructor
    Lima& operator=( const Lima& ); //!< Assignment operator
    //@}
};

}
}

#endif // __Lima_h_
