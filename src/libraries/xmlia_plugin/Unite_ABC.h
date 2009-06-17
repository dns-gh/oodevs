// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Unite_ABC_h_
#define __Unite_ABC_h_

#include "Entity_ABC.h"

namespace dispatcher
{
  class Agent;
}

namespace xml
{
  class xistream;
  class xostream;
}

namespace plugins
{
namespace xmlia
{

// =============================================================================
/** @class  Unite_ABC
    @brief  Unite_ABC
*/
// Created: MGD 2009-06-12
// =============================================================================
class Unite_ABC : public Entity_ABC
{

public:
    ///! @name Operations
    //@{
    void Serialize( xml::xostream& xos, const std::string& sQnameRapport  ) const;
    virtual void SerializeExtension( xml::xostream& xos, const std::string& sQnameRapport  ) const {};
    virtual void SerializeIncludeEntities( xml::xostream& xos, const std::string& sQnameRapport  ) const {};
    std::string QName() const;
    unsigned int GetId() const;
    static unsigned int QNameToId( std::string sQname );
    //@}

    //! @name Destructor
    //@{
    virtual ~Unite_ABC();
    //@}

protected:
    //! @name Constructors
    //@{
    Unite_ABC( unsigned int id, const std::string& name );
    Unite_ABC( xml::xistream& xis );
    //@}

protected:
    //! @name Member data
    //@{
    unsigned int id_;
    std::string name_;
    //@}
};

}// xmlia

}//plugin

#endif // __Unite_ABC_h_
