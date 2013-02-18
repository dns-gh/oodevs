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
    explicit DEC_Logger()
    {}
    virtual ~DEC_Logger() {}
    //@}

    //! @name Operations
    //@{
    virtual void Log( const char* name, unsigned int identifier )
    {
        MT_LOG_INFO_MSG( "DEC_CALL " << typeid( T ).name() << " " << identifier << " " << name );
    }
    static DEC_Logger_ABC* Instance()
    {
        static DEC_Logger< T > instance;
        return &instance;
    }
    //@}
};

}

#endif // DEC_Logger_h
