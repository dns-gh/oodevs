// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_IntelligenceOrdersVisitor_ABC_h_
#define __MIL_IntelligenceOrdersVisitor_ABC_h_

#include <boost/noncopyable.hpp>

// =============================================================================
/** @class  MIL_IntelligenceOrdersVisitor_ABC
    @brief  MIL_IntelligenceOrdersVisitor_ABC
*/
// Created: SBO 2007-11-12
// =============================================================================
class MIL_IntelligenceOrdersVisitor_ABC : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_IntelligenceOrdersVisitor_ABC() {}
    virtual ~MIL_IntelligenceOrdersVisitor_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void Visit( const MIL_IntelligenceOrder& intelligence ) = 0;
    //@}

};

#endif // __MIL_IntelligenceOrdersVisitor_ABC_h_
