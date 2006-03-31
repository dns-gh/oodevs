// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MissionLayer_h_
#define __MissionLayer_h_

#include "Layer_ABC.h"

class MissionPanel;
// =============================================================================
/** @class  MissionLayer
    @brief  MissionLayer
*/
// Created: AGE 2006-03-31
// =============================================================================
class MissionLayer : public Layer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit MissionLayer( MissionPanel& panel );
    virtual ~MissionLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MissionLayer( const MissionLayer& );            //!< Copy constructor
    MissionLayer& operator=( const MissionLayer& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    MissionPanel& panel_;
    //@}
};

#endif // __MissionLayer_h_
