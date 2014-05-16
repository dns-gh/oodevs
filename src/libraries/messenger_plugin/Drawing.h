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

#include "protocol/protocol.h"
#include <boost/noncopyable.hpp>
#include <boost/optional/optional.hpp>

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
class Drawing : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Drawing( unsigned int id, const sword::ShapeCreationRequest& asn, const kernel::CoordinateConverter_ABC& converter );
             Drawing( unsigned int id, xml::xistream& xis, const boost::optional< sword::Diffusion >& diffusion, const kernel::CoordinateConverter_ABC& converter );
             Drawing( unsigned int id, const Drawing& rhs );
    virtual ~Drawing();
    //@}

    //! @name Operations
    //@{
    unsigned long GetId() const;
    const std::vector< sword::CoordLatLong >& GetPoints() const;
    const kernel::CoordinateConverter_ABC& GetConverter() const;
    const boost::optional< sword::Diffusion >& GetDiffusion() const;

    void Update( const sword::ShapeUpdateRequest& asn );
    virtual void SendCreation   ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendUpdate     ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendFullState  ( dispatcher::ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher ) const;

    void Serialize( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadPoint( xml::xistream& xis );
    void SerializePoint( const sword::CoordLatLong& point, xml::xostream& xos ) const;
    //@}

    //! @name Helpers
    //@{
    typedef std::vector< sword::CoordLatLong > T_Points;
    typedef T_Points::const_iterator         CIT_Points;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::CoordinateConverter_ABC& converter_;
    unsigned long id_;
    std::string category_;
    std::string color_;
    std::string pattern_;
    T_Points points_;
    boost::optional< sword::Diffusion > diffusion_;
    boost::optional< sword::EnumPenStyle > style_;
    //@}
};

}
}

#endif // __Drawing_h_
