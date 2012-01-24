//*****************************************************************************
//
// $Created: AGN 03-08-27 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Genie/DEC_Gen_Object.h $
// $Author: Nld $
// $Modtime: 26/04/04 11:20 $
// $Revision: 1 $
// $Workfile: DEC_Gen_Object.h $
//
//*****************************************************************************

#ifndef __DEC_Gen_Object_h_
#define __DEC_Gen_Object_h_

#include "MIL.h"
#include "simulation_terrain/TER_Localisation.h"

namespace sword
{
    class PlannedWork;
    enum ObstacleType_DemolitionTargetType;
}

class MIL_EntityManager_ABC;
class MIL_Automate;

// =============================================================================
/** @class  DEC_Gen_Object
    @brief  DEC Gen object
*/
// Created: AGN 03-08-27
// =============================================================================
class DEC_Gen_Object
{
public:
    //! @name Type
    //@{
// $$$$ _RC_ FDS 2010-01-22: Test de suppression à voir si possible ou non
    typedef sword::ObstacleType_DemolitionTargetType E_DemolitionTargetType;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Gen_Object();
             DEC_Gen_Object( const sword::PlannedWork& asn, const MIL_EntityManager_ABC& entityManager );
             DEC_Gen_Object( const sword::PlannedWork& asn, const MIL_EntityManager_ABC& entityManager, unsigned int identifier );
             DEC_Gen_Object( std::string type, boost::shared_ptr< TER_Localisation > location, bool preliminary );
             DEC_Gen_Object( const DEC_Gen_Object& rhs );
    virtual ~DEC_Gen_Object();
    //@}

    //! @name Accessors
    //@{
    const std::string& GetTypeName() const;
    unsigned int GetExternalIdentifier() const;
    const TER_Localisation& GetLocalisation() const;
    E_DemolitionTargetType GetObstacleType() const;
    double GetDensity() const;
    unsigned int GetMinesActivityTime() const;
    unsigned int GetActivationTime() const;
    const std::string& GetName() const;
    const MIL_Automate* GetTC2() const;
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
    E_DemolitionTargetType pObstacleType_;
    double rDensity_;
    unsigned int nMinesActivityTime_;
    unsigned int nActivationTime_;
    std::string name_;
    const MIL_Automate* pTC2_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Gen_Object )

#include "DEC_Gen_Object.inl"

#endif // __DEC_Gen_Object_h_
