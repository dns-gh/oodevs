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

namespace xml
{
    class xostream;
    class xistream;
}

namespace urban
{
    class TerrainObject_ABC;
}

class MIL_Army_ABC;
class MIL_InhabitantType;
class MIL_StructuralStateNotifier_ABC;

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
             MIL_Inhabitant( const MIL_InhabitantType& type, MIL_Army_ABC& army, const MT_Vector2D& point, int number, const std::string& name );
    virtual ~MIL_Inhabitant();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    const MIL_InhabitantType& GetType() const;
    virtual MIL_Army_ABC& GetArmy() const;
    //@}

    //! @name Network
    //@{
    void SendCreation() const;
    void SendFullState() const;
    void UpdateState();
    void UpdateNetwork();
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
    //@}

protected:
    //! @name Constructor
    //@{
    MIL_Inhabitant( const MIL_InhabitantType& type );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Inhabitant( const MIL_Inhabitant& );            //!< Copy constructor
    MIL_Inhabitant& operator=( const MIL_Inhabitant& ); //!< Assignment operator
    //@}

    //! @name Network
    //@{
    void SendDestruction() const;
    void ReadExtension( xml::xistream& xis );
    void ReadUrbanBlock( xml::xistream& xis, float& totalArea );
    //@}

    //! @name Helpers
    //@{
    void DistributeHumans( float area );
    void ComputeHealthSatisfaction();
    //@}

private:
    //! @name Types
    //@{
    typedef std::pair< const urban::TerrainObject_ABC*, unsigned int > T_UrbanBlock;
    typedef std::vector< T_UrbanBlock >                                T_UrbanBlocks;
    typedef T_UrbanBlocks::iterator                                   IT_UrbanBlocks;
    typedef T_UrbanBlocks::const_iterator                            CIT_UrbanBlocks;

    typedef std::map< std::string, std::string > T_Extensions;
    //@}

private:
    //! @name Member Data
    //@{
    const MIL_InhabitantType* pType_;
    const unsigned int nID_;
    MIL_Army_ABC* pArmy_;
    std::string text_;
    unsigned long nNbrHealthyHumans_;
    unsigned long nNbrDeadHumans_;
    unsigned long nNbrWoundedHumans_;
    float healthNeed_;
    float healthSatisfaction_;
    bool healthChanged_;
    T_UrbanBlocks urbanBlocks_;
    T_Extensions extensions_;
    //@}

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Inhabitant* inhabitant, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Inhabitant* inhabitant, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( MIL_Inhabitant )

#endif // __MIL_Inhabitant_h_
