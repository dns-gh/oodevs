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

namespace gui
{

// =============================================================================
/** @class  SizeButton
    @brief  Size button
*/
// Created: SBO 2006-04-05
// =============================================================================
class SizeButton : public QSlider
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
    void SetSize( float value );
    float GetSize() const;
    void EnableValueLabel( const QString& prefix = "" );
    void Revert();
    void Commit();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SizeButton( const SizeButton& );            //!< Copy constructor
    SizeButton& operator=( const SizeButton& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    float   size_;
    QString prefix_;
    QLabel  label_;
    bool    changed_;
    float   previous_;
    bool    valueLabel_;
    //@}
};

}

#endif // __SizeButton_h_
