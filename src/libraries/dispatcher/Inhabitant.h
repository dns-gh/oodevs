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
#include <tools/Resolver.h>

namespace sword
{
    class PopulationCreation;
    class PopulationUpdate;
}

namespace kernel
{
    class InhabitantType;
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
             Inhabitant( Model_ABC& model, const sword::PopulationCreation& msg,
                         const tools::Resolver_ABC< kernel::InhabitantType >& types );
    virtual ~Inhabitant();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::InhabitantType& GetType() const;

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
    struct T_Block
    {
        std::map< std::string, unsigned int > persons_;
        bool alerted_;
        bool confined_;
        bool evacuated_;
        float angriness_;
    };
    typedef std::map< unsigned int, T_Block > T_UrbanBlocks;
    typedef std::map< std::string, std::string > T_Extensions;
    typedef std::vector< int > T_UrbanObjectId;

    typedef std::map< std::string, float >  T_MotivationSatisfactions;
    typedef std::map< unsigned int, float > T_ResourcesSatisfactions;

    typedef std::map< unsigned long, float > T_Affinities;
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    const kernel::InhabitantType& type_;
    const unsigned long nType_;
    const std::string text_;
    std::string motivation_;
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
