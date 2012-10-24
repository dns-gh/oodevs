-- --Neutralizable Implementation

eTirDirectNormal = 0
eTirDirectLibre = 1


local eActionTirDirect_Impossible = 0
local eActionTirDirect_EnemyDestroyed = 1
local eActionTirDirect_NoCapacity = 2
local eActionTirDirect_NoAmmo = 3
local eActionTirDirect_Running = 4
local eActionTirDirect_Finished = 5
local eActionTirDirect_TemporarilyBlocked = 6
local eActionTirDirect_None = 7


integration.neutralizationPriority  = function( target )
  return integration.reachPriority( target )
end

integration.startNeutralizingIt = function( target )
    target[myself] = target[myself] or {} 
    if not target[myself].actionNeutralize then
        target[myself].actionNeutralize = DEC_StartTirDirect( target.source, 100, eTirDirectNormal, -1 )
        actionCallbacks[ target[myself].actionNeutralize ] = function( arg ) target[myself].eTir = arg end
    end
end

integration.stopNeutralizingIt = function( target )
    target[myself] = target[myself] or {} 
    if target[myself].actionNeutralize then
        target[myself].actionNeutralize = DEC__StopAction( target[myself].actionNeutralize )
        target[myself].actionNeutralize = nil
        g_myEnemy = nil
    end
end

integration.isAgentNeutralized = function( )
    DEC_Agent_EstNeutralise()
end
