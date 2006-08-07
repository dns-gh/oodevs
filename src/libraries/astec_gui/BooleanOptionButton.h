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

#include "astec_gaming/OptionsObserver_ABC.h"
class Options;

// =============================================================================
/** @class  BooleanOptionButton
    @brief  BooleanOptionButton
*/
// Created: AGE 2006-03-30
// =============================================================================
class BooleanOptionButton : public QToolButton, public Observer_ABC, public OptionsObserver_ABC
{
    Q_OBJECT;
public:
    //! @name Constructors/Destructor
    //@{
             BooleanOptionButton( const QIconSet& iconSet, const QString& toolTip, QToolBar* parent, Options& options, const std::string& option );
    virtual ~BooleanOptionButton();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    BooleanOptionButton( const BooleanOptionButton& );            //!< Copy constructor
    BooleanOptionButton& operator=( const BooleanOptionButton& ); //!< Assignement operator
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnToggled( bool );
    //@}

    //! @name Helpers
    //@{
    virtual void OptionChanged( const std::string& name, const OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    Options& options_;
    std::string option_;
    QString toolTip_;
    //@}
};

#endif // __BooleanOptionButton_h_
