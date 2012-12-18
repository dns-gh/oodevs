// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Units/Dotations/PHY_DotationType.h $
// $Author: Nld $
// $Modtime: 17/03/05 17:26 $
// $Revision: 8 $
// $Workfile: PHY_DotationType.h $
//
// *****************************************************************************

#ifndef __PHY_DotationType_h_
#define __PHY_DotationType_h_

#include "MT_Tools/MT_String.h"
#include "ENT/ENT_Enums_Gen.h"

namespace xml
{
    class xisubstream;
}

class PHY_DotationCategory;

// =============================================================================
// @class  PHY_DotationType
// Created: JVT 2004-08-03
// =============================================================================
class PHY_DotationType : private boost::noncopyable
{
public:
    //! @name Member data
    //@{
    static PHY_DotationType* munition_;
    static PHY_DotationType* carburant_;
    static PHY_DotationType* explosif_;
    static PHY_DotationType* mine_;
    static PHY_DotationType* barbele_;
    static PHY_DotationType* piece_;
    static PHY_DotationType* ration_;
    static PHY_DotationType* agentExtincteur_;
    static PHY_DotationType* energie_;
    static PHY_DotationType* funeraire_;
    //@}

public:
    //! @name Operations
    //@{
    static void Initialize( xml::xisubstream xis );
    static void Terminate ();

    static const PHY_DotationType*     FindDotationType    ( const std::string& name );
    static const PHY_DotationType*     FindDotationType    ( unsigned int id );
    static const PHY_DotationCategory* FindDotationCategory( const std::string& name );
    static const PHY_DotationCategory* FindDotationCategory( unsigned int id );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
          unsigned int GetID  () const;
    //@}
    
    //! @name Operators
    //@{
    bool operator==( const PHY_DotationType& rhs ) const;
    //@}

private:
    //! @name Constructors/Destructor
    //@{
    PHY_DotationType( const std::string& name, E_DotationFamily type );
    //@}

    //! @name Helpers
    //@{
    void RegisterDotation( xml::xistream& xis );

    static void ReadDotation( xml::xistream& xis );
    //@}

private:
    //! @name Memnber data
    //@{
    const std::string      name_;
    const E_DotationFamily type_;
    //@}
};

#endif // __PHY_DotationType_h_
