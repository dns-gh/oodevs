<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" />

	<xsl:template match="text()"/>
	
	<xsl:template match="Pathfind">
		<pathfind>
			<xsl:apply-templates/>
		</pathfind>
	</xsl:template>
	
	<xsl:template match="Config">
		<configuration distance-threshold="{DistanceThreshold}" max-calculation-time="{TempsMaxCalcul}"/>
	</xsl:template>
	
	<xsl:template match="Rules/AgentRules">
		<unit-rules>
			<xsl:apply-templates/>
		</unit-rules>
	</xsl:template>
		
	<xsl:template match="Rule">
		<rule type="{@type}" flying="{@flying}" autonomous="{@autonomous}">
			<xsl:if test="@inherits">
				<xsl:attribute name="inherits">
					<xsl:value-of select="@inherits"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:if test="GoStraight">
				<optimisation shortest="{GoStraight}"/>
			</xsl:if>
			<xsl:if test="AltitudePreference">
				<preferred-altitude value="{AltitudePreference}"/>
			</xsl:if>
			<xsl:apply-templates/>
		</rule>
	</xsl:template>

	<xsl:template match="PreferTerrain">
		<preferred-terrains>
			<xsl:if test="PreferTerrain/@strength">
				<xsl:attribute name="strength">
					<xsl:value-of select="@strength"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:for-each select="./Terrain">
				<preferred-terrain>
					<xsl:attribute name="type">
						<xsl:value-of select="@type"/>
					</xsl:attribute>
				</preferred-terrain>
			</xsl:for-each>
		</preferred-terrains>
	</xsl:template>
	
	<xsl:template match="AvoidTerrain">
		<avoided-terrains>
			<xsl:if test="@strength">
				<xsl:attribute name="strength">
					<xsl:value-of select="@strength"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:for-each select="./Terrain">
				<avoided-terrain>
					<xsl:attribute name="type">
						<xsl:value-of select="@type"/>
					</xsl:attribute>
				</avoided-terrain>
			</xsl:for-each>
		</avoided-terrains>
	</xsl:template>
					
	
	<xsl:template match="Fuseau">
		<zone>
			<xsl:for-each select="ToleratedDistanceOutside">
				<outter-tolerance with-automat="{WithAutomata}" without-automat="{WithoutAutomata}">
					<xsl:if test="@costPerMeter">
						<xsl:attribute name="cost-per-meter">
							<xsl:value-of select="@costPerMeter"/>
						</xsl:attribute>
					</xsl:if>
				</outter-tolerance>
			</xsl:for-each>
			<xsl:for-each select="ComfortDistanceInside">
				<inner-comfort cost-per-meter="{@costPerMeter}" distance="{.}"/>
			</xsl:for-each>
		</zone>
	</xsl:template>
	
	<xsl:template match="AutomataFuseau">
		<automat-zone>
			<xsl:for-each select="./ToleratedDistanceOutside">
				<outter-tolerance cost-per-meter="{@costPerMeter}" distance="{.}"/>
			</xsl:for-each>
		</automat-zone>
	</xsl:template>
	
	<xsl:template match="DangerDirection">
		<danger-direction base-cost-beyond="{BaseCostBeyond}" cost-per-meter="{CostPerMeterBeyond}"/>
	</xsl:template>
	
	<xsl:template match="Enemies">
		<enemies cost-on-contact="{CostOnContact}" cost-at-security-range="{CostAtSecurityRange}">
			<xsl:if test="/MaximumCost">
				<xsl:attribute name="maximum-cost">
					<xsl:value-of select="MaximumCost"/>
				</xsl:attribute>
			</xsl:if>
		</enemies>
	</xsl:template>
	
	<xsl:template match="Objects">
		<object-costs avoid="{@avoid}">
			<xsl:apply-templates/>
		</object-costs>
	</xsl:template>
	
	<xsl:template match="Costs/Cost">
		<object-cost type="{@type}" value="{.}"/>
	</xsl:template>
	
	<xsl:template match="Populations">
		<population-costs security-range="{SecurityRange}" maximum="{MaximumCost}">
			<xsl:if test="CostOutsideOfPopulation">
				<xsl:attribute name="outside-of-population">
					<xsl:value-of select="CostOutsideOfPopulation"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates/>
		</population-costs>
	</xsl:template>
	
	<xsl:template match="CostsOnContact/CostOnContact">
		<population-cost attitude="{@attitude}" contact-cost="{.}"/>
	</xsl:template>
	
	<xsl:template match="Rules/PopulationRules">
		<population-rules>
			<xsl:for-each select="Rule">
				<rule type="{@type}" cost-out-of-channeling="{CostOutsideOfChanneling}" channeling-range="{ChannelingRange}"/>
			</xsl:for-each>
		</population-rules>
	</xsl:template>

</xsl:transform>