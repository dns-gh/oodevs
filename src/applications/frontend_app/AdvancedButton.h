// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AdvancedButton_h_
#define __AdvancedButton_h_

#include <QtGui/qpushbutton.h>

// =============================================================================
/** @class  AdvancedButton
    @brief  AdvancedButton
*/
// Created: SBO 2008-03-05
// =============================================================================
class AdvancedButton : public QPushButton
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit AdvancedButton( QWidget* widget );
    virtual ~AdvancedButton();
    //@}

    //! @name Operations
    //@{
    void Link( QWidget* widget );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnToggle( bool on );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AdvancedButton( const AdvancedButton& );            //!< Copy constructor
    AdvancedButton& operator=( const AdvancedButton& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< QWidget* > T_Widgets;
    //@}

private:
    //! @name Member data
    //@{
    T_Widgets widgets_;
    //@}
};

#endif // __AdvancedButton_h_
