// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisplayInfo_h_
#define __DisplayInfo_h_

namespace xml
{
    class xistream;
}

namespace plugins
{
namespace messenger
{

class DisplayInfo
{
public:
    DisplayInfo( xml::xistream& xis );
    std::string Color() const;
    std::string Level() const;
private:
    std::string color_;
    std::string level_;
};

}
}

#endif // __DisplayInfo_h_