// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Hla_Plugin_Class_ABC_h_
#define __Hla_Plugin_Class_ABC_h_

namespace hla
{
    class Federate;
}

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  ObjectClass_ABC
    @brief  Class definition
*/
// Created: AGE 2008-02-22
// =============================================================================
class ObjectClass_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectClass_ABC() {}
    virtual ~ObjectClass_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void RegisterTo( ::hla::Federate& federate ) = 0;
    //@}
};

}
}

#endif // __Hla_Plugin_Class_ABC_h_
