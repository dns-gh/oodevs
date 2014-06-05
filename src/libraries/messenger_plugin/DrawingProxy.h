// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DrawingProxy_h_
#define __DrawingProxy_h_

#include "dispatcher/Position.h"

namespace plugins
{
namespace messenger
{
    class Drawing;
    class DrawingsModel;

// =============================================================================
/** @class  DrawingProxy
    @brief  DrawingProxy
*/
// Created: AGE 2008-07-09
// =============================================================================
class DrawingProxy
{
public:
    //! @name Constructors/Destructor
    //@{
             DrawingProxy( DrawingsModel& model, std::unique_ptr< Drawing > drawing );
    virtual ~DrawingProxy();
    //@}

    //! @name Operations
    //@{
    dispatcher::Zone GetCoordinates();
    void Publish();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DrawingProxy( const DrawingProxy& );            //!< Copy constructor
    DrawingProxy& operator=( const DrawingProxy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    DrawingsModel& model_;
    std::unique_ptr< Drawing > drawing_;
    //@}
};

}
}

#endif // __DrawingProxy_h_
