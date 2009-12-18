// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ComposantesAbleToBeFiredComputerFactory_ABC_h_
#define __ComposantesAbleToBeFiredComputerFactory_ABC_h_

namespace firing
{
    class ComposantesAbleToBeFiredComputer_ABC;
// =============================================================================
/** @class  ComposantesAbleToBeFiredComputerFactory_ABC
    @brief  ComposantesAbleToBeFiredComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class ComposantesAbleToBeFiredComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             ComposantesAbleToBeFiredComputerFactory_ABC() {}
    virtual ~ComposantesAbleToBeFiredComputerFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual std::auto_ptr< ComposantesAbleToBeFiredComputer_ABC > Create( bool bFireOnlyOnMajorComposantes = false ) const = 0;
    //@}
};

}

#endif // __ComposantesAbleToBeFiredComputerFactory_ABC_h_
