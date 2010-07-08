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

namespace kernel
{
    class DetectionMap;
}

namespace urban
{
    class TerrainObject_ABC;
}

// =============================================================================
/** @class  UrbanBlockDetectionMap
    @brief  Urban block detection map
*/
// Created: SLG 2010-03-12
// =============================================================================
class UrbanBlockDetectionMap
{

public:
    //! @name Types
    //@{
    struct UrbanBlockEnvironment
    {
        urban::TerrainObject_ABC* data_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             UrbanBlockDetectionMap( const kernel::DetectionMap& map );
    virtual ~UrbanBlockDetectionMap();
    //@}

    //! @name Operations
    //@{
    void AddUrbanBlock( urban::TerrainObject_ABC& object );
    const urban::TerrainObject_ABC* GetUrbanBlock( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanBlockDetectionMap( const UrbanBlockDetectionMap& );            //!< Copy constructor
    UrbanBlockDetectionMap& operator=( const UrbanBlockDetectionMap& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::DetectionMap& map_;
    std::map< std::pair< int, int >, UrbanBlockEnvironment > urbanBlockEnvironment_;
    //@}
};

#endif // __UrbanBlockDetectionMap_h_
