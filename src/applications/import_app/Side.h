// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Side_h_
#define __Side_h_

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
             Side( unsigned int id, const std::string& name, Mapping& mapping );
    virtual ~Side();
    //@}

    //! @name Operations
    //@{
    void Write( xml::xostream& xos ) const;
    void ReadObjects( xml::xistream& xis );
    void ReadTactical( xml::xistream& xis );
    //@}

    //! @name Accessors
    //@{
    Entity& Root();
    //@}

private:
    //! @name Helpers
    //@{
    void WriteObjects( xml::xostream& xos ) const;
    void WriteTactical( xml::xostream& xos ) const;
    void WriteCommunications( xml::xostream& xos ) const;
    void ReadObject( xml::xistream& xis );
    void ReadObjectList( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int id_;
    unsigned int knowledgeGroupId_;
    std::string name_;
    Mapping* mapping_;
    std::map< std::string, Object > objects_;
    boost::shared_ptr< Entity > root_;
    //@}
};

#endif // __Side_h_
