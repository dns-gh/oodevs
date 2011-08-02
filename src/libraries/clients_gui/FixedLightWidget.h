// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FixedLightWidget_h_
#define __FixedLightWidget_h_

class FixedLighting;

namespace gui
{
    class ColorButton;

// =============================================================================
/** @class  FixedLightWidget
    @brief  FixedLightWidget
*/
// Created: AGE 2007-01-03
// =============================================================================
class FixedLightWidget : public Q3HBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             FixedLightWidget( QWidget* parent, FixedLighting& lighting );
    virtual ~FixedLightWidget();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnDirectionChanged( const geometry::Vector3f& direction );
    void OnAmbientChanged( const QColor& color );
    void OnDiffuseChanged( const QColor& color );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FixedLightWidget( const FixedLightWidget& );            //!< Copy constructor
    FixedLightWidget& operator=( const FixedLightWidget& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    FixedLighting& lighting_;
    ColorButton* ambient_;
    ColorButton* diffuse_;
    //@}
};

}

#endif // __FixedLightWidget_h_
