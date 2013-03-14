// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __Unit_h_
#define __Unit_h_

#include <boost/noncopyable.hpp>
#include <vector>
#include <string>

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
// =============================================================================
/** @class  Unit
    @brief  Unit
*/
// Created: ABR 2011-04-01
// =============================================================================
class Unit : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             Unit( unsigned int id, const std::string& name, unsigned int timePreviouslyExported );
    virtual ~Unit();
    //@}

    //! @name Operations
    //@{
    void Export( tools::Ofstream& file );
    void UpdatePosition( const sword::CoordLatLong& coord );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< std::string >    T_Positions;
    typedef T_Positions::const_iterator CIT_Positions;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int      id_;
    const std::string name_;
    T_Positions       positions_;
    //@}
};

}
}

#endif // __Unit_h_
