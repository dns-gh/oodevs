 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Inhabitant_h_
#define __Inhabitant_h_

#include "Inhabitant_ABC.h"

namespace sword
{
    class PopulationCreation;
    class PopulationUpdate;
}

namespace dispatcher
{
    class Model_ABC;
    class Team_ABC;

// =============================================================================
/** @class  Inhabitant
    @brief  Inhabitant
*/
// Created: SLG 2010-11-29
// =============================================================================
class Inhabitant : public Inhabitant_ABC
                 , public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< sword::PopulationUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Inhabitant( Model_ABC& model, const sword::PopulationCreation& msg );
    virtual ~Inhabitant();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::PopulationUpdate& msg );
    virtual void SendCreation( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Inhabitant( const Inhabitant& );            //!< Copy constructor
    Inhabitant& operator=( const Inhabitant& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    typedef std::map< unsigned int, unsigned int > T_UrbanBlocks;
    typedef std::map< std::string, std::string > T_Extensions;
    typedef std::vector< int > T_UrbanObjectId;
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    const unsigned long nType_;
    const std::string strName_;
    const std::string text_;
    Team_ABC& side_;
    unsigned long nNbrHealthyHumans_;
    unsigned long nNbrDeadHumans_;
    unsigned long nNbrWoundedHumans_;
    float healthSatisfaction_;
    T_UrbanObjectId urbanObjectId_;
    T_UrbanBlocks urbanBlocks_;
    T_Extensions extensions_;
    //@}
};

}

#endif // __Inhabitant_h_
