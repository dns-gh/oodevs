// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Drawing_h_
#define __Drawing_h_

#include "game_asn/Messenger.h"
#include <geometry/Types.h>

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class ClientPublisher_ABC;
}

namespace plugins
{
namespace messenger
{

// =============================================================================
/** @class  Drawing
    @brief  Drawing
*/
// Created: SBO 2008-06-06
// =============================================================================
class Drawing
{

public:
    //! @name Constructors/Destructor
    //@{
             Drawing( unsigned int id, const ASN1T_MsgShapeCreationRequest& asn, const kernel::CoordinateConverter_ABC& converter );
             Drawing( unsigned int id, xml::xistream& xis, const kernel::CoordinateConverter_ABC& converter );
             Drawing( unsigned int id, const Drawing& rhs );
    virtual ~Drawing();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    void Update( const ASN1T_MsgShapeUpdateRequest& asn );
    virtual void SendCreation   ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendUpdate     ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendFullState  ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;

    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Drawing( const Drawing& );            //!< Copy constructor
    Drawing& operator=( const Drawing& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    void SerializePoint( const ASN1T_CoordLatLong& point, xml::xostream& xos ) const;
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< ASN1T_CoordLatLong > T_Points;
    //@}

public:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    unsigned long id_;
    std::string category_;
    std::string color_;
    std::string template_;
    T_Points points_;
    //@}
};

}
}

#endif // __Drawing_h_
