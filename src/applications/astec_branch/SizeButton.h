// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SizeButton_h_
#define __SizeButton_h_

#include <qpushbutton.h>

// =============================================================================
/** @class  SizeButton
    @brief  Size button
*/
// Created: SBO 2006-04-05
// =============================================================================
class SizeButton : public QPushButton
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             SizeButton( QWidget* parent = 0, const char* name = 0 );
    virtual ~SizeButton();
    //@}

    //! @name Operations
    //@{
    void EnableValueLabel( const std::string& prefix = "" );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int value );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    SizeButton( const SizeButton& );            //!< Copy constructor
    SizeButton& operator=( const SizeButton& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    QSlider* slider_;
    int      size_;
    std::string prefix_;
    bool        valueLabel_;
    //@}
};

#endif // __SizeButton_h_
