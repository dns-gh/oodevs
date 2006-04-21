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

class IDManager;

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
             ObjectType( unsigned long id, const std::string& name, IDManager& manager );
    virtual ~ObjectType();
    //@}

    //! @name Operations
    //@{
    const std::string& GetName() const;
    virtual void Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const;
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
    IDManager& manager_;

    unsigned long id_;
    std::string name_;
    const char** xpm_;
    //@}
};

#endif // __ObjectType_h_
