--- MASA Life Brain default getOperationalCapacity
-- <br/>This file registers the getOperationalCapacity related default implementations.
-- <br/>The operational state of a knowledge can be modified by the simulation thanks to attribute "operationalLevel_".

masalife.brain.knowledge.addPerceptionCallBack( function( type, source, attributeList, lifeTime, ... )
       attributeList.operationalLevel_ = attributeList.operationalLevel_ or 100
       return attributeList
   end )
