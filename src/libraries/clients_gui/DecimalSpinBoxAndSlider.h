// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DecimalSpinBoxAndSlider_h_
#define __DecimalSpinBoxAndSlider_h_

namespace gui
{

// =============================================================================
/** @class  DecimalSpinBoxAndSlider
    @brief  DecimalSpinBoxAndSlider
*/
// Created: ABR 2011-01-28
// =============================================================================
class DecimalSpinBoxAndSlider : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             DecimalSpinBoxAndSlider( QWidget* parent, float& value, unsigned short precision = 2, float min = 0.f, float max = 10.f, float gap = 0.1f,
                                      Qt::Orientation orientation = Qt::Horizontal, Qt::Orientation sliderOrientation = Qt::Horizontal, bool spinboxFirst = true );
    virtual ~DecimalSpinBoxAndSlider();
    //@}

    //! @name Operations
    //@{
    void setMargin( int value );
    void setSpacing( int value );
    void setEnabled( bool value );
    float value() const;
    //@}

signals:
    //! @name Signals
    //@{
    void valueChanged( float value );
    //@}

public slots:
    //! @name Slots
    //@{
    void setValue( float value );
    //@}

private slots:
    //! @name Helpers slots
    //@{
    void setSpinboxValue( int value );
    void setSliderValue( double value );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecimalSpinBoxAndSlider( const DecimalSpinBoxAndSlider& );            //!< Copy constructor
    DecimalSpinBoxAndSlider& operator=( const DecimalSpinBoxAndSlider& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Q3BoxLayout* layout_;
    QDoubleSpinBox* spinbox_;
    QSlider* slider_;
    const float precision_;
    float& value_;
    //@}
};

}

#endif // __DecimalSpinBoxAndSlider_h_
