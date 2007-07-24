// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelDataVisitor_ABC_h_
#define __ModelDataVisitor_ABC_h_

#include "ModelVisitor_ABC.h"
#include "ModelDataAcceptor_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  ModelDataVisitor_ABC
    @brief  ModelDataVisitor_ABC
*/
// Created: JCR 2007-05-15
// =============================================================================
class ModelDataVisitor_ABC : public ModelVisitor_ABC
                           , public ModelDataAcceptor_ABC
{
public:
    //! @name Constructor/Destructor
    //@{
             ModelDataVisitor_ABC() {}
    virtual ~ModelDataVisitor_ABC() {}
    //@}
};

}

#endif // __ModelDataVisitor_ABC_h_
