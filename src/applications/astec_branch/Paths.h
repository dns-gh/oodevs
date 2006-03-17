// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Paths_h_
#define __Paths_h_

#include "ASN_Types.h"
#include "Extension_ABC.h"
#include "Updatable_ABC.h"
#include "Drawable_ABC.h"

class CoordinateConverter;

// =============================================================================
/** @class  Paths
    @brief  Paths
*/
// Created: AGE 2006-02-13
// =============================================================================
class Paths : public Extension_ABC
            , public Updatable_ABC< ASN1T_MsgUnitPathFind >
            , public Updatable_ABC< ASN1T_MsgUnitAttributes >
            , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Paths( const CoordinateConverter& converter );
    virtual ~Paths();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Paths( const Paths& );            //!< Copy constructor
    Paths& operator=( const Paths& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgUnitPathFind& message );
    virtual void DoUpdate( const ASN1T_MsgUnitAttributes& message );
    //@}

private:
    //! @name Member data
    //@{
    const CoordinateConverter& converter_;
    T_PointVector plannedPath_;
    T_PointVector previousPath_;
    //@}
};

#endif // __Paths_h_
