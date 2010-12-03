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

namespace Common
{
    class MsgCrowdOrder;
}

namespace MsgsSimToClient
{
    class MsgPopulationCreation;
    class MsgPopulationUpdate;
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
class Inhabitant : public dispatcher::Inhabitant_ABC
                 , public kernel::Extension_ABC
                 , public kernel::Updatable_ABC< MsgsSimToClient::MsgPopulationUpdate >
{
public:
    //! @name Constructors/Destructor
    //@{
             Inhabitant( Model_ABC& model, const MsgsSimToClient::MsgPopulationCreation& msg );
    virtual ~Inhabitant();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgPopulationUpdate&                 msg );

    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const;
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const;
    virtual void Accept( kernel::ModelVisitor_ABC& visitor ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const kernel::GlTools_ABC& tools ) const ;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Inhabitant( const Inhabitant& );            //!< Copy constructor
    Inhabitant& operator=( const Inhabitant& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Model_ABC& model_;
    const unsigned long nType_;
    const std::string strName_;
    dispatcher::Team_ABC& side_;
    std::vector< int > urbanObjectId_;
    std::map< std::string, std::string > extensions_;

    unsigned long nNbrHealthyHumans_;
    unsigned long nNbrDeadHumans_;
    unsigned long nNbrWoundedHumans_;

    //@}
};

}

#endif // __Inhabitant_h_
