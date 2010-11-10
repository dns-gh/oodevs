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

#include "Entity_ABC.h"
#include <wisevec3.h>
#include <map>

namespace MsgsSimToClient
{
    class EquipmentDotations;
    class HumanDotations;
    class ResourceDotations;
    class MsgUnitAttributes;
    class MsgUnitCreation;
}

class Entity_ABC;
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
class Agent : public Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             Agent( const Model& model, const MsgsSimToClient::MsgUnitCreation& message );
    virtual ~Agent();
    //@}

    //! @name Accessors
    //@{
    virtual unsigned long GetId() const;
    virtual WISE_HANDLE GetHandle() const;
    //@}

    //! @name Operations
    //@{
    virtual void Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const;
    virtual void Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const;

    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const MsgsSimToClient::MsgUnitAttributes& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Agent( const Agent& );            //!< Copy constructor
    Agent& operator=( const Agent& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    std::wstring MakeIdentifier() const;
    template< class M, class C >
    void UpdateComponents( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const M& message, std::map< unsigned long, C* >& components, const std::wstring& fieldName );
    //@}

private:
    //! @name Member data
    //@{
    const unsigned long id_;
    const std::wstring name_;
    const unsigned long type_;
    const Entity_ABC* superior_;
    std::map< unsigned long, Equipment* > equipments_;
    std::map< unsigned long, Personnel* > personnel_;
    std::map< unsigned long, Resource* > resources_;
    mutable WISE_HANDLE handle_;
    mutable std::map< std::wstring, WISE_HANDLE > attributes_;
    CWISEVec3 position_;
    //@}
};

#endif // __Agent_h_
