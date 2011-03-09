<?xml version="1.0" encoding="UTF-8"?>
<xsl:transform version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:fn="http://www.w3.org/2004/10/xpath-functions" xmlns:xdt="http://www.w3.org/2004/10/xpath-datatypes">

	<xsl:output method="xml" version="1.0" encoding="UTF-8" indent="yes"/>
	
	<xsl:template match="text()"/>
	
	<xsl:template match="/">
		<dotations>
			<xsl:apply-templates/>
		</dotations>		
	</xsl:template>
	
	<xsl:template match="Dotation">
		<xsl:variable name="Dotation_Name" select="@nom"/>
		<xsl:for-each select="Categories/Categorie">
			<dotation name="{$Dotation_Name}" category="{@nom}" type="{Type}" id="{MosID}" package-size="{Conditionnement/Nombre}" package-mass="{Conditionnement/Masse}" package-volume="{Conditionnement/Volume}" nature="{Nature}">
				<xsl:if test="CodeEMAT6">
					<xsl:attribute name="codeEMAT6">
						<xsl:value-of select="CodeEMAT6"/>
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="CodeEMAT8">
					<xsl:attribute name="codeEMAT8">
						<xsl:value-of select="CodeEMAT8"/>
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="CodeLFRIL">
					<xsl:attribute name="codeLFRIL">
						<xsl:value-of select="CodeLFRIL"/>
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="CodeNNO">
					<xsl:attribute name="codeNNO">
						<xsl:value-of select="CodeNNO"/>
					</xsl:attribute>
				</xsl:if>
				<xsl:if test="TrancheD">
					<xsl:attribute name="d-type">
						<xsl:text>true</xsl:text>
					</xsl:attribute>
				</xsl:if>
				<attritions>
					<xsl:apply-templates/>
				</attritions>
				<xsl:if test="TirIndirect">
					<indirect-fire type="{TirIndirect/@type}" intervention-type="{TirIndirect/InterventionType}" x-dispersion="{TirIndirect/DispersionX}" y-dispersion="{TirIndirect/DispersionY}">
						<xsl:if test="TirIndirect/CoefficientNeutralisation">
							<xsl:attribute name="neutralization-ratio">
								<xsl:value-of select="TirIndirect/CoefficientNeutralisation"/>
							</xsl:attribute>
						</xsl:if>
						<xsl:if test="TirIndirect/DureeDeploiement">
							<xsl:attribute name="setup-time">
								<xsl:value-of select="TirIndirect/DureeDeploiement"/>
							</xsl:attribute>
						</xsl:if>
						<xsl:if test="TirIndirect/DureeVie">
							<xsl:attribute name="life-time">
								<xsl:value-of select="TirIndirect/DureeVie"/>
							</xsl:attribute>
						</xsl:if>
						<xsl:if test="TirIndirect/NombreMines">
							<xsl:attribute name="mine-count">
								<xsl:value-of select="TirIndirect/NombreMines"/>
							</xsl:attribute>
						</xsl:if>
						<xsl:for-each select="TirIndirect/PHs/PostureCible/*">
							<ph target-posture="{name()}" value="{.}"/>
						</xsl:for-each>
					</indirect-fire>
				</xsl:if>
			</dotation>
		</xsl:for-each>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeCharLourd">
		<attrition protection="CharLourd" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeCharMoyen">
		<attrition protection="CharMoyen" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>	
	
	<xsl:template match="ProtectionTypeBlindeLeger">
		<attrition protection="BlindeLeger" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeTransportBlinde">
		<attrition protection="TransportBlinde" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeAeronef">
		<attrition protection="Aeronef" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeVehiculeNonProtege">
		<attrition protection="VehiculeNonProtege" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeCombattant">
		<attrition protection="Combattant" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeHeavyTank">
		<attrition protection="HeavyTank" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeMediumTank">
		<attrition protection="MediumTank" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>	
	
	<xsl:template match="ProtectionTypeLightTank">
		<attrition protection="LightTank" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeArmouredCarrier">
		<attrition protection="ArmouredCarrier" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeAircraft">
		<attrition protection="Aircraft" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeUnarmouredVehicle">
		<attrition protection="UnarmouredVehicle" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
	<xsl:template match="ProtectionTypeCombatant">
		<attrition protection="Combatant" destruction="{Destruction}" repairable-with-evacuation="{ReparableAvecEvacuation}" repairable-without-evacuation="{ReparableSansEvacuation}"/>
	</xsl:template>
	
</xsl:transform>
