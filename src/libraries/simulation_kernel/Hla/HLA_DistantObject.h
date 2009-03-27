// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-11-30 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Hla/HLA_DistantObject.h $
// $Author: Age $
// $Modtime: 13/12/04 10:29 $
// $Revision: 4 $
// $Workfile: HLA_DistantObject.h $
//
// *****************************************************************************

#ifndef __HLA_DistantObject_h_
#define __HLA_DistantObject_h_

#include "HLA_Object_ABC.h"
#include "simulation_terrain/TER_Localisation.h"
#include <hla/ObjectIdentifier.h>

class MIL_Object_ABC;
class HLA_InteractionManager_ABC;

// =============================================================================
/** @class  HLA_DistantObject
    @brief  HLA distant object
*/
// Created: AGE 2004-11-30
// =============================================================================
class HLA_DistantObject : public HLA_Object_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             HLA_DistantObject( const hla::ObjectIdentifier& objectId, HLA_InteractionManager_ABC& interactionManager );
    virtual ~HLA_DistantObject();
    //@}

    //! @name Operations
    //@{
    virtual void Construct ( double rDeltaPercentage );
    virtual void Destroy   ( double rDeltaPercentage );
    virtual void Mine      ( double rDeltaPercentage );
    virtual void Demine    ( double rDeltaPercentage );
    virtual void Bypass    ( double rDeltaPercentage );
//    virtual void Prepare   ();
    virtual void Activate  ();

    virtual void Deserialize( const hla::AttributeIdentifier& attributeID, const hla::Deserializer& deserializer );
    virtual void Serialize  ( hla::UpdateFunctor_ABC& functor, bool bUpdateAll ) const;

    virtual void Destroy();

    virtual bool HideObject() const;
    //@}

    //! @name Accessors
    //@{
    virtual hla::ObjectIdentifier GetId() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    HLA_DistantObject( const HLA_DistantObject& );            //!< Copy constructor
    HLA_DistantObject& operator=( const HLA_DistantObject& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    MIL_Object_ABC* InstanciateObject();
    void DeserializeAttribute( const hla::AttributeIdentifier& attributeID, hla::Deserializer deserializer );
    //@}

private:
    //! @name Member data
    //@{
    std::string strObjectType_;
    std::string strArmy_;
    std::string strOption_;
    std::string strExtra_;
    TER_Localisation localisation_;
    MIL_Object_ABC* pObject_;
    double rConstructionPercentage_;
    double rMiningPercentage_;
    double rBypassPercentage_;

    hla::ObjectIdentifier id_;
    HLA_InteractionManager_ABC& interactionManager_;
    //@}
};

#endif // __HLA_DistantObject_h_
