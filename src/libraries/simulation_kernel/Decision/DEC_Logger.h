// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef DEC_Logger_h
#define DEC_Logger_h

#include "DEC_Logger_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <typeinfo>

namespace sword
{
// =============================================================================
/** @class  DEC_Logger
    @brief  DEC_Logger
*/
// Created: SLI 2013-02-07
// =============================================================================
template< typename T >
class DEC_Logger : public DEC_Logger_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DEC_Logger( const T& entity )
        : type_      ( typeid( T ).name() )
        , identifier_( entity.GetID() )
    {}
    virtual ~DEC_Logger() {}
    //@}

    //! @name Operations
    //@{
    virtual void Log( const char* name )
    {
        MT_LOG_INFO_MSG( "DEC_CALL " << type_ << " " << identifier_ << " " << name );
    }
    //@}

private:
    //! @name Member data
    //@{
    const std::string type_;
    const unsigned int identifier_;
    //@}
};

class DEC_NullLogger : public DEC_Logger_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_NullLogger() {}
    virtual ~DEC_NullLogger() {}
    //@}

    //! @name Operations
    //@{
    virtual void Log( const char* ) {}
    //@}
};

}

#endif // DEC_Logger_h
