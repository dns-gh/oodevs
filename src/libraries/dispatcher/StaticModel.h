// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __dispatcher_StaticModel_h_
#define __dispatcher_StaticModel_h_

#include "clients_kernel/StaticModel.h"

namespace tools
{
    class ExerciseConfig;
}

namespace dispatcher
{
// =============================================================================
/** @class  StaticModel
    @brief  StaticModel
*/
// Created: SBO 2010-05-10
// =============================================================================
class StaticModel : public kernel::StaticModel
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit StaticModel( const tools::ExerciseConfig& config );
    virtual ~StaticModel();
    //@}
};

}

#endif // __dispatcher_StaticModel_h_
