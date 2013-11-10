// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef LOGISTIC_PLUGIN_CONSIGNWRITER_H
#define LOGISTIC_PLUGIN_CONSIGNWRITER_H

#include <sstream>
#include <string>
#include <boost/noncopyable.hpp>

namespace plugins
{
namespace logistic
{

class ConsignWriter: private boost::noncopyable
{
public:
    ConsignWriter() : written_( 0 )
    {
    }

    template< typename T >
    ConsignWriter& operator<<( const T& value )
    {
        if( written_ )
            buf_ << " ; ";
        written_++;
        buf_ << value;
        return *this;
    }

    void End()
    {
        buf_ << "\n";
        written_ = 0;
    }

    std::string GetLine() const
    {
        return buf_.str();
    }

private:
    std::stringstream buf_;
    int written_;
};

}  // namespace logistic
}  // namespace plugin

#endif // LOGISTIC_PLUGIN_CONSIGNWRITER_H
