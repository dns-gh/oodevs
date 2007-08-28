namespace Crossbow
{
    partial class InfoPanel
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
            this.labelPlaceholder = new System.Windows.Forms.Label();
            this.infoTabs = new System.Windows.Forms.TabControl();
            this.reportsTab = new System.Windows.Forms.TabPage();
            this.dotationsTab = new System.Windows.Forms.TabPage();
            this.reportsList = new System.Windows.Forms.ListBox();
            this.infoTabs.SuspendLayout();
            this.reportsTab.SuspendLayout();
            this.SuspendLayout();
            // 
            // labelPlaceholder
            // 
            this.labelPlaceholder.Dock = System.Windows.Forms.DockStyle.Fill;
            this.labelPlaceholder.Location = new System.Drawing.Point(0, 0);
            this.labelPlaceholder.Name = "labelPlaceholder";
            this.labelPlaceholder.Size = new System.Drawing.Size(340, 158);
            this.labelPlaceholder.TabIndex = 0;
            this.labelPlaceholder.Text = "Place controls on the canvas for your dockable window definition";
            this.labelPlaceholder.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // infoTabs
            // 
            this.infoTabs.Alignment = System.Windows.Forms.TabAlignment.Bottom;
            this.infoTabs.Controls.Add(this.reportsTab);
            this.infoTabs.Controls.Add(this.dotationsTab);
            this.infoTabs.Dock = System.Windows.Forms.DockStyle.Fill;
            this.infoTabs.Location = new System.Drawing.Point(0, 0);
            this.infoTabs.Multiline = true;
            this.infoTabs.Name = "infoTabs";
            this.infoTabs.SelectedIndex = 0;
            this.infoTabs.Size = new System.Drawing.Size(340, 158);
            this.infoTabs.TabIndex = 1;
            // 
            // reportsTab
            // 
            this.reportsTab.Controls.Add(this.reportsList);
            this.reportsTab.Location = new System.Drawing.Point(4, 4);
            this.reportsTab.Name = "reportsTab";
            this.reportsTab.Padding = new System.Windows.Forms.Padding(3);
            this.reportsTab.Size = new System.Drawing.Size(332, 132);
            this.reportsTab.TabIndex = 0;
            this.reportsTab.Text = "Reports";
            this.reportsTab.UseVisualStyleBackColor = true;
            // 
            // dotationsTab
            // 
            this.dotationsTab.Location = new System.Drawing.Point(4, 4);
            this.dotationsTab.Name = "dotationsTab";
            this.dotationsTab.Padding = new System.Windows.Forms.Padding(3);
            this.dotationsTab.Size = new System.Drawing.Size(332, 132);
            this.dotationsTab.TabIndex = 1;
            this.dotationsTab.Text = "Dotations";
            this.dotationsTab.UseVisualStyleBackColor = true;
            // 
            // reportsList
            // 
            this.reportsList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.reportsList.FormattingEnabled = true;
            this.reportsList.Location = new System.Drawing.Point(3, 3);
            this.reportsList.Name = "reportsList";
            this.reportsList.Size = new System.Drawing.Size(326, 121);
            this.reportsList.TabIndex = 0;
            // 
            // InfoPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.infoTabs);
            this.Controls.Add(this.labelPlaceholder);
            this.Name = "InfoPanel";
            this.Size = new System.Drawing.Size(340, 158);
            this.infoTabs.ResumeLayout(false);
            this.reportsTab.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label labelPlaceholder;
        private System.Windows.Forms.TabControl infoTabs;
        private System.Windows.Forms.TabPage reportsTab;
        private System.Windows.Forms.TabPage dotationsTab;
        private System.Windows.Forms.ListBox reportsList;
    }
}
