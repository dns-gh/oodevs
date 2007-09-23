// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectListener_h_
#define __ObjectListener_h_

#include "ESRI.h"
#include "Listener_ABC.h"
#include "game_asn/Asn.h"

namespace dispatcher
{
    class SimulationPublisher_ABC;
}

namespace crossbow
{
    class Connector;

// =============================================================================
/** @class  ObjectListener
    @brief  ObjectListener
*/
// Created: SBO 2007-09-23
// =============================================================================
class ObjectListener : public Listener_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectListener( Connector& connector, dispatcher::SimulationPublisher_ABC& publisher );
    virtual ~ObjectListener();
    //@}

    //! @name Operations
    //@{
    virtual void Listen();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectListener( const ObjectListener& );            //!< Copy constructor
    ObjectListener& operator=( const ObjectListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SendCreation( const IFeaturePtr row );
    void CleanPending();
    void SerializeLocation( ASN1T_Location& asn, IGeometryPtr geometry ) const;
    //@}

private:
    //! @name Member data
    //@{
    dispatcher::SimulationPublisher_ABC& publisher_;
    IFeatureClassPtr featureClass_;
    ITablePtr table_;
    IQueryFilterPtr pendingCreationFilter_;
    IFeatureCursorPtr cursor_;
    //@}
};

}

#endif // __ObjectListener_h_
