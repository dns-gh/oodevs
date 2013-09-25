// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Gen_Object_h_
#define __DEC_Gen_Object_h_

#include "simulation_terrain/TER_Localisation.h"
#include <boost/serialization/export.hpp>

namespace sword
{
    class PlannedWork;
}

class MIL_Automate;
class MIL_CheckPointInArchive;
class MIL_CheckPointOutArchive;
class MIL_EntityManager_ABC;
class ObjectTypeResolver_ABC;

// =============================================================================
/** @class  DEC_Gen_Object
    @brief  DEC Gen object
*/
// Created: AGN 03-08-27
// =============================================================================
class DEC_Gen_Object
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Gen_Object();
             DEC_Gen_Object( const sword::PlannedWork& asn, const MIL_EntityManager_ABC& entityManager,
                             const ObjectTypeResolver_ABC& resolver );
             DEC_Gen_Object( const sword::PlannedWork& asn, const MIL_EntityManager_ABC& entityManager, unsigned int identifier,
                             const ObjectTypeResolver_ABC& resolver );
             DEC_Gen_Object( std::string type, TER_Localisation* location, bool activated );
             DEC_Gen_Object( const DEC_Gen_Object& rhs );
    virtual ~DEC_Gen_Object();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetTypeName() const;
    unsigned int GetExternalIdentifier() const;
    const TER_Localisation& GetLocalisation() const;
    bool GetActivated() const;
    double GetDensity() const;
    unsigned int GetMinesActivityTime() const;
    unsigned int GetActivationTime() const;
    const std::string& GetName() const;
    const MIL_Automate* GetTC2() const;
    bool HasCapacity( const std::string& capacity ) const;
    int GetAltitudeModifier() const;
    int GetTimeLimit() const;
    int GetLodging() const;
    bool GetMining() const;
    //@}

    //! @name Operations
    //@{
    DEC_Gen_Object& operator=( const DEC_Gen_Object& rhs );
    void Serialize( sword::PlannedWork& asn ) const;
    //@}

    //! @name Serialization
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@

private:
    //! @name Member data
    //@{
    std::string type_;
    unsigned int identifier_;
    TER_Localisation localisation_;
    bool activated_;
    double rDensity_;
    unsigned int nMinesActivityTime_;
    unsigned int nActivationTime_;
    std::string name_;
    const MIL_Automate* pTC2_;
    int altitudeModifier_;
    int timeLimit_;
    int lodging_;
    bool mining_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Gen_Object )

#endif // __DEC_Gen_Object_h_
