using System;
using System.Drawing;
using System.Windows.Forms;
using ESRI.ArcGIS.esriSystem;
using ESRI.ArcGIS.ArcMapUI;
using ESRI.ArcGIS.Geodatabase;
using ESRI.ArcGIS.Display;
using ESRI.ArcGIS.Carto;

namespace Sword
{
    namespace Crossbow
    {
        sealed partial class Orbat
        {
            /// <summary> 
            /// Required designer variable.
            /// </summary>
            private System.ComponentModel.IContainer components = null;
            private IFeatureLayer m_featureLayer;

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
                this.m_symbolTree = new System.Windows.Forms.TreeView();
                this.SuspendLayout();
                // 
                // m_symbolTree
                // 
                this.m_symbolTree.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                            | System.Windows.Forms.AnchorStyles.Left)
                            | System.Windows.Forms.AnchorStyles.Right)));
                this.m_symbolTree.ItemHeight = 32;
                this.m_symbolTree.Location = new System.Drawing.Point(0, 0);
                this.m_symbolTree.Name = "m_symbolTree";
                this.m_symbolTree.ShowRootLines = false;
                this.m_symbolTree.Size = new System.Drawing.Size(340, 580);
                this.m_symbolTree.TabIndex = 0;
                // 
                // Orbat
                // 
                this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
                this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
                this.Controls.Add(this.m_symbolTree);
                this.Name = "Orbat";
                this.Size = new System.Drawing.Size(340, 580);
                this.ResumeLayout(false);

            }

            private void InitializeEvents()
            {
                m_symbolTree.MouseClick += new MouseEventHandler(OnSymbolTreeNodeClicked);
            }

            private void SelectFeature(TreeNode node)
            {
                if (m_featureLayer != null)
                {
                    IFeatureSelection selection = (IFeatureSelection)m_featureLayer;
                    IQueryFilter filter = new QueryFilterClass();
                    filter.WhereClause = "Public_OID = " + node.Name;
                    selection.SelectFeatures(filter, esriSelectionResultEnum.esriSelectionResultNew, true);
                    // m_featureLayer.Search(filter, true);
                }
            }

            private void OnSymbolTreeNodeClicked(object sender, MouseEventArgs e)
            {
                SelectFeature(m_symbolTree.GetNodeAt(e.X, e.Y));
                if (e.Button == MouseButtons.Right)
                {
                    OrderHandler handler = Tools.GetSwordExtension().OrderHandler;
                    if (handler != null)
                        handler.OnContextMenu(e.X, e.Y);
                }
            }

            #endregion

            #region SymbolTree processing
            /// <summary>
            /// Build Image list
            /// </summary>
            /// <param name="table"></param>
            void BuildTableSymbols(ITable table)
            {
                if (table == null)
                    return;
                ICursor cursor = GetCursor(table);
                if (cursor == null)
                    return;

                IDataStatistics dataStatistics = new DataStatisticsClass();
                dataStatistics.Field = "Symbol_ID";
                dataStatistics.Cursor = cursor;

                System.Collections.IEnumerator enumerator = dataStatistics.UniqueValues;
                enumerator.Reset();
                while (enumerator.MoveNext())
                {
                    string symbolID = enumerator.Current.ToString();
                    if (!m_symbolTree.ImageList.Images.ContainsKey(symbolID))
                    {
                        Image image = (Image)m_pSymbolFactory.GetSymbol(m_SimpleDisplay, symbolID, "", 32);
                        m_symbolTree.ImageList.Images.Add(symbolID, image);
                    }
                }
                cursor = null;
            }

            /// <summary>
            /// Retrieve table's cursor to run through the table
            /// </summary>        
            /// <param name="pTable">IFeatureClass or ITable</param>
            /// <returns></returns>
            private static ICursor GetCursor(ITable pTable)
            {
                ICursor pCursor;
                IQueryFilter pQueryFilter = new QueryFilterClass();
                pQueryFilter.SubFields = "Symbol_ID";
                pCursor = pTable.Search(pQueryFilter, true);
                pQueryFilter = null;
                return pCursor;
            }

            /// <summary>
            /// Load and initialize symbol tree with image initialization
            /// </summary>
            void BuildSymbols(ITable table)
            {
                if (m_symbolTree.ImageList == null)
                {
                    m_symbolTree.ImageList = new ImageList();
                    m_symbolTree.ImageList.ImageSize = new Size(32, 32);
                }
                BuildTableSymbols(table);
            }

            private void RunAgentCursorOnSubordinate(IFeatureClass pFeatureClass, string stOIDFormation, string stOIDParent)
            {
                IQueryFilter pQueryFilter = new QueryFilterClass();
                pQueryFilter.WhereClause = "Parent_OID = " + stOIDFormation + " AND Public_OID <> " + stOIDParent;

                IFeatureCursor pCursor = pFeatureClass.Search(pQueryFilter, true);
                IFeature pFeature = pCursor.NextFeature();
                TreeNode[] node = m_symbolTree.Nodes.Find(stOIDFormation, true);
                while (pFeature != null)
                {
                    int oid = Tools.GetValue<int>(pFeature, "Public_OID");
                    string symbol = Tools.GetValue<string>(pFeature, "Symbol_ID");
                    string name = Tools.GetValue<string>(pFeature, "Name");

                    TreeNode newNode = node[0].Nodes.Add(oid.ToString(), name, symbol);
                    newNode.SelectedImageKey = symbol;
                    pFeature = pCursor.NextFeature();
                }
                pCursor = null;
            }

            private void RunAgentCursor(IFeatureClass pFeatureClass)
            {
                IQueryFilter pQueryFilter = new QueryFilterClass();
                // pQueryFilter.WhereClause = "Symbol_ID LIKE '__________A____'";
                pQueryFilter.WhereClause = "Symbol_ID LIKE '??????????A????'";
                IFeatureCursor pCursor = pFeatureClass.Search(pQueryFilter, true);
                IFeature pFeature = pCursor.NextFeature();
                while (pFeature != null)
                {
                    int oid = Tools.GetValue<int>(pFeature, "Public_OID");
                    int oidParent = Tools.GetValue<int>(pFeature, "Parent_OID");
                    string name = Tools.GetValue<string>(pFeature, "Name");
                    string symbol = Tools.GetValue<string>(pFeature, "Symbol_ID");

                    TreeNode[] node = m_symbolTree.Nodes.Find(oidParent.ToString(), true);
                    TreeNode newNode = node[0].Nodes.Add(oid.ToString(), name, symbol);
                    newNode.SelectedImageKey = symbol;
                    RunAgentCursorOnSubordinate(pFeatureClass, oidParent.ToString(), oid.ToString());
                    pFeature = pCursor.NextFeature();
                }
                pQueryFilter = null;
                pCursor = null;
            }

            private void UpdateFormation(ITable table)
            {
                ICursor pCursor = table.Search(null, true);
                IRow pRow = pCursor.NextRow();
                while (pRow != null)
                {
                    int oidParent = Tools.GetValue<int>(pRow, "Parent_OID");
                    int oid = Tools.GetValue<int>(pRow, "Public_OID");
                    string name = Tools.GetValue<string>(pRow, "Name");
                    string symbolID = Tools.GetValue<string>(pRow, "Symbol_ID");

                    TreeNodeCollection parentNode = m_symbolTree.Nodes;
                    TreeNode[] query = m_symbolTree.Nodes.Find(oidParent.ToString(), true);
                    // if something has been found, 
                    // add as children otherwise add to the root                
                    if (query.Length > 0)
                        parentNode = query[0].Nodes;
                    TreeNode newNode = parentNode.Add(oid.ToString(), name, symbolID);
                    newNode.SelectedImageKey = symbolID;
                    pRow = pCursor.NextRow();
                }
                pCursor = null;
            }

            /// <summary>
            /// Setup orbat inner parameters
            /// </summary>
            public void LoadLayer(string layerName)
            {
                IFeatureLayer layer = Tools.GetFeatureLayerByName(layerName);
                if (layer == null || layer == m_featureLayer)
                    return;
                m_featureLayer = layer;
                IFeatureWorkspace workspace = Tools.RetrieveWorkspace(m_featureLayer);
                ITable formationTable = workspace.OpenTable("taranis_simulation.sde.Formations");
                m_symbolTree.BeginUpdate();
                BuildSymbols(formationTable);
                BuildSymbols(m_featureLayer.FeatureClass as ITable);
                UpdateFormation(formationTable);
                RunAgentCursor(m_featureLayer.FeatureClass);
                m_symbolTree.EndUpdate();
                formationTable = null;
            }
            #endregion

            private TreeView m_symbolTree;

        }
    }
}