// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligenceKarmas_h_
#define __IntelligenceKarmas_h_

#include "clients_kernel/Resolver.h"

namespace kernel
{
    class Karma;
}

// =============================================================================
/** @class  IntelligenceKarmas
    @brief  IntelligenceKarmas
*/
// Created: SBO 2006-10-27
// =============================================================================
class IntelligenceKarmas : public kernel::Resolver< kernel::Karma, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             IntelligenceKarmas();
    virtual ~IntelligenceKarmas();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    IntelligenceKarmas( const IntelligenceKarmas& );            //!< Copy constructor
    IntelligenceKarmas& operator=( const IntelligenceKarmas& ); //!< Assignement operator
    //@}
};

#endif // __IntelligenceKarmas_h_
