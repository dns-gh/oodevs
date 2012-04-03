// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireResultFactory_h_
#define __FireResultFactory_h_

namespace sword
{
    class UnitFireDamages;
    class CrowdFireDamages;
}

class AgentFireResult;
class PopulationFireResult;
class Model;
class Simulation;

// =============================================================================
/** @class  FireResultFactory
    @brief  FireResultFactory
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireResultFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             FireResultFactory( Model& model, const Simulation& simulation );
    virtual ~FireResultFactory();
    //@}

    //! @name Operations
    //@{
    virtual AgentFireResult*      CreateFireResult( const sword::UnitFireDamages& message );
    virtual PopulationFireResult* CreateFireResult( const sword::CrowdFireDamages& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireResultFactory( const FireResultFactory& );            //!< Copy constructor
    FireResultFactory& operator=( const FireResultFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    Model& model_;
    const Simulation& simulation_;
    //@}
};

#endif // __FireResultFactory_h_
