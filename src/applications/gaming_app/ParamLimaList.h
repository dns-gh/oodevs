// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamLimaList_h_
#define __ParamLimaList_h_

#include "EntityListParameter.h"
class Lima;

// =============================================================================
/** @class  ParamLimaList
    @brief  ParamLimaList
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamLimaList : public EntityListParameter< Lima >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamLimaList( QWidget* pParent, ASN1T_ListOID& asn, const QString& label, const QString& menu );
    virtual ~ParamLimaList();
    //@}
};

#endif // __ParamLimaList_h_
