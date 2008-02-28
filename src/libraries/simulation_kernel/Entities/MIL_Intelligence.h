// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MIL_Intelligence_h_
#define __MIL_Intelligence_h_

#include "game_asn/Simulation.h"
#include "Tools/MIL_MOSIDManager.h"

namespace xml
{
    class xistream;
    class xostream;
}

class MIL_Formation;

// =============================================================================
/** @class  MIL_Intelligence
    @brief  MIL_Intelligence
*/
// Created: SBO 2007-10-22
// =============================================================================
class MIL_Intelligence
{

public:
    //! @name Constructors/Destructor
    //@{
             MIL_Intelligence();
             MIL_Intelligence( xml::xistream& xis, MIL_Formation& formation );
             MIL_Intelligence( const ASN1T_MsgIntelligenceCreationRequest& message, MIL_Formation& formation );
    virtual ~MIL_Intelligence();
    //@}

    //! @name Accessors
    //@{
    unsigned int GetId() const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    void WriteODB( xml::xostream& xos ) const;
    //@}

    //! @name Network
    //@{
    void SendCreation() const;
    void SendFullState() const;
    void SendDestruction() const;
    void Update( const ASN1T_MsgIntelligenceCreationRequest&    message );
    void Update( const ASN1T_MsgIntelligenceUpdateRequest&      message );
    void Update( const ASN1T_MsgIntelligenceDestructionRequest& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Intelligence( const MIL_Intelligence& );            //!< Copy constructor
    MIL_Intelligence& operator=( const MIL_Intelligence& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void SendUpdate();
    //@}

private:
    //! @name Member data
    //@{
    MIL_Formation* formation_;
    unsigned int id_;
    std::string name_;
    std::string nature_;
    bool embarked_;
    ASN1T_EnumNatureLevel level_;
    ASN1T_EnumDiplomacy diplomacy_;
    MT_Vector2D position_;
    //@}

private:
    //! @name Static member data
    //@{
    static MIL_MOSIDManager idManager_;
    //@}
};

#endif // __MIL_Intelligence_h_
