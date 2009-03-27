// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivityTimePrototype_ABC_h_
#define __ActivityTimePrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"

class QSpinBox;

namespace gui
{
    
// =============================================================================
/** @class  PropagationPrototype_ABC
    @brief  PropagationPrototype_ABC
*/
// Created: SBO 2006-04-20
// =============================================================================
class ActivityTimePrototype_ABC : public ObjectAttributePrototype_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActivityTimePrototype_ABC( QWidget* parent );
    virtual ~ActivityTimePrototype_ABC();
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
    ActivityTimePrototype_ABC( const ActivityTimePrototype_ABC& );            //!< Copy constructor
    ActivityTimePrototype_ABC& operator=( const ActivityTimePrototype_ABC& ); //!< Assignement operator
    //@}

protected:
    //! @name Member data
    //@{    
    QTimeEdit*  activityTime_;
    //@}
};

}

#endif // __ActivityTimePrototype_ABC_h_
