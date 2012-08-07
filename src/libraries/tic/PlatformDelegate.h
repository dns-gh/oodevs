// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PlatformDelegate_h_
#define __PlatformDelegate_h_

#include "PlatformDelegate_ABC.h"
#include "dispatcher/Observer.h"
#include "protocol/Protocol.h"
#include <geometry/types.h>
#pragma warning (disable : 4511 4512 4127 )
#include <boost/ptr_container/ptr_vector.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent_ABC;
}

namespace tic
{
    class Platform;

// =============================================================================
/** @class  TicExtension
    @brief  TicExtension
*/
// Created: AGE 2008-03-31
// =============================================================================
class PlatformDelegate : public PlatformDelegate_ABC
                       , private dispatcher::Observer< sword::UnitAttributes >
                       , private dispatcher::Observer< sword::UnitEnvironmentType >
                       , private dispatcher::Observer< sword::UnitPathFind >
{
public:
    //! @name Constructors/Destructor
    //@{
             PlatformDelegate( dispatcher::Agent_ABC& holder, const kernel::CoordinateConverter_ABC& converter, float timeStep );
    virtual ~PlatformDelegate();
    //@}

    //! @name Operations
    //@{
    virtual void Accept( PlatformVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Observer
    //@{
    virtual void Notify( const sword::UnitPathFind& updateMessage );
    virtual void Notify( const sword::UnitAttributes& updateMessage );
    virtual void Notify( const sword::UnitEnvironmentType& updateMessage );
    //@}

    //! @name Helpers
    //@{
    void CreatePlatforms( float timeStep );
    void UpdatePlatforms( const sword::UnitAttributes& updateMessage );
    void UpdatePlatforms( const sword::EquipmentDotations_EquipmentDotation& updateMessage );
    void SortPlatforms();
    void UpdateFormation();
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::Agent_ABC& holder_;
    const kernel::CoordinateConverter_ABC& converter_;
    boost::ptr_vector< Platform > platforms_;
    std::vector< Platform* > sorted_;
    std::vector< geometry::Point2f > path_;
    geometry::Point2f position_;
    geometry::Vector2f direction_;
    bool onRoad_;
    //@}
};
}

#endif // __PlatformDelegate_h_
