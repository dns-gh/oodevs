// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Categories/PHY_Volume.h $
// $Author: Jvt $
// $Modtime: 14/04/05 17:41 $
// $Revision: 3 $
// $Workfile: PHY_Volume.h $
//
// *****************************************************************************

#ifndef __PHY_Volume_h_
#define __PHY_Volume_h_

#include <map>

namespace xml
{
    class xistream;
}

// =============================================================================
// @class  PHY_Volume
// Created: JVT 2004-08-03
// =============================================================================
class PHY_Volume : private boost::noncopyable
{
public:
    //! @name Types
    //@{
    typedef std::map< std::string, const PHY_Volume* > T_VolumeMap;
    typedef T_VolumeMap::const_iterator              CIT_VolumeMap;
    //@}

public:
    //! @name Manager
    //@{
    static void Initialize( xml::xistream& xis );
    static void Terminate ();

    static const T_VolumeMap& GetVolumes();
    static const PHY_Volume* FindVolume( const std::string& strName );
    static const PHY_Volume* FindVolume( unsigned int nID );
    //@}

    //! @name Accessors
    //@{
    const std::string& GetName() const;
          unsigned int         GetID  () const;
    //@}

private:
     PHY_Volume( const std::string& strName );
    virtual ~PHY_Volume();

private:
    const std::string strName_;
          unsigned int        nID_;

private:
    static T_VolumeMap volumes_;
    static unsigned int        nNextID_;

private:
    static void ReadVolume( xml::xistream& xis );
};

namespace boost
{
namespace archive
{
    template< class Archive >
    void save( Archive& ar, const PHY_Volume* t )
    {
        unsigned int id = t ? t->GetID() : std::numeric_limits< unsigned int >::max();
        ar << id;
    }
    template< class Archive >
    void load( Archive& ar, const PHY_Volume*& t )
    {
        unsigned int id;
        ar >> id;
        t = PHY_Volume::FindVolume( id );
    }
}
}

#endif // __PHY_Volume_h_
