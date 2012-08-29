// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef plugins_hla_TacticalObjectEventListenerComposite_H_
#define plugins_hla_TacticalObjectEventListenerComposite_H_

#include "TacticalObjectEventListener_ABC.h"
#include <set>

namespace plugins
{
namespace hla
{

// =============================================================================
/** @class  TacticalObjectEventListenerComposite
    @brief  TacticalObjectEventListenerComposite
*/
// Created: AHC 2012-07-27
// =============================================================================
class TacticalObjectEventListenerComposite : public TacticalObjectEventListener_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    TacticalObjectEventListenerComposite();
    virtual ~TacticalObjectEventListenerComposite();
    //@}

    //! @name Registration
    //@{
    virtual void Register( TacticalObjectEventListener_ABC& listener );
    virtual void Unregister( TacticalObjectEventListener_ABC& listener );
    //@}

    //! @name Operations
    //@{
    virtual void SpatialChanged( const TacticalObjectEventListener_ABC::T_PositionVector& pos );
    //@}

private:
    typedef std::set< TacticalObjectEventListener_ABC* > T_Listeners;

private:
    T_Listeners listeners_;
};

}
}
#endif // plugins_hla_TacticalObjectEventListenerComposite_H_
