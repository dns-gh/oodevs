// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Inhabitant_h_
#define __MIL_Inhabitant_h_

#include "MIL.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Objects/MIL_StructuralStateNotifier_ABC.h"
#include <memory>

namespace sword
{
    class UnitMagicAction;
}

namespace xml
{
    class xostream;
    class xistream;
}

class MIL_AffinitiesMap;
class MIL_Army_ABC;
class MIL_DictionaryExtensions;
class MIL_Object_ABC;
class MIL_InhabitantType;
class MIL_StructuralStateNotifier_ABC;
class MIL_LivingArea;
class MIL_Schedule_ABC;
class MIL_Agent_ABC;
class MIL_InhabitantSatisfactions;
class TER_Localisation;

// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_Inhabitant : public MIL_Entity_ABC
                     , public MIL_StructuralStateNotifier_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             MIL_Inhabitant( xml::xistream& xis, const MIL_InhabitantType& type, MIL_Army_ABC& army );
    virtual ~MIL_Inhabitant();
    //@}

    //! @name Initialization
    //@{
    virtual void Finalize();
    //@}

    //! @name Accessors
    //@{
    virtual MIL_Army_ABC& GetArmy() const;
    float GetAffinity( unsigned long teamID ) const;
    virtual bool CanEmitReports() const;
    //@}

    //! @name Network
    //@{
    void SendCreation() const;
    void SendFullState() const;
    void UpdateState();
    void UpdateNetwork();
    void OnReceiveUnitMagicAction( const sword::UnitMagicAction& msg );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name From MIL_StructuralStateNotifier_ABC
    //@{
    virtual void NotifyStructuralStateChanged( float /*structuralState*/, const MIL_Object_ABC& /*object*/ ) {}
    virtual void NotifyFired();
    //@}

    //! @name Operations
    //@{
    void NotifyAlerted( const TER_Localisation& localisation );
    void NotifyUndoAlerted( const TER_Localisation& localisation );
    void NotifyConfined( const TER_Localisation& localisation );
    void NotifyUndoConfined( const TER_Localisation& localisation );
    void NotifyEvacuated( const TER_Localisation& localisation );
    void NotifyUndoEvacuated( const TER_Localisation& localisation );
    bool IsAlerted( const TER_Localisation& localisation ) const;
    bool IsConfined( const TER_Localisation& localisation ) const;
    bool IsEvacuated( const TER_Localisation& localisation ) const;
    void ReStartMotivation();
    //@}

protected:
    //! @name Constructor
    //@{
    MIL_Inhabitant( const MIL_InhabitantType& type );
    //@}

private:
    //! @name Magic actions
    //@{
    void OnReceiveMsgChangeHealthState( const sword::UnitMagicAction& msg );
    void OnReceiveRename( const sword::MissionParameters& parameters );
    //@}

    //! @name Helpers
    //@{
    void DestroyInhabitantMovingObject();
    void CreateInhabitantMovingObject();
    //@}

private:
    //! @name Member Data
    //@{
    const MIL_InhabitantType&                    type_;
    MIL_Army_ABC*                                pArmy_;
    unsigned long                                movingObjectId_;
    std::auto_ptr< MIL_LivingArea >              pLivingArea_;
    std::auto_ptr< MIL_Schedule_ABC >            pSchedule_;
    std::auto_ptr< MIL_InhabitantSatisfactions > pSatisfactions_;
    std::auto_ptr< MIL_AffinitiesMap >           pAffinities_;
    std::auto_ptr< MIL_DictionaryExtensions >    pExtensions_;
    std::string                                  text_;
    unsigned long                                nNbrHealthyHumans_;
    unsigned long                                nNbrDeadHumans_;
    unsigned long                                nNbrWoundedHumans_;
    bool                                         healthStateChanged_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Inhabitant* inhabitant, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Inhabitant* inhabitant, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( MIL_Inhabitant )

#endif // __MIL_Inhabitant_h_
