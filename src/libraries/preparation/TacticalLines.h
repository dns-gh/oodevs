// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLines_h_
#define __TacticalLines_h_

#include "TacticalLine_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Extension_ABC.h"
#include <tools/Resolver.h>

// =============================================================================
/** @class  TacticalLines
    @brief  TacticalLines
*/
// Created: SBO 2006-11-07
// =============================================================================
class TacticalLines : public kernel::Extension_ABC
                    , public kernel::Serializable_ABC
                    , public tools::Resolver< TacticalLine_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLines();
    virtual ~TacticalLines();
    //@}

    //! @name Operations
    //@{
    void AddLine( TacticalLine_ABC& line );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLines( const TacticalLines& );            //!< Copy constructor
    TacticalLines& operator=( const TacticalLines& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}
};

#endif // __TacticalLines_h_
