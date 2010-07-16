// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef import_Side_h
#define import_Side_h

#include "Entity.h"
#include "Object.h"
#include <xeumeuleu/xml.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <map>

class Mapping;

// =============================================================================
/** @class  Side
    @brief  Side
*/
// Created: LDC 2010-07-07
// =============================================================================
class Side
{
public:
    //! @name Constructors/Destructor
    //@{
             Side();
             Side( xml::xisubstream xis, Mapping& mapping );
    virtual ~Side();
    //@}

    //! @name Operators
    //@{
    friend xml::xostream& operator<<( xml::xostream& xos, const Side& side );
    //@}

private:
    //! @name Helpers
    //@{
    void ReadObjects( xml::xistream& xis );
    void ReadObject( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    Mapping* mapping_;
    unsigned int id_;
    std::string name_;
    unsigned int knowledgeGroupId_;
    boost::shared_ptr< Entity > root_;
    std::map< std::string, Object > objects_;
    //@}
};

#endif // import_Side_h
