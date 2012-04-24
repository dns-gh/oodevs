--- MASA Life Brain default getIdentificationState
-- <br/>This file registers the getIdentificationState related default implementations.
-- <br/>The identification state of a knowledge can be modified by the simulation thanks to attribute "identificationLevel_".

masalife.brain.knowledge.addPerceptionCallBack( function( type, source, attributeList, lifeTime, ... )
       attributeList.identificationLevel_ = attributeList.identificationLevel_ or 100
       return attributeList
   end )
