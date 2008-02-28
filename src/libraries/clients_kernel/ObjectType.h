// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectType_h_
#define __ObjectType_h_

#include "Drawable_ABC.h"

namespace xml { class xistream; };

namespace kernel
{
// =============================================================================
/** @class  ObjectType
    @brief  ObjectType
*/
// Created: AGE 2006-02-16
// =============================================================================
class ObjectType : public Drawable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectType( xml::xistream& xis, unsigned long id );
    virtual ~ObjectType();
    //@}

    //! @name Operations
    //@{
    std::string GetName() const;
    virtual void Draw( const geometry::Point2f& where, const Viewport_ABC& viewport, const GlTools_ABC& tools ) const;

    bool CanBeReservedObstacle() const;
    bool CanBeValorized() const;
    bool CanBeBypassed() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectType( const ObjectType& );            //!< Copy constructor
    ObjectType& operator=( const ObjectType& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    const char** TypeIcon();
    //@}

public:
    //! @name Static members
    //@{
    unsigned long id_;
    std::string name_;
    const char** xpm_;

    bool canBeReservedObstacle_;
    bool canBeValorized_;
    bool canBeBypassed_;
    //@}
};

}

#endif // __ObjectType_h_
