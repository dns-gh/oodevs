// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DisaggregationStrategy_h_
#define __DisaggregationStrategy_h_

#include "DisaggregationStrategy_ABC.h"
#include <boost/shared_ptr.hpp>

namespace plugins
{
namespace vrforces
{
    class DisaggregationArea_ABC;
    class Facade;

// =============================================================================
/** @class  DisaggregationStrategy
    @brief  DisaggregationStrategy
*/
// Created: SBO 2011-03-16
// =============================================================================
class DisaggregationStrategy : public DisaggregationStrategy_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DisaggregationStrategy( Facade& vrforces );
    virtual ~DisaggregationStrategy();
    //@}

    //! @name Operations
    //@{
    virtual bool IsAggregatedLocation( double latitude, double longitude ) const;
    virtual void AddArea( const sword::ShapeCreation& message );
    virtual void RemoveArea( const sword::ShapeDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    Facade& vrforces_;
    std::map< unsigned long, boost::shared_ptr< DisaggregationArea_ABC > > area_;
    //@}
};
}
}

#endif // __DisaggregationStrategy_h_
