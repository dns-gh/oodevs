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

#include <boost/noncopyable.hpp>

namespace gui
{

// =============================================================================
/** @class  DecimalSpinBox
    @brief  DecimalSpinBox
*/
// Created: ABR 2011-01-28
// =============================================================================
class DecimalSpinBox : public QSpinBox
                     , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit DecimalSpinBox( QWidget* parent, double value = 0.f, unsigned short precision = 2, double min = 0.f, double max = 10.f, double gap = 0.1f );
    virtual ~DecimalSpinBox();
    //@}

public slots:
    //! @name Slots
    //@{
    virtual void setValue( double value );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual QString mapValueToText( int value );
    virtual int mapTextToValue( bool* ok );
    //@}

private:
    //! @name Member data
    //@{
    double precision_;
    //@}
};

} // namespace gui


#endif // __DecimalSpinBox_h_
