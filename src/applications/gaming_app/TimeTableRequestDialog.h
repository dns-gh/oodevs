// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __TimeTableRequestDialog_h_
#define __TimeTableRequestDialog_h_

#include <boost/noncopyable.hpp>

class Publisher_ABC;

// =============================================================================
/** @class  TimeTableRequestDialog
    @brief  TimeTableRequestDialog
*/
// Created: JSR 2011-07-26
// =============================================================================
class TimeTableRequestDialog : public QDialog
                             , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimeTableRequestDialog( QWidget* parent, Publisher_ABC& network, unsigned int maxTick );
    virtual ~TimeTableRequestDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& network_;
    QSpinBox* beginTick_;
    QSpinBox* endTick_;
    //@}
};

#endif // __TimeTableRequestDialog_h_
