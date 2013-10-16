// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Team_h_
#define __Team_h_

#include <tools/Resolver.h>

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
    class Unit;

// =============================================================================
/** @class  Team
    @brief  Team
*/
// Created: ABR 2011-04-01
// =============================================================================
class Team : public tools::Resolver< Unit >
{
public:
    //! @name Constructors/Destructor
    //@{
             Team( unsigned int id, const std::string& name );
    virtual ~Team();
    //@}

    //! @name Operations
    //@{
    void AddUnit( unsigned int unitId, const std::string& name, unsigned int timePreviouslyExported );

    void Export( tools::Ofstream& file ) const;

    void UpdatePosition( unsigned int unitId, const sword::CoordLatLong& coord );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int      id_;
    const std::string name_;
    //@}
};

}
}

#endif // __Team_h_
