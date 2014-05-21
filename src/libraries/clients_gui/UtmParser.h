// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __UtmParser_h_
#define __UtmParser_h_

#include "LocationParser_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
}

namespace gui
{
// =============================================================================
/** @class  UtmParser
    @brief  UtmParser
*/
// Created: MCO 2014-05-21
// =============================================================================
class UtmParser : public LocationParser_ABC
{
public:
             UtmParser( const kernel::CoordinateConverter_ABC& converter, const std::string& code );
    virtual ~UtmParser();

    virtual const LocationParserDescriptor& GetDescriptor() const;
    virtual bool Parse( const QStringList& content, geometry::Point2f& result, QStringList& hint, bool small ) const;
    virtual QStringList Split( const QString& input ) const;
    virtual std::string GetStringPosition( const geometry::Point2f& position ) const;

private:
    const kernel::CoordinateConverter_ABC& converter_;
    const std::string code_;
};

}

#endif // __UtmParser_h_
