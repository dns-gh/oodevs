return
{
    canExtinguishIt = function()
        return true --bci todo
    end,
    
    extinguishIt = masalife.brain.integration.startStopAction(
    { 
        start = function( self )
            self.actionExtinguish = DEC_StartExtinguishObject( self.source )
            actionCallbacks[ self.actionExtinguish ] = function( arg ) self.actionExtinguishState = arg end
        end, 

        started = function( self )
            if self.actionExtinguishState == eActionObjetImpossible then
                DEC_Trace( "impossible works" )
                return false
            elseif self.actionExtinguishState == eActionObjetManqueDotation then
                DEC_Trace( "not enough dotation" )
                return false
            elseif self.actionExtinguishState == eActionObjetPasDeCapacite then
                DEC_Trace( "no capacity" ) 
                return false
            end
            return true
        end, 
        stop = function( self )
            if self.actionExtinguishState == eActionObjetTerminee then
                meKnowledge:RC( eRC_ObjectExtinguished )
            else
                DEC_Trace( "pause work" )
            end
            self.actionExtinguish = DEC__StopAction( self.actionExtinguish )
        end,
    } ),
    
    burningLevel = function( self )
        return DEC_ConnaissanceObjet_BurningLevel( self.source ) * 100
    end,
    
    areFlamesNearby = function( self )
        return self:proximityLevel() > 80
    end,
    
    predicate "isExtinguished"
    {
        method = function( self )
            return self:burningLevel() == 0
        end,
    },
    
    supply = masalife.brain.integration.startStopAction(
    { 
        start = function( self, resourceTypes )
            self.actionSupply = DEC_StartSupplyObject( self.source, resourceTypes, 1000 )--valeur arbitraire pour prendre une bonne quantité...
            actionCallbacks[ self.actionSupply ] = function( arg ) self.actionSupplyState = arg end
        end, 

        started = function( self )
            if self.actionSupplyState == eActionObjetImpossible then
                DEC_Trace( "impossible works" )
                return false
            elseif self.actionSupplyState == eActionObjetManqueDotation then
                DEC_Trace( "not enough dotation" )
                return false
            elseif self.actionSupplyState == eActionObjetPasDeCapacite then
                DEC_Trace( "no capacity" ) 
                return false
            end
            return true
        end, 
        stop = function( self )
            if self.actionSupplyState == eActionObjetTerminee then
                DEC_Trace( "supplied" )
            else
                DEC_Trace( "pause work" )
            end
            self.actionSupply = DEC__StopAction( self.actionSupply )
        end,
    } ),
    
    extract = masalife.brain.integration.startStopAction(
    { 
        start = function( self, resourceTypes )
            self.actionExtract = DEC_StartExtractFromStockObject( self.source, resourceTypes, 1000 )--valeur arbitraire pour prendre une bonne quantité...
            actionCallbacks[ self.actionExtract ] = function( arg ) self.actionExtractState = arg end
        end, 

        started = function( self )
            if self.actiobExtractState == eActionObjetImpossible then
                DEC_Trace( "impossible works" )
                return false
            elseif self.actiobExtractState == eActionObjetManqueDotation then
                DEC_Trace( "not enough dotation" )
                return false
            elseif self.actiobExtractState == eActionObjetPasDeCapacite then
                DEC_Trace( "no capacity" ) 
                return false
            end
            return true
        end, 
        stop = function( self )
            if self.actiobExtractState == eActionObjetTerminee then
                DEC_Trace( "extracted" )
            else
                DEC_Trace( "pause work" )
            end
            self.actionExtract = DEC__StopAction( self.actionExtract )
        end,
    } ),
    
    distribute = masalife.brain.integration.startStopAction(
    { 
        start = function( self )
            self.actionDistribute = DEC_StartDistributionObjet( self.source, 1000 )--valeur arbitraire pour prendre une bonne quantité...
            actionCallbacks[ self.actionDistribute ] = function( arg ) self.actionDistributeState = arg end
        end, 

        started = function( self )
            if self.actionDistributeState == eActionObjetImpossible then
                DEC_Trace( "impossible works" )
                return false
            elseif self.actionDistributeState == eActionObjetManqueDotation then
                DEC_Trace( "not enough dotation" )
                return false
            elseif self.actionDistributeState == eActionObjetPasDeCapacite then
                DEC_Trace( "no capacity" ) 
                return false
            end
            return true
        end, 
        stop = function( self )
            if self.actionDistributeState == eActionObjetTerminee then
                DEC_Trace( "distributed" )
            else
                DEC_Trace( "pause work" )
            end
            self.actionDistribute = DEC__StopAction( self.actionDistribute )
        end,
    } ),
    
    isExtractPossible = function( self, resourceTypes )
        return DEC_Stock_IsExtractPossible( self.source, resourceTypes )
    end,
    
    isSupplyPossible = function( self, resourceTypes )
        return DEC_Stock_IsSupplyPossible( self.source, resourceTypes )
    end,
    
    isDistributePossible = function( self, population )
        return DEC_Stock_IsDistributePossible( self.source, population )
    end,
}

