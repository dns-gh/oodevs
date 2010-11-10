// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FireEngagement_h_
#define __FireEngagement_h_

#include <list>
#include <map>
#include <wiseinterfacetypes.h>

namespace MsgsSimToClient
{
    class MsgStartUnitFire;
    class MsgStopUnitFire;
    class MsgUnitEquipmentFireDamage;
    class MsgUnitFireDamages;
    class UnitHumanFireDamage;
}

class Agent;
class CWISEAttributeGroup;
class CWISEDriver;
class IWISEStringCache;
class Model;

// =============================================================================
/** @class  FireEngagement
    @brief  FireEngagement
*/
// Created: SEB 2010-10-14
// =============================================================================
class FireEngagement
{

public:
    //! @name Constructors/Destructor
    //@{
             FireEngagement( const Model& model, const MsgsSimToClient::MsgStartUnitFire& message );
    virtual ~FireEngagement();
    //@}

    //! @name Accessors
    //@{
    unsigned long GetId() const;
    //@}

    //! @name Operations
    //@{
    void Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const MsgsSimToClient::MsgStopUnitFire& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FireEngagement( const FireEngagement& );            //!< Copy constructor
    FireEngagement& operator=( const FireEngagement& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SendDamages( CWISEDriver& driver, const WISE_HANDLE& handle, IWISEStringCache& cache, std::map< std::wstring, WISE_HANDLE >& attributes, const MsgsSimToClient::MsgUnitFireDamages& message ) const;
    void SendEquipmentDamages( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache, const MsgsSimToClient::MsgUnitEquipmentFireDamage& message ) const;
    void SendPersonnelDamages( std::list< CWISEAttributeGroup >& list, IWISEStringCache& cache, const MsgsSimToClient::UnitHumanFireDamage& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    const Model& model_;
    const unsigned long id_;
    const Agent* shooter_;
    const Agent* target_;
    const unsigned long type_;
    const unsigned long ammunition_;
    //@}
};

#endif // __FireEngagement_h_
