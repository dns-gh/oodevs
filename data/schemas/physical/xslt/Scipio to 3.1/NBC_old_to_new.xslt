<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>

	<xsl:template match="text()"/>
	
	<xsl:template match="NBC">
		<nbc>
			<propagation wind-speed-limit="{VitesseVentLimiteAvantPropagationDirectionnelle}"/>
			<xsl:apply-templates/>
		</nbc>
	</xsl:template>
	
	<xsl:template match="ImpactTenueNBC">
		<nbc-suit max-speed-modifier="{CoefModificationVitesseMax}" reloading-time-modifier="{CoefModificationTempsRechargement}"/>
	</xsl:template>
	
	<xsl:template match="AgentsNBC">
		<agents>
			<xsl:for-each select="AgentNBC">
				<agent name="{@nom}" id="{MosID}">
					<xsl:for-each select="./Liquide|./Gaz">
						<effects>
							<xsl:attribute name="type">
								<xsl:choose>
									<xsl:when test="name() = 'Liquide'">liquid</xsl:when>
									<xsl:when test="name() = 'Gaz'">gaseous</xsl:when>
								</xsl:choose>
							</xsl:attribute>
							<xsl:for-each select="./*">
								<xsl:if test="name() = 'Intoxication'">
									<xsl:attribute name="affliction">intoxication</xsl:attribute>
								</xsl:if>
								<xsl:if test="../Contamination">
									<xsl:attribute name="contamination">true</xsl:attribute>
								</xsl:if>
								<xsl:if test="../DureeDeVie">
									<xsl:attribute name="life-time">
										<xsl:value-of select="../DureeDeVie"/>
									</xsl:attribute>
								</xsl:if>
								<xsl:if test="../AngleDePropagation">
									<xsl:attribute name="propagation">
										<xsl:value-of select="../AngleDePropagation"/>
									</xsl:attribute>
								</xsl:if>
							</xsl:for-each>
							<xsl:for-each select="./Intoxication/*">
								<effect percentage="{.}">
									<xsl:attribute name="wound">
										<xsl:call-template name="HumanWound">
											<xsl:with-param name="wound" select="name()"/>
										</xsl:call-template>
									</xsl:attribute>
								</effect>
							</xsl:for-each>
						</effects>
					</xsl:for-each>
				</agent>
			</xsl:for-each>
		</agents>
	</xsl:template>
	
	<xsl:template name="HumanWound">
		<xsl:param name="wound"/>
		<xsl:if test="$wound = 'Humains_U1'">
			<xsl:text>u1</xsl:text>
		</xsl:if>
		<xsl:if test="$wound = 'Humains_U2'">
			<xsl:text>u2</xsl:text>
		</xsl:if>
		<xsl:if test="$wound = 'Humains_U3'">
			<xsl:text>u3</xsl:text>
		</xsl:if>
		<xsl:if test="$wound = 'Humains_UE'">
			<xsl:text>uE</xsl:text>
		</xsl:if>
		<xsl:if test="$wound = 'Humains_Mort'">
			<xsl:text>mort</xsl:text>
		</xsl:if>
		<xsl:if test="$wound = 'Humains_NonBlesse'">
			<xsl:text>nonblesse</xsl:text>
		</xsl:if>
	</xsl:template>

</xsl:transform>
