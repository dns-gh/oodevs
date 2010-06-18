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
#include "Entities/Objects/ObstacleAttribute.h"
#include "simulation_terrain/TER_Localisation.h"

namespace Common
{
    class MsgPlannedWork;
    enum ObstacleType_DemolitionTargetType;
}

class DEC_AutomateDecision;
class MIL_ObjectType_ABC;
class MIL_EntityManager_ABC;

//*****************************************************************************
// Created: AGN 03-08-27
//*****************************************************************************
class DEC_Gen_Object
{
public:
    //! @name Type
    //@{
// $$$$ _RC_ FDS 2010-01-22: Test de suppression à voir si possible ou non
    typedef Common::ObstacleType_DemolitionTargetType E_DemolitionTargetType;
    //@}

public:
             DEC_Gen_Object( const Common::MsgPlannedWork& asn, const MIL_EntityManager_ABC& entityManager );
             DEC_Gen_Object( std::string type, boost::shared_ptr< TER_Localisation > location, bool preliminary );
             DEC_Gen_Object( const DEC_Gen_Object& rhs );
    virtual ~DEC_Gen_Object();

    //! @name Accessors
    //@{
    const std::string&        GetTypeName         () const;
    const TER_Localisation&   GetLocalisation     () const;
    E_DemolitionTargetType    GetObstacleType     () const;
          MT_Float            GetDensity          () const;
          unsigned int        GetMinesActivityTime() const;
    const MIL_Automate*       GetTC2              () const;
    //@}

    //! @name Operations
    //@{
    DEC_Gen_Object& operator=( const DEC_Gen_Object& rhs );
    void Serialize( Common::MsgPlannedWork& asn ) const;
    //@}

private:
    //! @name Data members
    //@{
          std::string               type_;
          TER_Localisation          localisation_;
          E_DemolitionTargetType    pObstacleType_;
          MT_Float                  rDensity_;
          unsigned int              nMinesActivityTime_;
    const MIL_Automate*             pTC2_;
    //@}
};

#include "DEC_Gen_Object.inl"

#endif // __DEC_Gen_Object_h_
