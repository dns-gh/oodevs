// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __ADN_ListView_DescriptionParameter_h_
#define __ADN_ListView_DescriptionParameter_h_

#include <boost/noncopyable.hpp>
#include "ADN_ListView.h"

// =============================================================================
/** @class  ADN_ListView_DescriptionParameter
    @brief  ADN_ListView_DescriptionParameter
*/
// Created: NPT 2013-01-16
// =============================================================================
class ADN_ListView_DescriptionParameter : public ADN_ListView
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_ListView_DescriptionParameter();
    virtual ~ADN_ListView_DescriptionParameter();
    //@}

    //! @name Operations
    //@{
    void ConnectItem( bool bConnect );
    virtual void Warn( ADN_ErrorStatus errorStatus, const QString& errorMsg );
    //@}
};

#endif // __ADN_ListView_DescriptionParameter_h_
