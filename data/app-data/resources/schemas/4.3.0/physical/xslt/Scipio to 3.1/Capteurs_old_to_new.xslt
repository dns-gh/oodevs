<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	<xsl:template match="/">
	
		<sensors>
			<xsl:apply-templates/>
		</sensors>
	</xsl:template>
	
	<xsl:template match="SurveillanceALAT">
		<alat-monitoring-times>
			<xsl:for-each select="TempsSurveillance/*">
				<alat-monitoring-time terrain="{name()}" time="{node()}"/>
			</xsl:for-each>
		</alat-monitoring-times>
	</xsl:template>
	
	<xsl:template match="RadarCOBRA">
		<cobra-radar action-range="{Portee/RayonAction}"/>
	</xsl:template>
	
	<xsl:template match="Radars">
		<radars>
			<xsl:apply-templates/>
		</radars>
	</xsl:template>
	
	<xsl:template match="Radar">
		<radar type="{@type}" name="{@nom}" action-range="{Portee/RayonAction}">
			<xsl:if test="Portee/HauteurMinimale">
				<xsl:attribute name="min-height"><xsl:value-of select="Portee/HauteurMinimale"/></xsl:attribute>
			</xsl:if>
			<xsl:if test="Portee/HauteurMaximale">
				<xsl:attribute name="max-height"><xsl:value-of select="Portee/HauteurMaximale"/></xsl:attribute>
			</xsl:if>
			<xsl:apply-templates/>
		</radar>
	</xsl:template>
	
	<xsl:template match="ActivitePionsDetectables">
		<detectable-activities>
			<xsl:for-each select="*">
				<detectable-activity type="{name()}" value="true"/>
			</xsl:for-each>
		</detectable-activities>
	</xsl:template>
	
	<xsl:template match="DureesAcquisition">
		<acquisition-times>
			<xsl:for-each select="*[name()!='DureesSpecifiquesPionPC']">
				<acquisition-time base-time="{.}">
					<xsl:attribute name="level">
						<xsl:call-template name="GetAcquisitionLevel">
							<xsl:with-param name="input" select="substring-after(name(),'Duree')"/>
						</xsl:call-template>
					</xsl:attribute>
					<xsl:variable name="nodeName" select="name()"/>
					<xsl:for-each select="../DureesSpecifiquesPionPC/*">
						<xsl:if test="name() = $nodeName">
							<xsl:attribute name="command-post-time"><xsl:value-of select="."/></xsl:attribute>
						</xsl:if>
					</xsl:for-each>
				</acquisition-time>
			</xsl:for-each>
			<xsl:apply-templates/>
		</acquisition-times>
	</xsl:template>
	
	<xsl:template match="DureesSpecifiquesPionPC/*">
		<xsl:variable name="currentName" select="name()"/>
		<xsl:if test="not(../../*[name()=$currentName])">
			<acquisition-time command-post-time="{.}">
				<xsl:attribute name="level">
					<xsl:call-template name="GetAcquisitionLevel">
						<xsl:with-param name="input" select="substring-after(name(),'Duree')"/>
					</xsl:call-template>
				</xsl:attribute>
			</acquisition-time>
		</xsl:if>
	</xsl:template>
	
	<xsl:template name="GetAcquisitionLevel">
		<xsl:param name="input"/>
		<xsl:variable name="lowerCase" select="translate($input, 'ABCDEFGHIJKLMNOPQRSTUVWXYZ', 'abcdefghijklmnopqrstuvwxyz')"/>
		<xsl:choose>
			<xsl:when test="$lowerCase='reconnaissance'">recognition</xsl:when>
			<xsl:when test="$lowerCase='identification'">identification</xsl:when>
			<xsl:when test="$lowerCase='detection'">detection</xsl:when>
		</xsl:choose>
	</xsl:template>
	
	<xsl:template match="Senseurs">
		<sensors>
			<xsl:apply-templates/>
		</sensors>
	</xsl:template>
	
	<xsl:template match="Senseur">
		<sensor name="{@nom}">
			<xsl:apply-templates/>
		</sensor>
	</xsl:template>
	
	<xsl:template match="DetectionAgents">
		<unit-detection scanning="{Angle/@balayage}" angle="{Angle}">
			<xsl:apply-templates/>
		</unit-detection>
	</xsl:template>
	
	<xsl:template match="DistancesDeBase">
		<base-distances>
			<xsl:attribute name="close-range"><xsl:value-of select="../DistanceDeProximite"/></xsl:attribute>
			<xsl:if test="DI">
				<base-distance level="identification" distance="{DI}"/>
			</xsl:if>
			<xsl:if test="DR">
				<base-distance level="recognition" distance="{DR}"/>
			</xsl:if>
			<xsl:if test="DD">
				<base-distance level="detection" distance="{DD}"/>
			</xsl:if>
		</base-distances>
	</xsl:template>
	
	<xsl:template match="DetectionAgents/ModificateursDeDistance">
		<distance-modifiers>
			<population-modifier density="{PresencePopulation/@densitePopulation}" modifier="{PresencePopulation/@modificateur}"/>
			<xsl:apply-templates/>
		</distance-modifiers>
	</xsl:template>
	
	
	<xsl:template match="Objet/ModificateursDeDistance">
		<population-modifier density="{PresencePopulation/@densitePopulation}" modifier="{PresencePopulation/@modificateur}"/>
		<xsl:apply-templates/>
	</xsl:template>
	
	<xsl:template match="VolumesCible">
		<size-modifiers>
			<xsl:for-each select="./*">
				<distance-modifier>
					<xsl:call-template name="PutModifier"/>
				</distance-modifier>
			</xsl:for-each>
		</size-modifiers>
	</xsl:template>
	
	<xsl:template match="Precipitations">
		<precipitation-modifiers>
			<xsl:for-each select="./*">
				<distance-modifier>
					<xsl:call-template name="PutModifier"/>
				</distance-modifier>
			</xsl:for-each>
		</precipitation-modifiers>
	</xsl:template>
	
	<xsl:template match="Eclairements">
		<visibility-modifiers>
			<xsl:for-each select="./*">
				<distance-modifier>
					<xsl:call-template name="PutModifier"/>
				</distance-modifier>
			</xsl:for-each>
		</visibility-modifiers>
	</xsl:template>
	
	<xsl:template match="PosturesSource">
		<source-posture-modifiers>
			<xsl:for-each select="./*">
				<source-posture-modifier>
					<xsl:call-template name="PutModifier"/>
				</source-posture-modifier>
			</xsl:for-each>
		</source-posture-modifiers>
	</xsl:template>
	
	<xsl:template match="PosturesCible">
		<target-posture-modifiers>
			<xsl:for-each select="./*">
				<distance-modifier>
					<xsl:call-template name="PutModifier"/>
				</distance-modifier>
			</xsl:for-each>
		</target-posture-modifiers>
	</xsl:template>
	
	<xsl:template match="Environnements">
		<terrain-modifiers>
			<xsl:for-each select="./*">
				<distance-modifier>
					<xsl:call-template name="PutModifier"/>
				</distance-modifier>
			</xsl:for-each>
		</terrain-modifiers>
	</xsl:template>
	
	<xsl:template match="DetectionObjets">
		<object-detection><xsl:apply-templates/>
		</object-detection>
	</xsl:template>
	
	<xsl:template match="Objets/Objet">
		<object type="{@type}" detection-distance="{DD}">
			<xsl:apply-templates/>
		</object>
	</xsl:template>
	
	<xsl:template match="ModificateursDeDistance/PosturesSource">
		<source-posture-modifiers>
			<xsl:for-each select="./*">
				<distance-modifier>
					<xsl:call-template name="PutModifier"/>
				</distance-modifier>
			</xsl:for-each>
		</source-posture-modifiers>
	</xsl:template>
	
	<xsl:template name="PutModifier">
		<xsl:attribute name="type"><xsl:value-of select="name()"/></xsl:attribute>
		<xsl:attribute name="value"><xsl:value-of select="."/></xsl:attribute>
	</xsl:template>
	
</xsl:stylesheet>
