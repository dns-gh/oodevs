--- MASA Life Brain default getDestructionState
-- <br/>This file registers the getDestructionState related default implementations.
-- <br/>The destruction state of a knowledge can be modified by the simulation thanks to attribute "destructionLevel_"

masalife.brain.knowledge.addPerceptionCallBack( function( type, source, attributeList, lifeTime, ... )
       attributeList.destructionLevel_ = attributeList.destructionLevel_ or 0
       return attributeList
   end )
