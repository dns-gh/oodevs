<?xml version="1.0" encoding="UTF-8"?>

<xsl:transform xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    version="1.0">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="Pions">
		<units>
			<xsl:apply-templates/>
		</units>
	</xsl:template>
	
	<xsl:template match="Unite">
		<unit name="{@nom}" type="{@type}" id="{MosID}" decisional-model="{ModeleDecisionnel}">
			<xsl:if test="PeutVoler">
				<xsl:attribute name="can-fly">
					<xsl:text>true</xsl:text>
				</xsl:attribute>
			</xsl:if>
			<xsl:if test="EstAutonome">
				<xsl:attribute name="is-autonomous">
					<xsl:text>true</xsl:text>
				</xsl:attribute>
			</xsl:if>
			<xsl:apply-templates/>
		</unit>
	</xsl:template>
	
	<xsl:template match="DelaiDecontaminationNBC">
		<nbc decontamination-delay="{.}"/>
	</xsl:template>
	
	<xsl:template match="Nature">
		<nature level="{Niveau/@type}" atlas-nature="{QualificatifAtlas/@type}" nature="{Nature/@type}"/>
	</xsl:template>
	
	<xsl:template match="Equipements">
		<equipments>
			<xsl:for-each select="Equipement">
				<equipment crew="{@equipage}" type="{@nom}" count="{.}">
					<xsl:if test="@convoyeur">
						<xsl:attribute name="convoyer">
							<xsl:value-of select="@convoyeur"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="@embarquable">
						<xsl:attribute name="loadable">
							<xsl:value-of select="@embarquable"/>
						</xsl:attribute>
					</xsl:if>
					<xsl:if test="@majeur">
						<xsl:attribute name="major">
							<xsl:value-of select="@majeur"/>
						</xsl:attribute>
					</xsl:if>
				</equipment>
			</xsl:for-each>
		</equipments>
	</xsl:template>
	
	<xsl:template match="RepartitionDuCommandement">
		<crew-ranks>
			<xsl:for-each select="./*">
				<crew-rank type="{name()}" count="{.}"/>
			</xsl:for-each>
		</crew-ranks>
	</xsl:template>
	
	<xsl:template match="ContenanceTC1">
		<logistics>
			<xsl:for-each select="Dotation">
				<category name="{@nom}">
					<xsl:for-each select="Categories/Categorie">
						<dotation normalized-consumption="{@consommationNormalisee}" capacity="{@contenance}" name="{@nom}" logistic-threshold="{@seuilLogistique}"/>
					</xsl:for-each>
				</category>
			</xsl:for-each>
		</logistics>
	</xsl:template>
	
	<xsl:template match="Stocks">
		<stocks>
			<xsl:for-each select="SeuilsLogistiques/SeuilLogistique">
				<stock category="{@categorie}" threshold="{@seuil}"/>
			</xsl:for-each>
		</stocks>
	</xsl:template>
	
	<xsl:template match="TempsMiseEnPosture">
		<postures>
			<xsl:for-each select="./*">
				<posture name="{name()}" setup-time="{.}"/>
			</xsl:for-each>
		</postures>
	</xsl:template>
	
	<xsl:template match="Installation">
		<setup installation-time="{DelaiInstallation}" uninstallation-time="{DelaiDesinstallation}"/>
	</xsl:template>
	
	<xsl:template match="DistancesAvantPoints">
		<distance-before-points>
			<xsl:for-each select="Point">
				<distance-before-point type="{@type}" value="{.}"/>
			</xsl:for-each>
		</distance-before-points>
	</xsl:template>
	
	<xsl:template match="CoupDeSonde">
		<drill-blow width="{Largeur}" length="{Longueur}"/>
	</xsl:template>
	
	<xsl:template match="RapportDeForce">
		<force-ratio feedback-time="{TempsDeRemontee}"/>
	</xsl:template>
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
</xsl:transform>
