// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __FireData_ABC_h_
#define __FireData_ABC_h_

// =============================================================================
/** @class   FireData_ABC
    @brief   FireData_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class PHY_ComposantePion;
class PHY_Weapon;

namespace firing
{

    class FireData_ABC : private boost::noncopyable
    {

    public:
        //! @name Constructors/Destructor
        //@{
                 FireData_ABC();
        virtual ~FireData_ABC();
        //@}

        //! @name Operation
        //@{
        virtual void operator()( const PHY_ComposantePion& firer, PHY_Weapon& weapon ) = 0;
        //@}
    };

} // namespace firing

#endif // __FireData_ABC_h_
