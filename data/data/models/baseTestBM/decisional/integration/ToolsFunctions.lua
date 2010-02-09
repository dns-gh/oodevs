---\ Knowledge management /---


--- Bind DIA4 event interface to DIA5 interface
rawset( getfenv(), "StartEvent", function( ... )
  behavior_model.startTask( ... )
end )

rawset( getfenv(), "StopEvents", function( ... )
  pcall( behavior_model.stopTask, select(1,...) )
end )

--- Create a knowledge
-- @param strKnowledgeType The concret knowledge type to instanciate
-- @param knowledgeSource  The cpp source object  
-- @param knowledgedPosition The knowledge current position
-- @author MGD
-- @release 2010-01-22
function CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition )
    return kBase.create( strKnowledgeType, knowledgeSource, { ["sim_pos"] = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] } } )
end

--- Create a knowledge
-- @TODO update name 
-- @author DDA
-- @release 2010-02-2
function CreateKnowledgeWithoutPos( strKnowledgeType, knowledgeSource )
    return kBase.create( strKnowledgeType, knowledgeSource )
end

--- Fill mission parameters and create corresponding knowledges
-- @param params The table to fill 
-- @param strKnowledgeType  The concret knowledge type to instanciate for the parameter
-- @param name The parameter name
-- @param knowledgeSource  The cpp source objects  
-- @param knowledgedPosition The knowledges' current positions
-- @param list Indicate if the parameter is a list
-- @author MGD
-- @release 2010-01-22
function InitTaskParameter( params, strKnowledgeType, name, knowledgeSource, knowledgedPosition, list )
  if list then
    params[name] = {};
    for i = 1, #knowledgeSource do
      params[name][i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i], knowledgedPosition[i] )
    end
  else
    params[name] = { CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition ) }
  end
end

--- Fill queries result and create corresponding knowledges
-- @param params The table to fill 
-- @param strKnowledgeType  The concret knowledge type to instanciate
-- @param knowledgeSource  The cpp source objects  
-- @param knowledgedPosition The knowledges' current positions
-- @param list Indicate if the parameter is a list
-- @author MGD
-- @release 2010-01-22
function InitQueryReturn( params, strKnowledgeType, knowledgeSource, knowledgedPosition, list)
  if list then
    for i = 1, #knowledgeSource do
      params[i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i], knowledgedPosition[i] )
    end
  else
    params[1] = CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition )
  end
end

--- Update Pion's data push to lua
-- @param strKnowledgeType The brain's role type
-- @param knowledgeSource  The cpp source object  
-- @param groupName The name of the leader  
-- @param posx, posy, posz The current brain position
-- @author MGD
-- @release 2010-01-22
function UpdateMe( strKnowledgeType, knowledgeSource, groupName, posx, posy ,posz )
  if not kBase.me.body then
    kBase.me.body = kBase.create( strKnowledgeType, knowledgeSource, { sim_pos = { x=posx, y=posy, z=posz } } )
    behavior_model.createGroup( groupName )
  else
    kBase.me.body.sim_pos = { x=posx, y=posy, z=posz }
  end
end

--- Update Automat's data push to lua
-- @param strKnowledgeType The brain's role type
-- @param knowledgeSource  The cpp source object  
-- @param groupName The name of the leader  itself
-- @author MGD
-- @release 2010-01-22
function UpdateLeaderMe( strKnowledgeType, knowledgeSource, groupName )
  if not kBase.me.body then
    kBase.me.body = kBase.create( strKnowledgeType, knowledgeSource )
    behavior_model.createGroup( groupName )
  end
end

--- Unregister brain from communication group before deletion
-- @param groupName The name of the leader
-- @author MGD
-- @release 2010-01-22
function CleanBrainBeforeDeletion( groupName )
  behavior_model.leaveGroup( groupName )
end  

---\ Callback management /---

actionCallbacks = {}
actionKnowledgeCallbacks = {}

CallbackAction = function( action, value )
  if actionCallbacks[ action ] then
      actionCallbacks[ action ]( value )
  end
end

KnowledgeCallbackAction = function( action, value )
  if actionKnowledgeCallbacks[ action ] then
      actionKnowledgeCallbacks[ action ]( value )
  end
end

RemoveAction = function( action )
  actionCallbacks[ action ] = nil
  actionKnowledgeCallbacks[ action ] = nil
end

---\Tools /---

--- Return a linear interpolation between to point
-- @param min The min value on Y axis
-- @param min The max value on Y axis
-- @param start The value on x where interpolation begin
-- @param stop The value on x where interpolation stop
-- @param upslop Indicate if the function inscrease or decrease
-- @param value The x value where we want f(x)
-- @author MGD
-- @release 2010-01-22
LinearInterpolation = function( min, max, start, stop, upslope, value )
  local res = 0;
  if( upslope ) then
    if( value < start ) then
      res = min
    elseif( value > stop ) then
      res = max
    else
      res = min + ( value - start ) * ( max - min ) / ( stop - start )
    end
  else
    if( value < start ) then
      res = max
    elseif( value > stop ) then
      res = min
    else
      res = max + ( value - start ) * ( min - max ) / ( stop - start )
    end
  end
  return res
end


--[[local taskListener = {}@TODO MGD 
taskListener.TaskStarted = function() BreakForDebug( "startTask" ) end
taskListener.StageChanged = function() BreakForDebug( "StageChanged" ) end
taskListener.TaskFinished = function() BreakForDebug( "stopTask" ) end
behavior_model.registerTaskListener( taskListener ) ]]--

--- Tools for print function which create a string for all var type 
-- @param var The variable to convert to string
-- @author MGD
-- @release 2010-01-22
-- @TODO manage cycle to replace lua system print
local printRec
printRec = function( var )
  if( type( var ) == "table" ) then
    local res = "table:"
    for key, value in pairs( var ) do
      res = res.."["..tostring( key )..","..printRec( value ).."]"
    end
    return res
  else
    return tostring( var )
  end   
end

--- Redirect print function to sword gaming and log
-- @param ... 
-- @author MGD
-- @release 2010-01-22
printGaming = function( ... )
  local res = ""
    for _,i in pairs( {...} ) do
      res = res..printRec( i )
    end
  
  DEC_Trace( res )
  DEC_Debug( res )
end