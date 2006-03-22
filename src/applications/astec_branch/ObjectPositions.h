// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectPositions_h_
#define __ObjectPositions_h_

#include "ASN_Types.h"
#include "Positions.h"
#include "Updatable_ABC.h"
#include "Drawable_ABC.h"

class CoordinateConverter;

// =============================================================================
/** @class  ObjectPositions
    @brief  ObjectPositions
*/
// Created: AGE 2006-03-22
// =============================================================================
class ObjectPositions : public Positions
                      , public Updatable_ABC< ASN1T_MsgObjectUpdate >
                      , public Updatable_ABC< ASN1T_MsgObjectCreation >
                      , public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectPositions( const CoordinateConverter& converter );
    virtual ~ObjectPositions();
    //@}

    //! @name Operations
    //@{
    virtual geometry::Point2f GetPosition() const;
    virtual void Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectPositions( const ObjectPositions& );            //!< Copy constructor
    ObjectPositions& operator=( const ObjectPositions& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgObjectUpdate& message );
    virtual void DoUpdate( const ASN1T_MsgObjectCreation& message );
    void Update( const ASN1T_Localisation& localisation );
    //@}

private:
    //! @name Member data
    //@{
    const CoordinateConverter& converter_;
    T_PointVector points_;
    geometry::Point2f center_;
    //@}
};

#endif // __ObjectPositions_h_
