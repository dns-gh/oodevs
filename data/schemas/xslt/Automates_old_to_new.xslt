<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes" />

	<xsl:template match="text()"/>
	
	<xsl:template match="Automates">
		<automats>
			<xsl:apply-templates/>
		</automats>
	</xsl:template>
	
	<xsl:template match="Unite">
		<automat name="{@nom}" type="{@type}" id="{MosID}" decisional-model="{Automate/ModeleDecisionnel}">
			<xsl:if test="RapportDeForce/TempsDeRemontee">
				<xsl:attribute name="force-ratio-feedback-time">
					<xsl:value-of select="RapportDeForce/TempsDeRemontee"/>
				</xsl:attribute>
			</xsl:if>
			<xsl:if test="PionPC"><!-- invalid if not found !!! -->
				<xsl:call-template name="t_automat">
					<xsl:with-param name="pc_type" select="PionPC/@type"/>
				</xsl:call-template>
			</xsl:if>
		</automat>
	</xsl:template>
	
	<xsl:template name="t_automat">
		<xsl:param name="pc_type"/>
		
		<xsl:if test="count(Automate/Constitution/Pion[@nom=$pc_type]) = 0">
			<unit type="{$pc_type}" command-post="true" min-occurs="1" max-occurs="1"/>
		</xsl:if>
		
		<xsl:for-each select="Automate/Constitution/Pion">
			<unit type="{@nom}">
				<xsl:variable name="minOccurs" >
					<xsl:choose>
						<xsl:when test="$pc_type=@nom">1</xsl:when>
						<xsl:otherwise>0</xsl:otherwise>
					</xsl:choose>
				</xsl:variable>
			
				<xsl:if test="$pc_type = @nom">
					<xsl:attribute name="command-post">true</xsl:attribute>
				</xsl:if>
				<xsl:choose>
					<xsl:when test="contains(node(), '..')">
						<xsl:variable name="currentMinimum">
							<xsl:call-template name="max">
								<xsl:with-param name="first" select="substring-before(.,'..')"/>
								<xsl:with-param name="second" select="$minOccurs"/>
							</xsl:call-template>
						</xsl:variable>
						<xsl:attribute name="min-occurs">
							<xsl:value-of select="$currentMinimum"/>
						</xsl:attribute>
						<xsl:attribute name="max-occurs">
							<xsl:call-template name="max">
								<xsl:with-param name="first" select="$currentMinimum"/>
								<xsl:with-param name="second" select="substring-after(.,'..')"/>
							</xsl:call-template>
						</xsl:attribute>
					</xsl:when>
					<xsl:when test="node()='*'">
						<xsl:attribute name="min-occurs">
							<xsl:value-of select="$minOccurs"/>
						</xsl:attribute>
					</xsl:when>
					<xsl:when test="node()='+'">
						<xsl:attribute name="min-occurs">1</xsl:attribute>
					</xsl:when>
					<xsl:otherwise>
						<xsl:attribute name="min-occurs">
							<xsl:call-template name="max">
								<xsl:with-param name="first" select="node()"/>
								<xsl:with-param name="second" select="$minOccurs"/>
							</xsl:call-template>
						</xsl:attribute>
						<xsl:attribute name="max-occurs">
							<xsl:call-template name="max">
								<xsl:with-param name="first" select="node()"/>
								<xsl:with-param name="second" select="$minOccurs"/>
							</xsl:call-template>
						</xsl:attribute>
					</xsl:otherwise>
				</xsl:choose>
			</unit>
		</xsl:for-each>
	</xsl:template>

	<xsl:template name="max">
		<xsl:param name="first"/>
		<xsl:param name="second"/>
		<xsl:choose>
			<xsl:when test="number($first) > number($second)">
				<xsl:value-of select="$first"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="$second"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

</xsl:transform>
