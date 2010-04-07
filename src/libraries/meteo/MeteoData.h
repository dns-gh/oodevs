// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MeteoData_h_
#define __MeteoData_h_

#include "meteo/PHY_Meteo.h"
#include "geometry/Types.h"

namespace Common
{
    class MsgMeteoAttributes;
}
class MeteoModel_ABC;

namespace dispatcher
{
    class ClientPublisher_ABC;
}

// =============================================================================
/** @class  MeteoData
    @brief  MeteoData
*/
// Created: HBD 2010-03-26
// =============================================================================
class MeteoData: public PHY_Meteo
{

public:
    //! @name Constructors/Destructor
    //@{
    MeteoData( unsigned int id, const geometry::Point2f& upLeft, const geometry::Point2f& downRight, const Common::MsgMeteoAttributes&, MeteoModel_ABC* );
    virtual ~MeteoData();
    //@}

    //! @name Operations
    //@{
    virtual bool IsInside( const geometry::Point2f& point ) const;
    virtual void SendCreation( dispatcher::ClientPublisher_ABC& publisher) const;
    virtual void SendDestruction( dispatcher::ClientPublisher_ABC& publisher) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MeteoData( const MeteoData& );            //!< Copy constructor
    MeteoData& operator=( const MeteoData& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    geometry::Rectangle2f rect_;
    MeteoModel_ABC& listener_;

    //@}
};

#endif // __MeteoData_h_
