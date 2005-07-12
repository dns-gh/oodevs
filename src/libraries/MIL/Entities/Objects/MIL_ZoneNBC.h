//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ZoneNBC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 5 $
// $Workfile: MIL_ZoneNBC.h $
//
//*****************************************************************************

#ifndef __MIL_ZoneNBC_h_
#define __MIL_ZoneNBC_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_NbcAgentType;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_ZoneNBC : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_ZoneNBC );

public:
             MIL_ZoneNBC();
    virtual ~MIL_ZoneNBC();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();

    virtual bool                      Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( uint nID, MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( uint nID, const ASN1T_MagicActionCreateObject& asnCreateObject );
    //@}

    //! @name Accessors
    //@{
    const MIL_NbcAgentType& GetNbcAgentType() const;
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Object& CreateKnowledge( const MIL_Army& teamKnowing );
    //@}

    //! @name HLA
    //@{
    virtual bool Initialize( const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    virtual void Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer );
    virtual void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

    //! @name Operations
    //@{
    void DecontaminateZone( const TER_Localisation& zone );
    //@}

private:
    //! @name Events
    //@{
    virtual void ProcessAgentInside  ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Agent_ABC& agent );
    //@}

    //! @name Network
    //@{
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
    //@}

    //! @name Tools
    //@{
    bool IsAgentInsideDecontaminatedZone( const MIL_Agent_ABC& agent ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< TER_Localisation* >       T_LocalisationVector;
    typedef T_LocalisationVector::const_iterator   CIT_LocalisationVector;
    //@}

private:
    const MIL_NbcAgentType* pNbcAgentType_;

    // Specific attributes
    ASN1T_AttrObjectZoneNBC asnAttributes_;
    T_LocalisationVector    decontaminatedZones_;
};

#include "MIL_ZoneNBC.inl"

#endif // __MIL_ZoneNBC_h_
