// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __VisionMeteoModel_h_
#define __VisionMeteoModel_h_

#include "meteo/Meteo.h"
#include <boost/noncopyable.hpp>

class MeteoModel;
// =============================================================================
/** @class  VisionMeteoModel
    @brief  VisionMeteoModel
*/
// Created: LGY 2014-04-09
// =============================================================================
class VisionMeteoModel : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit VisionMeteoModel( const MeteoModel& model );
    virtual ~VisionMeteoModel();
    //@}

public:
    //! @name Accessors
    //@{
    const weather::Meteo& GetMeteo( const geometry::Point2f& point ) const;
    //@}

private:
    //! @name Member data
    //@{
    weather::Meteo global_;
    std::vector< weather::Meteo > locals_;
    //@}
};

#endif // __VisionMeteoModel_h_
