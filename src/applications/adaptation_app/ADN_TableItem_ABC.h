// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_TableItem_ABC_h_
#define __ADN_TableItem_ABC_h_

#include "ADN_Gfx_ABC.h"

// =============================================================================
/** @class  ADN_TableItem_ABC
    @brief  ADN_TableItem_ABC
*/
// Created: JDY 2003-07-09
// =============================================================================
class ADN_TableItem_ABC : public Q3TableItem
                        , public QObject
                        , public ADN_Gfx_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ADN_TableItem_ABC( Q3Table* parent, void* data, Q3TableItem::EditType nEditType = WhenCurrent );
    virtual ~ADN_TableItem_ABC();
    //@}

    //! @name Operations
    //@{
    void* GetData();
    virtual void DoValueChanged() = 0;
    //@}

    //! @name Colors
    //@{
    virtual bool UseColor() const;
    virtual void SetUseColor( bool );
    virtual std::pair< double, double > GetRangeForColor() const;
    //@}

protected:
    //! @name Member data
    //@{
    void* pData_;
    //@}
};


//-----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC::GetData
// Created: JDY 03-07-09
//-----------------------------------------------------------------------------
inline
void* ADN_TableItem_ABC::GetData()
{
    return pData_;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC::GetUserColor
// Created: ABR 2012-02-09
// -----------------------------------------------------------------------------
inline
bool ADN_TableItem_ABC::UseColor() const
{
    return false;
}

// -----------------------------------------------------------------------------
// Name: ADN_TableItem_ABC::SetUseColor
// Created: ABR 2012-02-09
// -----------------------------------------------------------------------------
inline
void ADN_TableItem_ABC::SetUseColor( bool )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: std::pair< double, double > ADN_TableItem_ABC::GetRangeForColor
// Created: ABR 2012-02-09
// -----------------------------------------------------------------------------
inline
std::pair< double, double > ADN_TableItem_ABC::GetRangeForColor() const
{
    return std::pair< double, double >( 0.f, 0.f );
}

#endif // __ADN_TableItem_ABC_h_