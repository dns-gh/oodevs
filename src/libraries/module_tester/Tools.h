// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MODULE_TESTER_TOOLS_H
#define MODULE_TESTER_TOOLS_H

#include <core/ModelPrinter.h>
#include <core/Convert.h>
#include <module_api/Log.h>
#include <ostream>

inline std::ostream& operator<<( std::ostream& os, const SWORD_Model* model )
{
    if( model )
        os << "\'" << core::Convert( model )->Context() << "\'";
    return os;
}

inline std::ostream& operator<<( std::ostream& os, SWORD_Model* model )
{
    if( model )
        core::ModelPrinter( *core::Convert( model ), os, false, 2 );
    return os;
}

inline std::ostream& operator<<( std::ostream& os, SWORD_LogLevel model )
{
    return os <<( ( model == SWORD_LOG_LEVEL_INFO ) ?    "info"
                : ( model == SWORD_LOG_LEVEL_WARNING ) ? "warning"
                : ( model == SWORD_LOG_LEVEL_ERROR ) ?   "error"
                :                                        "fatal" );
}

#endif // MODULE_TESTER_TOOLS_H
