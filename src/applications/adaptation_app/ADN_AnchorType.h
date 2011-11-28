// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_AnchorType_h_
#define __ADN_AnchorType_h_

#include "ADN_TableItem_ComboBox.h"

// =============================================================================
/** @class  ADN_AnchorType
    @brief  ADN_AnchorType
*/
// Created: LGY 2011-11-28
// =============================================================================
class ADN_AnchorType : public ADN_TableItem_ComboBox
{
public:
    //! @name Constructors/Destructor
    //@{
             ADN_AnchorType( ADN_Table* parent, void* data, T_ConnectorVector& itemConnectors );
    virtual ~ADN_AnchorType();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ADN_AnchorType( const ADN_AnchorType& );            //!< Copy constructor
    ADN_AnchorType& operator=( const ADN_AnchorType& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_ConnectorVector& itemConnectors_;
    //@}
};

#endif // __ADN_AnchorType_h_
