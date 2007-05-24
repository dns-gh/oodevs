//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_Rota.h $
// $Author: Nld $
// $Modtime: 26/04/05 12:07 $
// $Revision: 6 $
// $Workfile: MIL_Rota.h $
//
//*****************************************************************************

#ifndef __MIL_Rota_h_
#define __MIL_Rota_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_NbcAgentType;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_Rota : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_Rota );

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_NbcAgentType* > T_NbcAgentSet;
    typedef T_NbcAgentSet::const_iterator       CIT_NbcAgentSet;
    //@}

public:
             MIL_Rota( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_Rota();
    virtual ~MIL_Rota();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );

    virtual bool                      Initialize( const MIL_ObstacleType& obstacleType, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( const ASN1T_MagicActionCreateObject& asnCreateObject );
    //@}

    //! @name Accessors
    //@{
          uint              GetDanger   () const;
    const T_NbcAgentSet&    GetNbcAgents() const;
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Object& CreateKnowledge( const MIL_Army& teamKnowing );
    //@}

    //! @name HLA
    //@{
    virtual bool Initialize( const TER_Localisation& localisation, const std::string& strOption, const std::string& strExtra, double rCompletion, double rMining, double rBypass );
    virtual void Deserialize( const AttributeIdentifier& attributeID, Deserializer deserializer );
    virtual void Serialize( HLA_UpdateFunctor& functor ) const;
    //@}

private:
    //! @name Network
    //@{
    virtual void WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const;
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
            void CleanSpecificAttributes();
    //@}

    //! @name Helpers
    //@{
    bool        ReadAgents ( const std::string& strAgents );
    std::string WriteAgents() const;
    //@}

private:
    T_NbcAgentSet nbcAgents_;
    uint          nDanger_;

    // Specific attributes
    ASN1T_AttrObjectROTA asnAttributes_;
};

#include "MIL_Rota.inl"

#endif // __MIL_Rota_h_
