// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-07-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MT_SpinTableItem.h $
// $Author: Sbo $
// $Modtime: 29/07/05 11:11 $
// $Revision: 1 $
// $Workfile: MT_SpinTableItem.h $
//
// *****************************************************************************

#ifndef __MT_SpinTableItem_h_
#define __MT_SpinTableItem_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <qtable.h>

namespace MT {

// =============================================================================
/** @class  MT_SpinTableItem
    @brief  MT_SpinTableItem
    @par    Using example
    @code
    MT_SpinTableItem;
    @endcode
*/
// Created: SBO 2005-07-29
// =============================================================================
class MT_SpinTableItem : public QTableItem
{
    MT_COPYNOTALLOWED( MT_SpinTableItem );

public:
    //! @name Constructors/Destructor
    //@{
             MT_SpinTableItem( QTable* pTable );
             MT_SpinTableItem( QTable* pTable, int nMinValue, int nMaxValue, int nStep = 1 );
    virtual ~MT_SpinTableItem();
    //@}

    //! @name Operations
    //@{
    QWidget* createEditor        () const;
    void     setContentFromEditor( QWidget* pWidget );
    //@}

protected:
    //! @name Member data
    //@{
    QSpinBox*   pSpinBox_;
	int         nMinValue_;
    int         nMaxValue_;
    int         nStep_;
    //@}
};

} // end namespace MT

#include "MT_SpinTableItem.inl"

#endif // __MT_SpinTableItem_h_
