// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ItemDecorationGetter_ABC_h_
#define __ItemDecorationGetter_ABC_h_

namespace gui
{

// =============================================================================
/** @class  ItemDecorationGetter_ABC
    @brief  ItemDecorationGetter_ABC
*/
// Created: JSR 2012-09-14
// =============================================================================
class ItemDecorationGetter_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ItemDecorationGetter_ABC() {}
    virtual ~ItemDecorationGetter_ABC() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual const QPixmap* GetDecoration( const QModelIndex &index ) = 0;
    //@}
};

} //! namespace gui

#endif // __ItemDecorationGetter_ABC_h_
