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
    class Object_ABC;
}

namespace gui
{
    class TerrainObjectProxy;
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
        kernel::Object_ABC* data_;
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
    void AddUrbanBlock( gui::TerrainObjectProxy& object );
    const kernel::Object_ABC* GetUrbanBlock( const geometry::Point2f& point ) const;
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
