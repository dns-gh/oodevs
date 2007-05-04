// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MagicOrdersInterface_h_
#define __MagicOrdersInterface_h_

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace gui
{
    class ParametersLayer;
}

class Publisher_ABC;
class StaticModel;

// =============================================================================
/** @class  MagicOrdersInterface
    @brief  MagicOrdersInterface
*/
// Created: SBO 2007-05-04
// =============================================================================
class MagicOrdersInterface
{

public:
    //! @name Constructors/Destructor
    //@{
             MagicOrdersInterface( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const StaticModel& staticModel, gui::ParametersLayer& layer, const kernel::Profile_ABC& profile );
    virtual ~MagicOrdersInterface();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MagicOrdersInterface( const MagicOrdersInterface& );            //!< Copy constructor
    MagicOrdersInterface& operator=( const MagicOrdersInterface& ); //!< Assignment operator
    //@}
};

#endif // __MagicOrdersInterface_h_
