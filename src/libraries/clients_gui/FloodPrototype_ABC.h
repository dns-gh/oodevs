// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodPrototype_ABC_h_
#define __FloodPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui {

// =============================================================================
/** @class  FloodPrototype_ABC
    @brief  FloodPrototype_ABC
*/
// Created: JSR 2010-12-07
// =============================================================================
class FloodPrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit FloodPrototype_ABC( QWidget* parent );
    virtual ~FloodPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodPrototype_ABC( const FloodPrototype_ABC& );            //!< Copy constructor
    FloodPrototype_ABC& operator=( const FloodPrototype_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    QLineEdit* depth_;
    QLineEdit* refDist_;
    //@}
};

} // end namespace gui

#endif // __FloodPrototype_ABC_h_
