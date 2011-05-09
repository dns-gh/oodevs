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

#include "tools/ElementObserver_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include <boost/noncopyable.hpp>

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
                             , public tools::ElementObserver_ABC< gui::TerrainObjectProxy >
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
    virtual void NotifyCreated( const gui::TerrainObjectProxy& object );
    const kernel::Object_ABC* GetUrbanBlock( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Types
    //@{
    struct UrbanBlockEnvironment
    {
        const kernel::Object_ABC* data_;
    };
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& map_;
    std::map< std::pair< int, int >, UrbanBlockEnvironment > urbanBlockEnvironment_;
    //@}
};

#endif // __UrbanBlockDetectionMap_h_
