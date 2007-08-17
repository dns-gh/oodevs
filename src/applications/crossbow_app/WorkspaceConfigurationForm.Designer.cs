namespace Crossbow
{
    partial class WorkspaceConfigurationForm
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.exerciseFileButton = new System.Windows.Forms.Button();
            this.exerciseFileText = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.exerciseFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.layersGrid = new System.Windows.Forms.PropertyGrid();
            this.databaseButton = new System.Windows.Forms.Button();
            this.databaseText = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.exerciseFileButton);
            this.groupBox1.Controls.Add(this.exerciseFileText);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Location = new System.Drawing.Point(15, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(467, 55);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Exercise file";
            // 
            // exerciseFileButton
            // 
            this.exerciseFileButton.Location = new System.Drawing.Point(371, 22);
            this.exerciseFileButton.Name = "exerciseFileButton";
            this.exerciseFileButton.Size = new System.Drawing.Size(75, 23);
            this.exerciseFileButton.TabIndex = 2;
            this.exerciseFileButton.Text = "Browse...";
            this.exerciseFileButton.UseVisualStyleBackColor = true;
            this.exerciseFileButton.Click += new System.EventHandler(this.exerciseFileButton_Click);
            // 
            // exerciseFileText
            // 
            this.exerciseFileText.Location = new System.Drawing.Point(82, 22);
            this.exerciseFileText.Name = "exerciseFileText";
            this.exerciseFileText.Size = new System.Drawing.Size(289, 20);
            this.exerciseFileText.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(16, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(26, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "File:";
            // 
            // exerciseFileDialog
            // 
            this.exerciseFileDialog.Filter = "Exercise files|*.xml";
            this.exerciseFileDialog.Tag = "";
            this.exerciseFileDialog.FileOk += new System.ComponentModel.CancelEventHandler(this.exerciseFileDialog_FileOk);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.layersGrid);
            this.groupBox2.Controls.Add(this.databaseButton);
            this.groupBox2.Controls.Add(this.databaseText);
            this.groupBox2.Controls.Add(this.label2);
            this.groupBox2.Location = new System.Drawing.Point(15, 73);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(467, 311);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Layers";
            // 
            // layersGrid
            // 
            this.layersGrid.Location = new System.Drawing.Point(19, 46);
            this.layersGrid.Name = "layersGrid";
            this.layersGrid.Size = new System.Drawing.Size(427, 249);
            this.layersGrid.TabIndex = 3;
            // 
            // databaseButton
            // 
            this.databaseButton.Location = new System.Drawing.Point(371, 20);
            this.databaseButton.Name = "databaseButton";
            this.databaseButton.Size = new System.Drawing.Size(75, 23);
            this.databaseButton.TabIndex = 2;
            this.databaseButton.Text = "Browse...";
            this.databaseButton.UseVisualStyleBackColor = true;
            this.databaseButton.Click += new System.EventHandler(this.databaseButton_Click);
            // 
            // databaseText
            // 
            this.databaseText.Location = new System.Drawing.Point(82, 20);
            this.databaseText.Name = "databaseText";
            this.databaseText.Size = new System.Drawing.Size(289, 20);
            this.databaseText.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(19, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(56, 13);
            this.label2.TabIndex = 0;
            this.label2.Text = "Database:";
            // 
            // okButton
            // 
            this.okButton.Location = new System.Drawing.Point(328, 390);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(76, 24);
            this.okButton.TabIndex = 2;
            this.okButton.Text = "Ok";
            this.okButton.UseVisualStyleBackColor = true;
            this.okButton.Click += new System.EventHandler(this.okButton_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelButton.Location = new System.Drawing.Point(404, 390);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 24);
            this.cancelButton.TabIndex = 3;
            this.cancelButton.Text = "Cancel";
            this.cancelButton.UseVisualStyleBackColor = true;
            this.cancelButton.Click += new System.EventHandler(this.cancelButton_Click);
            // 
            // WorkspaceConfigurationForm
            // 
            this.AcceptButton = this.okButton;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelButton;
            this.ClientSize = new System.Drawing.Size(494, 424);
            this.Controls.Add(this.cancelButton);
            this.Controls.Add(this.okButton);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.groupBox1);
            this.Name = "WorkspaceConfigurationForm";
            this.Text = "CSword Workspace Configuration";
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button exerciseFileButton;
        private System.Windows.Forms.TextBox exerciseFileText;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.OpenFileDialog exerciseFileDialog;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Button databaseButton;
        private System.Windows.Forms.TextBox databaseText;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.PropertyGrid layersGrid;
    }
}