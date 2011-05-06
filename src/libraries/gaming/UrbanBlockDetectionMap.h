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

#include <boost/noncopyable.hpp>

namespace kernel
{
    class DetectionMap;
    class Object_ABC;
}

// =============================================================================
/** @class  UrbanBlockDetectionMap
    @brief  Urban block detection map
*/
// Created: SLG 2010-03-12
// =============================================================================
class UrbanBlockDetectionMap : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit UrbanBlockDetectionMap( const kernel::DetectionMap& map );
    virtual ~UrbanBlockDetectionMap();
    //@}

    //! @name Operations
    //@{
    void AddUrbanBlock( kernel::Object_ABC& object );
    const kernel::Object_ABC* GetUrbanBlock( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Types
    //@{
    struct UrbanBlockEnvironment
    {
        kernel::Object_ABC* data_;
    };
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& map_;
    std::map< std::pair< int, int >, UrbanBlockEnvironment > urbanBlockEnvironment_;
    //@}
};

#endif // __UrbanBlockDetectionMap_h_
