using System;
using System.Drawing;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Carto;

namespace crossbow
{
    partial class Orbat
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            // this.labelPlaceholder = new System.Windows.Forms.Label();
            this.m_pSymbolTree = new System.Windows.Forms.TreeView();
            this.SuspendLayout();            
            // 
            // m_pSymbolTree
            // 
            this.m_pSymbolTree.Location = new System.Drawing.Point(0, 0);
            this.m_pSymbolTree.Name = "m_pSymbolTree";
            this.m_pSymbolTree.Size = new System.Drawing.Size(340, 580);
            this.m_pSymbolTree.TabIndex = 0;
            // 
            // Orbat
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(m_pSymbolTree);
            this.Name = "Orbat";
            this.Size = new System.Drawing.Size(340, 580);
            this.ResumeLayout(false);

        }
        #endregion

        #region SymbolTree processing
        /// <summary>
        /// Build Image list
        /// </summary>
        /// <param name="pCursor"></param>
        void InitializeSymbolOnCursor(IDisplay pDisplay, ICursor pCursor, int[] mFields)
        {
            IRow pRow = pCursor.NextRow();
            while (pRow != null)
            {
                string symbolID = (string)pRow.get_Value(mFields[(int)FieldsProperty.EnumFields.eSymbol]);                
                Image pImage = (Image)m_pSymbolFactory.GetSymbol(pDisplay, symbolID, "", 32);
                m_pSymbolTree.ImageList.Images.Add(symbolID, pImage);
                pRow = pCursor.NextRow();                
            }
        }

        /// <summary>
        /// Retrieve table's cursor to run through the table
        /// </summary>        
        /// <param name="pTable">IFeatureClass or ITable</param>
        /// <returns></returns>
        ICursor GetCursor(ITable pTable)
        {
            ICursor pCursor;
            IQueryFilter pQueryFilter = new QueryFilterClass();
            pQueryFilter.SubFields = "distinct (Symbol_ID)";

            pCursor = pTable.Search(pQueryFilter, false);
            pQueryFilter = null;
            return pCursor;
        }

        /// <summary>
        /// Load and initialize symbol tree
        /// </summary>
        void SetupSymbols(IFeatureLayer pLayer)
        {
            m_pSymbolTree.ImageList = new System.Windows.Forms.ImageList();
            m_pSymbolTree.ImageList.ImageSize = new Size(32, 32);

            ESRI.ArcGIS.ArcMapUI.IMxDocument mxDocument = Tools.GetMxDocument(m_application);
            IFeatureWorkspace pWorkspace = Tools.RetrieveWorkspace(pLayer);
            ICursor pCursor = GetCursor(pWorkspace.OpenTable("Formations"));
            if (pCursor != null)
                InitializeSymbolOnCursor(m_SimpleDisplay, pCursor, m_Fields.Formation);
            pCursor = GetCursor((ITable)pLayer.FeatureClass);
            if (pCursor != null)
                InitializeSymbolOnCursor(m_SimpleDisplay, pCursor, m_Fields.Element);
        }

        private void RunAgentCursorOnSubordinate(IFeatureClass pFeatureClass, string stOIDFormation, string stOIDParent)
        {
            IQueryFilter pQueryFilter = new QueryFilterClass();
            pQueryFilter.WhereClause = "Parent_OID = " + stOIDFormation + " AND Public_OID <> " + stOIDParent;

            IFeatureCursor pCursor = pFeatureClass.Search(pQueryFilter, false);
            IFeature pFeature = pCursor.NextFeature();
            System.Windows.Forms.TreeNode[] node = m_pSymbolTree.Nodes.Find(stOIDFormation, true);
            while (pFeature != null)
            {
                int oid = (int)pFeature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eOID]);
                string symbol = (string)pFeature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eSymbol]);
                string name = (string)pFeature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eName]);

                // m_pSymbolTree.Nodes.Insert(m_pSymbolTree.Nodes.IndexOfKey(stOIDFormation), oid.ToString(), name, symbol);
                node[0].Nodes.Add(oid.ToString(), name, symbol);
                pFeature = pCursor.NextFeature();
            }
        }

        private void RunAgentCursor(IFeatureClass pFeatureClass)
        {
            IQueryFilter pQueryFilter = new QueryFilterClass();
            pQueryFilter.WhereClause = "Symbol_ID Like \"??????????A????\"";

            IFeatureCursor pCursor = pFeatureClass.Search(pQueryFilter, false);
            IFeature pFeature = pCursor.NextFeature();
            while (pFeature != null)
            {
                int oid = (int)pFeature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eOID]);
                int oidParent = (int)pFeature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eParentOID]);
                int formation = (int)pFeature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eFormationOID]);
                string name = (string)pFeature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eName]);
                string symbol = (string)pFeature.get_Value(m_Fields.Element[(int)FieldsProperty.EnumFields.eSymbol]);

                System.Windows.Forms.TreeNode[] node = m_pSymbolTree.Nodes.Find(oidParent.ToString(), true);
                node[0].Nodes.Add(oid.ToString(), name, symbol);                
                RunAgentCursorOnSubordinate(pFeatureClass, oidParent.ToString(), oid.ToString());
                pFeature = pCursor.NextFeature();
            }
        }

        private void UpdateFormation(IFeatureLayer pLayer)
        {
            IFeatureWorkspace pWorkspace = Tools.RetrieveWorkspace(pLayer);
            ITable pTable = pWorkspace.OpenTable("Formations");
            ICursor pCursor = pTable.Search(null, false);
            IRow    pRow = pCursor.NextRow();
            
            while (pRow != null)
            {
                int oid = (int)pRow.get_Value(m_Fields.Formation[(int)FieldsProperty.EnumFields.eOID]);
                int oidParent = (int)pRow.get_Value(m_Fields.Formation[(int)FieldsProperty.EnumFields.eParentOID]);
                string name = (string)pRow.get_Value(m_Fields.Formation[(int)FieldsProperty.EnumFields.eName]);
                string symbolID = (string)pRow.get_Value(m_Fields.Formation[(int)FieldsProperty.EnumFields.eSymbol]);

                if (oidParent > 0 && oidParent < 4472872)
                {
                    System.Windows.Forms.TreeNode[] node = m_pSymbolTree.Nodes.Find(oidParent.ToString(), true);
                    node[0].Nodes.Add(oid.ToString(), name, symbolID);
                }
                else
                    m_pSymbolTree.Nodes.Add(oid.ToString(), name, symbolID);
                pRow = pCursor.NextRow();
            }
        }

        /// <summary>
        /// Setup orbat inner parameters
        /// </summary>
        public void SetupOrbatCommand()
        {
            ESRI.ArcGIS.ArcMapUI.IMxDocument mxDocument = Tools.GetMxDocument(m_application);
            IFeatureLayer pLayer = Tools.GetIFeatureLayerFromLayerName(mxDocument.ActiveView, "UnitForces");
            
            if (pLayer != null)
            {
                IFeatureWorkspace pWorkspace = Tools.RetrieveWorkspace(pLayer);
                m_Fields.SetupFormationFields(pWorkspace.OpenTable("Formations"));
                m_Fields.SetupTacticalElementFields(pLayer.FeatureClass);
                SetupSymbols(pLayer);

                UpdateFormation(pLayer);
                RunAgentCursor(pLayer.FeatureClass);
            }
        }
        #endregion

        private System.Windows.Forms.TreeView m_pSymbolTree;
    }
}
