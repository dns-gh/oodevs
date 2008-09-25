// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FederateFacade_h_
#define __FederateFacade_h_

namespace hla
{
    class TimeFactory_ABC;
    class TimeIntervalFactory_ABC;
    class RtiAmbassador_ABC;
    class Federate;
}

namespace plugins
{
namespace hla
{
    class ObjectClass_ABC;

// =============================================================================
/** @class  FederateFacade
    @brief  FederateFacade
*/
// Created: SBO 2008-02-18
// =============================================================================
class FederateFacade
{

public:
    //! @name Constructors/Destructor
    //@{
             FederateFacade( const std::string& name, unsigned int timeStepDuration );
    virtual ~FederateFacade();
    //@}

    //! @name Operations
    //@{
    void AddClass( ObjectClass_ABC& objectClass );
    bool Join( const std::string& name );

    void Step();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FederateFacade( const FederateFacade& );            //!< Copy constructor
    FederateFacade& operator=( const FederateFacade& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< ObjectClass_ABC* > T_Classes;
    //@}

private:
    //! @name Member data
    //@{
    bool joined_;
    std::auto_ptr< ::hla::TimeFactory_ABC >         timeFactory_;
    std::auto_ptr< ::hla::TimeIntervalFactory_ABC > intervalFactory_;
    std::auto_ptr< ::hla::RtiAmbassador_ABC >       ambassador_;
    std::auto_ptr< ::hla::Federate >                federate_;
    T_Classes classes_;
    //@}
};

}
}

#endif // __FederateFacade_h_
