// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __kernel_TacticalLine_ABC_h_
#define __kernel_TacticalLine_ABC_h_

#include "Entity_ABC.h"

namespace kernel
{

// =============================================================================
/** @class  TacticalLine_ABC
    @brief  TacticalLine_ABC
*/
// Created: SBO 2006-10-12
// =============================================================================
class TacticalLine_ABC : public Entity_ABC
{
public:
    //! @name Static
    //@{
    static const QString typeName_;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             TacticalLine_ABC();
    virtual ~TacticalLine_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool IsLimit() const = 0;
    virtual QString GetTypeName() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalLine_ABC( const TacticalLine_ABC& );            //!< Copy constructor
    TacticalLine_ABC& operator=( const TacticalLine_ABC& ); //!< Assignment operator
    //@}

};

}

#endif // __kernel_TacticalLine_ABC_h_
