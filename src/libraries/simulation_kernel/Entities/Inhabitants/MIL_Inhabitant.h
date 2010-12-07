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

namespace xml
{
    class xostream;
    class xistream;
}

namespace urban
{
    class TerrainObject_ABC;
}

class MIL_Formation;
class MIL_Army_ABC;
class MIL_InhabitantType;


// =============================================================================
// Created: NLD 2005-09-28
// =============================================================================
class MIL_Inhabitant : public MIL_Entity_ABC
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
    void UpdateNetwork();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Visitor
    //@{
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
    void ReadUrbanBlock( xml::xistream& xis );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const urban::TerrainObject_ABC* >   T_livingUrbanBlockVector;
    typedef T_livingUrbanBlockVector::iterator              IT_livingUrbanBlockVector;
    typedef T_livingUrbanBlockVector::const_iterator       CIT_livingUrbanBlockVector;
    //@}

private:
    const MIL_InhabitantType* pType_;
    const unsigned int nID_;
    MIL_Army_ABC* pArmy_;
    unsigned long                 healthy_;
    unsigned long                 wounded_;
    unsigned long                 dead_;
    std::string                   text_;
    T_livingUrbanBlockVector      livingUrbanObject_;


    // Misc
    std::map< std::string, std::string > extensions_;

    template< typename Archive > friend  void save_construct_data( Archive& archive, const MIL_Inhabitant* inhabitant, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, MIL_Inhabitant* inhabitant, const unsigned int /*version*/ );
};

BOOST_CLASS_EXPORT_KEY( MIL_Inhabitant )

#endif // __MIL_Inhabitant_h_
