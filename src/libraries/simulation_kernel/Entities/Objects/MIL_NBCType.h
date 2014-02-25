//*****************************************************************************
//
// $Created: JVT 03-01-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_NbcAgentType.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:29 $
// $Revision: 4 $
// $Workfile: MIL_NbcAgentType.h $
//
//*****************************************************************************

#ifndef __MIL_NBCType_h_
#define __MIL_NBCType_h_

namespace xml
{
    class xistream;
}

//*****************************************************************************
// Created: JVT 03-01-06
// Modified: JVT 2004-10-28
//*****************************************************************************
class MIL_NBCType : private boost::noncopyable
{
public:
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();
    static const MIL_NBCType* Find( const std::string& strName );
    static const MIL_NBCType* Find( unsigned int nID );
    //@}

private:
    //!@ Constructor and destructor
    //@{
     MIL_NBCType( const std::string& strName, xml::xistream& xis );
    ~MIL_NBCType();
    //@}

private:
    //! @name Helpers
    //@{
    static void ReadAgent( xml::xistream& xis );
    void ReadEffects( xml::xistream& xis );
    //@}

public:
    //§ @name Get
    //@{
    const std::string GetName() const;
    unsigned int GetID() const;
    bool IsContaminating() const;
    bool IsPoisoning() const;
    static unsigned int GetWidth();
    static unsigned int GetLength();
    static unsigned int GetPropagationAngle();
    static double GetConcentrationIncreaseRate();
    static double GetConcentrationDecreaseRate();
    //@}

    //! @name Operations
    //@{
    int ComputeAgentConcentrationEvolution( bool bHasASource, int concentration ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, const MIL_NBCType* > T_NBCTypeMap;
    typedef T_NBCTypeMap::const_iterator              CIT_NBCTypeMap;
    //@}

private:
    //! @name members Data
    //@{
    const std::string strName_;
    unsigned int nID_;
    bool bCanContaminate_;
    bool bCanPoison_;
    static T_NBCTypeMap nbcAgentTypes_;
    static unsigned int length_;
    static unsigned int width_;
    static unsigned int propagationAngle_;
    static double concentrationIncreaseRate_;
    static double concentrationDecreaseRate_;
    static double edgeEffects_;
    //@}
};

#endif // __MIL_NBCType_h_
