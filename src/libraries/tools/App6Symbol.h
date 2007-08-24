// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __App6Symbol_h_
#define __App6Symbol_h_

namespace tools
{
    namespace app6
    {
        //! @name Operations
        //@{
        void Align( std::string& symbol );
        std::string FilterSymbol( const std::string& source, const std::string& filter );
        std::string MergeSymbol( const std::string& lhs, const std::string& rhs );
        void FilterPerceptionLevel( std::string& symbol, unsigned int level ); // $$$$ SBO 2007-08-24: ASN1T_EnumUnitIdentificationLevel
        //@}

        //! @name Field access
        //@{
        void SetAffiliation( std::string& symbol, unsigned int affiliation ); // $$$$ SBO 2007-08-23: 
        void SetBattleDimension( std::string& symbol, char dimension );
        void SetFunction( std::string& symbol, const std::string& function );
        void SetCommandPost( std::string& symbol, bool commandPost );
        void SetLevel( std::string& symbol, unsigned int level ); // $$$$ SBO 2007-08-23: 
        void SetLevel( std::string& symbol, const std::string& level ); // $$$$ SBO 2007-08-23: 

        unsigned int GetLevel( const std::string& symbol );
        std::string GetAffiliation( const std::string& symbol );
        //@}
    }
}

#endif // __App6Symbol_h_
