// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ZoomSlider.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_ZoomSlider.h $
//
// *****************************************************************************

#ifndef __MOS_ZoomSlider_h_
#define __MOS_ZoomSlider_h_

#include <qslider.h>

// =============================================================================
/** @class  MOS_ZoomSlider
    @brief  MOS_ZoomSlider
    @par    Using example
    @code
    MOS_ZoomSlider;
    @endcode
*/
// Created: AGN 2004-04-20
// =============================================================================
class MOS_ZoomSlider
: public QSlider
{

public:
    //! @name Constructors/Destructor
    //@{
             MOS_ZoomSlider( QWidget* pParent );
    virtual ~MOS_ZoomSlider();
    //@}

    //! @name Operations
    //@{
    void HandleExternalWheelEvent( QWheelEvent* e );
    //@}

    //! @name Accessors
    //@{
    //@}

    //! @name Modifiers
    //@{
    //@}

    //! @name Operators
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_ZoomSlider( const MOS_ZoomSlider& );            //!< Copy constructor
    MOS_ZoomSlider& operator=( const MOS_ZoomSlider& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#ifdef MOS_USE_INLINE
#   include "MOS_ZoomSlider.inl"
#endif

#endif // __MOS_ZoomSlider_h_
