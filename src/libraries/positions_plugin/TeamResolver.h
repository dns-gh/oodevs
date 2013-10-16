// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __TeamResolver_h_
#define __TeamResolver_h_

#include <tools/Resolver.h>
#include <boost/noncopyable.hpp>

namespace sword
{
    class CoordLatLong;
}

namespace tools
{
    class Ofstream;
}

namespace plugins
{
namespace positions
{
    class Team;

// =============================================================================
/** @class  TeamResolver
    @brief  TeamResolver
*/
// Created: ABR 2011-04-01
// =============================================================================
class TeamResolver : public tools::Resolver< Team >
                   , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TeamResolver();
    virtual ~TeamResolver();
    //@}

    //! @name Operations
    //@{
    void AddTeam( unsigned int teamId, const std::string& teamName );
    void AddAutomat( unsigned int teamId, unsigned int automatId );
    void AddUnit( unsigned int automatId, unsigned int unitId, const std::string& name, unsigned int timePreviouslyExported );

    void UpdatePosition( unsigned int unitId, const sword::CoordLatLong& coord );

    void Export( tools::Ofstream& file ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< unsigned int, Team* > T_Automats;
    typedef T_Automats::const_iterator    CIT_Automats;
    //@}

private:
    //! @name Member data
    //@{
    T_Automats automats_;
    //@}
};

}
}

#endif // __TeamResolver_h_
