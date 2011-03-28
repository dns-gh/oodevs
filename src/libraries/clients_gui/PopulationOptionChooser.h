// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __PopulationOptionChooser_h_
#define __PopulationOptionChooser_h_

#include "tools/Observer_ABC.h"

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  PopulationOptionChooser
    @brief  PopulationOptionChooser
*/
// Created: LDC 2011-03-23
// =============================================================================
class PopulationOptionChooser : public QDockWindow
                              , public tools::Observer_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             PopulationOptionChooser( QMainWindow* parent, kernel::Controllers& controllers );
    virtual ~PopulationOptionChooser();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PopulationOptionChooser( const PopulationOptionChooser& );            //!< Copy constructor
    PopulationOptionChooser& operator=( const PopulationOptionChooser& ); //!< Assignment operator
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOccupationToggled( bool mode );
    void OnDensityToggled( bool mode );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __PopulationOptionChooser_h_
