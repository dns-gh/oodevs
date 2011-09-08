// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GroupDisplayer_h_
#define __GroupDisplayer_h_

#include "clients_kernel/Displayer_ABC.h"
#include <map>

namespace gui
{
    class LabelDisplayer;
    class ItemFactory_ABC;

// =============================================================================
/** @class  GroupDisplayer
    @brief  GroupDisplayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class GroupDisplayer : public kernel::Displayer_ABC, public Q3GroupBox
{
public:
    //! @name Constructors/Destructor
    //@{
             GroupDisplayer( QWidget* parent, const QString& name, ItemFactory_ABC& factory );
    virtual ~GroupDisplayer();
    //@}

    //! @name Operations
    //@{
    GroupDisplayer& AddLabel( const QString& name, bool bold = false );
    GroupDisplayer& AddSpinBox( const QString& name, int min, int max, int step );
    GroupDisplayer& AddCheckBox( const QString& name );
    virtual void Hide();
    virtual void Clear();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< QString, kernel::Displayer_ABC* > T_Items;
    typedef T_Items::iterator                          IT_Items;
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
    //@}

private:
    //! @name Member data
    //@{
    ItemFactory_ABC& factory_;
    T_Items items_;
    //@}
};

}

#endif // __GroupDisplayer_h_
