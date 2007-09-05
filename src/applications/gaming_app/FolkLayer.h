// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FolkLayer_h_
#define __FolkLayer_h_

#include "clients_gui/FolkLayer.h"

class FolkModel;

// =============================================================================
/** @class  FolkLayer
    @brief  FolkLayer
*/
// Created: AGE 2007-09-04
// =============================================================================
class FolkLayer : public gui::FolkLayer
{

public:
    //! @name Constructors/Destructor
    //@{
             FolkLayer( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter, FolkModel& model );
    virtual ~FolkLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FolkLayer( const FolkLayer& );            //!< Copy constructor
    FolkLayer& operator=( const FolkLayer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    FolkModel& model_;
    //@}
};

#endif // __FolkLayer_h_
