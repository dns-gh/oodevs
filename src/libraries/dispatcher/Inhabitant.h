 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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

    typedef std::map< std::string, float >              T_MotivationSatisfactions;
    typedef T_MotivationSatisfactions::const_iterator CIT_MotivationSatisfactions;

    typedef std::map< unsigned int, float >            T_ResourcesSatisfactions;
    typedef T_ResourcesSatisfactions::const_iterator CIT_ResourcesSatisfactions;

    typedef std::map< unsigned long, float > T_Affinities;
    typedef T_Affinities::const_iterator   CIT_Affinities;
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
    float safetySatisfaction_;
    float lodgingSatisfaction_;
    T_MotivationSatisfactions motivationSatisfactions_;
    T_ResourcesSatisfactions resourcesSatisfactions_;
    T_UrbanObjectId urbanObjectId_;
    T_UrbanBlocks urbanBlocks_;
    T_Extensions extensions_;
    T_Affinities affinities_;
    //@}
};

}

#endif // __Inhabitant_h_
