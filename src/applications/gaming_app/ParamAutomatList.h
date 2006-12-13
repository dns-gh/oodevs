// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamAutomatList_h_
#define __ParamAutomatList_h_

#include "gaming/ASN_Types.h"
#include "EntityListParameter.h"

namespace kernel
{
    class Automat_ABC;
}


// =============================================================================
/** @class  ParamAutomatList
    @brief  ParamAutomatList
*/
// Created: AGE 2006-11-29
// =============================================================================
class ParamAutomatList : public EntityListParameter< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamAutomatList( QWidget* pParent, ASN1T_ListAgent*& asn, const QString& label, const QString& menu );
    virtual ~ParamAutomatList();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamAutomatList( const ParamAutomatList& );            //!< Copy constructor
    ParamAutomatList& operator=( const ParamAutomatList& ); //!< Assignement operator
    //@}
};

#endif // __ParamAutomatList_h_
