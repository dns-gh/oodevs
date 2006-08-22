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

namespace gui
{

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
             SizeButton( QWidget* parent = 0, const char* name = 0, float value = 1 );
    virtual ~SizeButton();
    //@}

    //! @name Operations
    //@{
    float GetSize() const;
    void EnableValueLabel( const std::string& prefix = "" );
    void Revert();
    void Commit();
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

private:
    //! @name Member data
    //@{
    QSlider*    slider_;
    float       size_;
    std::string prefix_;
    bool        valueLabel_;
    bool changed_;
    float previous_;
    //@}
};

}

#endif // __SizeButton_h_
