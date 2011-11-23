// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __weather_MeteoManager_h_
#define __weather_MeteoManager_h_

#include <set>
#include <boost/shared_ptr.hpp>
#include "Meteo.h"

namespace weather
{
    class PHY_Lighting;
    class Meteo;

// =============================================================================
/** @class  MeteoManager_ABC
    @brief  Meteo manager declaration
*/
// Created: HBD 2010-03-10
// =============================================================================
class MeteoManager_ABC
{
public:
    //! @name Types
    //@{
    typedef std::set< boost::shared_ptr< Meteo > > T_MeteoSet;
    typedef T_MeteoSet::iterator                  IT_MeteoSet;
    typedef T_MeteoSet::const_iterator           CIT_MeteoSet;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             MeteoManager_ABC() {}
    virtual ~MeteoManager_ABC() {}
    //@}

    template< typename Archive > void serialize( Archive&, const unsigned int ) {}

    //! @name Operations
    //@{
    virtual void AddMeteo( weather::Meteo& element ) = 0;
    Meteo* Find( unsigned int id ) const
    {
        for( CIT_MeteoSet it = meteos_.begin(); it != meteos_.end(); ++it )
            if( ( *it )->GetId() == id )
                return ( *it ).get();
        return 0;
    }
    //@}

protected:
    //! @name Member data
    //@{
    T_MeteoSet meteos_;
    //@}
};

}

#endif // __weather_MeteoManager_ABC_h_
