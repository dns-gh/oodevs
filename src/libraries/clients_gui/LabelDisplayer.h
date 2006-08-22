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
{

public:
    //! @name Constructors/Destructor
    //@{
             LabelDisplayer( QWidget* parent, const char* name, bool bold, ItemFactory_ABC& factory );
    virtual ~LabelDisplayer();
    //@}

    //! @name Operations
    //@{
    virtual void Hide();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LabelDisplayer( const LabelDisplayer& );            //!< Copy constructor
    LabelDisplayer& operator=( const LabelDisplayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual Displayer_ABC& SubItem( const char* name );
    virtual void StartDisplay();
    virtual void DisplayFormatted( const QString& formatted );
    virtual void EndDisplay();
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
