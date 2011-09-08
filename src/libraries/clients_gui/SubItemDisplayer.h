// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SubItemDisplayer_h_
#define __SubItemDisplayer_h_

#include "BaseDisplayer.h"

namespace gui
{
    class ItemFactory_ABC;

// =============================================================================
/** @class  SubItemDisplayer
    @brief  SubItemDisplayer
*/
// Created: AGE 2006-02-28
// =============================================================================
class SubItemDisplayer : public BaseDisplayer
{
public:
    //! @name Constructors/Destructor
    //@{
             SubItemDisplayer( const QString& name, ItemFactory_ABC& factory );
    virtual ~SubItemDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    SubItemDisplayer& AddChild( const QString& child );

    Displayer_ABC& operator()( Q3ListViewItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    Q3ListViewItem* FindChild( const QString& name );
    //@}

    //! @name Types
    //@{
    typedef std::vector< QString > T_Children;
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC& factory_;
    QString name_;
    Q3ListViewItem* parent_;
    Q3ListViewItem* current_;
    QString message_;
    T_Children children_;
    //@}
};

}

#endif // __SubItemDisplayer_h_
