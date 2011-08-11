// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __ComboTableItem_h_
#define __ComboTableItem_h_

#include <boost/noncopyable.hpp>
#include <Qt3Support/Q3Table.h>

namespace gui
{

// =============================================================================
/** @class  ComboTableItem
    @brief  ComboTableItem
*/
// Created: ABR 2011-07-20
// =============================================================================
class ComboTableItem : public Q3TableItem
                     , private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ComboTableItem( Q3Table* parent, const QStringList& content, Q3TableItem::EditType editType, int currentIndex = 0 );
    virtual ~ComboTableItem();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* createEditor() const;
    virtual void setContentFromEditor( QWidget* widget );
    //@}

    //! @name Accessors
    //@{
    int CurrentItem() const;
    void SetCurrentItem( int index );
    QString CurrentText() const;
    void SetCurrentText( const QString& text );
    const QStringList& GetTexts() const;
    //@}

private:
    //! @name Member data
    //@{
    const QStringList& contents_;
    int                currentIndex_;
    mutable QComboBox* currentCombo_;
    //@}
};

} //! namespace gui

#endif // __ComboTableItem_h_
