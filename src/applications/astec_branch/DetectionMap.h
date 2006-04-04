// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DetectionMap_h_
#define __DetectionMap_h_

#include "graphics/ElevationMap.h"

// =============================================================================
/** @class  DetectionMap
    @brief  DetectionMap
*/
// Created: AGE 2006-04-04
// =============================================================================
class DetectionMap : public ElevationMap
{

public:
    //! @name Types
    //@{
    struct Environment
    {
        bool          InInTown() const    { return data_ & town_; };
        bool          InInForest() const  { return data_ & forest_; };
        unsigned char MeteoEffect() const { return data_ & meteoMask_; };

        static const unsigned char town_      = 0x80;
        static const unsigned char forest_    = 0x40;
        static const unsigned char meteoMask_ = 0x3f;
        unsigned char data_;
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    explicit DetectionMap( const std::string& filename );
    virtual ~DetectionMap();
    //@}

    //! @name Operations
    //@{
    const Environment* EnvironmentData( unsigned x, unsigned y ) const;
    Environment        EnvironmentAt( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    DetectionMap( const DetectionMap& );            //!< Copy constructor
    DetectionMap& operator=( const DetectionMap& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    Environment* environment_;
    float cellsize_;
    //@}
};

#endif // __DetectionMap_h_
