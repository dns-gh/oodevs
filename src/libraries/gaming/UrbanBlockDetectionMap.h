// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __UrbanBlockDetectionMap_h_
#define __UrbanBlockDetectionMap_h_

#include "clients_kernel/UrbanObject_ABC.h"
#include <tools/ElementObserver_ABC.h>
#include <boost/noncopyable.hpp>
#include <boost/optional/optional_fwd.hpp>

namespace kernel
{
    class DetectionMap;
    class Controller;
    class Object_ABC;
}

// =============================================================================
/** @class  UrbanBlockDetectionMap
    @brief  Urban block detection map
*/
// Created: SLG 2010-03-12
// =============================================================================
class UrbanBlockDetectionMap : public tools::Observer_ABC
                             , public tools::ElementObserver_ABC< kernel::UrbanObject_ABC >
                             , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanBlockDetectionMap( kernel::Controllers& controllers, const kernel::DetectionMap& map );
    virtual ~UrbanBlockDetectionMap();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::UrbanObject_ABC& object );
    const boost::optional< std::string > GetEnvironment( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< int, int > T_Pair;
    typedef std::map< T_Pair, std::string > T_Environment;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& map_;
    T_Environment urbanBlockEnvironment_;
    //@}
};

#endif // __UrbanBlockDetectionMap_h_
