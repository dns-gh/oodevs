// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Platform_h_
#define __Platform_h_

#include "dispatcher/Entity_ABC.h"
#include "game_asn/Simulation.h"

namespace kernel
{
    class ComponentType;
}

namespace tic
{

// =============================================================================
/** @class  Platform
    @brief  Platform
*/
// Created: AGE 2008-03-31
// =============================================================================
class Platform : public dispatcher::Entity_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Platform( const kernel::ComponentType& type );
    virtual ~Platform();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_MsgUnitAttributes& updateMessage );
    void Spread( ASN1T_EquipmentDotations& updateMessage );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Platform( const Platform& );            //!< Copy constructor
    Platform& operator=( const Platform& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
    enum E_State
    {
        okay,
        broken,
        destroyed
    };
    //@}

    //! @name Helpers
    //@{
    virtual void SendFullUpdate( dispatcher::ClientPublisher_ABC& ) const {};
    virtual void SendCreation  ( dispatcher::ClientPublisher_ABC& ) const {};
    void Apply( ASN1T_EquipmentDotations& updateMessage );
    bool SetStatus( int& number, E_State state );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::ComponentType& type_;
    E_State state_;

    unsigned int nSpeed_;
    unsigned int nDirection_;
    unsigned int nHeight_;
    //@}
};

}

#endif // __Platform_h_
