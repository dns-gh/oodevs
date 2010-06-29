// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectBouchonMines.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectBouchonMines.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeObstacle_h_
#define __DEC_Knowledge_ObjectAttributeObstacle_h_

#include "DEC_Knowledge_ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace Common
{
    enum ObstacleType_DemolitionTargetType;
}

class ObstacleAttribute;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectAttributeObstacle : public DEC_Knowledge_ObjectAttribute_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectAttributeObstacle();
    explicit DEC_Knowledge_ObjectAttributeObstacle( const ObstacleAttribute& attr );
    virtual ~DEC_Knowledge_ObjectAttributeObstacle();
    //@}

    //! @name Update
    //@{
    virtual void UpdateOnPerceptionLevel( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateOnPerception( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateOnCollision( const DEC_Knowledge_ObjectCollision& collision );
    virtual void Send( Common::MsgObjectAttributes& asnMsg ) const;
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    void Register( DEC_Knowledge_Object& knObject );
    //@}

    //! @name Update
    //@{
    bool IsActivated() const;
    //@}

private:
    //! @name Update
    //@{
    void UpdateAttributes();
    //@}

private:
    //! @name Member data
    //@{
    const ObstacleAttribute* attr_;
    Common::ObstacleType_DemolitionTargetType obstacle_;
    bool            bActivated_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_ObjectAttributeObstacle )

#endif // __DEC_Knowledge_ObjectAttributeObstacle_h_
