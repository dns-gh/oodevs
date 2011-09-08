// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LabelDisplayer_h_
#define __LabelDisplayer_h_

#include "BaseDisplayer.h"
#include "clients_kernel/Styles.h"

namespace gui
{
    class RichLabel;
    class ItemFactory_ABC;

// =============================================================================
/** @class  LabelDisplayer
    @brief  LabelDisplayer
*/
// Created: AGE 2006-02-09
// =============================================================================
class LabelDisplayer : public BaseDisplayer
                     , public tools::Caller< kernel::Styles::Style >
{
public:
    //! @name Constructors/Destructor
    //@{
             LabelDisplayer( QWidget* parent, const QString& name, bool bold, ItemFactory_ABC& factory );
    virtual ~LabelDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    virtual void Clear();
    //@}

private:
    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const QString& name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();

    virtual void Call( const kernel::Styles::Style& );
    //@}

private:
    //! @name Member data
    //@{
    RichLabel* valueLabel_;
    QString message_;
    //@}
};

}

#endif // __LabelDisplayer_h_
