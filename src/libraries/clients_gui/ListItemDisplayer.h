// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ListItemDisplayer_h_
#define __ListItemDisplayer_h_

#include "BaseDisplayer.h"
#include "clients_kernel/Styles.h"

namespace gui
{
    class RichListItem;

// =============================================================================
/** @class  ListItemDisplayer
    @brief  ListItemDisplayer
*/
// Created: AGE 2006-02-22
// =============================================================================
class ListItemDisplayer : public BaseDisplayer
                        , public tools::Caller< QColor >
                        , public tools::Caller< QPixmap >
                        , public tools::Caller< kernel::Styles::Style >
{
public:
    //! @name Constructors/Destructor
    //@{
             ListItemDisplayer();
    virtual ~ListItemDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    ListItemDisplayer& AddColumn( const QString& column );

    Displayer_ABC& operator()( RichListItem* item );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< QString > T_Columns;
    //@}

    //! @name Helpers
    //@{
    virtual void Call( const QColor& color );
    virtual void Call( const QPixmap& pixmap );
    virtual void Call( const kernel::Styles::Style& );

    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    int FindColumn( const QString& name ) const;
    //@}

private:
    //! @name Member data
    //@{
    RichListItem* item_;
    int column_;
    QString message_;
    QPixmap pixmap_;
    T_Columns columns_;
    //@}
};

}

#endif // __ListItemDisplayer_h_
