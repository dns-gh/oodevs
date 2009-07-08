// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __XMLIA_Mission_h_
#define __XMLIA_Mission_h_

#include "Entity_ABC.h"

namespace kernel
{
  class MissionType;
}

namespace plugins
{
namespace xmlia
{
  class Point;
// =============================================================================
/** @class  Mission
    @brief  Mission
*/
// Created: MGD 2009-06-12
// =============================================================================
class Mission : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    Mission( xml::xistream& xis );
    Mission( kernel::MissionType& mission );
    virtual ~Mission();
    //@}

    ///! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void Update( kernel::MissionType& agent );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    Point* gaucheDebut_;
    Point* gaucheFin_;
    Point* droiteDebut_;
    Point* droiteFin_;
    //@}
};

}// xmlia

}//plugin

#endif // __XMLIA_Mission_h_
