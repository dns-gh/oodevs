// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Agent_h_
#define __Agent_h_

#include "WiseEntity.h"
#include <wise/wisevec3.h>

namespace sword
{
    class EquipmentDotations;
    class HumanDotations;
    class ResourceDotations;
    class UnitAttributes;
    class UnitCreation;
}

class Equipment;
class Model;
class Personnel;
class Resource;

// =============================================================================
/** @class  Agent
    @brief  Agent
*/
// Created: SEB 2010-10-13
// =============================================================================
class Agent : public WiseEntity
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const Model& model, const sword::UnitCreation& message );
    virtual ~Agent();
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::UnitAttributes& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual std::wstring MakeIdentifier() const;
    template< class M, class C >
    void UpdateComponents( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const M& message, std::map< unsigned long, C* >& components, const std::wstring& fieldName );
    //@}

private:
    //! @name Member data
    //@{
    const std::wstring name_;
    const unsigned long type_;
    const Entity_ABC* superior_;
    std::map< unsigned long, Equipment* > equipments_;
    std::map< unsigned long, Personnel* > personnel_;
    std::map< unsigned long, Resource* > resources_;
    CWISEVec3 position_;
    //@}
};

#endif // __Agent_h_
