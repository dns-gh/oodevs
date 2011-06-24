// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_DebugInfo_ABC_h
#define shield_DebugInfo_ABC_h

#include <boost/noncopyable.hpp>
#include <ostream>

namespace shield
{
// =============================================================================
/** @class  DebugInfo_ABC
    @brief  Debug info declaration
*/
// Created: MCO 2011-06-23
// =============================================================================
class DebugInfo_ABC : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DebugInfo_ABC() {}
    virtual ~DebugInfo_ABC() {}
    //@}

    //! @name Operators
    //@{
    friend std::ostream& operator<<( std::ostream& s, const DebugInfo_ABC& info )
    {
        info.Serialize( s );
        return s;
    }
    //@}

private:
    //! @name Operations
    //@{
    virtual void Serialize( std::ostream& ) const = 0;
    //@}
};

}

#endif // shield_DebugInfo_ABC_h
