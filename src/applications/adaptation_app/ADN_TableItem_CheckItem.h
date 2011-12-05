// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_TableItem_CheckItem.h $
// $Author: Ape $
// $Modtime: 28/01/05 11:35 $
// $Revision: 2 $
// $Workfile: ADN_TableItem_CheckItem.h $
//
// *****************************************************************************

#ifndef __ADN_TableItem_CheckItem_h_
#define __ADN_TableItem_CheckItem_h_

#include "ADN_Table.h"
#include "ADN_TableItem_ABC.h"

class ADN_BoolEditor;

// =============================================================================
/** @class  ADN_TableItem_CheckItem
    @brief  ADN_TableItem_CheckItem
    @par    Using example
    @code
    ADN_TableItem_CheckItem;
    @endcode
*/
// Created: AGN 2004-03-10
// =============================================================================
class ADN_TableItem_CheckItem
: public ADN_TableItem_ABC
{
public:

    explicit ADN_TableItem_CheckItem(ADN_Table* parent,void *data);
    virtual ~ADN_TableItem_CheckItem();

public:

    virtual QWidget *createEditor() const;
    virtual void     setContentFromEditor( QWidget *w );
    virtual void     paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected );

    int rtti() const;
    static int RTTI;

public:

    //-------------------------------------------------------------------------
    /** @name Methods used by Connector */
    //-------------------------------------------------------------------------
    //@{
    void setChecked( bool b );
    void setHidden( bool b );
    //@}


protected:

    virtual void        DoValueChanged();

private:
    ADN_BoolEditor*                            pEditor_;
    bool                                    bIsChecked_;

};


// -----------------------------------------------------------------------------
// Name: ADN_TableItem_CheckItem::rtti
// Created: AGN 2004-03-10
// -----------------------------------------------------------------------------
inline
int ADN_TableItem_CheckItem::rtti() const
{
    return RTTI;
}

#endif // __ADN_TableItem_CheckItem_h_
