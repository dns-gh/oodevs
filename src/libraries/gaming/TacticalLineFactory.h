// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalLineFactory_h_
#define __TacticalLineFactory_h_

namespace kernel
{
    class Controllers;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class Profile_ABC;
}

namespace sword
{
    class PhaseLineCreation;
    class LimitCreation;
}

class TacticalLine_ABC;
class Publisher_ABC;
class Model;

// =============================================================================
/** @class  TacticalLineFactory
    @brief  TacticalLineFactory
*/
// Created: SBO 2006-11-17
// =============================================================================
class TacticalLineFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             TacticalLineFactory( kernel::Controllers& controllers,
                                  const kernel::CoordinateConverter_ABC& converter,
                                  Model& model,
                                  Publisher_ABC& publisher,
                                  const kernel::Profile_ABC& profile );
    virtual ~TacticalLineFactory();
    //@}

    //! @name Operations
    //@{
    TacticalLine_ABC* Create( const sword::PhaseLineCreation&  message );
    TacticalLine_ABC* Create( const sword::LimitCreation& message );

    void CreateLimit( const T_PointVector& points, const kernel::Entity_ABC& superior );
    void CreateLima ( const T_PointVector& points, const kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalLineFactory( const TacticalLineFactory& );            //!< Copy constructor
    TacticalLineFactory& operator=( const TacticalLineFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::CoordinateConverter_ABC& converter_;
    Model& model_;
    Publisher_ABC& publisher_;
    const kernel::Profile_ABC& profile_;
    //@}
};

#endif // __TacticalLineFactory_h_
