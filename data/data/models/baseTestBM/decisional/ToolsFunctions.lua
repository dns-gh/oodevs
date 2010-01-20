--- Create a knowledge
-- @param strKnowledgeType The concret knowledge type to instanciate
-- @param knowledgeSource  The cpp source object  
-- @param knowledgedPosition The knowledge current position
-- @todo MGD Remove this ugly, find why scriptref error
function CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition )
  if( strKnowledgeType == "net.masagroup.sword.military.world.Point" ) then
    kn = kBase.create( net.masagroup.sword.military.world.Point, knowledgeSource, { ["sim_pos"] = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] } } )
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Area" ) then
    kn = kBase.create( net.masagroup.sword.military.world.Area, knowledgeSource, { ["sim_pos"] = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] } } )
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Object" ) then
    kn = kBase.create( net.masagroup.sword.military.world.Object, knowledgeSource, { ["sim_pos"] = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] } } )
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Section" ) then
    kn = kBase.create( net.masagroup.sword.military.world.Section, knowledgeSource, { ["sim_pos"] = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] } } )
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Automat" ) then
    kn = kBase.create( net.masagroup.sword.military.world.Automat, knowledgeSource, { ["sim_pos"] = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] } } )
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.Population" ) then
    kn = kBase.create( net.masagroup.sword.military.world.Population, knowledgeSource, { ["sim_pos"] = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] } } )
    return kn
  elseif( strKnowledgeType == "net.masagroup.sword.military.world.UrbanBlock" ) then
    kn = kBase.create( net.masagroup.sword.military.world.UrbanBlock, knowledgeSource, { ["sim_pos"] = { x=knowledgedPosition[1], y=knowledgedPosition[2], z=knowledgedPosition[3] } } )
    return kn
  else
    error( "Knowledge unknown : " .. strKnowledgeType )
  end
end

--- Fill mission parameters and create corresponding knowledges
-- @param params The table to fill 
-- @param strKnowledgeType  The concret knowledge type to instanciate for the parameter
-- @param name The parameter name
-- @param knowledgeSource  The cpp source objects  
-- @param knowledgedPosition The knowledge's current positions
-- @param list Indicate if the parameter is a list
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

function InitQueryReturn( params, strKnowledgeType, knowledgeSource, knowledgedPosition, list)
  if list then
    for i = 1, #knowledgeSource do
      params[i] = CreateKnowledge( strKnowledgeType, knowledgeSource[i], knowledgedPosition[i] )
    end
  else
    params[1] = CreateKnowledge( strKnowledgeType, knowledgeSource, knowledgedPosition )
  end
end

function InitializeMe( strKnowledgeType, knowledgeSource, groupName, posx, posy ,posz )
  if( strKnowledgeType == "net.masagroup.sword.military.world.Section" ) then
    if not kBase.me.body then
      kBase.me.body = kBase.create( net.masagroup.sword.military.world.Section, knowledgeSource, { sim_pos = { x=posx, y=posy, z=posz } } )
      behavior_model.createGroup( groupName )
    end
  else
    error( "Knowledge unknown : " .. strKnowledgeType )
  end
end

function InitializeLeaderMe( strKnowledgeType, knowledgeSource, groupName )
  if( strKnowledgeType == "net.masagroup.sword.military.world.Compagnie" ) then
    if not kBase.me.body then
      kBase.me.body = kBase.create( net.masagroup.sword.military.world.Compagnie, knowledgeSource )
      behavior_model.createGroup( groupName )  
    end
  else
    error( "Knowledge unknown : " .. strKnowledgeType )
  end
end

function CleanBrainBeforeDeletion( groupName )
  BreakForDebug( "ok delete :"..tostring( groupName ) )
  behavior_model.leaveGroup( groupName )
end  



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

print = function( ... )
  local res = ""
  for _,i in pairs({...}) do
    res = res..printRec( i )
  end
  DEC_Trace( res )
  DEC_Debug( res )
end