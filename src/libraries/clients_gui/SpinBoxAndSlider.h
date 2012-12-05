// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __gui_SpinBoxAndSlider_h_
#define __gui_SpinBoxAndSlider_h_

namespace gui
{

// =============================================================================
/** @class  SpinBoxAndSlider
    @brief  SpinBoxAndSlider
*/
// Created: ABR 2012-06-12
// =============================================================================
class SpinBoxAndSlider : public QWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             SpinBoxAndSlider( QWidget* parent, int& value, int min = 0, int max = 10, int gap = 1,
                               Qt::Orientation orientation = Qt::Horizontal, Qt::Orientation sliderOrientation = Qt::Horizontal,
                               bool spinboxFirst = true );
    virtual ~SpinBoxAndSlider();
    //@}

    //! @name Operations
    //@{
    void setMargin( int value );
    void setSpacing( int value );
    void setEnabled( bool value );
    int value() const;
    //@}

public slots:
    //! @name Slots
    //@{
    void setValue( int value );
    //@}

private:
    //! @name Member data
    //@{
    Q3BoxLayout*layout_;
    QSpinBox*   spinbox_;
    QSlider*    slider_;
    int&        value_;
    //@}
};

} //! namespace gui

#endif // __gui_SpinBoxAndSlider_h_
