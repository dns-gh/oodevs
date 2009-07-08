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
    Mission::Mission( kernel::MissionType& mission, std::vector< Point >& limit1, std::vector< Point >& limit2 );
    virtual ~Mission();
    //@}

    ///! @name Operations
    //@{
    void Serialize( xml::xostream& xos ) const;
    void ReadPoint( xml::xistream& xis, std::vector< Point >& limite );
    void Update( kernel::MissionType& mission );
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::vector< Point > limiteGauche_;
    std::vector< Point > limiteDroite_;
    //@}
};

}// xmlia

}//plugin

#endif // __XMLIA_Mission_h_
