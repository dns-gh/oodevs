// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Decision_h_
#define __DEC_Decision_h_

#include "MIL.h"
#include "Decision/DEC_Decision_ABC.h"

class MIL_Entity_ABC;

// =============================================================================
/** @class  DEC_Decision
    @brief  DEC_Decision
*/
// Created: LDC 2009-02-27
// =============================================================================
class DEC_Decision : public DEC_Decision_ABC
                   , public DIA_Engine
{

public:
    //! @name Cosntructor
    //@{
             DEC_Decision( MIL_Entity_ABC& entity, const std::string& type );
    explicit DEC_Decision( const std::string& type );
    virtual ~DEC_Decision();
    //@}

    //! @name Operations
    //@{
    virtual void UpdateDecision();
    virtual void Reset();
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void HandleUpdateDecisionError() = 0;
    //@}
};

#endif // __DEC_Decision_h_
