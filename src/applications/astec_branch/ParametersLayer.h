// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParametersLayer_h_
#define __ParametersLayer_h_

#include "graphics/MapLayer_ABC.h"

// =============================================================================
/** @class  ParametersLayer
    @brief  ParametersLayer
*/
// Created: AGE 2006-03-23
// =============================================================================
class ParametersLayer : public MapLayer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ParametersLayer();
    virtual ~ParametersLayer();
    //@}

    //! @name Operations
    //@{
    void Start( int type, unsigned expected );

    virtual void Initialize( const geometry::Rectangle2f& extent );
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParametersLayer( const ParametersLayer& );            //!< Copy constructor
    ParametersLayer& operator=( const ParametersLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMouseMove       ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );
    virtual bool HandleMouseDoubleClick( QMouseEvent* mouse, const geometry::Point2f& point );

    bool IsDone() const;
    void NotifyDone();
    //@}

private:
    //! @name Member data
    //@{
    bool focus_;
    geometry::Rectangle2f world_;
    int type_;
    unsigned expected_;
    T_PointVector points_;
    //@}
};

#endif // __ParametersLayer_h_
