// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TicExtension_h_
#define __TicExtension_h_

#include "TicExtension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "protocol/protocol.h"
#include <geometry/types.h>
#pragma warning (disable : 4511 4512 4127 )
#include <boost/ptr_container/ptr_vector.hpp>

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace dispatcher
{
    class Agent;
}

namespace plugins
{
namespace tic
{
    class Platform;

// =============================================================================
/** @class  TicExtension
    @brief  TicExtension
*/
// Created: AGE 2008-03-31
// =============================================================================
class TicExtension : public TicExtension_ABC
                   , public kernel::Updatable_ABC< sword::UnitAttributes >
                   , public kernel::Updatable_ABC< sword::UnitEnvironmentType >
                   , public kernel::Updatable_ABC< sword::UnitPathFind >
{
public:
    //! @name Constructors/Destructor
    //@{
             TicExtension( dispatcher::Agent& holder, const kernel::CoordinateConverter_ABC& converter, float timeStep );
    virtual ~TicExtension();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::UnitPathFind& updateMessage );
    virtual void DoUpdate( const sword::UnitAttributes& updateMessage );
    virtual void DoUpdate( const sword::UnitEnvironmentType& updateMessage );

    virtual void Accept( PlatformVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TicExtension( const TicExtension& );            //!< Copy constructor
    TicExtension& operator=( const TicExtension& ); //!< Assignment operator
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
    dispatcher::Agent& holder_;
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
}

#endif // __TicExtension_h_
