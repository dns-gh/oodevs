// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BooleanOptionButton_h_
#define __BooleanOptionButton_h_

#include "OptionWidgets.h"

namespace gui
{

// =============================================================================
/** @class  BooleanOptionButton
    @brief  BooleanOptionButton
*/
// Created: AGE 2006-03-30
// =============================================================================
class BooleanOptionButton : public OptionWidget< RichWidget< QToolButton > >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             BooleanOptionButton( kernel::OptionsController& options,
                                  const QString& objectName,
                                  const std::string& optionName,
                                  const QIcon& iconSet,
                                  const QString& toolTip,
                                  QWidget* parent = 0 );
    virtual ~BooleanOptionButton();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnOptionChanged( const kernel::OptionVariant& value );
    //@}
};

} //! namespace gui

#endif // __BooleanOptionButton_h_
