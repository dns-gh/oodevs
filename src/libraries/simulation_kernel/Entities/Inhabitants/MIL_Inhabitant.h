// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Math�matiques Appliqu�es SA (MASA)
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
class MIL_InhabitantType;
class MIL_StructuralStateNotifier_ABC;
class MIL_LivingArea;
class MIL_Schedule_ABC;
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

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    virtual MIL_Army_ABC& GetArmy() const;
    //@}

    //! @name Network
    //@{
    void SendCreation() const;
    void SendFullState() const;
    void UpdateState();
    void UpdateNetwork();
    void OnReceiveInhabitantMagicAction( const sword::UnitMagicAction& msg );
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
    virtual void NotifyStructuralStateChanged( unsigned int structuralState, const MIL_Object_ABC& object );
    virtual void NotifyFired();
    //@}

    //! @name Operations
    //@{
    void NotifyAlerted( const TER_Localisation& localisation );
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
    void OnReceiveMsgChangeAlertedState( const sword::UnitMagicAction& msg );
    //@}

    //! @name Network
    //@{
    void SendDestruction() const;
    void ReadExtension( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, std::string > T_Extensions;
    typedef T_Extensions::const_iterator       CIT_Extensions;
    //@}

private:
    //! @name Member Data
    //@{
    const MIL_InhabitantType& type_;
    const unsigned int nID_;
    MIL_Army_ABC* pArmy_;
    std::auto_ptr< MIL_LivingArea > pLivingArea_;
    std::auto_ptr< MIL_Schedule_ABC > pSchedule_;
    std::auto_ptr< MIL_InhabitantSatisfactions > pSatisfactions_;
    std::auto_ptr< MIL_AffinitiesMap > pAffinities_;
    std::string text_;
    bool affinitiesChanged_;
    unsigned long nNbrHealthyHumans_;
    unsigned long nNbrDeadHumans_;
    unsigned long nNbrWoundedHumans_;
    bool healthStateChanged_;
    T_Extensions extensions_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Inhabitant* inhabitant, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Inhabitant* inhabitant, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( MIL_Inhabitant )

#endif // __MIL_Inhabitant_h_
