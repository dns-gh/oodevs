-- -------------------------------------------------------------------------------- 
-- agent body interface
-- --------------------------------------------------------------------------------
method "moveTo" ( 
    function( self, destination, urgency, pathType ) 
        return destination:reachIt( destination, pathType )
    end )

method "stopMovement" ( 
    function( self, destination ) 
        return integration.stopMoveToItCrowd( self )
    end )

method "getDecontaminated" ( 
    function( self, decontaminationPlot )
        return decontaminationPlot:useToGetDecontaminated()
    end )

method "sendReport" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, ... )
        self:RC( ... )
    end
} ) )

method "sendMessage" ( masalife.brain.integration.startStopAction( 
{ 
    start = function( self, message )
        integration.displayTrace( message )
    end
} ) )

-- -------------------------------------------------------------------------------- 
-- Specific methods of a crowd body
-- --------------------------------------------------------------------------------
method "isAgressive" ( 
    function( self )
        return integration.getAttitude() == eAttitudeAgressive
    end )

method "isUnderControl" ( 
    function( self )
        return integration.getMyDomination( self ) == 0
    end )

method "adoptAgressiveAttitude" (
    function( self )
       integration.setAttitude( eAttitudeAgressive )
    end )

method "isContaminated" (
    function( self )
        return integration.crowdIsContamined( self )
    end )

method "demonstrate" ( masalife.brain.integration.startStopAction(
{ 
    start = function( self )
        integration.startDemonstrate( self )
        --myself.domination.default = 1  -- $$$ MIA TODO?
        --meKnowledge:setDomination( 1 ) -- $$$ MIA TODO?
        return true
    end,
    started = function( self ) 
        integration.uptateDemonstrate( self )
        return true
    end,
    stop = function( self )
        integration.stopDemonstrate( self )
        return false
    end,
} ) )

method "canAttack" (
    function( self, unit )
        return true -- $$$ TODO Add the rule: if not self:isUnderControl() return true else return false end
    end )

method "attack" (
    function( self, unit )
        return unit:attackIt()
    end )

method "canDamage" (
    function( self, urbanBlock )
        return true -- $$$ TODO Add the rule: if not self:isUnderControl() return true else return false end
    end )

method "damage" (
    function( self, installation, quantityOfDamage )
        return installation:damageIt( quantityOfDamage )
    end )

method "getPosition" (
    function( self )
        return integration.getCrowdPosition( self )
    end )

method "getPositions" (
    function( self )
        return { integration.getCrowdPosition( self ) } -- $$$ TODO add positions around concentration?
   end )

method "RC" (
    function( self, ... )
        integration.crowdRC( ... )
    end )

return {}