// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef shield_DebugInfo_h
#define shield_DebugInfo_h

#include "DebugInfo_ABC.h"
#include <ostream>

namespace shield
{
// =============================================================================
/** @class  DebugInfo
    @brief  Debug info implementation
*/
// Created: MCO 2011-07-06
// =============================================================================
template< typename T >
class DebugInfo : public DebugInfo_ABC
{
public:
    DebugInfo( const char* prefix, const T& message )
        : prefix_  ( prefix )
        , pMessage_( &message )
    {}
    virtual void Serialize( std::ostream& s ) const
    {
        s << prefix_ << pMessage_->ShortDebugString();
    }
private:
    const char* prefix_;
    const T* pMessage_;
};

}

#endif // shield_DebugInfo_h
