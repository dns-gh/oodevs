// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __MIL_UrbanMotivationsVisitor_ABC_h_
#define __MIL_UrbanMotivationsVisitor_ABC_h_

#include <urban/MotivationsVisitor_ABC.h>
// =============================================================================
/** @class  MIL_UrbanMotivationsVisitor_ABC
    @brief  MIL_UrbanMotivationsVisitor_ABC
*/
// Created: JSR 2012-07-31
// =============================================================================
class MIL_UrbanMotivationsVisitor_ABC : public urban::MotivationsVisitor_ABC
    //private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_UrbanMotivationsVisitor_ABC() {}
    virtual ~MIL_UrbanMotivationsVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    //virtual void Visit( const std::string& motivation, float proportion ) = 0;
    //@}
};

#endif // __MIL_UrbanMotivationsVisitor_ABC_h_
