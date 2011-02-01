// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __DecimalSpinBox_h_
#define __DecimalSpinBox_h_

namespace gui
{

// =============================================================================
/** @class  DecimalSpinBox
    @brief  DecimalSpinBox
*/
// Created: ABR 2011-01-28
// =============================================================================
class DecimalSpinBox : public QSpinBox
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit DecimalSpinBox( QWidget* parent, float value = 0.f, unsigned short precision = 2, float min = 0.f, float max = 10.f, float gap = 0.1f );
    virtual ~DecimalSpinBox();
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void setValue( float value );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual QString mapValueToText( int value );
    virtual int mapTextToValue( bool* ok );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecimalSpinBox( const DecimalSpinBox& );            //!< Copy constructor
    DecimalSpinBox& operator=( const DecimalSpinBox& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    float precision_;
    float value_;
    //@}
};    

} // namespace gui


#endif // __DecimalSpinBox_h_
