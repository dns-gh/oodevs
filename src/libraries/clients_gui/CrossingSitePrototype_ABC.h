// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CrossingSitePrototype_ABC_h_
#define __CrossingSitePrototype_ABC_h_

#include "ObjectPrototypeAttributes_ABC.h"

class QSpinBox;
class QCheckBox;

namespace gui
{

// =============================================================================
/** @class  CrossingSitePrototype_ABC
    @brief  CrossingSitePrototype_ABC
*/
// Created: SBO 2006-04-19
// =============================================================================
class CrossingSitePrototype_ABC : public ObjectPrototypeAttributes_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit CrossingSitePrototype_ABC( QWidget* parent );
    virtual ~CrossingSitePrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    virtual void Commit() = 0;
    virtual void Clean() = 0;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CrossingSitePrototype_ABC( const CrossingSitePrototype_ABC& );            //!< Copy constructor
    CrossingSitePrototype_ABC& operator=( const CrossingSitePrototype_ABC& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{
    QSpinBox* width_;
    QSpinBox* depth_;
    QSpinBox* speed_;
    QCheckBox* needsConstruction_;
    //@}
};

}

#endif // __CrossingSitePrototype_ABC_h_
