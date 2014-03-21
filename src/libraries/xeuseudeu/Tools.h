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
#include <ostream>

typedef enum SWORD_eLogLevel
{
    SWORD_LOG_LEVEL_INFO,
    SWORD_LOG_LEVEL_WARNING,
    SWORD_LOG_LEVEL_ERROR,
    SWORD_LOG_LEVEL_FATAL
} SWORD_LogLevel;

inline std::ostream& operator<<( std::ostream& os, SWORD_LogLevel model )
{
    return os <<( ( model == SWORD_LOG_LEVEL_INFO ) ?    "info"
                : ( model == SWORD_LOG_LEVEL_WARNING ) ? "warning"
                : ( model == SWORD_LOG_LEVEL_ERROR ) ?   "error"
                :                                        "fatal" );
}

namespace mock
{
    template<>
    class matcher< const core::Model&, double >
        : public detail::matcher_base< const core::Model& >
    {
    public:
        explicit matcher( double expected )
            : expected_( expected )
        {}
        virtual bool operator()( const core::Model& actual )
        {
            return std::abs( actual - expected_ ) < 0.0001;
        }
    private:
        virtual void serialize( std::ostream& s ) const
        {
            s << mock::format( expected_ );
        }
    private:
        double expected_;
    };
}


namespace sword
{
    namespace test
    {
        inline bool operator==( const sword::test::Model& expected, const core::Model& actual )
        {
            return expected.Check( actual );
        }
    }
}
namespace core
{
    inline std::ostream& operator<<( std::ostream& os, const core::Model& model )
    {
        core::ModelPrinter( model, os, true, 2 );
        return os;
    }
}

#endif // MODULE_TESTER_TOOLS_H
