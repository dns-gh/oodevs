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

#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Options;
};

namespace gui
{

// =============================================================================
/** @class  BooleanOptionButton
    @brief  BooleanOptionButton
*/
// Created: AGE 2006-03-30
// =============================================================================
class BooleanOptionButton : public QToolButton, public tools::Observer_ABC, public kernel::OptionsObserver_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             BooleanOptionButton( const QIcon& iconSet, const QString& toolTip, QWidget* parent, kernel::Options& options, const std::string& option );
    virtual ~BooleanOptionButton();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    BooleanOptionButton( const BooleanOptionButton& );            //!< Copy constructor
    BooleanOptionButton& operator=( const BooleanOptionButton& ); //!< Assignment operator
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnToggled( bool );
    //@}

    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Options& options_;
    std::string option_;
    QString toolTip_;
    //@}
};

}

#endif // __BooleanOptionButton_h_
