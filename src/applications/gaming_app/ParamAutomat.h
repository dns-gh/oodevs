// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAutomat_h_
#define __ParamAutomat_h_

#include "EntityParameter.h"

namespace kernel
{
    class Automat_ABC;
}

// =============================================================================
/** @class  ParamAutomat
    @brief  ParamAutomat
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAutomat : public EntityParameter< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAutomat(  QWidget* pParent, ASN1T_OID& id, const QString& label, const QString& menu );
    virtual ~ParamAutomat();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamAutomat( const ParamAutomat& );            //!< Copy constructor
    ParamAutomat& operator=( const ParamAutomat& ); //!< Assignement operator
    //@}
};

#endif // __ParamAutomat_h_
