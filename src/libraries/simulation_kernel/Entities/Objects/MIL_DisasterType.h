// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef MIL_DisasterType_h
#define MIL_DisasterType_h

#include <map>
#include <boost/shared_array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>

namespace xml
{
    class xistream;
}

class PHY_NbcSuit;

// =============================================================================
/** @class  MIL_DisasterType
    @brief  MIL Disaster type
*/
// Created: LGY 2012-11-21
// =============================================================================
class MIL_DisasterType : private boost::noncopyable
{
public:
    //! @name Factory
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate();

    static boost::shared_ptr< const MIL_DisasterType > Find( const std::string& strName )
    {
        CIT_DisasterTypes it = disasterTypes_.find( strName );
        if( it == disasterTypes_.end() )
            throw std::runtime_error( "unknown disaster type '" + strName + "'" );
        return it->second;
    }
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
    //@}

    //! @name Constructors/Destructor
    //@{
             MIL_DisasterType( const std::string& strName, xml::xistream& xis );
    virtual ~MIL_DisasterType();
    //@}

private:
    //! @name Types
    //@{
    typedef boost::shared_ptr< const MIL_DisasterType > T_DisasterType;
    typedef std::map< std::string, T_DisasterType >     T_DisasterTypes;
    typedef T_DisasterTypes::const_iterator           CIT_DisasterTypes;

    typedef std::map< const PHY_NbcSuit*, double > T_Protections;

    typedef std::map< unsigned int, double >            T_Wounds;
    typedef boost::tuple< std::string, T_Wounds, bool > T_threshold;
    typedef std::map< double, T_threshold >             T_Attritions;
    //@}

private:
    //! @name Helpers
    //@{
    struct LoadingWrapper;
    static void ReadDisaster( xml::xistream& xis );
    void ReadProtection( xml::xistream& xis );
    void ReadThreshold( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    const std::string strName_;
    T_Protections protections_;
    T_Attritions attritions_;
    double toxicityExponent_;
    static T_DisasterTypes disasterTypes_;
    //@}
};

#endif // MIL_DisasterType_h
