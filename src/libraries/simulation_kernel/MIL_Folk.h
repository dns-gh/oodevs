// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Folk_h_
#define __MIL_Folk_h_

#include "folk/population.h"

namespace population
{
    class FlowManager_ABC;
    class ObjectManager_ABC;
}

class MIL_Config;

// =============================================================================
/** @class  MIL_Folk
    @brief  MIL_Folk
*/
// Created: JCR 2007-08-24
// =============================================================================
class MIL_Folk
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit MIL_Folk( const MIL_Config& config );
    virtual ~MIL_Folk();
    //@}

    //! @name Modifiers
    //@{
    void Update( unsigned utime, unsigned timeStep );    
    void SendStateToNewClient() const;    
    //@}

    //! @name Accessors
    //@{
    population::ObjectManager_ABC* GetObjectManager() const;
    //@}

private:
    //! @name 
    //@{
    void UpdateNetwork() const;
    void SendUpdate() const;
    void SendCreation() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MIL_Folk( const MIL_Folk& );            //!< Copy constructor
    MIL_Folk& operator=( const MIL_Folk& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    boost::scoped_ptr< population::FlowManager_ABC >    pFlow_;
    mutable bool first_update_;
    mutable unsigned step_;
    unsigned start_;
    //@}
};

#endif // __MIL_Folk_h_
