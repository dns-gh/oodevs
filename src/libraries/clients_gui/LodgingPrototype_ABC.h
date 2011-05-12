// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LodgingPrototype_ABC_h_
#define __LodgingPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

namespace gui
{
// =============================================================================
/** @class  LodgingPrototype_ABC
    @brief  LodgingPrototype_ABC
*/
// Created: MARTIN 2011-05-02
// =============================================================================
class LodgingPrototype_ABC : public ObjectAttributePrototype_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LodgingPrototype_ABC( QWidget* parent );
    virtual ~LodgingPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LodgingPrototype_ABC( const LodgingPrototype_ABC& );            //!< Copy constructor
    LodgingPrototype_ABC& operator=( const LodgingPrototype_ABC& ); //!< Assignment operator
    //@}

protected:
    //! @name Member data
    //@{
    QSpinBox* lodgingCapacity_;
    //@}
};
}

#endif // __LodgingPrototype_ABC_h_
