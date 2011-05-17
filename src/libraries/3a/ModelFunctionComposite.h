// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ModelFunctionComposite_h_
#define __ModelFunctionComposite_h_

#include "ModelFunction_ABC.h"
#include <vector>
#include <boost/shared_ptr.hpp>

// =============================================================================
/** @class  ModelFunctionComposite
    @brief  ModelFunction_ABC Composite
*/
// Created: AGE 2007-08-30
// =============================================================================
class ModelFunctionComposite : public ModelFunction_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ModelFunctionComposite();
    virtual ~ModelFunctionComposite();
    //@}

    //! @name Operations
    //@{
    void Add( boost::shared_ptr< ModelFunction_ABC > function );

    virtual void BeginTick();
    virtual void Receive( const sword::SimToClient& message );
    virtual void EndTick();
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< boost::shared_ptr< ModelFunction_ABC > > T_Functions;
    //@}

private:
    //! @name Member data
    //@{
    T_Functions functions_;
    //@}
};

#endif // __ModelFunctionComposite_h_
